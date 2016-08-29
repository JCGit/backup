#include "GenStrengthenControl.h"
#include "UserObj.h"

#include "PlayerEnforceCfgMaster.h"

CGenStrengthenControl::CGenStrengthenControl(){
	m_uAtkAdd = 0;
	m_uHpAdd = 0;
	m_uDefAdd = 0;
	m_uIsAccept = 0;
	m_uGenType = vmsg::REGULAR_PLAYER_ENFORCE;
}

CGenStrengthenControl::~CGenStrengthenControl(){

}

CGenStrengthenPanel* CGenStrengthenControl::getGenPanel(){
	if(!m_stGenPanel.isInited()){
		m_stGenPanel.init();
	}

	return &m_stGenPanel;
}

CGenStrengthenEffectPanel* CGenStrengthenControl::getGenEffPanel(){
	if(!m_stGenEffPanel.isInited()){
		m_stGenEffPanel.init();
	}

	return &m_stGenEffPanel;
}

void CGenStrengthenControl::sendQueryGenStrengthenRqst(){
	m_stProxy.sendQueryGenStrengthenRqst();
}

void CGenStrengthenControl::sendGenStrengthenRqst(unsigned int uType){
	m_stProxy.sendGenStrengthenRqst(uType);
}

void CGenStrengthenControl::sendGenStrengthenConfirmRqst(unsigned int uIsAccept){
	m_stProxy.sendGenStrengthenConfirmRqst(uIsAccept);
}

void CGenStrengthenControl::doQueryRsp(const vmsg::CSPlayerEnforceMyselfQueryRsp& stRsp){
	P_USER_OBJ->updateGenInfo(stRsp.totalatkadd(), stRsp.totaldefadd(), stRsp.totalhpadd());

	if(m_stGenPanel.isInited() && m_stGenPanel.isOpen()){
		m_stGenPanel.updateUI();
	}
}

void CGenStrengthenControl::doGenStrengthenRsp(const vmsg::CSPlayerEnforceMyselfRsp& stRsp){
	P_USER_OBJ->updateGenInfo(stRsp.totalatkadd(), stRsp.totaldefadd(), stRsp.totalhpadd());

	m_uAtkAdd = stRsp.atkadd();
	m_uDefAdd = stRsp.defadd();
	m_uHpAdd = stRsp.hpadd();

	if(m_stGenEffPanel.isInited() && m_stGenEffPanel.isOpen()){
		m_stGenEffPanel.updateUI();
	}
}

void CGenStrengthenControl::doConfirmRsp(const vmsg::CSPlayerEnforceMyselfConfirmRsp& stRsp){
	P_USER_OBJ->updateGenInfo(stRsp.totalatkadd(), stRsp.totaldefadd(), stRsp.totalhpadd());

	if(m_stGenPanel.isInited() && m_stGenPanel.isOpen()){
		m_stGenPanel.updateUI();
	}
}

unsigned int CGenStrengthenControl::getCostByType(unsigned int uType){
	unsigned int uLv = P_USER_OBJ->getUserInfo()->level;
	const PlayerEnforceCfg* stCfg = PLAYER_ENFORCE_CFG_MASTER->GetCfgByLev(uLv);

	if(stCfg == NULL){
		return 0;
	}

	unsigned int uCost = 0;

	switch (uType)
	{
	case vmsg::REGULAR_PLAYER_ENFORCE:
		uCost = stCfg->regularcost();
		break;
	case vmsg::ADVANCED_PLAYER_ENFORCE:
		uCost = stCfg->advancedcost();
		break;
	case vmsg::MASTER_PLAYER_ENFORCE:
		uCost = stCfg->mastercost();
		break;
	case vmsg::SUPER_PLAYER_ENFOCE:
		uCost = stCfg->supercost();
		break;
	default:
		uCost = 0;
		break;
	}

	return uCost;
}
