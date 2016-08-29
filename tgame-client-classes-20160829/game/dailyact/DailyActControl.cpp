#include "DailyActControl.h"
#include "TextHelper.h"
#include "notification/NotificationControl.h"

#include "game/notification/NotificationControl.h"

DailyActControl::DailyActControl()
{
    m_pGuardActScene = NULL;
}

DailyActControl::~DailyActControl()
{
}

DailyActProxy *DailyActControl::getProxy() {
    return &m_stDailyActProxy;
}


DailyActPanel* DailyActControl::getDailyActPanel() {
    if( !(m_stDailyActPanel.isInited()) ) {
        m_stDailyActPanel.init();
    }

    return &m_stDailyActPanel;
}

bool DailyActControl::isCanGetManual() {
	bool bRet = false;

	for(int i = 0; i < m_stActInfo.actinfos_size(); i++) {
		const vmsg::CSActInfo &stActInfo = m_stActInfo.actinfos(i);

		if(stActInfo.actid() == ACT_ID_MANUAL) {
			bRet = stActInfo.actstat() == ACT_STAT_START; // start状态可领取
			break;
		}
	}

	return bRet;
}

void DailyActControl::notifyGetManual() {
	if(isCanGetManual()) {
		PUSH_NOTIFICATION(ENTRY_DAILY_ACT, NOTIFY_TAG_GET_MANUAL, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_DAILY_ACT, NOTIFY_TAG_GET_MANUAL, 0);
	}
}

void DailyActControl::doDailyActQueryRsp(const CSActQueryRsp& stMsg){
	m_stActInfo = stMsg;

	if(m_stDailyActPanel.isInited() && m_stDailyActPanel.isOpen()){
		m_stDailyActPanel.updateUI();
	}

	notifyDailyAct();
}

void DailyActControl::doDailyActNotify(const CSActStatChange& stInfo){
	//更新
	for(int i = 0; i < m_stActInfo.actinfos_size(); i++){
		if(m_stActInfo.actinfos(i).actid() == stInfo.actid()){
			m_stActInfo.mutable_actinfos(i)->set_actstat(stInfo.actstat());
		}
	}

	if(m_stDailyActPanel.isInited() && m_stDailyActPanel.isOpen()){
		m_stDailyActPanel.updateOneItem(stInfo);
	}

	notifyDailyAct();
}

void DailyActControl::doGetManualRsp(){
	CSActStatChange stInfo;
	stInfo.set_actid(ACT_ID_MANUAL);
	stInfo.set_actstat(ACT_STAT_PARTED);
	PUSH_NOTIFICATION(ENTRY_DAILY_ACT, NOTIFY_TAG_GET_MANUAL, 0); // 已领取体力

	if(m_stDailyActPanel.isInited() && m_stDailyActPanel.isOpen()){
		m_stDailyActPanel.updateOneItem(stInfo);
	}
}

void DailyActControl::notifyDailyAct(){
	if(hasDailyActStart()){
		PUSH_NOTIFICATION(ENTRY_DAILY_ACT, NOTIFY_TAG_DAILY_ACT, 1);
	}else{
		PUSH_NOTIFICATION(ENTRY_DAILY_ACT, NOTIFY_TAG_DAILY_ACT, 0);
	}
}

bool DailyActControl::hasDailyActStart(){
	CSActInfo stInfo;
	for(int i = 0; i < m_stActInfo.actinfos_size(); i++){
		stInfo = m_stActInfo.actinfos(i);
		if(stInfo.actid() == ACT_ID_MANUAL && stInfo.actstat() == ACT_STAT_START){
			return true;
		}

		if(stInfo.actid() == ACT_ID_CVZ || stInfo.actid() == ACT_ID_WORLD_BOSS){
			if(stInfo.actstat() == ACT_STAT_PREPARE || stInfo.actstat() == ACT_STAT_START){
				return true;
			}
		}

	}
	return false;
}

