#include "GamePacketManager.h"
#include "LoginPacket.h"
#include "MessageBoxPage.h"
#include "waitingManager.h"
#include "GameMessages.h"
#include "SeverConsts.h"

bool ret = PacketManager::Get()->registerPacketSendListener(GamePacketManager::Get());

void GamePacketManager::onPreSend( int opcode, ::google::protobuf::Message*, bool needWaiting/*=true*/ )
{
	if(mNeedReConnect)
	{
		mNeedReConnect = false;
		CCLOG("GamePacketManager::1onPreSend(%d)", opcode);
		if(opcode != LOGIN_C)
			LoginPacket::Get()->forceSentPacket();
	}
}

void GamePacketManager::onPreSend( int opcode, char* buff, int length, bool needWaiting /*= true*/ )
{
	if(mNeedReConnect)
	{
		mNeedReConnect = false;
		CCLOG("GamePacketManager::2onPreSend(%d)", opcode);
		if(opcode != LOGIN_C)
			LoginPacket::Get()->forceSentPacket();
	}
}

void GamePacketManager::onPostSend( int opcode, ::google::protobuf::Message*, bool needWaiting/*=true*/, int targetOpcode/*=0*/ )
{
	if (mShowRelogin)
	{
		return;
	}
	if(needWaiting)
		waitingManager::Get()->startWaiting(opcode,targetOpcode ? targetOpcode : opcode+1);
}

void GamePacketManager::onPostSend( int opcode, char* buff, int length, bool needWaiting /*= true*/ )
{
	if (mShowRelogin)
	{
		return;
	}
	if(needWaiting)
		waitingManager::Get()->startWaiting(opcode,opcode+1);
}

void GamePacketManager::onBoardcastPacketToHandler(int id, const ::google::protobuf::Message* msg, const std::string& msgStr/* =0 */)
{
	mReconnectTimes = 0;	

	if (mShowRelogin)
	{
		MsgMainFramePopPage msg;
		msg.pageName = "ConnectFailedPage";
		MessageManager::Get()->sendMessage(&msg);
		mShowRelogin = false;
	}		
}

void GamePacketManager::onBoardcastConnectionFailed(std::string ip, int port)
{
	++mReconnectTimes;
	bool b = StringConverter::parseBool(VaribleManager::Get()->getSetting("ReconnectShowFlag"));
	if (b )
	{
		unsigned int max = StringConverter::parseUnsignedInt(VaribleManager::Get()->getSetting("ReconnectMaxTimes"));
		if (mReconnectTimes >= max && !mShowRelogin)
		{
			MsgMainFramePushPage msg;
			msg.pageName = "ConnectFailedPage";
			MessageManager::Get()->sendMessage(&msg);	

			waitingManager::Get()->endWaiting();
			mNeedReConnect = false;
			mShowRelogin = true;

			mReconnectTimes = 0;

			SeverConsts::Get()->Free();
			waitingManager::Get()->clearReasones();
		}
	}	
	else
	{
		mNeedReConnect = true;
		waitingManager::Get()->endWaiting();
		waitingManager::Get()->clearReasones();
		MSG_BOX_LAN("@ConnectGameSvrFailed");
	}
}

void GamePacketManager::onBoardcastSendFailed( int opcode )
{
	if (mShowRelogin)
	{
		waitingManager::Get()->endWaiting();
		mNeedReConnect = false;
		return;
	}
	if(!GamePrecedure::Get()->getUserKickedByServer())
	{				
		if(opcode != 3)//心跳的opcode
		{
			if(libOS::getInstance()->getNetWork()==NotReachable)
			{
				MSG_BOX_LAN("@NoNetWork");
			}
			else
			{
				MSG_BOX_LAN("@SendPacketFailed");
				//todo: 不明原因会AsyncSocket::Send::(mSocket == INVALID_SOCKET) 是否需要reconnect dylan
				//PacketManager::Get()->reconnect();
				//zhengxin 2014-08-19
				mNeedReConnect = false;
				waitingManager::Get()->endWaiting();
				waitingManager::Get()->clearReasones();//手动触发超时，结束菊花
				LoginPacket::Get()->setEnabled(true);//zhengxin 2014-08-19
				LoginPacket::Get()->sendPacket();
				//--end
			}
		}
		else
		{
			CCLOG("GamePacketManager::onBoardcastSendFailed(%d)->setNeedReConnect", opcode);
			setNeedReConnect();
			MSG_BOX_LAN("@NeedReconnectGameSvr");
		}
	}
	else
	{
		setNeedReConnect();
		MSG_BOX_LAN("@UserKickoutMsg");
	}
}

void GamePacketManager::onBoardcastReceiveFailed()
{
	if(libOS::getInstance()->getNetWork()==NotReachable)
	{
		MSG_BOX_LAN("@NoNetWork");
	}
	else
	{
		//MSG_BOX_LAN("@ReceivePacketFailed");
	}
	//--begin zhengxin at 2014-08-20
	if(!GamePrecedure::Get()->isInLoadingScene())
	{
		CCLOG("GamePacketManager::onBoardcastReceiveFailed! Not isInLoadingScene LoginPacket::Get()->sendPacket(); ");
		mNeedReConnect = false;
		waitingManager::Get()->endWaiting();
		waitingManager::Get()->clearReasones();//手动触发超时，结束菊花
		LoginPacket::Get()->setEnabled(true);//zhengxin 2014-08-19
		LoginPacket::Get()->sendPacket();
	}
	else
	{
		CCLOG("GamePacketManager::onBoardcastReceiveFailed| isInLoadingScene showEnter");
		//PacketManager::Get()->disconnect();
		waitingManager::Get()->endWaiting();
		waitingManager::Get()->clearReasones();//手动触发超时，结束菊花
	}
	//--end
}

void GamePacketManager::onBoardcastPacketError( int opcode, const std::string &errmsg)
{
	if (errmsg.size() != 0)
	{
		MSG_BOX_LAN(errmsg);
	}
	else
	{
		MSG_BOX_LAN("@PacketError");
	}
}

GamePacketManager::GamePacketManager()
{
	mNeedReConnect = false;
	mReconnectTimes = 0;
	mShowRelogin = false;
}

void GamePacketManager::onBoardcastReceiveTimeout( int opcode )
{
	CCLOG("GamePacketManager::onBoardcastReceiveTimeout! LoginPacket::Get()->sendPacket(); ");
	//LoginPacket::Get()->setEnabled(true);//zhengxin 2014-08-19
	//LoginPacket::Get()->sendPacket();
	char szTmp[32] = {0};
	sprintf(szTmp, "%d", opcode);
	std::string str = Language::Get()->getString("@ReceivedTimeout");
	str.append(szTmp);
	MSG_BOX(str);
	//
	setNeedReConnect();//重连吧，会有能收到心跳包，但服务器以为掉线的情况，其他包都超时，且不重新发登录包恢复不了
	//MSG_BOX_LAN("@NeedReconnectGameSvr");
}
