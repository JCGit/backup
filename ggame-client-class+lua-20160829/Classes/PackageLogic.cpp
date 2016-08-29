
#include "stdafx.h"

#include "PackageLogic.h"
#include "ServerDateManager.h"
#include "GameMessages.h"
#include "BlackBoard.h"
#include "GamePrecedure.h"
#include "CCBManager.h"
#include "StringConverter.h"
#include "cocos2d.h"
#include "libOS.h"
#include "MessageBoxPage.h"
#include "SoundManager.h"
//#include "ThreadSocket.h"
#include "TimeCalculator.h"
#include "LoginPacket.h"
#include "SeverConsts.h"
#include "GamePacketManager.h"
#include "waitingManager.h"
#include "com4loves.h"
#include <algorithm>    // std::sort

static int num;
void PacketHandlerGeneral::onSendPacketFailed( const int opcode )
{
	if(opcode != _getOpcode() - 1)
		return;
	CCLOG("PacketHandlerGeneral::onSendPacketFailed(%d,%d)", opcode, _getOpcode());
}

void PacketHandlerGeneral::onReceivePacketFailed()
{
	CCLOG("PacketHandlerGeneral::onReceivePacketFailed!");
}

void PacketHandlerGeneral::onTimeout( const int opcode )
{
	if(opcode != _getOpcode())
		return;
	CCLOG("PacketHandlerGeneral::onTimeout(%d,%d)", opcode, _getOpcode());
	//MSG_BOX("@ReceivedTimeout");
}

void PacketHandlerGeneral::onPacketError( const int opcode )
{
	if(opcode != _getOpcode())
		return;
	CCLOG("PacketHandlerGeneral::onPacketError(%d,%d)", opcode, _getOpcode());
	//普通包接受失败后，暂时不做处理，by zhenhui 2014/7/30
	//GamePrecedure::Get()->requestPackages();
	//MSG_BOX("@ReceivedErrorPackage");
}

