
#include "stdafx.h"

#include "LoginPacket.h"
#include "PacketManager.h"
#include "GamePackets.h"
#include "lib91.h"
#include "libOS.h"
//#include "ThreadSocket.h"
#include "Base64.h"
#include "md5.h"
#include "DataTableManager.h"
#include "StringConverter.h"
#include "waitingManager.h"
#include "GamePrecedure.h"
#include "SeverConsts.h"
#include "MessageBoxPage.h"

LoginPacket::LoginPacket( void )
	: mCanSend(true)
	, mWaitingTime(LoginPacketWaitingTime)
{

}

void LoginPacket::update( float dt )
{
	if(mCanSend)
	{
		return;
	}
	if(mWaitingTime <= 0 || !waitingManager::Get()->getWaiting())
	{
		mWaitingTime=0;
		mCanSend = true;
	}
	else
	{
		mWaitingTime -= dt;
	}
}

void LoginPacket::sendPacket()
{
	CCLOG("LoginPacket::sendPacket");
	if(mCanSend) 
	{
		_sendPacket();
		mWaitingTime = LoginPacketWaitingTime;
		mCanSend = false;
	}
}

void LoginPacket::forceSentPacket()
{
	CCLOG("LoginPacket::forceSentPacket");
	mEnable = true;
	_sendPacket();
}

void LoginPacket::_sendPacket()
{
	if(!mEnable)
		return;
	CCLOG("LoginPacket::_sendPacket->PacketManager::reconnect");
	//MSG_BOX_LAN("@ReconnectGameServer");
	PacketManager::Get()->reconnect();
	//modify by dylan at 20131112 login interface Base64 And MD5
	HPLogin loginPack;
	std::string uin = libPlatformManager::getPlatform()->loginUin();
#ifdef PROJECT91Quasi
		uin=GamePrecedure::Get()->getLoginUin();
#endif
	if(uin.empty()) uin="none";

	std::string platformInfo = libPlatformManager::getPlatform()->getPlatformInfo()+"#"+StringConverter::toString(libPlatformManager::getPlatform()->getPlatformId());
	std::string deviceID = libOS::getInstance()->getDeviceID();

	if(uin.empty()) uin="none";
	if(deviceID.empty()) deviceID="none";
	if(!platformInfo.empty())
		loginPack.set_platform(platformInfo);

	loginPack.set_puid(uin);
	loginPack.set_deviceid(deviceID);

	loginPack.set_version(SeverConsts::Get()->getVersion());

	PacketManager::Get()->sendPakcet(LOGIN_C,&loginPack);
}
