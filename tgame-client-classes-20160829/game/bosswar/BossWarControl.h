#ifndef BossWarControl_h__
#define BossWarControl_h__

#include "CSBossChallengeMsg.pb.h"
#include "BossWarProxy.h"
#include "BossWarFightScene.h"
#include "BossWarQuitPanel.h"
#include "BossWarOverPanel.h"

using namespace vmsg;

class CBossWarControl
{
public:
	CBossWarControl();
	~CBossWarControl();

	void setBossWarScene(CBossWarFightScene *scene){
		m_pBossWarScene = scene;
	}

	CBossWarFightScene* getBossWarScene(){
		return m_pBossWarScene;
	}

	unsigned int getBossLv(){
		return m_uBossLv;
	}

	void sendQueryBossChallengeRqst();
	void sendDamageReportRqst(CSBossChallengeBigDamageReport& stRqst);
	void sendDelegateRqst(unsigned int uDelegate);		//0��ʾȡ����1��ʾ�й�
	void sendAttendOrLeaveRqst(unsigned int uLeave);	//0��ʾ�뿪��1��ʾ����
	void sendReliveRqst();								//����

	void onRecvMsg(VLogicSvr2CliMsg &stMsg);			//Э��ذ�����

	CBossWarQuitPanel* getQuitPanel();

	CBossWarOverPanel* getOverPanel();

	void doQueryBossChallenge(const CSBossChallengeInfo& stRsp);

	const CSBossChallengeInfo& getBossChallengeInfo(){
		return m_stChallengeInfo;
	}

	void doStateNotify(const CSBossChallengeStateNotification& stRsp);

	const CSBossChallengeStateNotification& getStateNotify(){
		return m_stStateNotify;
	}

	void doAttendOrLeave(unsigned int uMark);

	void doDelegateRsp(unsigned int uMark);


protected:
private:
	CBossWarQuitPanel m_stQuitPanel;
	CBossWarOverPanel m_stOverPanel;

	CBossWarProxy m_stProxy;
	CBossWarFightScene* m_pBossWarScene;

	CSBossChallengeInfo m_stChallengeInfo;		//��ʼ��ս��Ϣ
	CSBossChallengeStateNotification m_stStateNotify;		//bossս״̬����

	unsigned int m_uBossLv;

};

#define P_BOSS_WAR_CTRL CVLib::CVSingleton<CBossWarControl>::Instance()

#endif // BossWarControl_h__
