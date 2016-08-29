#ifndef ActFightGeekLayout_h__
#define ActFightGeekLayout_h__

#include "ActLayout.h"

class ActFightGeekLayout : public ActLayout, public CCTableViewDataSource, public CCTableViewDelegate
{
public:

	enum eRechargeLevel
	{
		LIST_ITEM_TAG = 1,

	};

	ActFightGeekLayout();
	virtual ~ActFightGeekLayout();

	static ActFightGeekLayout* create();

	// override start

	virtual bool init();

	virtual void updateInfo(const vmsg::CSCouponActStatusItem& info);


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

	CCTableView*	mRewardTableView;
	ListView*		mRewardList;

	vmsg::FightGeekPkgStatus mFightGeekPkgStatus;
	

};

#endif // ActFightGeekLayout_h__