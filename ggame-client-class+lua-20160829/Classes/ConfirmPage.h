#pragma once

#include "CustomPage.h"
#include "MainFrame.h"

typedef void (*ConfirmCallback)(bool);
class ConfirmPage
	: public CustomPage
	, public State<MainFrame>
{
public:
	ConfirmPage(void);
	~ConfirmPage(void);

	virtual PAGE_TYPE getPageType() {return PT_CONTENT;}

	CREATE_FUNC(ConfirmPage);
	virtual void Enter(MainFrame*);
	virtual void Execute(MainFrame*) {};
	virtual void Exit(MainFrame*);
	virtual void load(void);
	void showPage(std::string confirmMsg,std::string confirmTitle, ConfirmCallback cb);

	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
private:
	void _clear();
	void _popSelf();

	std::string mMsg;
	std::string mTitle;
	ConfirmCallback mCb;
};

#define ShowConfirmPage(msg,title, cbFunc) dynamic_cast<ConfirmPage* >(CCBManager::Get()->getPage("ConfirmPage"))->showPage(msg,title, cbFunc);