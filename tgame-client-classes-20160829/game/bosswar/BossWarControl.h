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
	void sendDelegateRqst(unsigned int uDelegate);		//0标示取消，1标示托管
	void sendAttendOrLeaveRqst(unsigned int uLeave);	//0标示离开，1标示加入
	void sendReliveRqst();								//复活

	void onRecvMsg(VLogicSvr2CliMsg &stMsg);			//协议回包处理

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

	CSBossChallengeInfo m_stChallengeInfo;		//开始挑战信息
	CSBossChallengeStateNotification m_stStateNotify;		//boss战状态推送

	unsigned int m_uBossLv;

};

#define P_BOSS_WAR_CTRL CVLib::CVSingleton<CBossWarControl>::Instance()

#endif // BossWarControl_h__
