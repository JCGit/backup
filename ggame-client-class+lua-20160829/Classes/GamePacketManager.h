#pragma once
#include "PacketManager.h"
#include "Singleton.h"

class GamePacketManager:public PacketManagerListener, public Singleton<GamePacketManager>
{
private:
	bool mNeedReConnect;
	unsigned int mReconnectTimes;
	bool		 mShowRelogin;
public:

	GamePacketManager();

	void setNeedReConnect() {mNeedReConnect = true;};

	virtual void onPreSend(int opcode, ::google::protobuf::Message*, bool needWaiting=true);
	virtual void onPreSend(int opcode, char* buff, int length, bool needWaiting = true);
	virtual void onPostSend(int opcode, ::google::protobuf::Message*, bool needWaiting=true, int targetOpcode = 0);
	virtual void onPostSend(int opcode, char* buff, int length, bool needWaiting = true);

	virtual void onBoardcastPacketToHandler( int id, const ::google::protobuf::Message* msg, const std::string& msgStr=0);
	virtual void onBoardcastConnectionFailed(std::string ip, int port);
	virtual void onBoardcastSendFailed(int opcode);
	virtual void onBoardcastReceiveFailed();
	virtual void onBoardcastReceiveTimeout(int opcode);
	virtual void onBoardcastPacketError(int opcode, const std::string &errmsg);
};

