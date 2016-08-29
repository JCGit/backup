#include "ActGradeGeekLayout.h"

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
#include "GradeGeekAwardCfgMaster.h"

#include "UserObj.h"

using namespace cocos2d::ui;

#define CELL_PREFIX_NAME "cell_"
#define COL_CELL_CNT 6

GradeGeekLayout::GradeGeekLayout()
{
}

GradeGeekLayout::~GradeGeekLayout()
{

}

GradeGeekLayout* GradeGeekLayout::create() {
	GradeGeekLayout *pRet = new GradeGeekLayout();

	if (pRet != NULL && pRet->init()) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GradeGeekLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void GradeGeekLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {
	
	if (info.ByteSize() > 0) {

		if (info.has_gradegeekstatus())
		{
			mGradeGeekPkgStatus = info.gradegeekstatus();

			//1.
			std::string _last;
			formatActLastTime(mGradeGeekPkgStatus.starttime(), mGradeGeekPkgStatus.stoptime(), _last);
			mActLastTime->setText(_last.c_str());
			//CCLog("GradeGeekLayout last Time:%s", _last.c_str());

			
			//2.
			char szTime[64] = { 0, };
			snprintf(szTime, 64, TextStr(TEXT_ACT_CUR_LEVEL), P_USER_OBJ->getUserInfo()->level);
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

void GradeGeekLayout::initUI() {

	initLayoutFromJsonFile("ui_v2/ActLevelMasterLayoutUI.ExportJson");
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

void GradeGeekLayout::formatActLastTime(unsigned int _start, unsigned int _end, std::string& _szTime)
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

CCSize GradeGeekLayout::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width - 40, 120);
}

unsigned int GradeGeekLayout::numberOfCellsInTableView(CCTableView *table)
{
	return GradeGeekAward_CFG_MASTER->getGradeGeekAwardTypeNum();
}

void GradeGeekLayout::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
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

CCTableViewCell* GradeGeekLayout::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	RewardItem *pRewardItem = NULL;

	if (!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pRewardItem = RewardItem::create(vmsg::ACT_LIST_TYPE::GRADE_GEEK_ACT);

		cell->addChild(pRewardItem, 0, LIST_ITEM_TAG);
	}
	else{
		pRewardItem = dynamic_cast<RewardItem*>(cell->getChildByTag(LIST_ITEM_TAG));
	}

	const GradeGeekAwardCfg* pChargeLevel = GradeGeekAward_CFG_MASTER->getGradeGeekAwardCfg(idx + 1);
	if (!pChargeLevel)
		return cell;
	
	int curLast = 0;
	if (mGradeGeekPkgStatus.getnum_size() != 0 && idx <= mGradeGeekPkgStatus.getnum_size() - 1)
	{
		curLast = pChargeLevel->limitnum() - mGradeGeekPkgStatus.getnum(idx);
	}
	
	pRewardItem->setRewardItemLast(curLast, pChargeLevel->limitnum());

	unsigned int _state = mGradeGeekPkgStatus.status();
	bool _b_state = P_USER_OBJ->getUserInfo()->level >= pChargeLevel->grade() ? true : false;
	_b_state = curLast > 0 ? _b_state : false;
	bool _isNoLeft = curLast > 0 ? false : true;
	int _i_state = CV_BIT_TEST(_state, idx);// 0 Î´ÁìÈ¡
	pRewardItem->setInfo(pChargeLevel->level(), pChargeLevel->grade(), TEXT_ACT_LEVEL, _b_state, _i_state,_isNoLeft);
	return cell;
}