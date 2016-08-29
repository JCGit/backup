#include "ActFightGeekLayout.h"
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
#include "FightGeekAwardCfgMaster.h"

#include "UserObj.h"


using namespace cocos2d::ui;

#define CELL_PREFIX_NAME "cell_"
#define COL_CELL_CNT 6

ActFightGeekLayout::ActFightGeekLayout()
{
}

ActFightGeekLayout::~ActFightGeekLayout()
{

}

ActFightGeekLayout* ActFightGeekLayout::create() {
	ActFightGeekLayout *pRet = new ActFightGeekLayout();

	if (pRet != NULL && pRet->init()) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ActFightGeekLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void ActFightGeekLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {

	if (info.ByteSize() > 0) {

		if (info.has_fightgeekstatus())
		{
			mFightGeekPkgStatus = info.fightgeekstatus();

			//1.
			std::string _last;
			formatActLastTime(mFightGeekPkgStatus.starttime(), mFightGeekPkgStatus.stoptime(), _last);
			mActLastTime->setText(_last.c_str());
			//CCLog("ActFightGeekLayout last Time:%s", _last.c_str());

			//2.
			char szTime[64] = { 0, };
			snprintf(szTime, 64, TextStr(TEXT_ACT_TOTAL_FIGHT), P_USER_OBJ->getUserInfo()->fightPower);
			mCurCharge->setText(szTime);
			
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

void ActFightGeekLayout::initUI() {

	initLayoutFromJsonFile("ui_v2/ActFightNumMasterLayoutUI.ExportJson");
	addChild(m_pRootLayout);

	mActLastTime = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_val_txt"));
	mCurCharge = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cur_desc_text"));
	mRewardLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("reward_bg"));

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

void ActFightGeekLayout::formatActLastTime(unsigned int _start, unsigned int _end, std::string& _szTime)
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

CCSize ActFightGeekLayout::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width - 40, 120);
}

unsigned int ActFightGeekLayout::numberOfCellsInTableView(CCTableView *table)
{
	return FightGeekAward_CFG_MASTER->getFightGeekAwardTypeNum();
}

void ActFightGeekLayout::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
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

CCTableViewCell* ActFightGeekLayout::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	RewardItem *pRewardItem = NULL;

	if (!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pRewardItem = RewardItem::create(vmsg::ACT_LIST_TYPE::FIGHT_GEEK_ACT);

		cell->addChild(pRewardItem, 0, LIST_ITEM_TAG);
	}
	else{
		pRewardItem = dynamic_cast<RewardItem*>(cell->getChildByTag(LIST_ITEM_TAG));
	}

	const FightGeekAwardCfg* pChargeLevel = FightGeekAward_CFG_MASTER->getFightGeekAwardActCfg(idx + 1);
	if (!pChargeLevel)
		return cell;

	unsigned int _state = mFightGeekPkgStatus.status();
	bool _b_state = P_USER_OBJ->getUserInfo()->fightPower >= pChargeLevel->fight() ? true : false;
	int _i_state = CV_BIT_TEST(_state, idx);// 0 Î´ÁìÈ¡

	pRewardItem->setInfo(pChargeLevel->level(), pChargeLevel->fight(), TEXT_ACT_FIGHTING_LEVEL, _b_state, _i_state);
	return cell;
}