class LoginHandle : public PacketHandlerGeneral, public libOSListener
{ 
public: 
		static const int mOpcode = LOGIN_S; 
		LoginHandle() 
		{ 
			PacketManager::Get()->registerPacketHandler(mOpcode,this); 
		} 
		virtual void onMessageboxEnter( int tag)
		{
			if(tag == 102)
			{
				exit(0);
			}
		}
		virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet)
		{
			const HPLoginRet * loginRet = dynamic_cast<const HPLoginRet*>(packet);
#ifdef WIN32
			//记录登录成功后的puid到本地
			if (loginRet)
			{
				std::string sPuidKey = "LastLoginPUID";
				std::string sLoginName = libPlatformManager::getPlatform()->loginUin();
				if (sLoginName.length()>0)
				{
					cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey(sPuidKey.c_str(), sLoginName);	
				}
				
			}
#endif
			/*int SeverGameVersion,SeverAppVersion,SeverResVersion;
			int LocalGameVersion,LocalAppVersion,LocalResVersion;
			std::string versionStr = loginRet->resourceversion();
			if(versionStr!="")
			{
			SeverConsts::Get()->checkVersion(versionStr,SeverGameVersion,SeverAppVersion,SeverResVersion);
			SeverConsts::Get()->checkVersion(SeverConsts::Get()->getVersion(),LocalGameVersion,LocalAppVersion,LocalResVersion);

			bool isOk = true;
			if(LocalGameVersion<SeverGameVersion)
			isOk = false;
			if(LocalGameVersion == SeverGameVersion && LocalAppVersion<SeverAppVersion)
			isOk = false;
			if(LocalGameVersion == SeverGameVersion && LocalAppVersion==SeverAppVersion && LocalResVersion < SeverResVersion)
			isOk = false;
			if(isOk == false)
			{
			libOS::getInstance()->registerListener(this);
			std::string lan = Language::Get()->getString("@LOGIN_VERSION_FAILED");
			libOS::getInstance()->showMessagebox(lan,102);
			return;
			}
			}*/
//			if(loginRet && loginRet->status() == 5)
//			{
////modify by dylan merge Platform loadingFrameSeversConnection differentiation  
//				std::string tips="none";
//#ifdef WIN32
//#else
//				const PlatformRoleItem* item=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
//				if(item)
//				{
//					tips=item->loadingFrameSeversConnection;
//				}
//#endif
//				if(tips=="none")
//				{
//					tips="";
//				}
//				//服务器开服提示信息 修正为serverConfig内的文案
//				std::list<std::string> strList;
//				strList.push_back(tips);
//				CCMessageBox(GameMaths::replaceStringWithStringList(SeverConsts::Get()->getServerInGrayMsg(),&strList).c_str(),Language::Get()->getString("@ShowMsgBoxTitle").c_str());
//				return;
//			}
//			if(loginRet && loginRet->status() == 4)
//			{
//				return;
//			}
			if(loginRet && !GamePrecedure::Get()->isInLoadingScene())
			{
				//静默重启服务器，关闭标示位，打开MessageBox和LoadingAniPage
				ServerDateManager::Get()->setServerWillRestart(false);
				//在非登录页面重连后，同步基础数据信息
				//非登录页面重新连接之后，暂时不重新发包，后期有可能添加，或者服务器推送过来。 by zhenhui 2014/7/30
				//GamePrecedure::Get()->reRequestPackages();
				//
				//

				if ( loginRet->playerid() > 0 )
				{
					if (ServerDateManager::Get()->mLoginInfo.m_iPlayerID > 0)
					{
						//大于0说明不是大退新登，是重新登，重新登就reset
						ServerDateManager::Get()->reset(false);
					}
					ServerDateManager::Get()->mLoginInfo.m_iPlayerID = loginRet->playerid();
				}
				else
				{
					CCAssert(false, "logret->playerid()>0");
					return;
				}
				//
				if (loginRet->has_roleitemid())
				{
					ServerDateManager::Get()->mLoginInfo.m_iRoleItemID = loginRet->roleitemid();
				}
				else
				{
					ServerDateManager::Get()->mLoginInfo.m_iRoleItemID = 0;//允许从MainFrame退回LoadingFrame就需要这样
				}
				if (loginRet->has_timestamp())
				{
					if (loginRet->timestamp() > 0)
					{
						ServerDateManager::Get()->mLoginInfo.m_iTimeStamp = loginRet->timestamp();
						GamePrecedure::Get()->setServerTime(loginRet->timestamp());
						GamePrecedure::Get()->setGotHeartBeatTime();
					}
				}
			}
			
		}
		virtual void onSendPacketFailed(const int opcode)
		{
			//Show a messagebox when server is stoped
			if(SeverConsts::Get()->isServerInUpdatingCode()&& (!GamePrecedure::Get()->hasAlertServerUpdating())&&(GamePrecedure::Get()->isInLoadingScene()))
			{
				//CCMessageBox(Language::Get()->getString("@ServerInUpdateCode").c_str(),"Tip");
				if(libOS::getInstance()->getNetWork()==NotReachable)
				{
					CCMessageBox(Language::Get()->getString("@NoNetWork").c_str(),Language::Get()->getString("@ShowMsgBoxTitle").c_str());
				}
				else
				{
					CCMessageBox(SeverConsts::Get()->getServerInUpdateMsg().c_str(),Language::Get()->getString("@ShowMsgBoxTitle").c_str());
				}
				GamePrecedure::Get()->setAlertServerUpdating(true);
			}
		}
		virtual void onConnectFailed(std::string ip, int port)
		{
			//close this messagebox, LoadingFrame Recevive onConnectFailed Message
			//onSendPacketFailed(1);
		}
private: 
		 virtual int _getOpcode() { return mOpcode;};
}; 
LoginHandle * _handler_loginHandle = new LoginHandle;




class OPHeartbeatHandle : public PacketHandlerGeneral, public libOSListener
{ 
public: 
	static const int mOpcode = HEART_BEAT; 
	std::vector<unsigned int> serverStopTimes;
	OPHeartbeatHandle() 
	{
		PacketManager::Get()->registerPacketHandler(mOpcode,this); 
		serverStopTimes.clear();
	} 
	virtual void onMessageboxEnter(lib91*, int tag)
	{
		if(tag == HEART_BEAT)
		{
			exit(0);
		}
	}
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet)
	{
		const  HPHeartBeat* ret = dynamic_cast<const HPHeartBeat*>(packet);
		if(ret)
		{
			//add by zhaozhen for refresh server time 
			GamePrecedure::Get()->setServerTime(ret->timestamp());
			GamePrecedure::Get()->setGotHeartBeatTime();
			//心跳回包，暂时不做处理，有可能后期会做处理  by zhenhui 2014/7/30
			CCLOG("Got the heart beat");
		}
	}
	virtual void onSendPacketFailed(const int opcode){};
	virtual void onConnectFailed(std::string ip, int port){}
