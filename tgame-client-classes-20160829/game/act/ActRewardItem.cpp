#include "ActRewardItem.h"
#include "defines.h"
#include "TextHelper.h"
#include "tip/TipControl.h"
#include "ActControl.h"
#include "IconCell.h"
#include "basic/ToolUtils.h"
#include "SumChargeActCfgMaster.h"
#include "DailyChargeActCfgMaster.h"
#include "DailyLoginActCfgMaster.h"
#include "FightGeekAwardCfgMaster.h"
#include "GradeGeekAwardCfgMaster.h"
#include "VipGiftCfgMaster.h"
#include "DailyConsumeAwardCfgMaster.h"
#include "SumConsumeActCfgMaster.h"
#include "CfgMaster/PackageCfgMaster.h"
#include "CfgMaster/ItemCfgMaster.h"

RewardStateBtn::RewardStateBtn()
{

}

RewardStateBtn::~RewardStateBtn()
{

}

RewardStateBtn* RewardStateBtn::create()
{
	RewardStateBtn *pRet = new RewardStateBtn();

	if (pRet != NULL && pRet->init()) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RewardStateBtn::init()
{
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void RewardStateBtn::initUI()
{
	initLayoutFromJsonFile("ui_v2/ActRechargeStateBtn.ExportJson");
	addChild(m_pRootLayout);

	mItemBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("get_btn_0_0"));

	mItemBtn->addTouchEventListener(this, toucheventselector(RewardStateBtn::touchEventHandler));


	for (int i = 0; i != REWARD_BTN_MAX; ++i)
	{
		std::string szName = "state" + intToString(i+1);
		
		mItemState[i] = dynamic_cast<UIImageView*>(UIHelper::seekWidgetByName(m_pRootLayout, szName.c_str()));
	}
}

void RewardStateBtn::setState(unsigned int iState)
{
	mState = (eRewardBtnState)iState;
	if (mState < REWARD_BTN_UNREACHABLE || mState >= REWARD_BTN_MAX)
		return;

	bool bstate = mState == REWARD_BTN_CANGET ? 1 : 0;
	mItemBtn->setBright(bstate);
	mItemBtn->setTouchEnabled(bstate);
	
	for (int i = 0; i != REWARD_BTN_MAX; ++i)
	{
		if (i == (int)mState)
		{
			mItemState[i]->setVisible(true);
		}
		else
		{
			mItemState[i]->setVisible(false);
		}
	}
}

void RewardStateBtn::setBtnEnabled( bool bEnable )
{
	mItemBtn->setBright(bEnable);
	mItemBtn->setTouchEnabled(bEnable);
}
bool RewardStateBtn::isBtnEnasble()
{
	return mItemBtn->isTouchEnabled();
}


void RewardStateBtn::touchEventHandler(CCObject *pSender, TouchEventType type)
{
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		{
			if (strcmp(this->getName(),"VipGift") == 0)
			{
				if (isBtnEnasble()&& getState() == RewardStateBtn::REWARD_BTN_CANGET)
				{
					setBtnEnabled(false);
					P_ACT_CTRL->getActPanel()->showLoading();
					P_ACT_CTRL->sendVipGiftRqst(getLevel());
				}
			}
		}
		//P_ACT_CTRL->sendDailyLoginRqst(getLevel());
		break;
	default:;
	}
}

RewardItem::RewardItem(vmsg::ACT_LIST_TYPE eActType) : mRewardTabType(eActType)
{

}

RewardItem::~RewardItem()
{

}

RewardItem* RewardItem::create(vmsg::ACT_LIST_TYPE eActType)
{
	RewardItem *pRet = new RewardItem(eActType);

	if (pRet != NULL && pRet->init()) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RewardItem::init()
{
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);


		this->setTouchEnabled(true);

		bRet = true;
	} while (0);

	return bRet;
}

void RewardItem::initUI()
{
	initLayoutFromJsonFile("ui_v2/ActRechargeTab.ExportJson");
	addChild(m_pRootLayout);

	mRewardRMB = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
	mRewaardLastText = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("reward_last"));

	mRewardIcon1 = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_1"));
	mRewardIcon2 = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_2"));


	mIconCell1 = IconCell::create();
	mIconCell1->showNumTxt();
	mRewardIcon1->addChild(mIconCell1);

	mIconCell2 = IconCell::create();
	mIconCell2->showNumTxt();
	mRewardIcon2->addChild(mIconCell2);


	mRewardLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("item_state_layout"));

	// 初始化button
	mButtonState = RewardStateBtn::create();
	mButtonState->setState(0);
	mRewardLayout->addChild(mButtonState);


	//m_pRootLayout->addTouchEventListener(this, toucheventselector(RewardItem::touchEventHandler));
	this->setTouchEnabled(false);
	m_pRootLayout->setTouchEnabled(false);
}

