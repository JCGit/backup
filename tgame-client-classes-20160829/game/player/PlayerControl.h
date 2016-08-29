#ifndef PlayerControl_h__
#define PlayerControl_h__

#include "vlib/base/CVSingleton.h"

#include "PlayerProxy.h"

class PlayerControl
{
public:
	PlayerControl();
	~PlayerControl();
	
	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg); // 消息处理

	void sendGetMonthCardAwardRqst(); // 领取月卡奖励

	void sendGetVIPDailyAwardRqst(); // 领取vip日常奖励

private:
	PlayerProxy m_stPlayerProxy;

};

#define P_PLAYER_CTRL CVLib::CVSingleton<PlayerControl>::Instance()

#endif // PlayerControl_h__