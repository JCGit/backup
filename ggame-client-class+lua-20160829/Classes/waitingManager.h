#pragma once
#include "Singleton.h"
#include "PacketManager.h"

class waitingManager
	:	public Singleton<waitingManager>
	,	public PacketHandler
{
protected:
	float mStillWaitingTime;
	std::map<int,int> mReasons;
	std::map<int,int> mWaitCodeRelation;//发送的opcode和等待的opcode关联表,300左右的int关联包,性能内存也ok吧!
	friend class Singleton<waitingManager>;
	waitingManager(void);;
	void _decreaseReason(int opcode);
public:
	void update(float dt);
	bool getWaiting();
	std::string getWaitingOpcode();
	void startWaiting(int send_opcode,int opcode_to_hide, float showtime = 10.0f);
	void endWaiting();
	void registerErrReportHandler();
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet);
	virtual void onSendPacketFailed(const int opcode);
	virtual void onConnectFailed(std::string ip, int port){};
	virtual void onTimeout(const int opcode){};
	virtual void onPacketError(const int opcode){};
	int getWaitingOpcode(const int opcode);
	virtual Handler_Type getHandleType(void) {return Double_Handler;};
	void clearReasones(){mReasons.clear();}
};