void RewardItem::setBtnState(unsigned int iState, bool isShow /*= true*/)
{
	if (mButtonState)
		mRewardLayout->setEnabled(isShow);
		mButtonState->setState(iState);
}

void RewardItem::setRewardItemLevel(int _level, enumActTipTextConsts eText)
{
	char szLevel[64] = { 0, };
	snprintf(szLevel, 64, TextStr(eText), _level);

	if (mRewardRMB)
		mRewardRMB->setText(szLevel);

	if (mButtonState)
		mButtonState->setLevel(_level);
}

void RewardItem::setRewardItemLast(int _cur, int _max)
{
	char szLast[64] = { 0, };
	snprintf(szLast, 64, TextStr(TEXT_ACT_REWARD_LAST), _cur, _max);
	if (mRewaardLastText)
	{
		mRewaardLastText->setVisible(true);
		mRewaardLastText->setText(szLast);
	}
}

bool RewardItem::onTouch(const CCPoint& stGlobalPt)
{
	bool pRet = false;
	if (mRewardIcon1->hitTest(stGlobalPt)) {
		pRet = true;
		bool pIsGift = false;
		if (mIconCell1->getItemType() == vmsg::THING_TYPE_ITEM)
		{
			const ItemCfg* pItemCfg = ITEM_CFG_MASTER->GetItemCfgByID(mIconCell1->getItemCfgid());
			if(pItemCfg->usable())
				pIsGift = true;
		}
		P_TIP_CTRL->showItemTipPanel(mIconCell1->getItemType(), mIconCell1->getItemCfgid(), mIconCell1->getItemNum(),true ,
			pIsGift);
	}
	else if (mRewardIcon2->hitTest(stGlobalPt)) {
		pRet = true;
		bool pIsGift = false;
		if ( mIconCell2->getItemType() == vmsg::THING_TYPE_ITEM)
		{
			const ItemCfg* pItemCfg = ITEM_CFG_MASTER->GetItemCfgByID(mIconCell2->getItemCfgid());
			if(pItemCfg->usable())
				pIsGift = true;
		}
		P_TIP_CTRL->showItemTipPanel(mIconCell2->getItemType(), mIconCell2->getItemCfgid(), mIconCell2->getItemNum(),true,
			pIsGift);
	}
	else if (mButtonState->isBtnEnasble()&&mButtonState->hitTest(stGlobalPt)) {

		if (mButtonState->getState() == RewardStateBtn::REWARD_BTN_CANGET)
		{
			int _level = this->getLevel();
			mButtonState->setBtnEnabled(false);
			P_ACT_CTRL->getActPanel()->showLoading();
			switch (mRewardTabType)
			{
			case vmsg::SUM_CHARGE_ACT:
				P_ACT_CTRL->sendSumChargeRqst(_level);
				break;
			case vmsg::DAILY_LOGIN_ACT:
				P_ACT_CTRL->sendDailyLoginRqst(_level);
				break;
			case vmsg::DAILY_CHARGE_ACT:
				P_ACT_CTRL->sendDailyChargeRqst(_level);
				break;
			case vmsg::GRADE_GEEK_ACT:
				P_ACT_CTRL->sendGradeGeekRqst(_level);
				break;
			case vmsg::FIGHT_GEEK_ACT:
				P_ACT_CTRL->sendFightGeekRqst(_level);
				break;
			case vmsg::VIP_GIFT_ACT:
				P_ACT_CTRL->sendVipGiftRqst(_level);
				break;
			case vmsg::DAILY_CONSUME_ACT:
				P_ACT_CTRL->sendDailyConsumeRqst(_level);
				break;
			case vmsg::SUM_CONSUME_ACT:
				P_ACT_CTRL->sendSumConsumeRqst(_level);
				break;
			default:
				break;
			}
		}

	}
	return pRet;
}

