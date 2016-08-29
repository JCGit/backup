#ifndef __HTMLTEST_H__
#define __HTMLTEST_H__
#pragma once
#include "MainFrame.h"
#include "CustomPage.h"
#include "cocos2d.h"
#include <renren-ext.h>
#include "MessageManager.h"
#include "GameMessages.h"
USING_NS_CC;
USING_NS_CC_EXT;

class TestHTMLPopup 
	:public CustomPage
	, public State<MainFrame>
{
public:
	TestHTMLPopup(void);
	~TestHTMLPopup(void);
	virtual PAGE_TYPE getPageType() {return PT_CONTENT;}
	CREATE_FUNC(TestHTMLPopup);
	virtual void Enter(MainFrame*);
	virtual void Execute(MainFrame*);
	virtual void Exit(MainFrame*);
	virtual void load(void);
	virtual void unload(void);
	bool		 _init();
	virtual void onMenuItemAction(const std::string& itemName,cocos2d::CCObject* sender);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);

	// HTML events
	void onHTMLClicked(
		IRichNode* root, IRichElement* ele, int _id);
	void onHTMLMoved(
		IRichNode* root, IRichElement* ele, int _id,
		const CCPoint& location, const CCPoint& delta);
    
    // implement the "static node()" method manually
};

#endif // __HTMLTEST_H__
