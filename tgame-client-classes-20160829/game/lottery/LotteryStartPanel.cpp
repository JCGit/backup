#include "LotteryStartPanel.h"

#include "defines.h"

#include "CSPlayerMsg.pb.h"

#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/VIPLvCfgMaster.h"

#include "game/UserObj.h"
#include "game/tip/TipControl.h"

#include "LotteryControl.h"

#define BAT_LOTTERY_NUM 10

LotteryStartPanel::LotteryStartPanel()
:m_pCloseBtn(NULL)
,m_pCoinLotteryLayout(NULL)
,m_pCoinOnceLotteryImg(NULL)
,m_pOnceCoinValTxt(NULL)
,m_pCoinBatchLotteryImg(NULL)
,m_pBatchCoinValTxt(NULL)
,m_pGoldLotteryLayout(NULL)
,m_pGoldOnceLotteryImg(NULL)
,m_pOnceGoldValTxt(NULL)
,m_pGoldBatchLotteryImg(NULL)
,m_pBatchGoldValTxt(NULL)
,m_pGoldLotCertainlyTxt(NULL)
,m_pGoldWeaponAdvTxt(NULL)
,m_pVIPLotteryLayout(NULL)
,m_pVIPLotteryImg(NULL)
,m_pVIPGoldValTxt(NULL)
,m_pVIPLotCertainlyTxt(NULL)
,m_pVIPWeaponAdvTxt(NULL)
{
}

LotteryStartPanel::~LotteryStartPanel()
{
}

