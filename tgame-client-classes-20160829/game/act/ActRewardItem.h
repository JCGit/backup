#ifndef ACT_REWARD_ITEM_H
#define ACT_REWARD_ITEM_H

#include "basic/BaseLayout.h"
#include "TextHelper.h"
#include "ClientSvrMsg.pb.h"
#include "vlib/QEventDispatcher.h"

#include "cocos2d.h"
USING_NS_CC;

class RewardStateBtn : public BaseLayout
{
public:
	enum eRewardBtnState {
		REWARD_BTN_UNREACHABLE = 0,	// current can't get
		REWARD_BTN_GETTED,		// current getted
		REWARD_BTN_CANGET,		// current can get
		REWARD_BTN_NOLEFT,			//current no left
		REWARD_BTN_MAX,
	};

public:
	RewardStateBtn();
	virtual ~RewardStateBtn();

	static RewardStateBtn* create();

	// override start

	virtual bool init();

	// override end

	eRewardBtnState getState() { return mState;  }
	void	setState(unsigned int iState);
	void	setBtnEnabled(bool bEnable);
	bool	isBtnEnasble();
private:

	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

protected:

	eRewardBtnState mState;
	UIButton*		mItemBtn;
	UIImageView*	mItemState[REWARD_BTN_MAX];

	CC_SYNTHESIZE(int, mLevel, Level);
};

class IconCell;
class RewardItem : public BaseLayout, public CQEventDispatcher, public cocos2d::CCTargetedTouchDelegate
{
public:

	RewardItem(vmsg::ACT_LIST_TYPE eActType);
	virtual ~RewardItem();

	static RewardItem* create(vmsg::ACT_LIST_TYPE eActType);

	// override start

	virtual bool init();
	virtual bool onTouchBegan(CCTouch *touch, CCEvent *unused_event);
	virtual void onTouchEnded(CCTouch *touch, CCEvent *unused_event);

	// override end

	void	setInfo(int _level, int lvValue, enumActTipTextConsts eText, bool bCfgState, int realState ,bool isNoLeft=false);

	void	setRewardItemLevel(int _level, enumActTipTextConsts eText);
	void	setRewardItemLast(int _cur, int _max);
	void	setBtnState(unsigned int iState, bool isShow = true);


	void	setSumChargeRewardIcon();
	void	setSumLoginRewardIcon();
	void	setDailyChargeRewardIcon();
	void	setFightGeekRewardIcon();
	void	setGradeGeekRewardIcon();
	void setVipGiftRewardIcon();
	void setDailyConsumeRewardIcon();
	void setSumConsumeRewardIcon();

	bool onTouch(const CCPoint& stGlobalPt);
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	void initUI();

protected:

	vmsg::ACT_LIST_TYPE	mRewardTabType;

	UILabel*		mRewardRMB;
	UILabel*		mRewaardLastText;
	UIImageView*	mRewardIcon1;
	UIImageView*	mRewardIcon2;
	UILayout*		mRewardLayout;

	IconCell*		mIconCell1;
	IconCell*		mIconCell2;

	CC_SYNTHESIZE(int, mLevel, Level);
	RewardStateBtn*		mButtonState;
};






#endif//ACT_REWARD_ITEM_H