#include "ManualControl.h"

#include "UserObj.h"
#include "game/city/CityControl.h"
#include "game/city/CityScreenView.h"
#include "game/Constant.h"
#include "game/notification/NotificationControl.h"

#include "cocos2d.h"

USING_NS_CC;

ManualControl::ManualControl()
{
	
}

ManualControl::~ManualControl()
{
	
}

const vmsg::CSBuyManualInfo& ManualControl::getBuyManualInfo() {
	return m_stManualObj.getBuyManualInfo();
}

void ManualControl::startSyncManual() {
	CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->scheduleSelector(schedule_selector(CityScreenView::manualTimer), P_CITY_CTRL->getCityScreenView(), 1.0f, false);
}

void ManualControl::endSyncManual() {
	CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->unscheduleSelector(schedule_selector(CityScreenView::manualTimer), P_CITY_CTRL->getCityScreenView());
}

void ManualControl::notifyManualNotEnough() {
	const NotificationCfg *pNotificationCfg = NOTIFICATION_CFG_MASTER->getNotificationCfg(ENTRY_NOTHING_LAND, NOTIFY_TAG_MANUAL_NOT_ENOUGH);
	
	if(pNotificationCfg != NULL) {
		unsigned int uRemainManual = P_USER_OBJ->getUserInfo()->remainManual;
		unsigned int uThresholdManual = pNotificationCfg->wordsparams(0);

		if(uRemainManual > 0 && uRemainManual < uThresholdManual) {
			PUSH_NOTIFICATION(ENTRY_NOTHING_LAND, NOTIFY_TAG_MANUAL_NOT_ENOUGH, 1);
		} else {
			PUSH_NOTIFICATION(ENTRY_NOTHING_LAND, NOTIFY_TAG_MANUAL_NOT_ENOUGH, 0);
		}
	}
}

void ManualControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	unsigned int uCmd = stMsg.msg_head().msg_id();

	if(stMsg.has_errmsg()) {
		vmsg::CSCommonErrMsg stErrMsg = stMsg.errmsg();

	} else if(stMsg.has_playerrespond()) {
		vmsg::CSPlayerRespond stPlayerRsp = stMsg.playerrespond();

		if(stPlayerRsp.has_querymanual_rsp()) { // 查询体力
			vmsg::CSPlayerManualNotify stManualNotify;
			stManualNotify.set_remainmanual(stPlayerRsp.querymanual_rsp().remainmanual());
			stManualNotify.set_updateremaintime(stPlayerRsp.querymanual_rsp().updateremaintime());
			P_USER_OBJ->updateManual(stManualNotify);
			m_stManualObj.setBuyManualInfo(stPlayerRsp.querymanual_rsp().manualinfo());
			P_CITY_CTRL->updateManual();
			notifyManualNotEnough();
		} else if(stPlayerRsp.has_buymanual_rsp()) { // 购买体力
			m_stManualObj.setBuyManualInfo(stPlayerRsp.buymanual_rsp());
			P_CITY_CTRL->updateManual();
		} else if(stPlayerRsp.has_querybuymanual_rsp()) { // 查询购买体力
			m_stManualObj.setBuyManualInfo(stPlayerRsp.querybuymanual_rsp());
			P_CITY_CTRL->updateManual();
		} else if(stPlayerRsp.has_manual_notify()) { // 体力通知
			P_USER_OBJ->updateManual(stPlayerRsp.manual_notify());
			P_CITY_CTRL->updateManual();
			notifyManualNotEnough();
		}
	}
}

void ManualControl::sendQueryManualRqst() {
	m_stManualProxy.sendQueryManualRqst();
}

void ManualControl::sendBuyManualRqst() {
	m_stManualProxy.sendBuyManualRqst();
}

void ManualControl::sendQueryBuyManualRqst() {
	m_stManualProxy.sendQueryBuyManualRqst();
}
