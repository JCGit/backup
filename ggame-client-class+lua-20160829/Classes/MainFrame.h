#pragma once


#include "cocos2d.h"
#include "cocos-ext.h"
#include "StateMachine.h"
#include "GamePrecedure.h"
#include "MessageManager.h"
#include "CCBManager.h"
#include "Language.h"
#include "GamePackets.h"
#include "lib91.h"
#include "libOS.h"
#include "effects/CCGridBlur.h"

// class MainPage;
// class TeamPage;
using namespace cocos2d; 

#define PUSH_TAG_BLUR 100232

class MainFrame
	: public BasePage
	, public State<GamePrecedure>
	, public MessageHandler
    , public platformListener
	, public libOSListener
{
private:
	
	enum MsgFrameErrCode
	{
		RELOGIN_ERROR=101,
		UserKickout=103,
	};

	cocos2d::CCScene* mScene;


	StateMachine<MainFrame> *m_pStateMachine;

	cocos2d::CCNode * mNodeMid;
	cocos2d::CCNode * mNodeBack;
	cocos2d::CCNode * mNodeFront;
	cocos2d::CCNode * mNodeFight;
	cocos2d::CCNode * mNodeMsg;
	cocos2d::CCNode* mNodeMsgForLua;
	cocos2d::CCLayer * mNoTouch;
	cocos2d::CCNode * mMainFrameButtons;

	BasePage* mCurShowPage;
	BasePage* mFightPage;
	typedef std::map<std::string, BasePage* > PageList;
	PageList mPageList;

	CCGridBlur*		mGridBlur;


	bool isInChooseRolePage;
	std::string mCurShowPageName;

	void showCover(const std::string& pagename);
	void hideCover();

	void showMsg();

	void _showPage(const std::string& name);
	
	void _initFightPage();

	std::string mLastChangePageName;

public:
	MainFrame(void);
	~MainFrame(void);

	static MainFrame* getInstance();
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);

	CREATE_FUNC(MainFrame);
    virtual void onReLogin();
	virtual void load(void);

	void showPage(const std::string& name);
	inline BasePage* getCurShowPage() {return mCurShowPage;}
	std::string getCurShowPageName() { return mCurShowPageName; }

	CCNode * getMsgNodeForLua(){return mNodeMsgForLua;};

	void pushPage(const std::string& name);
	void popPage(const std::string& name);
	void popPage(BasePage* page);
	void doPopPageByName(CCNode* node,void* param);
	void doPopPageByPage(CCNode* node,void* param);
	void popAllPage();
	void showFightPage();
	bool isPageInList(BasePage* page);
	bool isPageInList(const std::string& name);

	void onScaleToShowDone(CCNode* node,void* param);

	//this will execute when the state is entered
	virtual void Enter(GamePrecedure*);

	//this is the states normal update function
	virtual void Execute(GamePrecedure*);

	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(GamePrecedure*);

	virtual void onReceiveMassage(const GameMessage * message);
	//void _setContent(cocos2d::CCNode* child, bool zero = true);

	void reEnter(GamePrecedure*);

	void _loadPopPage();
	void showNoTouch();
	void hideNoTouch();
	void resetAllMenuCCBI();

	std::string lastChangePageName(){ return mLastChangePageName;};

	void onPlatformLogout(libPlatform*);
	void onMessageboxEnter(int tag);
	//
	CCScene* getRootSceneNode();
	/*
		platformListener
	*/
	virtual void onRequestBindTryUserToOkUser(const char* tyrUin, const char* okUin);
	virtual void onTryUserRegistSuccess();

	//ÉèÖÃ±³¾°Ä£ºý
	void setBlurEnabled(bool b);
	void setBlurEnabledForMultiPush(bool b);

	void removeAllPageFromNode(CCNode* node);

	void setChildVisible(const char* childName, bool visible);
};