private: 
	virtual int _getOpcode() { return mOpcode;};
}; 
OPHeartbeatHandle * _handler_OPHeartbeatHandle = new OPHeartbeatHandle; 




DEFINE_CREATE_REGISTER_HANDLER(OPErrorMsgHandle, ERROR_CODE)
{
	const HPErrorCode * errorMsg = dynamic_cast<const HPErrorCode*>(packet);
	if (errorMsg == NULL)
	{
		CCLOG("ERROR, error code is null");
		return;
	}
	

	if (errorMsg->has_errmsg())
	{
		//optional int32 errFlag =4 [default = 0];// 0 is ingame's error message popup, 1 is platform's message box
		
		if (errorMsg->has_errflag())
		{
			int errorFlag = errorMsg->errflag();
			if (errorFlag == 1)
			{
				CCMessageBox(errorMsg->errmsg().c_str(),Language::Get()->getString("@ShowMsgBoxTitle").c_str());
			}
		}else{
			MSG_BOX(errorMsg->errmsg());
		}
	}
	else
	{
		//错误码机制，需要已登录就加载ErrMsgTableManager 的配置，并且判断hpCode，如果为登录回包，则需要用别的弹窗弹出。 by zhenhui 2014/7/31

		const ErrMsgItem* errMsgItem=ErrMsgTableManager::Get()->getErrMsgItemByID(errorMsg->errcode());
		std::string _errMsg="";
		if(errMsgItem)
		{
			_errMsg=errMsgItem->msgContent;
		}
		else
		{
			_errMsg=Language::Get()->getString("@ErrorReportCommon")+"["+StringConverter::toString(errorMsg->hpcode())+"|"+StringConverter::toString(errorMsg->errcode())+"]";
		}

		if (errorMsg->has_errflag())
		{
			int errorFlag = errorMsg->errflag();
			if (errorFlag == 1)
			{
				CCMessageBox(_errMsg.c_str(),Language::Get()->getString("@ShowMsgBoxTitle").c_str());
			}
		}else{
			MSG_BOX(_errMsg);
		}

		//表示为登录出的错误码，直接返回登录页面
		if (errorMsg->hpcode() == 1000)
		{
			GamePrecedure::Get()->showLoadingEnter();
		} 
		
		
	}
}
DEFINE_CREATE_REGISTER_HANDLER_END



DEFINE_CREATE_REGISTER_HANDLER(HPRoleCreateRetHandler,ROLE_CREATE_S)
{
	const HPRoleCreateRet * roleRet = dynamic_cast<const HPRoleCreateRet*>(packet);

	if (roleRet)
	{

		if (roleRet->status() == 0 )
		{
			ServerDateManager::Get()->mUserRoleInfo = roleRet->roleinfo();
		}else{
			const ErrMsgItem* errMsgItem=ErrMsgTableManager::Get()->getErrMsgItemByID(roleRet->status());
			std::string _errMsg="";
			if(errMsgItem)
			{
				_errMsg=errMsgItem->msgContent;
			}
			else
			{
				_errMsg=Language::Get()->getString("@ErrorReportCommon")+"["+StringConverter::toString(ROLE_CREATE_S)+"|"+StringConverter::toString(roleRet->status())+"]";
			}

			MSG_BOX(_errMsg);
		}

		//创建角色后，有可能需要服务器推送所有数据信息，之后再进入主页面 by zhenhui 2014/7/31
		/*if (GamePrecedure::Get()->getLoginAssemblyFinished())
		{
			MsgMainFrameCoverHide msg;
			msg.pageName="ChooseRolePage";
			MessageManager::Get()->sendMessage(&msg);

			MsgMainFrameChangePage msg2;
			msg2.pageName = "MainScenePage";
			MessageManager::Get()->sendMessage(&msg2);
		}*/
		
		
	}
}
DEFINE_CREATE_REGISTER_HANDLER_END

