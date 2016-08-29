#include "EvolutionControl.h"

#include "CfgMaster/EvolutionSkillCfgMaster.h"
#include "CfgMaster/GlobalCfgMaster.h"

#include "game/UserObj.h"
#include "game/bag/BagControl.h"
#include "game/login/LoginControl.h"
#include "game/notification/NotificationControl.h"

EvolutionControl::EvolutionControl()
:m_pEvolutionPanel(NULL)
{
}

EvolutionControl::~EvolutionControl()
{
    finalizePanel();
}

void EvolutionControl::openEvolutionPanel() {
	getEvolutionPanel()->open();
	sendQueryEvolutionInfoRqst();
}

bool EvolutionControl::isEvolutionPanelOpen() {
    if(m_pEvolutionPanel == NULL) {
        return false;
    }
    return getEvolutionPanel()->isOpen();
}
EvolutionPanel* EvolutionControl::getEvolutionPanel() {
	if(m_pEvolutionPanel == NULL) {
		m_pEvolutionPanel = EvolutionPanel::create();
		m_pEvolutionPanel->retain();
	}

	return m_pEvolutionPanel;
}

void EvolutionControl::evolve() {
	getEvolutionPanel()->evolve();
}

unsigned int EvolutionControl::getEvolutionID() {
	return m_stEvolutionObj.getEvolutionID();
}

const std::vector<vmsg::CSEvolutionSkill>& EvolutionControl::getSkills() {
	return m_stEvolutionObj.getSkills();
}

unsigned int EvolutionControl::getUsedSkillIdx() {
	return m_stEvolutionObj.getUsedSkillIdx();
}

void EvolutionControl::notifyEvolution() {
	unsigned int uEvolutionID = P_EVOLUTION_CTRL->getEvolutionID();

	EvolutionSkillCfg *pNextEvolutionSkillCfg = EVOLUTION_SKILL_CFG_MASTER->getEvolutionSkillCfgByID(uEvolutionID + 1);

	if(pNextEvolutionSkillCfg != NULL) {
		unsigned int uNeedLevel  = pNextEvolutionSkillCfg->needlevel();
		unsigned int uNeedCoin  = pNextEvolutionSkillCfg->needcoin();
		unsigned int uNeedStoneCnt  = pNextEvolutionSkillCfg->needpowerstone();
		unsigned int uLevel = P_USER_OBJ->getUserInfo()->level;
		unsigned int uCoin = P_USER_OBJ->getUserInfo()->coin;
		unsigned int uStoneCnt = P_BAG_CTRL->getItemCnt(GLOBAL_CFG_MASTER->GetCfg().powerstoneid());

		if(uLevel >= uNeedLevel && uCoin >= uNeedCoin && uStoneCnt >= uNeedStoneCnt) {
			PUSH_NOTIFICATION(ENTRY_EVOLUTION, NOTIFY_TAG_EVOLUTION, 1);
		} else {
			PUSH_NOTIFICATION(ENTRY_EVOLUTION, NOTIFY_TAG_EVOLUTION, 0);
		}
	}

}

void EvolutionControl::onRecvMsg(vmsg::VLogicSvr2CliMsg& stMsg) {
	if(stMsg.has_errmsg()) {

	} else if(stMsg.has_playerrespond()) {
		vmsg::CSPlayerRespond stPlayerRsp = stMsg.playerrespond();

		if(stPlayerRsp.has_evolution_rsp()) {
			P_BAG_CTRL->sendQueryBagInfoRqst();
			m_stEvolutionObj.setEvolutionID(stPlayerRsp.evolution_rsp().evolutionid());
			
			if(m_pEvolutionPanel != NULL && m_pEvolutionPanel->isOpen()) {
				m_pEvolutionPanel->updateUI();
			}

			if(stPlayerRsp.evolution_rsp().evolutionid() % 16 == 0) { // 获得一个技能
				sendQueryEvolutionSkillInfoRqst();
			}

			notifyEvolution();
			P_LOGIN_CTRL->getProxy()->sendQueryPlayerDetailInfoRqst();
		} else if(stPlayerRsp.has_changeevolutionskill_rsp()) {
			m_stEvolutionObj.setUsedSkillIdx(stPlayerRsp.changeevolutionskill_rsp().useskillidx());

			if(m_stEvolutionObj.getSkills().size() > 0) {
				vmsg::CSEvolutionSkill stSkill = m_stEvolutionObj.mutableSkills().at(m_stEvolutionObj.getUsedSkillIdx());
				
				if(m_pEvolutionPanel != NULL && m_pEvolutionPanel->isOpen()) {
					m_pEvolutionPanel->updateUI();
				}
			}
		} else if(stPlayerRsp.has_queryevolution_rsp()) {
			m_stEvolutionObj.setEvolutionID(stPlayerRsp.queryevolution_rsp().evolutionid());
			
			if(m_pEvolutionPanel != NULL && m_pEvolutionPanel->isOpen()) {
				m_pEvolutionPanel->updateUI();
			}

			notifyEvolution();
		} else if(stPlayerRsp.has_queryevolutionskill_rsp()) {
			const google::protobuf::RepeatedPtrField<vmsg::CSEvolutionSkill> &stSkills = stPlayerRsp.queryevolutionskill_rsp().skills();
			google::protobuf::RepeatedPtrField<vmsg::CSEvolutionSkill>::const_iterator iter;

			m_stEvolutionObj.mutableSkills().clear();

			for(iter = stSkills.begin(); iter != stSkills.end(); iter++) {
				m_stEvolutionObj.mutableSkills().push_back(*iter);
			}

			m_stEvolutionObj.setUsedSkillIdx(stPlayerRsp.queryevolutionskill_rsp().useskillidx());

			if(m_stEvolutionObj.getSkills().size() > 0) {
				vmsg::CSEvolutionSkill stSkill = m_stEvolutionObj.mutableSkills().at(m_stEvolutionObj.getUsedSkillIdx());
				
				if(m_pEvolutionPanel != NULL && m_pEvolutionPanel->isOpen()) {
					m_pEvolutionPanel->updateUI();
				}
			}
		}
	}
}

void EvolutionControl::sendEvolutionSkillRqst() {
	m_stEvolutionProxy.sendEvolutionSkillRqst();
}

void EvolutionControl::sendChangeEvolutionSkillRqst(unsigned int uSkillID) {
	m_stEvolutionProxy.sendChangeEvolutionSkillRqst(uSkillID);
}

void EvolutionControl::sendQueryEvolutionInfoRqst() {
	m_stEvolutionProxy.sendQueryEvolutionInfoRqst();
}

void EvolutionControl::sendQueryEvolutionSkillInfoRqst() {
	m_stEvolutionProxy.sendQueryEvolutionSkillInfoRqst();
}


void EvolutionControl::finalizePanel() {
    CC_SAFE_RELEASE_NULL(m_pEvolutionPanel);
}