void RewardItem::setSumChargeRewardIcon()
{
	const SumChargeActCfg* pChargeCfg = SumChargeAct_CFG_MASTER->getSumChargeActCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const SumChargeActCfgElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid()); 
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::setInfo(int _level, int lvValue, enumActTipTextConsts eText, bool bCfgState, int realState, bool isNoLeft /*= false*/)
{
	mLevel = _level;

	//1. level value
	setRewardItemLevel(lvValue, eText);

	//2. 礼包icon
	switch (mRewardTabType)
	{
	case vmsg::SUM_CHARGE_ACT:
		setSumChargeRewardIcon();
		break;
	case vmsg::DAILY_LOGIN_ACT:
		setSumLoginRewardIcon();
		break;
	case vmsg::DAILY_CHARGE_ACT:
		setDailyChargeRewardIcon();
		break;
	case vmsg::GRADE_GEEK_ACT:
		setGradeGeekRewardIcon();
		break;
	case vmsg::FIGHT_GEEK_ACT:
		setFightGeekRewardIcon();
		break;
	case vmsg::VIP_GIFT_ACT:
		setVipGiftRewardIcon();
		break;
	case vmsg::DAILY_CONSUME_ACT:
		setDailyConsumeRewardIcon();
		break;
	case vmsg::SUM_CONSUME_ACT:
		setSumConsumeRewardIcon();
		break;
	default:
		break;
	}

	//3. 按钮state
	unsigned int iBtnState = (unsigned int)RewardStateBtn::REWARD_BTN_UNREACHABLE;
	if (!bCfgState)
	{
		iBtnState = (unsigned int)RewardStateBtn::REWARD_BTN_UNREACHABLE;
		if (isNoLeft)
		{
			iBtnState = (unsigned int)RewardStateBtn::REWARD_BTN_NOLEFT;
		}
	}
	else{
		if (realState)
		{
			iBtnState = (unsigned int)RewardStateBtn::REWARD_BTN_GETTED;
		}
		else{
			iBtnState = (unsigned int)RewardStateBtn::REWARD_BTN_CANGET;
		}
	}
	setBtnState(iBtnState);
}

void RewardItem::setSumLoginRewardIcon()
{
	const DailyLoginAwardCfg* pChargeCfg = DailyLoginAct_CFG_MASTER->getDailyLoginActCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const DailyLoginAwardElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}
		else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::setDailyChargeRewardIcon()
{
	const DailyChargeAwardCfg* pChargeCfg = DailyChargeAct_CFG_MASTER->getDailyChargeActCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const DailyChargeAwardCfgElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}
		else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::setFightGeekRewardIcon()
{
	const FightGeekAwardCfg* pChargeCfg = FightGeekAward_CFG_MASTER->getFightGeekAwardActCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const FightGeekActCfgElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}
		else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::setGradeGeekRewardIcon()
{
	const GradeGeekAwardCfg* pChargeCfg = GradeGeekAward_CFG_MASTER->getGradeGeekAwardCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const GradeGeekAwardElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}
		else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::setVipGiftRewardIcon()
{
	const VipGiftCfg* pChargeCfg = VIPGIFT_CFG_MASTER->getVipGiftActCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const VipGiftCfgElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}
		else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::setDailyConsumeRewardIcon()
{
	const DailyConsumeAwardCfg* pChargeCfg = DAILYCONSUME_CFG_MASTER->getDailyConsumeActCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const DailyConsumeAwardCfgElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}
		else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::setSumConsumeRewardIcon()
{
	const SumConsumeActCfg* pChargeCfg = SumConsumeAct_CFG_MASTER->getSumConsumeActCfg(getLevel());
	if (!pChargeCfg)
		return;

	for (int i = 0; i != pChargeCfg->elmtlst_size(); ++i)
	{
		const SumConsumeActCfgElmt& stElmt = pChargeCfg->elmtlst(i);

		if (i == 0)
		{
			mIconCell1->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon1);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon1);
		}
		else if (i == 1)
		{
			mIconCell2->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
			CToolUtils::adjustIconBgSize(stElmt.elementtype(), stElmt.elementid(), mRewardIcon2);
			CToolUtils::adjustFragIcon(stElmt.elementtype(), mRewardIcon2);
		}
	}
}

void RewardItem::touchEventHandler(CCObject *pSender, TouchEventType type)
{
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if (mRewardTabType == vmsg::ACT_LIST_TYPE::DAILY_LOGIN_ACT)
		{
			P_ACT_CTRL->sendDailyLoginRqst(getLevel());
		}
		break;
	default:;
	}
}

bool RewardItem::onTouchBegan(CCTouch *touch, CCEvent *unused_event)
{
	Widget::onTouchBegan(touch, unused_event);

// 	CCPoint pt = touch->getLocation();
// 	CCRect rect = mRewardLayout->boundingBox();
// 	if (rect.containsPoint(pt))
// 	{
// 		return true;
// 	}
	return false;
}

void RewardItem::onTouchEnded(CCTouch *touch, CCEvent *unused_event)
{
	if (mRewardTabType == vmsg::ACT_LIST_TYPE::DAILY_LOGIN_ACT)
	{
		this->onTouch(touch->getLocation());
	}
}
