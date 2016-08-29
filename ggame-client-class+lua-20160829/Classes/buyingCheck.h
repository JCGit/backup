#pragma once
#include "lib91.h"
#include "Singleton.h"
#include "GamePackets.h"

class buyingCheck 
	: public Singleton<buyingCheck>
	, public platformListener
	, public PacketHandler
{
private:
	buyingCheck(void);
	~buyingCheck(void);
	friend class Singleton<buyingCheck>;
public:
	void init();
	void check();

	virtual void onBuyinfoSent(libPlatform*, bool success, const BUYINFO&, const std::string& log);
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet);
	virtual void onSendPacketFailed(const int opcode){};
	virtual void onConnectFailed(std::string ip, int port){};
	virtual void onTimeout(const int opcode){};
	virtual void onPacketError(const int opcode){};
};