LotteryStartPanel* LotteryStartPanel::create() {
	LotteryStartPanel *pRet = new LotteryStartPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LotteryStartPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void LotteryStartPanel::openWithLotteryType(unsigned int uLotteryType, bool isFree/* = false*/) {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	m_pCoinLotteryLayout->setEnabled(uLotteryType == vmsg::CS_LOTTERY_TYPE_NORMAL || uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_NORMAL);
	m_pGoldLotteryLayout->setEnabled(uLotteryType == vmsg::CS_LOTTERY_TYPE_ADVANCE || uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_ADVANCE);
	m_pVIPLotteryLayout->setEnabled(uLotteryType == vmsg::CS_LOTTERY_TYPE_VIP);

	const GlobalCfg &stGlobalCfg = GLOBAL_CFG_MASTER->GetCfg();
	unsigned int uCoinLotCostVal = uLotteryType == vmsg::CS_LOTTERY_TYPE_NORMAL && isFree ? 0 : stGlobalCfg.weaponlotnormalcoin();
	unsigned int uGoldLotCostVal = uLotteryType == vmsg::CS_LOTTERY_TYPE_ADVANCE && isFree ? 0 : stGlobalCfg.weaponlotadvancegold();
	unsigned int uCoinLotBatchCostVal = BAT_LOTTERY_NUM * stGlobalCfg.weaponlotnormalcoin() * stGlobalCfg.weaponbatlotdiscount() / 10000;
	unsigned int uGoldLotBatchCostVal = BAT_LOTTERY_NUM * stGlobalCfg.weaponlotadvancegold() * stGlobalCfg.weaponbatlotdiscount() / 10000;
	unsigned int uVIPLotGoldVal = stGlobalCfg.weaponlotvipcost();

	m_pOnceCoinValTxt->setText(intToString(uCoinLotCostVal));
	m_pBatchCoinValTxt->setText(intToString(uCoinLotBatchCostVal));

	m_pOnceGoldValTxt->setText(intToString(uGoldLotCostVal));
	m_pBatchGoldValTxt->setText(intToString(uGoldLotBatchCostVal));
	unsigned int uGoldLotCertainlyCnt = P_LOTTERY_CTRL->getGoldLotCertainlyCnt();
	m_pGoldLotCertainlyTxt->setStringValue(intToString(uGoldLotCertainlyCnt));
	m_pGoldWeaponAdvTxt->setStringValue(intToString(4));

	m_pVIPGoldValTxt->setText(intToString(uVIPLotGoldVal));
	unsigned int uVIPLotCertainlyCnt = P_LOTTERY_CTRL->getVIPLotCertainlyCnt();
	m_pVIPLotCertainlyTxt->setStringValue(intToString(uVIPLotCertainlyCnt));
	m_pVIPWeaponAdvTxt->setStringValue(intToString(5));

	closeLoading();
}

void LotteryStartPanel::lottery(unsigned int uLotteryType) {
	const GlobalCfg &stGlobalCfg = GLOBAL_CFG_MASTER->GetCfg();

	if(uLotteryType == vmsg::CS_LOTTERY_TYPE_NORMAL) { // 金币抽卡
		unsigned int uNeedCoinVal = stGlobalCfg.weaponlotnormalcoin();

		if(uNeedCoinVal <= P_USER_OBJ->getUserInfo()->coin) {
			P_LOTTERY_CTRL->sendLotteryRqst(vmsg::CS_LOTTERY_TYPE_NORMAL);
		} else {
			P_LOTTERY_CTRL->getCoinGetWayPanel()->open();
		}
	} else if(uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_NORMAL) { // 金币十连抽
		unsigned int uNeedCoinVal = BAT_LOTTERY_NUM * stGlobalCfg.weaponlotnormalcoin() * stGlobalCfg.weaponbatlotdiscount() / 10000;

		if(uNeedCoinVal <= P_USER_OBJ->getUserInfo()->coin) {
			P_LOTTERY_CTRL->sendLotteryRqst(vmsg::CS_LOTTERY_TYPE_BAT_NORMAL);
		} else {
			P_LOTTERY_CTRL->getCoinGetWayPanel()->open();
		}
	} else if(uLotteryType == vmsg::CS_LOTTERY_TYPE_ADVANCE) { // 钻石抽卡
		unsigned int uNeedGoldVal = stGlobalCfg.weaponlotadvancegold();
		P_TIP_CTRL->GoldCostTip(uNeedGoldVal, this, dialogbtneventselector(LotteryStartPanel::onGoldLotteryDialog));
	} else if(uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_ADVANCE) { // 钻石十连抽
		unsigned int uNeedGoldVal = BAT_LOTTERY_NUM * stGlobalCfg.weaponlotadvancegold() * stGlobalCfg.weaponbatlotdiscount() / 10000;
		P_TIP_CTRL->GoldCostTip(uNeedGoldVal, this, dialogbtneventselector(LotteryStartPanel::onGoldBatchLotteryDialog));
	} else if(uLotteryType == vmsg::CS_LOTTERY_TYPE_VIP) { // VIP抽卡
		unsigned int uNeedGoldVal = stGlobalCfg.weaponlotvipcost();
		unsigned int uVIPLotTodayCnt = P_LOTTERY_CTRL->getVIPLotTodayCnt();

		if(uVIPLotTodayCnt > 0) {
			P_TIP_CTRL->GoldCostTip(uNeedGoldVal, this, dialogbtneventselector(LotteryStartPanel::onVipLotteryDialog));
		} else {
			P_TIP_CTRL->getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_WEAPON_LOTTERY);
		}
	}
}

void LotteryStartPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/LotteryStartPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(LotteryStartPanel::touchEventHandler));

	m_pCoinLotteryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("coin_lottery_layout"));

	m_pCoinOnceLotteryImg = dynamic_cast<UIImageView*>(m_pCoinLotteryLayout->getChildByName("coin_once_lottery_img"));
	m_pCoinOnceLotteryImg->setTouchEnabled(true);
	m_pCoinOnceLotteryImg->addTouchEventListener(this, toucheventselector(LotteryStartPanel::touchEventHandler));

	m_pOnceCoinValTxt = dynamic_cast<UILabel*>(m_pCoinLotteryLayout->getChildByName("once_coin_val_txt"));

	m_pCoinBatchLotteryImg = dynamic_cast<UIImageView*>(m_pCoinLotteryLayout->getChildByName("coin_batch_lottery_img"));
	m_pCoinBatchLotteryImg->setTouchEnabled(true);
	m_pCoinBatchLotteryImg->addTouchEventListener(this, toucheventselector(LotteryStartPanel::touchEventHandler));

	m_pBatchCoinValTxt = dynamic_cast<UILabel*>(m_pCoinLotteryLayout->getChildByName("batch_coin_val_txt"));

	m_pGoldLotteryLayout  = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("gold_lottery_layout"));
	m_pGoldLotteryLayout->setVisible(true);

	m_pGoldOnceLotteryImg = dynamic_cast<UIImageView*>(m_pGoldLotteryLayout->getChildByName("gold_once_lottery_img"));
	m_pGoldOnceLotteryImg->setTouchEnabled(true);
	m_pGoldOnceLotteryImg->addTouchEventListener(this, toucheventselector(LotteryStartPanel::touchEventHandler));

	m_pOnceGoldValTxt = dynamic_cast<UILabel*>(m_pGoldLotteryLayout->getChildByName("once_gold_val_txt"));

	m_pGoldBatchLotteryImg = dynamic_cast<UIImageView*>(m_pGoldLotteryLayout->getChildByName("gold_batch_lottery_img"));
	m_pGoldBatchLotteryImg->setTouchEnabled(true);
	m_pGoldBatchLotteryImg->addTouchEventListener(this, toucheventselector(LotteryStartPanel::touchEventHandler));

	m_pBatchGoldValTxt = dynamic_cast<UILabel*>(m_pGoldLotteryLayout->getChildByName("batch_gold_val_txt"));

	m_pGoldLotCertainlyTxt = dynamic_cast<UILabelAtlas*>(m_pGoldLotteryLayout->getChildByName("gold_lot_certainly_txt"));

	m_pGoldWeaponAdvTxt = dynamic_cast<UILabelAtlas*>(m_pGoldLotteryLayout->getChildByName("gold_weapon_adv_txt"));

	m_pVIPLotteryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("vip_lottery_layout"));
	m_pVIPLotteryLayout->setVisible(true);

	m_pVIPLotteryImg = dynamic_cast<UIImageView*>(m_pVIPLotteryLayout->getChildByName("vip_lottery_img"));
	m_pVIPLotteryImg->setTouchEnabled(true);
	m_pVIPLotteryImg->addTouchEventListener(this, toucheventselector(LotteryStartPanel::touchEventHandler));

	m_pVIPGoldValTxt = dynamic_cast<UILabel*>(m_pVIPLotteryLayout->getChildByName("vip_gold_val_txt"));

	m_pVIPLotCertainlyTxt = dynamic_cast<UILabelAtlas*>(m_pVIPLotteryLayout->getChildByName("vip_lot_certainly_txt"));

	m_pVIPWeaponAdvTxt = dynamic_cast<UILabelAtlas*>(m_pVIPLotteryLayout->getChildByName("vip_weapon_adv_txt"));

	m_bUIInited = true;

}

void LotteryStartPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pCoinOnceLotteryImg->getName()) == 0) { // 金币抽卡
				lottery(vmsg::CS_LOTTERY_TYPE_NORMAL);
			} else if(strcmp(pszWidgetName, m_pCoinBatchLotteryImg->getName()) == 0) { // 金币十连抽
				lottery(vmsg::CS_LOTTERY_TYPE_BAT_NORMAL);
			} else if(strcmp(pszWidgetName, m_pGoldOnceLotteryImg->getName()) == 0) { // 钻石抽卡
				lottery(vmsg::CS_LOTTERY_TYPE_ADVANCE);
			} else if(strcmp(pszWidgetName, m_pGoldBatchLotteryImg->getName()) == 0) { // 钻石十连抽
				lottery(vmsg::CS_LOTTERY_TYPE_BAT_ADVANCE);
			} else if(strcmp(pszWidgetName, m_pVIPLotteryImg->getName()) == 0) { // VIP抽卡
				lottery(vmsg::CS_LOTTERY_TYPE_VIP);
			}
			break;
		default:;
	}
}

void LotteryStartPanel::onGoldLotteryDialog() {
	P_LOTTERY_CTRL->sendLotteryRqst(vmsg::CS_LOTTERY_TYPE_ADVANCE);
}

void LotteryStartPanel::onGoldBatchLotteryDialog() {
	P_LOTTERY_CTRL->sendLotteryRqst(vmsg::CS_LOTTERY_TYPE_BAT_ADVANCE);
}

void LotteryStartPanel::onVipLotteryDialog() {
	P_LOTTERY_CTRL->sendLotteryRqst(vmsg::CS_LOTTERY_TYPE_VIP);
}