DEFINE_CREATE_REGISTER_HANDLER(HPPlayerInfoSyncHandler,PLAYER_INFO_SYNC_S)
{
	const HPPlayerInfoSync * playerInfo = dynamic_cast<const HPPlayerInfoSync*>(packet);

	//暂时playerinfo数量为1， 后期有可能会扩展
	if (playerInfo && playerInfo->playerinfos_size() ==1)
	{
		PlayerInfo singleInfo =   playerInfo->playerinfos(0);
		ServerDateManager::Get()->mUserPlayerInfo = singleInfo;
	}
}
DEFINE_CREATE_REGISTER_HANDLER_END


DEFINE_CREATE_REGISTER_HANDLER(HPRoleInfoSyncHandler,ROLE_INFO_SYNC_S)
{
	const HPRoleInfoSync * roleInfo = dynamic_cast<const HPRoleInfoSync*>(packet);

	//暂时playerinfo数量为1， 后期有可能会扩展
	for (int i=0;i<roleInfo->roleinfos_size();i++)
	{
		RoleInfo oneRole = roleInfo->roleinfos(i);
		if (oneRole.type() == MAIN_ROLE)
		{
			//主角数据
			ServerDateManager::Get()->mUserRoleInfo = oneRole;
		}else{
			//佣兵数据
			if (ServerDateManager::Get()->hasRoleInfoById(oneRole.roleid()))
			{
				ServerDateManager::Get()->mUserRoleInfoMap[oneRole.roleid()] = oneRole;
				MsgMainFrameRefreshPage refMsg;
				refMsg.pageName = "MercenaryUpStepPage";
				MessageManager::Get()->sendMessage(&refMsg);
			} 
			else
			{
				ServerDateManager::Get()->insertRoleInfoInMap(oneRole);
			}
			
		}

	}
}
DEFINE_CREATE_REGISTER_HANDLER_END


DEFINE_CREATE_REGISTER_HANDLER(HPEquipInfoSyncHandler,EQUIP_INFO_SYNC_S)
{
	const HPEquipInfoSync * equipInfos = dynamic_cast<const HPEquipInfoSync*>(packet);

	for (int i=0;i<equipInfos->equipinfos_size();i++)
	{
		EquipInfo oneEquip = equipInfos->equipinfos(i);
		//如果没有数据，直接insert,有的话，直接同步数据，服务器保证传过来的数据是完整的。 by zhenhui 2014/8/3
		if (ServerDateManager::Get()->hasEquipInfoById(oneEquip.id()))
		{
			ServerDateManager::Get()->mUserEquipInfoMap[oneEquip.id()] = oneEquip;
		}else{
			ServerDateManager::Get()->insertEquipInfoInMap(oneEquip);
		}
		
	}
}
DEFINE_CREATE_REGISTER_HANDLER_END



DEFINE_CREATE_REGISTER_HANDLER(HPItemInfoSyncHandler,ITEM_INFO_SYNC_S)
{
	const HPItemInfoSync * itemInfos = dynamic_cast<const HPItemInfoSync*>(packet);

	
	for (int i=0;i<itemInfos->iteminfos_size();i++)
	{
		ItemInfo oneItem = itemInfos->iteminfos(i);
		//如果没有数据，直接insert,有的话，直接同步数据，服务器保证传过来的数据是完整的。 by zhenhui 2014/8/3
		if (ServerDateManager::Get()->hasItemInfoById(oneItem.id()))
		{
			ServerDateManager::Get()->mUserItemInfoMap[oneItem.id()] = oneItem;
		}else{
			ServerDateManager::Get()->insertItemInfoInMap(oneItem);
		}
		
	}
}
DEFINE_CREATE_REGISTER_HANDLER_END


