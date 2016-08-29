#include "LotteryControl.h"

#include "CfgMaster/GlobalCfgMaster.h"

#include "game/UserObj.h"
#include "game/Global.h"
#include "game/notification/NotificationControl.h"

LotteryControl::LotteryControl()
:m_bInitialized(false)
,m_pLotteryPanel(NULL)
,m_pLotteryStartPanel(NULL)
,m_pLotteryVIPTipPanel(NULL)
,m_pCoinGetWayPanel(NULL)
,m_pLotteryResultPanel(NULL)
,m_uCoinLotFreeTime(0)
,m_uGoldLotFreeTime(0)
,m_uGoldLotCertainlyCnt(0)
,m_uVIPLotTodayCnt(0)
,m_uVIPLotCertainlyCnt(0)
,m_uCoinLotTotalTime(0)
,m_uGoldLotTotalTime(0)
,m_uLastLotteryType(0)
{
}

LotteryControl::~LotteryControl()
{
	finalize();
}

void LotteryControl::init() {
	if(!m_bInitialized) {
		m_pLotteryPanel = LotteryPanel::create();
		CC_SAFE_RETAIN(m_pLotteryPanel);

		m_pLotteryStartPanel = LotteryStartPanel::create();
		CC_SAFE_RETAIN(m_pLotteryStartPanel);

		m_pLotteryVIPTipPanel = LotteryVIPTipPanel::create();
		CC_SAFE_RETAIN(m_pLotteryVIPTipPanel);

		m_pCoinGetWayPanel = CoinGetWayPanel::create();
		CC_SAFE_RETAIN(m_pCoinGetWayPanel);

		m_pLotteryResultPanel = WeaponLotResultPanel::create();
		CC_SAFE_RETAIN(m_pLotteryResultPanel);

		m_bInitialized = true;
	}
}

void LotteryControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pLotteryPanel);

		CC_SAFE_RELEASE_NULL(m_pLotteryStartPanel);

		CC_SAFE_RELEASE_NULL(m_pLotteryVIPTipPanel);

		CC_SAFE_RELEASE_NULL(m_pCoinGetWayPanel);

		CC_SAFE_RELEASE_NULL(m_pLotteryResultPanel);

		m_bInitialized = false;
	}
}

LotteryPanel* LotteryControl::getLotteryPanel() {
	if(!m_bInitialized) {
		init();
	}

	return m_pLotteryPanel;
}

LotteryStartPanel* LotteryControl::getLotteryStartPanel() {
	if(!m_bInitialized) {
		init();
	}

	return m_pLotteryStartPanel;
}

LotteryVIPTipPanel* LotteryControl::getLotteryVIPTipPanel() {
	if(!m_bInitialized) {
		init();
	}

	return m_pLotteryVIPTipPanel;
}

CoinGetWayPanel* LotteryControl::getCoinGetWayPanel() {
	if(!m_bInitialized) {
		init();
	}

	return m_pCoinGetWayPanel;
}

WeaponLotResultPanel* LotteryControl::getLotteryResultPanel() {
	if(!m_bInitialized) {
		init();
	}

	return m_pLotteryResultPanel;
}

unsigned int LotteryControl::getCoinLotFreeTime() {
	return m_uCoinLotFreeTime;
}

unsigned int LotteryControl::getGoldLotFreeTime() {
	return m_uGoldLotFreeTime;
}

unsigned int LotteryControl::getGoldLotCertainlyCnt() {
	return m_uGoldLotCertainlyCnt;
}

unsigned int LotteryControl::getVIPLotTodayCnt() {
	return m_uVIPLotTodayCnt;
}

unsigned int LotteryControl::getVIPLotCertainlyCnt() {
	return m_uVIPLotCertainlyCnt;
}

bool LotteryControl::hasFreeLottery() {
	unsigned int uSvrTime = P_GLOBAL->getSvrTimeRelative();
	if(m_uCoinLotTotalTime <= uSvrTime || m_uGoldLotTotalTime <= uSvrTime) {
		return true;
	}
	return false;
}

