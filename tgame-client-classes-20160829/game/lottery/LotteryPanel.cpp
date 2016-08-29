#include "LotteryPanel.h"

#include "defines.h"

#include "CfgMaster/GlobalCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/TimeEx.h"

#include "game/UserObj.h"
#include "game/bag/BagControl.h"

#include "LotteryControl.h"

LotteryPanel::LotteryPanel()
:m_pCloseBtn(NULL)
,m_pCoinLotteryLayout(NULL)
,m_pCoinFreeImg(NULL)
,m_pCoinFreeTimeTxt(NULL)
,m_pCoinFreeTxt(NULL)
,m_pIconCoinImg(NULL)
,m_pCoinValTxt(NULL)
,m_pGoldLotteryLayout(NULL)
,m_pGoldFreeImg(NULL)
,m_pGoldFreeTimeTxt(NULL)
,m_pGoldFreeTxt(NULL)
,m_pIconGoldImg(NULL)
,m_pGoldValTxt(NULL)
,m_pVIPLotteryLayout(NULL)
,m_pTodayTimesValTxt(NULL)
,m_pVIPLevelValTxt(NULL)
,m_uCoinLotFreeTime(0)
,m_uGoldLotFreeTime(0)
{
}

LotteryPanel::~LotteryPanel()
{
}

LotteryPanel* LotteryPanel::create() {
	LotteryPanel *pRet = new LotteryPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LotteryPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setShowMoneyTopBar(true);

		bRet = true;
	} while(0);

	return bRet;
}

void LotteryPanel::open() {
	BasePanel::open();

	showLoading();

	P_LOTTERY_CTRL->sendQueryLotteryRqst();

	if(!m_bUIInited) {
		initUI();
	}

}

void LotteryPanel::close() {
	BasePanel::close();

	unschedule(schedule_selector(LotteryPanel::updateTime));

	P_BAG_CTRL->sendQueryBagInfoRqst();
}

void LotteryPanel::updateUI() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	m_uCoinLotFreeTime = P_LOTTERY_CTRL->getCoinLotFreeTime();
	m_uGoldLotFreeTime = P_LOTTERY_CTRL->getGoldLotFreeTime();

	const GlobalCfg &stGlobalCfg = GLOBAL_CFG_MASTER->GetCfg();
	unsigned int uCoinLotCostVal = stGlobalCfg.weaponlotnormalcoin();
	unsigned int uGoldLotCostVal = stGlobalCfg.weaponlotadvancegold();

	m_pCoinValTxt->setText(intToString(uCoinLotCostVal));
	m_pGoldValTxt->setText(intToString(uGoldLotCostVal));

	updateTimeTxt();

	if(m_uCoinLotFreeTime > 0 || m_uGoldLotFreeTime > 0) {
		unschedule(schedule_selector(LotteryPanel::updateTime));
		schedule(schedule_selector(LotteryPanel::updateTime), 1.0f);
	}

	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
	unsigned int vipLv = pUserInfo->vipLv;

	unsigned int uVIPLotTodayCnt = P_LOTTERY_CTRL->getVIPLotTodayCnt();
	m_pTodayTimesValTxt->setText(intToString(uVIPLotTodayCnt));
	m_pVIPLevelValTxt->setText(intToString(vipLv));

	GUIDE_SHOW(GUIDE_UI_LOTTERY);
	closeLoading();
}

void LotteryPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/LotteryPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(LotteryPanel::touchEventHandler));

	m_pCoinLotteryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("coin_lottery_layout"));
	m_pCoinLotteryLayout->addTouchEventListener(this, toucheventselector(LotteryPanel::touchEventHandler));

	m_pCoinFreeImg = dynamic_cast<UIImageView*>(m_pCoinLotteryLayout->getChildByName("text_coin_free_img"));

	m_pCoinFreeTimeTxt = dynamic_cast<UILabel*>(m_pCoinLotteryLayout->getChildByName("coin_free_time_txt"));

	m_pCoinFreeTxt = dynamic_cast<UILabel*>(m_pCoinLotteryLayout->getChildByName("coin_free_txt"));

	m_pIconCoinImg = dynamic_cast<UIImageView*>(m_pCoinLotteryLayout->getChildByName("icon_coin_img"));

	m_pCoinValTxt = dynamic_cast<UILabel*>(m_pCoinLotteryLayout->getChildByName("coin_val_txt"));

	m_pGoldLotteryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("gold_lottery_layout"));
	m_pGoldLotteryLayout->addTouchEventListener(this, toucheventselector(LotteryPanel::touchEventHandler));

	m_pGoldFreeImg = dynamic_cast<UIImageView*>(m_pGoldLotteryLayout->getChildByName("text_gold_free_img"));

	m_pGoldFreeTimeTxt = dynamic_cast<UILabel*>(m_pGoldLotteryLayout->getChildByName("gold_free_time_txt"));

	m_pGoldFreeTxt = dynamic_cast<UILabel*>(m_pGoldLotteryLayout->getChildByName("gold_free_txt"));

	m_pIconGoldImg = dynamic_cast<UIImageView*>(m_pGoldLotteryLayout->getChildByName("icon_gold_img"));

	m_pGoldValTxt = dynamic_cast<UILabel*>(m_pGoldLotteryLayout->getChildByName("gold_val_txt"));

	m_pVIPLotteryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("vip_lottery_layout"));
	m_pVIPLotteryLayout->addTouchEventListener(this, toucheventselector(LotteryPanel::touchEventHandler));

	m_pTodayTimesValTxt = dynamic_cast<UILabel*>(m_pVIPLotteryLayout->getChildByName("today_times_val_txt"));

	m_pVIPLevelValTxt = dynamic_cast<UILabel*>(m_pVIPLotteryLayout->getChildByName("vip_level_val_txt"));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void LotteryPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				GUIDE_TOUCH(m_pCloseBtn);
				close();
			} else if(strcmp(pszWidgetName, m_pCoinLotteryLayout->getName()) == 0) {
				if(m_uCoinLotFreeTime == 0) { // 免费
					P_LOTTERY_CTRL->sendLotteryRqst(vmsg::CS_LOTTERY_TYPE_NORMAL);
				} else {
					P_LOTTERY_CTRL->getLotteryStartPanel()->openWithLotteryType(vmsg::CS_LOTTERY_TYPE_NORMAL);
				}
				
			} else if(strcmp(pszWidgetName, m_pGoldLotteryLayout->getName()) == 0) {
				if(m_uGoldLotFreeTime == 0) {
					GUIDE_TOUCH(m_pGoldLotteryLayout);
					P_LOTTERY_CTRL->sendLotteryRqst(vmsg::CS_LOTTERY_TYPE_ADVANCE);
				} else {
					P_LOTTERY_CTRL->getLotteryStartPanel()->openWithLotteryType(vmsg::CS_LOTTERY_TYPE_ADVANCE);
				}
			} else if(strcmp(pszWidgetName, m_pVIPLotteryLayout->getName()) == 0) {
				P_LOTTERY_CTRL->getLotteryStartPanel()->openWithLotteryType(vmsg::CS_LOTTERY_TYPE_VIP);
			}
			break;
		default:;
	}
}

void LotteryPanel::updateTimeTxt() {
	if (m_uCoinLotFreeTime == 0) {
		m_pCoinFreeImg->setVisible(true);
		m_pCoinFreeTimeTxt->setVisible(false);
		m_pCoinFreeTxt->setVisible(false);
		m_pIconCoinImg->setVisible(false);
		m_pCoinValTxt->setVisible(false);
	} else {
		m_pCoinFreeImg->setVisible(false);
		m_pCoinFreeTimeTxt->setVisible(true);
		m_pCoinFreeTxt->setVisible(true);
		m_pIconCoinImg->setVisible(true);
		m_pCoinValTxt->setVisible(true);
		m_pCoinFreeTimeTxt->setText(TimeEx::getStringTimeForInt(m_uCoinLotFreeTime));
	}

	if(m_uGoldLotFreeTime == 0) {
		m_pGoldFreeImg->setVisible(true);
		m_pGoldFreeTimeTxt->setVisible(false);
		m_pGoldFreeTxt->setVisible(false);
		m_pIconGoldImg->setVisible(false);
		m_pGoldValTxt->setVisible(false);

		if(m_uCoinLotFreeTime == 0) {
			// 都CD完了
			this->unschedule(schedule_selector(LotteryPanel::updateTime));
		}
	} else {
		m_pGoldFreeImg->setVisible(false);
		m_pGoldFreeTimeTxt->setVisible(true);
		m_pGoldFreeTxt->setVisible(true);
		m_pIconGoldImg->setVisible(true);
		m_pGoldValTxt->setVisible(true);
		m_pGoldFreeTimeTxt->setText(TimeEx::getStringTimeForInt(m_uGoldLotFreeTime));
	}
}

void LotteryPanel::updateTime(float dt) {
	if(m_uCoinLotFreeTime > 0) {
		m_uCoinLotFreeTime--;
	}

	if(m_uGoldLotFreeTime > 0) {
		m_uGoldLotFreeTime--;
	}

	updateTimeTxt();
}

CCNode* LotteryPanel::getGuideNodeByName(const std::string &name)
{
	CCNode* rNode= NULL;

	if (name == "lottery_gold_btn")
	{
		rNode = m_pGoldLotteryLayout;
	}else if(name == "lottery_close_btn")
	{
		rNode = m_pCloseBtn;
	}

	return rNode;
}
