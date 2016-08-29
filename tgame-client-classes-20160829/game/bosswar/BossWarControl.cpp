#include "BossWarControl.h"
#include "SceneManager.h"
#include "UserObj.h"
#include "dailyact/DailyActControl.h"

CBossWarControl::CBossWarControl(){
	m_uBossLv = 1;
	m_pBossWarScene = NULL;
}

CBossWarControl::~CBossWarControl(){

}


void CBossWarControl::sendQueryBossChallengeRqst(){
	m_stProxy.sendQueryBossChallengeRqst();
}

void CBossWarControl::sendDamageReportRqst(vmsg::CSBossChallengeBigDamageReport& stRqst){
	m_stProxy.sendDamageReportRqst(stRqst);
}

void CBossWarControl::sendAttendOrLeaveRqst(unsigned int uLeave){
	m_stProxy.sendAttendOrLeaveRqst(uLeave);
}

void CBossWarControl::sendDelegateRqst(unsigned int uDelegate){
	m_stProxy.sendDelegateRqst(uDelegate);
}

void CBossWarControl::sendReliveRqst(){
	m_stProxy.sendReliveRqst();
}

void CBossWarControl::doQueryBossChallenge(const CSBossChallengeInfo& stRsp){
	m_stChallengeInfo = stRsp;
	m_uBossLv = stRsp.bosslevel();

	SceneManager::sharedSceneManager()->enterScene(CBossWarFightScene::WORLD_BOSS_SCENE_TYPE);
}

void CBossWarControl::doDelegateRsp(unsigned int uMark){
	P_DAILYACT_CTRL->getDailyActPanel()->updateWorldBossActItem(uMark);
}

void CBossWarControl::doAttendOrLeave(unsigned int uMark){
	if(uMark == 1){   //����
		sendQueryBossChallengeRqst();
	}else if(uMark == 0){   //�뿪
		getOverPanel()->close();
		getQuitPanel()->close();

		if(m_pBossWarScene){
			m_pBossWarScene->doWarOver();
		}
		m_pBossWarScene = NULL;		//�ÿ�
		SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
	}
}

void CBossWarControl::doStateNotify(const CSBossChallengeStateNotification& stRsp){
	m_stStateNotify = stRsp;
	if(stRsp.state() != BOSS_CHALLENGE_OVER){  //��������״̬������������
		return;
	}

	getOverPanel()->open();
	getOverPanel()->updateUI();

	if(m_pBossWarScene){
		m_pBossWarScene->doWarOver();
	}
}


void CBossWarControl::onRecvMsg(VLogicSvr2CliMsg &stMsg){
	if(stMsg.has_bosschallengersp()){
		const CSBossChallengeRsp& stRsp = stMsg.bosschallengersp();
		if(stRsp.has_attendorleave()){  //���� OR �뿪�Э�鷵��
			doAttendOrLeave(stRsp.attendorleave());
			return;
		}else if(stRsp.has_delegatetype()){  //�й�Э�鷵��
			doDelegateRsp(stRsp.delegatetype());
			return;
		}else if(stRsp.has_challengeinfo()){  //��ѯbossս��Ϣ
			doQueryBossChallenge(stRsp.challengeinfo());
			return;
		}
	}

	if(m_pBossWarScene == NULL){
		return;
	}

	switch(stMsg.msg_head().msg_id()) {
	case CS_CMD_BOSS_CHALLENGE_RELIVE:				//����
		m_pBossWarScene->onBuyReviveCallBack();
		return;
	}

	if(!stMsg.has_bosschallengersp()){
		return;
	}

	const CSBossChallengeRsp& rsp = stMsg.bosschallengersp();
	if(rsp.has_bosschallengestatenotify()){	//bossվ״̬ͬ��
		doStateNotify(rsp.bosschallengestatenotify());
	}else if(rsp.has_damages()){	//�˺�ͬ��
		m_pBossWarScene->doDamageNotify(rsp.damages());
	}
}

CBossWarQuitPanel* CBossWarControl::getQuitPanel(){
	if(!m_stQuitPanel.isInited()){
		m_stQuitPanel.init();
	}

	return &m_stQuitPanel;
}

CBossWarOverPanel* CBossWarControl::getOverPanel(){
	if(!m_stOverPanel.isInited()){
		m_stOverPanel.init();
	}

	return &m_stOverPanel;
}

