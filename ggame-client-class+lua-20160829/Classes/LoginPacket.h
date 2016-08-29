#pragma once
#include "Singleton.h"

#define LoginPacketWaitingTime 3

#define LoginVersion 1

class LoginPacket
	:	public Singleton<LoginPacket>
{
protected:
	friend class Singleton<LoginPacket>;
	LoginPacket(void);
public:
	void update(float dt);

	void sendPacket();

	void forceSentPacket();
	
	void setEnabled(bool enable) { mEnable = enable;mCanSend = true;mWaitingTime = 0; }

	bool canSendLoginPacket(){ return mCanSend;}
private:
	void _sendPacket();
	bool mCanSend;
	float mWaitingTime;
	bool mEnable;
};

