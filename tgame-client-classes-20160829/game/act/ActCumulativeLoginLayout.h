#ifndef ActCumulativeLoginLayout_h__
#define ActCumulativeLoginLayout_h__

#include "ActLayout.h"

class RewardItem;
class ActCumulativeLoginLayout : public ActLayout, public CCTableViewDataSource, public CCTableViewDelegate
{
public:

	enum eRechargeLevel
	{
		LIST_ITEM_TAG = 1,

	};

	ActCumulativeLoginLayout();
	virtual ~ActCumulativeLoginLayout();

	static ActCumulativeLoginLayout* create();

	// override start

	virtual bool init();

	virtual void updateInfo(const vmsg::CSCouponActStatusItem& info);

	//virtual bool onTouchBegan(CCTouch *touch, CCEvent *unused_event);
	//virtual void onTouchEnded(CCTouch *touch, CCEvent *unused_event);


	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

	// override end



private:
	void initUI();
	void formatActLastTime(unsigned int _start, unsigned int _end, std::string& _szTime);


private:

	UILabel*		mActLastTime;
	UILabel*		mCurCharge;
	UILayout*		mRewardLayout;
	UILayout*		mBestReward;

	RewardItem*		mBestRewardItem;
	CCTableView*	mRewardTableView;
	ListView*		mRewardList;

	vmsg::DailyLoginPkgStatus mDailyLoginStatus;

};

#endif // ActCumulativeLoginLayout_h__