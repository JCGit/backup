#include "ActVipGiftLayout.h"
#include "defines.h"

#include "ClientSvrMsg.pb.h"
#include "CSCouponMsg.pb.h"

#include "CfgMaster/ActCfgMaster.h"

#include "basic/ToolUtils.h"
#include "basic/TimeEx.h"
#include "ActControl.h"
#include "TextHelper.h"
#include "vlib/base/CVBaseDefine.h"

#include "GUI/CCScrollView/CCScrollView.h"
#include "ActRewardItem.h"
#include "VipGiftCfgMaster.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "UserObj.h"

using namespace cocos2d::ui;

#define CELL_PREFIX_NAME "cell_"
#define COL_CELL_CNT 6

ActVipGiftLayout::ActVipGiftLayout()
{
}

ActVipGiftLayout::~ActVipGiftLayout()
{

}

ActVipGiftLayout* ActVipGiftLayout::create() {
	ActVipGiftLayout *pRet = new ActVipGiftLayout();

	if (pRet != NULL && pRet->init()) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ActVipGiftLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void ActVipGiftLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {

	if (info.ByteSize() > 0) {

		if (info.has_vipgiftpkgstatus())
		{
			mVipGiftStatus = info.vipgiftpkgstatus();

			//1.
			std::string _last;
			formatActLastTime(mVipGiftStatus.starttime(), mVipGiftStatus.stoptime(), _last);
			mActLastTime->setText(_last.c_str());
			//CCLog("ActFightGeekLayout last Time:%s", _last.c_str());

			//2.
			char szTime[64] = { 0, };
			snprintf(szTime, 64, TextStr(TEXT_ACT_VIP_NOW), P_USER_OBJ->getUserInfo()->vipLv);
			mCurCharge->setText(szTime);
			
			unsigned int _state = mVipGiftStatus.status();
			mButtonState->setLevel(P_USER_OBJ->getUserInfo()->vipLv);
			RewardStateBtn::eRewardBtnState istate = RewardStateBtn::eRewardBtnState::REWARD_BTN_UNREACHABLE;
			
			if (P_USER_OBJ->getUserInfo()->vipLv==0)
			{
				istate = RewardStateBtn::eRewardBtnState::REWARD_BTN_UNREACHABLE;
			}else
			{
				if (mVipGiftStatus.status() == 0)
				{
					istate = RewardStateBtn::eRewardBtnState::REWARD_BTN_CANGET;
				}else
				{
					istate = RewardStateBtn::eRewardBtnState::REWARD_BTN_GETTED;
				}
			}
			mButtonState->setState(istate);
			//3.
			mRewardTableView->setTouchEnabled(true);
			mRewardTableView->reloadData();
			if(mOffsetPoint.y<=0)
			{
				mRewardTableView->setContentOffset(mOffsetPoint);
			}
		}
	}
}

void ActVipGiftLayout::initUI() {

	initLayoutFromJsonFile("ui_v2/ActVipGiftLayoutUI.ExportJson");
	addChild(m_pRootLayout);

	mActLastTime = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_val_txt"));
	mCurCharge = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cur_desc_text"));
	mRewardLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("reward_bg"));
	mRewardBtnLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("item_state_layout"));
	Widget* pGetBtn = dynamic_cast<Widget*>(m_pRootLayout->getChildByName("get_btn_0"));
	pGetBtn->addTouchEventListener(this,toucheventselector(ActVipGiftLayout::touchEventHandler));

	mRewardTableView = CCTableView::create(this, mRewardLayout->getContentSize());
	mRewardTableView->setDirection(kCCScrollViewDirectionVertical);
	mRewardTableView->setAnchorPoint(CCPointZero);
	mRewardTableView->setPosition(ccp(5, 5));
	mRewardTableView->setDelegate(this);
	mRewardTableView->setScaleY(1.0f);
	mRewardTableView->setTouchPriority(2);
	mRewardTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

	mRewardLayout->addNode(mRewardTableView);

	mButtonState = RewardStateBtn::create();
	mButtonState->setName("VipGift");
	mRewardBtnLayout->addChild(mButtonState);
	
	m_bUIInited = true;
}

void ActVipGiftLayout::touchEventHandler(CCObject* pSender, TouchEventType type)
{
	if (type ==TOUCH_EVENT_ENDED )
	{
		Widget* pWidget = dynamic_cast<Widget*>(pSender);
		string pName = pWidget->getName();
		if (pName == "get_btn_0")
		{
			P_SHOP_CTRL->getRmbShopPanel()->open();
		}
	}
}

void ActVipGiftLayout::formatActLastTime(unsigned int _start, unsigned int _end, std::string& _szTime)
{
	_szTime.clear();

	time_t _spanStart = (time_t)_start;
	time_t _spanEnd = (time_t)_end;

	CVTime spanStart(_spanStart);
	CVTime spanEnd(_spanEnd);
	
	char szTime[64] = { 0, };
	snprintf(szTime, 64, TextStr(TEXT_ACT_TIME_LAST_FORMAT), 
			spanStart.GetYear(), spanStart.GetMonth(), spanStart.GetDay(),
			spanEnd.GetYear(), spanEnd.GetMonth(), spanEnd.GetDay());
	_szTime = szTime;
}

CCSize ActVipGiftLayout::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width - 40, 120);
}

unsigned int ActVipGiftLayout::numberOfCellsInTableView(CCTableView *table)
{
	return VIPGIFT_CFG_MASTER->getVipGiftTypeNum();
}

void ActVipGiftLayout::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	RewardItem *pItem = dynamic_cast<RewardItem*>(cell->getChildByTag(LIST_ITEM_TAG));
	if (NULL == pItem) {
		return;
	}
	mOffsetPoint = mRewardTableView->getContentOffset();
	if(pItem->onTouch(m_stGlobalTouchPt))
	{
		mRewardTableView->setTouchEnabled(false);
	}
}

CCTableViewCell* ActVipGiftLayout::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	RewardItem *pRewardItem = NULL;

	if (!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pRewardItem = RewardItem::create(vmsg::ACT_LIST_TYPE::VIP_GIFT_ACT);
		
		cell->addChild(pRewardItem, 0, LIST_ITEM_TAG);
	}
	else{
		pRewardItem = dynamic_cast<RewardItem*>(cell->getChildByTag(LIST_ITEM_TAG));
	}

	const VipGiftCfg* pVipLevel = VIPGIFT_CFG_MASTER->getVipGiftActCfg(idx + 1);
	if (!pVipLevel)
		return cell;
	/*
	unsigned int _state = mVipGiftStatus.status();
	bool _b_state = P_USER_OBJ->getUserInfo()->vipLv >= pChargeLevel->viplevel() ? true : false;
	int _i_state = CV_BIT_TEST(_state, idx);// 0 Î´ÁìÈ¡
	*/

	pRewardItem->setInfo(pVipLevel->level(), pVipLevel->viplevel(), TEXT_ACT_VIP_GIFT, false, false);
	pRewardItem->setBtnState(0,false);

	return cell;
}