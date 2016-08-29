
#include "stdafx.h"
#include "waitingManager.h"
#include "libOS.h"
#include "HP.pb.h"
#include "GamePackets.h"
#include "cocos2d.h"
#include "LoginPacket.h"
#include "MessageBoxPage.h"
#include "ThreadSocket.h"
#include "GameMessages.h"
#include "LoadingAniPage.h"
#include "LoadingFrame.h"

waitingManager::waitingManager( void ) :mStillWaitingTime(0)
{
	//�ھջ������࣬ע����Ҫ��ע��opcode�������⴦��

	//��������ƣ���ʱע�͵�������Ҫ�� by zhenhui 2014/7/30
	//PacketManager::Get()->registerPacketHandler(OPCODE_ERROR_RET_S, this);
	registerErrReportHandler();
}

////////////////////////////////////////////////////////
//��ʱ�����ɹ������¼���ErrReport
////////////////////////////////////////////////////////
void waitingManager::registerErrReportHandler()
{
	PacketManager::Get()->registerPacketHandler(ERROR_CODE, this);
}

void waitingManager::endWaiting()
{
	if(GamePrecedure::Get()->isInLoadingScene())
	{			
		LoadingFrame* frame = GamePrecedure::Get()->getLoadingFrame();
		if (frame)
		{
			frame->hidLoadingAniPage();
		}
	}
	HideWaitingPageAni();
}

//GamePacketManager�ڵ�onPostSend��������
void waitingManager::startWaiting(int send_opcode, int opcode_to_hide, float showtime /*= 10.0f*/ )
{
	if(GamePrecedure::Get()->isInLoadingScene())
	{
		//libOS::getInstance()->setWaiting(true);
		LoadingFrame* frame = GamePrecedure::Get()->getLoadingFrame();
		if (frame)
		{
			frame->showLoadingAniPage();
		}
	}
	else
	{
		ShowWaitingPageAni();
	}


    PacketManager::Get()->registerPacketHandler(opcode_to_hide, this);
	if(showtime>mStillWaitingTime)
		mStillWaitingTime = showtime;

	std::map<int,int>::iterator it = mReasons.find(opcode_to_hide);
	if(it == mReasons.end())
	{
		mReasons.insert(std::make_pair(opcode_to_hide,1));
	}
	else
		it->second++;

	std::map<int,int>::iterator itRelation = mWaitCodeRelation.find(send_opcode);
	if(itRelation==mWaitCodeRelation.end())
	{
		mWaitCodeRelation.insert(std::make_pair(send_opcode,opcode_to_hide));
	}

	CCLOG("waitingManager::startWaiting!sendOpcode:%d waitOpcode:%d count:%d time:%f:",send_opcode,opcode_to_hide,mReasons[opcode_to_hide],mStillWaitingTime);
}

//�����Ƿ���ڵȴ�����Ϣ���г�ʱ�Ľ��д���
void waitingManager::update( float dt )
{
	if(mReasons.size() >0)
	{
		mStillWaitingTime-=dt;
		if(mStillWaitingTime<=0)
		{
			CCLOG("waitingManager update Timeout");
			PacketManager::Get()->removePacketHandler(this);
			registerErrReportHandler();

			//TODO::û�жԶ��������ʱ����¼��
			//�����еĴ��ڶ����ʱ�����ĳ������ʱ��������������ű���ʱ

			libOS::getInstance()->setWaiting(false);
			if(GamePrecedure::Get()->isInLoadingScene())
			{			
				LoadingFrame* frame = GamePrecedure::Get()->getLoadingFrame();
				if (frame)
				{
					frame->hidLoadingAniPage();
				}
			}
			HideWaitingPageAni();

			std::map<int,int> reasons = mReasons;
			mReasons.clear();
			std::map<int,int>::iterator itr = reasons.begin();
			while (itr != reasons.end())
			{
				PacketManager::Get()->_boardcastReceiveTimeout(itr->first);
				++itr;
			}

			if (ThreadSocket::Get()->getState() == ThreadSocket::SS_UNINITIALIZED)
			{
				if(libOS::getInstance()->getNetWork()==NotReachable)
				{
					MSG_BOX_LAN("@NoNetWork");
				}
				else
				{
					MSG_BOX_LAN("@SendPacketFailed");
				}
			}
		}
	}
}

void waitingManager::_decreaseReason( int opcode )
{
	std::map<int,int>::iterator it = mReasons.find(opcode);
	if(it != mReasons.end())
	{
		it->second--;
		CCLOG("waitingManager::decreaseReason! opcode:%d count:%d",opcode,it->second);
		if(it->second<=0)
		{
			mReasons.erase(it);
            PacketManager::Get()->removePacketHandler(opcode, this);
		}
	}
	else
		CCLOG("waitingManager::decreaseReason! opcode:%d is not find",opcode);
	if(mReasons.empty())
	{
		//libOS::getInstance()->setWaiting(false);
		libOS::getInstance()->setWaiting(false);
		if(GamePrecedure::Get()->isInLoadingScene())
		{			
			LoadingFrame* frame = GamePrecedure::Get()->getLoadingFrame();
			if (frame)
			{
				frame->hidLoadingAniPage();
			}
		}
		HideWaitingPageAni();
	}

}

bool waitingManager::getWaiting()
{
	return !mReasons.empty();
}


std::string waitingManager::getWaitingOpcode()
{
	std::string waitCode=Language::Get()->getString("@SendPacketWaiting")+"\n";
	std::map<int,int>::iterator it = mReasons.begin();
	for(;it!=mReasons.end();++it)
	{
		if(it->second>0)
		{
			waitCode+=StringConverter::toString(it->first)+"|"+StringConverter::toString(it->second)+",";
		}
	}
	return waitCode;
}


void waitingManager::onReceivePacket( const int opcode, const ::google::protobuf::Message* packet )
{
	_decreaseReason(opcode);
	if(opcode != ERROR_CODE )
		_decreaseReason(opcode);
	else
	{
		
		const HPErrorCode * errMsgRet = dynamic_cast<const HPErrorCode*>(packet);
		unsigned int _decreaseOpcode=getWaitingOpcode(errMsgRet->hpcode());
		CCLOG("waitingManager::onReceivePacket ! opcode:%d _decreaseReason_opcode:%d",opcode,_decreaseOpcode);
		_decreaseReason(_decreaseOpcode);
		
	}
}

void waitingManager::onSendPacketFailed( const int opcode )
{
	_decreaseReason(getWaitingOpcode(opcode));
}

int waitingManager::getWaitingOpcode(const int opcode)
{
	int wait_op_code=opcode+1;
	std::map<int,int>::iterator itRelation = mWaitCodeRelation.find(opcode);
	if(itRelation!=mWaitCodeRelation.end())
	{
		wait_op_code=itRelation->second;
	}
	return wait_op_code;
}
