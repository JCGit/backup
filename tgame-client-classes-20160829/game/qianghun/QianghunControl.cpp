#include "QianghunControl.h"

#include "CfgMaster/GlobalCfgMaster.h"

#include "game/notification/NotificationControl.h"
#include "game/bag/BagControl.h"

CQianghunControl::CQianghunControl(){

}

CQianghunControl::~CQianghunControl(){

}

CQianghunPanel* CQianghunControl::getQianghunPanel(){
	if(!m_stQianghunPanel.isInited()){
		m_stQianghunPanel.init();
	}

	return &m_stQianghunPanel;
}

void CQianghunControl::sendQiangHunQueryRqst(){
	m_stProxy.sendQiangHunQueryRqst();
}

void CQianghunControl::sendQiangHunLayerQueryRqst(unsigned int layer){
	m_stProxy.sendQiangHunLayerQueryRqst(layer);
}

void CQianghunControl::sendQiangHunFlopRqst(unsigned int pos, unsigned int layer){
	m_stProxy.sendQiangHunFlopRqst(pos, layer);
}

void CQianghunControl::doQianghunInfoRsp(const CSQiangHunInfo& stMsg){
	m_stInfo = stMsg;
	notifyWeaponBook();

	if(m_stQianghunPanel.isInited() && m_stQianghunPanel.isOpen()){
		m_stQianghunPanel.updateUI();
	}
}

void CQianghunControl::doFlopRsp(const CSQiangHunInfo& stMsg){
	m_stInfo = stMsg;
	notifyWeaponBook();

	m_stOpenInfo = stMsg.elmts(stMsg.elmts_size() - 1);		//新翻开的牌信息

	if(m_stQianghunPanel.isInited() && m_stQianghunPanel.isOpen()){
		m_stQianghunPanel.updateCard(m_stOpenInfo.pos());
		m_stQianghunPanel.unLock();
	}

	P_BAG_CTRL->sendQueryBagInfoRqst();
}

bool CQianghunControl::hasFreeTimes() {
	return m_stInfo.elmts_size() < (int)GLOBAL_CFG_MASTER->GetCfg().freeqianghuncnt();
}

void CQianghunControl::notifyWeaponBook() {
	if(hasFreeTimes()) {
		PUSH_NOTIFICATION(ENTRY_WEAPON_BOOK, NOTIFY_TAG_WEAPON_BOOK, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_WEAPON_BOOK, NOTIFY_TAG_WEAPON_BOOK, 0);
	}
}
