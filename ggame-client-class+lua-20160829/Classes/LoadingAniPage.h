#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "StateMachine.h"
#include "MainFrame.h"
#include "MessageManager.h"
#include "CCBManager.h"
#include "CustomPage.h"
#include "Language.h"
#include "GameMessages.h"

class LoadingAniPage
	:  public CustomPage
	, public State<MainFrame>
	, public CCBContainer::CCBContainerListener
{
public:
	LoadingAniPage(void);
	~LoadingAniPage(void);

	virtual PAGE_TYPE getPageType() {return PT_CONTENT;}

	CREATE_FUNC(LoadingAniPage);

	virtual void load(void);

	void refreshPage(void);

	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender, int tag){};

	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);

	virtual void onAnimationDone(const std::string& animationName);
	//this will execute when the state is entered
	virtual void Enter(MainFrame*);

	//this is the states normal update function
	virtual void Execute(MainFrame*);

	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(MainFrame*);

	void showPage();

	void hidePage();

	void setID(int ids){mID=ids;}

	void runLoadingAni();
	void stopLoadingAni();
	inline bool getIsInWaiting(){return isInWaitingPage;}
private:
	int mID;
	bool isInWaitingPage;
	int mTargetCount;
	float mActionTime;
	float mNextSelTime;
	int mSelCount;
	enum{
		ANIMATION_CHILD = 100,
	};
	enum STAT
	{
		STAT_PREPARE,
		STAT_ANIMATION,
		STAT_GENERAL,
		STAT_SELECTING_SOUL,
		STAT_GETSOUL,
	};
	STAT mStat;
};

#define ShowWaitingPageAni() dynamic_cast<LoadingAniPage*>(CCBManager::Get()->getPage("LoadingAniPage"))->showPage();

#define HideWaitingPageAni() dynamic_cast<LoadingAniPage*>(CCBManager::Get()->getPage("LoadingAniPage"))->hidePage();
