#pragma once
#include "MainFrame.h"
#include "BasePage.h"
#include "Language.h"
#include "libOS.h"
class ActiveCodePage
	: public BasePage
	, public libOSListener
{
public:
	ActiveCodePage(void);
	~ActiveCodePage(void);

	CREATE_FUNC(ActiveCodePage);

	virtual void load(void);
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
	virtual void onInputboxEnter(const std::string& content);
	void inputMethodCallBack(const std::string& content);
private:	
	std::string mInputStr;
};