DEFINE_CREATE_REGISTER_HANDLER(HPAssembleFinishHander,ASSEMBLE_FINISH_S)
{
	const HPAssembleFinish * skillInfos = dynamic_cast<const HPAssembleFinish*>(packet);

	GamePrecedure::Get()->setLoginAssemblyFinished(true);
	waitingManager::Get()->registerErrReportHandler();
	LoginPacket::Get()->setEnabled(true);
	//--zhengxin 2014-08-26
	PlayerInfo& pInfo = ServerDateManager::Get()->mUserPlayerInfo;
	RoleInfo& rInfo = ServerDateManager::Get()->mUserRoleInfo;
	if (!rInfo.name().empty())
	{
		//name有效->已登录->退出时向账号服务器刷新
		bool putSvr = false;
		if (VaribleManager::Get()->getSetting("PushSvrID") == "1")
			putSvr = true;

		std::string szServerName = "";
		SeverConsts::SEVERLIST::const_iterator it = SeverConsts::Get()->getSeverList().find(GamePrecedure::Get()->getServerID());
		if(it!=SeverConsts::Get()->getSeverList().end())
		{
			szServerName = it->second->name;
		}

		com4loves::updateServerInfo(GamePrecedure::Get()->getServerID(), szServerName,
			rInfo.name(), pInfo.playerid(), rInfo.level(), 
			pInfo.viplevel(), pInfo.coin(), pInfo.gold(), putSvr);
	}
	//
}
DEFINE_CREATE_REGISTER_HANDLER_END

DEFINE_CREATE_REGISTER_HANDLER(HPSkillInfoSyncHandler,SKILL_INFO_SYNC_S)
{
	const HPSkillInfoSync * skillInfos = dynamic_cast<const HPSkillInfoSync*>(packet);


	for (int i=0;i<skillInfos->skillinfos_size();i++)
	{
		SkillInfo oneSkill = skillInfos->skillinfos(i);
		if (ServerDateManager::Get()->hasSkillInfoById(oneSkill.id()))
		{
			ServerDateManager::Get()->mUserSkillInfoMap[oneSkill.id()] = oneSkill;
		}else{
			ServerDateManager::Get()->insertSkillInfoInMap(oneSkill);
		}
		
	}
}
DEFINE_CREATE_REGISTER_HANDLER_END

DEFINE_CREATE_REGISTER_HANDLER(HPPlayerRewardHandler,PLAYER_AWARD_S)
{
	const HPPlayerReward * playerReward = dynamic_cast<const HPPlayerReward*>(packet);

	if (playerReward->has_flag())
	{
		//预留位，暂时不处理
	}

	if (playerReward->has_rewards())
	{
		RewardInfo reward = playerReward->rewards();
		if (reward.has_gold())
		{
			unsigned int data = reward.gold();
			ServerDateManager::Get()->mUserPlayerInfo.set_gold(data);
		}
		if (reward.has_coin())
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_coin(reward.coin());
		}
		if (reward.has_level())
		{
			//等级是RoleInfo里同步数据，包括主角和佣兵
			unsigned int data = reward.level();
			ServerDateManager::Get()->mUserRoleInfo.set_level(data);

			//佣兵数据
			for (std::map<int,RoleInfo>::iterator it=ServerDateManager::Get()->mUserRoleInfoMap.begin(); it!=ServerDateManager::Get()->mUserRoleInfoMap.end(); ++it)
			{
				it->second.set_level(data);
			}
		}

		if (reward.has_exp())
		{
			unsigned int data = reward.exp();
			ServerDateManager::Get()->mUserRoleInfo.set_exp(data);
		}

		if (reward.has_viplevel())
		{
			unsigned int data = reward.viplevel();
			ServerDateManager::Get()->mUserPlayerInfo.set_viplevel(data);
		}

		if ( reward.has_smeltvalue() )
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_smeltvalue(reward.smeltvalue());
		}
		if (reward.has_contribution())
		{
		}
		if (reward.has_bossfighttimes())
		{
		}
		if ( reward.has_honorvalue() )
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_honorvalue(reward.honorvalue());
		}
		if ( reward.has_reputationvalue() )
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_reputationvalue(reward.reputationvalue());
		}
	}
}
DEFINE_CREATE_REGISTER_HANDLER_END