void LotteryControl::notifyFreeLottery() {
	if(hasFreeLottery()) {
		PUSH_NOTIFICATION(ENTRY_LOTTERY, NOTIFY_TAG_FREE_LOTTERY, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_LOTTERY, NOTIFY_TAG_FREE_LOTTERY, 0);
	}
}

void LotteryControl::onLoginCheckTime() {
	sendQueryLotteryRqst();
}

void LotteryControl::onHeartBeat() {
	notifyFreeLottery();
}

void LotteryControl::lottery(unsigned int uLotteryType/* = 0*/) {
	uLotteryType = uLotteryType == 0 ? m_uLastLotteryType : uLotteryType;

	if(m_pLotteryStartPanel != NULL) {
		m_pLotteryStartPanel->lottery(uLotteryType);
	}
}

void LotteryControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	unsigned int uCmd = stMsg.msg_head().msg_id();

	switch(uCmd) {
		case vmsg::CS_CMD_ERR:
			if(stMsg.has_errmsg()) {
				unsigned int uRqstCmd = stMsg.errmsg().rqstcmdid();

			}
			break;
		case vmsg::CS_CMD_PLAYER_LOT_QUERY:
		case vmsg::CS_CMD_PLAYER_LOT:
			if(stMsg.has_playerrespond()) {
				const vmsg::CSPlayerRespond &stPlayerRsp = stMsg.playerrespond();

				if(stPlayerRsp.has_queryweaponlot_rsp()) {
					const vmsg::CSWeaponLotQueryRsp &stQueryLotteryRsp = stPlayerRsp.queryweaponlot_rsp();
					m_uCoinLotFreeTime = stQueryLotteryRsp.normallotremaintime();
					m_uGoldLotFreeTime = stQueryLotteryRsp.advancelotremaintime();
					m_uGoldLotCertainlyCnt = stQueryLotteryRsp.remaingoldcardcnt();
					m_uVIPLotTodayCnt = stQueryLotteryRsp.remainvipawardcnttoday();
					m_uVIPLotCertainlyCnt = stQueryLotteryRsp.remainvipawardcnttotal();

					if(m_pLotteryPanel != NULL) {
						m_pLotteryPanel->updateUI();
					}

					if(m_pLotteryStartPanel != NULL && m_pLotteryStartPanel->isOpen()) {
						m_pLotteryStartPanel->openWithLotteryType(m_uLastLotteryType); // 更新，不会重复打开
					}

					//计算下次免费时间，用于提醒
					const int iSvrTime = P_GLOBAL->getSvrTimeRelative();
					m_uCoinLotTotalTime = iSvrTime + m_uCoinLotFreeTime;
					m_uGoldLotTotalTime = iSvrTime + m_uGoldLotFreeTime;

					notifyFreeLottery();
				} else if(stPlayerRsp.has_weaponlot_rsp()) {
					const vmsg::CSWeaponLotRsp &stLotteryRsp = stPlayerRsp.weaponlot_rsp();

					if(stLotteryRsp.elmtlst_size() == 1) {
						if(m_pLotteryResultPanel != NULL) {
							m_pLotteryResultPanel->showSingleResult(stLotteryRsp);
							sendQueryLotteryRqst();
						}
					} else {
						if(m_pLotteryResultPanel != NULL) {
							m_pLotteryResultPanel->open();
							m_pLotteryResultPanel->showResult(stLotteryRsp);
						}
					}

				}
			}
			break;
		default:;
	}
}

void LotteryControl::sendQueryLotteryRqst() {
	m_stLotteryProxy.sendQueryLotteryRqst();
}

void LotteryControl::sendLotteryRqst(unsigned int uLotteryType) {
	m_uLastLotteryType = uLotteryType;
	m_stLotteryProxy.sendLotteryRqst(uLotteryType);
}
