#ifndef PlayerControl_h__
#define PlayerControl_h__

#include "vlib/base/CVSingleton.h"

#include "PlayerProxy.h"

class PlayerControl
{
public:
	PlayerControl();
	~PlayerControl();
	
	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg); // ��Ϣ����

	void sendGetMonthCardAwardRqst(); // ��ȡ�¿�����

	void sendGetVIPDailyAwardRqst(); // ��ȡvip�ճ�����

private:
	PlayerProxy m_stPlayerProxy;

};

#define P_PLAYER_CTRL CVLib::CVSingleton<PlayerControl>::Instance()

#endif // PlayerControl_h__