DEFINE_CREATE_REGISTER_HANDLER(HPConsumeInfoHandler,PLAYER_CONSUME_S)
{
	const HPConsumeInfo * infoData = dynamic_cast<const HPConsumeInfo*>(packet);
	//玩家基本信息，直接同步
	if (infoData->has_attrinfo())
	{
		SyncAttrInfo attInfo = infoData->attrinfo();
		if (attInfo.has_gold())
		{
			unsigned int data = attInfo.gold();
			ServerDateManager::Get()->mUserPlayerInfo.set_gold(data);
		}
		if (attInfo.has_coin())
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_coin(attInfo.coin());
		}
		if (attInfo.has_level())
		{
			//等级是RoleInfo里同步数据，包括主角和佣兵
			unsigned int data = attInfo.level();
			ServerDateManager::Get()->mUserRoleInfo.set_level(data);

			//佣兵数据
			for (std::map<int,RoleInfo>::iterator it=ServerDateManager::Get()->mUserRoleInfoMap.begin(); it!=ServerDateManager::Get()->mUserRoleInfoMap.end(); ++it)
			{
				it->second.set_level(data);
			}
		}

		if (attInfo.has_exp())
		{
			unsigned int data = attInfo.exp();
			ServerDateManager::Get()->mUserRoleInfo.set_exp(data);

		}

		if (attInfo.has_viplevel())
		{
			unsigned int data = attInfo.viplevel();
			ServerDateManager::Get()->mUserPlayerInfo.set_viplevel(data);
		}

		if ( attInfo.has_smeltvalue() )
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_smeltvalue(attInfo.smeltvalue());
		}

		if ( attInfo.has_honorvalue() )
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_honorvalue(attInfo.honorvalue());
		}

		if ( attInfo.has_reputationvalue() )
		{
			ServerDateManager::Get()->mUserPlayerInfo.set_reputationvalue(attInfo.reputationvalue());
		}
	}

	//实体对象移除的推送 
	if (infoData->consumeitem_size()>0)
	{
		for (int i=0;i<infoData->consumeitem_size();i++)
		{
			ConsumeItem itemData = infoData->consumeitem(i);
			if (itemData.type() == CHANGE_ROLE)
			{
				int dataId = itemData.id();
				if (ServerDateManager::Get()->hasRoleInfoById(dataId))
				{
					ServerDateManager::Get()->removeRoleInfoById(dataId);
				}else{
					CCLOG("ERROR: NOT FOUND THE ROLE ID %d IN MAP",dataId);
				}
			}else if (itemData.type() == CHANGE_TOOLS)
			{
				int dataId = itemData.id();
				if (ServerDateManager::Get()->hasItemInfoById(dataId))
				{
					ItemInfo oldItem = ServerDateManager::Get()->mUserItemInfoMap.find(dataId)->second;
					unsigned int  oldCount = oldItem.count();
					unsigned int  costCount = itemData.count();
					unsigned int  newCount = oldCount - costCount;
					if (newCount<0)
					{
						CCLOG("ERROR: ITEM ID %d new count < 0 IN MAP",dataId);
					}else if (newCount ==0)
					{
						ServerDateManager::Get()->removeItemInfoById(dataId);
					}else{//new count >0
						ServerDateManager::Get()->mUserItemInfoMap.find(dataId)->second.set_count(newCount);
					}
					
				}else{
					CCLOG("ERROR: NOT FOUND THE ITEM ID %d IN MAP",dataId);
				}
			}else if (itemData.type() == CHANGE_EQUIP)
			{
				int64_t dataId = itemData.id();
				if (ServerDateManager::Get()->hasEquipInfoById(dataId))
				{
					ServerDateManager::Get()->removeEquipInfoById(dataId);

				}else{
					CCLOG("ERROR: NOT FOUND THE EQUIP ID %d IN MAP",dataId);
				}
			}
		}
	}


}
DEFINE_CREATE_REGISTER_HANDLER_END

DEFINE_CREATE_REGISTER_HANDLER(HPPlayerKickOutHandler, PLAYER_KICKOUT_S)
{
	const HPPlayerKickout * msg = dynamic_cast<const HPPlayerKickout*>(packet);

	if ( msg )
	{
		int msgReason = msg->reason();
		if (msgReason == SERVER_RESTART)
		{
			//静默重启服务器标志位，设为true, add by zhenhui 
			ServerDateManager::Get()->setServerWillRestart(true);
		}
	}
}

DEFINE_CREATE_REGISTER_HANDLER_END

//DEFINE_CREATE_REGISTER_HANDLER(HPPlayerStateSyncHandler, STATE_INFO_SYNC_S)
//{
//	const HPPlayerStateSync * playerState = dynamic_cast<const HPPlayerStateSync*>(packet);
//
//	if ( playerState )
//	{
//		ServerDateManager::Get()->mPlayerState = *playerState;
//	}
//}
//
//DEFINE_CREATE_REGISTER_HANDLER_END