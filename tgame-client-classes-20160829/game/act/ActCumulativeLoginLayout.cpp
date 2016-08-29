#include "ActCumulativeLoginLayout.h"

#include "defines.h"

#include "ClientSvrMsg.pb.h"
#include "CSCouponMsg.pb.h"

#include "CfgMaster/DailyLoginActCfgMaster.h"
#include "CfgMaster/ActCfgMaster.h"

#include "basic/ToolUtils.h"
#include "basic/TimeEx.h"
#include "ActControl.h"
#include "TextHelper.h"
#include "vlib/base/CVBaseDefine.h"

#include "GUI/CCScrollView/CCScrollView.h"
#include "ActRewardItem.h"

using namespace cocos2d::ui;

#define CELL_PREFIX_NAME "cell_"
#define COL_CELL_CNT 6

ActCumulativeLoginLayout::ActCumulativeLoginLayout():mBestRewardItem(NULL)
{
}

ActCumulativeLoginLayout::~ActCumulativeLoginLayout()
{

}

ActCumulativeLoginLayout* ActCumulativeLoginLayout::create() {
	ActCumulativeLoginLayout *pRet = new ActCumulativeLoginLayout();

	if (pRet != NULL && pRet->init()) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ActCumulativeLoginLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		//this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

// bool ActCumulativeLoginLayout::onTouchBegan(CCTouch *touch, CCEvent *unused_event)
// {
// 	return true;
// }
// 
// void ActCumulativeLoginLayout::onTouchEnded(CCTouch *touch, CCEvent *unused_event)
// {
// 	mBestRewardItem->onTouch(touch->getLocation());
// }

void ActCumulativeLoginLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {

	if (info.ByteSize() > 0) {

		if (info.has_dailyloginstatus())
		{
			mDailyLoginStatus = info.dailyloginstatus();

			//1.
			std::string _last;
			formatActLastTime(mDailyLoginStatus.starttime(), mDailyLoginStatus.stoptime(), _last);
			mActLastTime->setText(_last.c_str());
			//CCLog("ActCumulativeLoginLayout last Time:%s", _last.c_str());


			//2.
			char szTime[64] = { 0, };
			snprintf(szTime, 64, TextStr(TEXT_ACT_TOTAL_LOGINED), mDailyLoginStatus.logincnt());
			mCurCharge->setText(szTime);
			
			//3.
			mRewardTableView->setTouchEnabled(true);
			mRewardTableView->reloadData();
			if(mOffsetPoint.y<=0)
			{
				mRewardTableView->setContentOffset(mOffsetPoint);
			}

			//4. 终极礼包
			const DailyLoginAwardCfg* pBestCharge = DailyLoginAct_CFG_MASTER->getBestDailyLoginActCfg();
			if (!pBestCharge)
				return ;

			if (!mBestRewardItem)
			{ 
				mBestRewardItem = RewardItem::create(vmsg::ACT_LIST_TYPE::DAILY_LOGIN_ACT);
				mBestReward->addChild(mBestRewardItem);
			}
			
			mBestRewardItem->setPosition(ccp(5, 5));

			unsigned int _state = mDailyLoginStatus.status();
			bool _b_state = mDailyLoginStatus.logincnt() >= (pBestCharge->day() -1)? true : false;
			int _i_state = CV_BIT_TEST(_state, (DailyLoginAct_CFG_MASTER->getDailyLoginTypeNum() - 1));// 0 未领取

			mBestRewardItem->setInfo(pBestCharge->day(), pBestCharge->day(), TEXT_ACT_REWARD_BEST, _b_state, _i_state);
		}
	}
}

void ActCumulativeLoginLayout::initUI() {

	initLayoutFromJsonFile("ui_v2/ActSumLoginLayoutUI.ExportJson");
	addChild(m_pRootLayout);

	mActLastTime = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_val_txt"));
	mCurCharge = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cur_desc_text"));
	mRewardLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("reward_bg"));
	mBestReward = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("best_reward"));


	mRewardTableView = CCTableView::create(this, mRewardLayout->getContentSize());
	mRewardTableView->setDirection(kCCScrollViewDirectionVertical);
	mRewardTableView->setAnchorPoint(CCPointZero);
	mRewardTableView->setPosition(ccp(5, 5));
	mRewardTableView->setDelegate(this);
	mRewardTableView->setScaleY(1.0f);
	mRewardTableView->setTouchPriority(-1);
	mRewardTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

	mRewardLayout->addNode(mRewardTableView);
	m_bUIInited = true;
}

void ActCumulativeLoginLayout::formatActLastTime(unsigned int _start, unsigned int _end, std::string& _szTime)
{
	_szTime.clear();

	if (_start == _end)
	{
		_szTime = TextStr(TEXT_ACT_TIME_FOREVER);
		return;
	}

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

CCSize ActCumulativeLoginLayout::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width - 40, 120);
}

unsigned int ActCumulativeLoginLayout::numberOfCellsInTableView(CCTableView *table)
{
	return DailyLoginAct_CFG_MASTER->getDailyLoginTypeNum() - 1;
}

void ActCumulativeLoginLayout::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
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

CCTableViewCell* ActCumulativeLoginLayout::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	RewardItem *pRewardItem = NULL;

	if (!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pRewardItem = RewardItem::create(vmsg::ACT_LIST_TYPE::DAILY_LOGIN_ACT);

		cell->addChild(pRewardItem, 0, LIST_ITEM_TAG);
	}
	else{
		pRewardItem = dynamic_cast<RewardItem*>(cell->getChildByTag(LIST_ITEM_TAG));
	}

	const DailyLoginAwardCfg* pChargeLevel = DailyLoginAct_CFG_MASTER->getDailyLoginActCfg(idx + 1);
	if (!pChargeLevel)
		return cell;

	unsigned int _state = mDailyLoginStatus.status();
	bool _b_state = mDailyLoginStatus.logincnt() >= pChargeLevel->day() ? true : false;
	int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取

	pRewardItem->setInfo(pChargeLevel->day(), pChargeLevel->day(), TEXT_ACT_LOGIN_DAYS, _b_state, _i_state);
	return cell;
}
