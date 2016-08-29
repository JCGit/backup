#pragma once
#include "MainFrame.h"
#include "CustomPage.h"
enum Popup1stPay_Page_Type
{
	SilverNotEnough,
	GoldNotEnough,
};
class Popup1stPayTipPage
	: public CustomPage
	, public State<MainFrame>
{
public:
	Popup1stPayTipPage(void);
	~Popup1stPayTipPage(void);
	virtual PAGE_TYPE getPageType() {return PT_CONTENT;}
	CREATE_FUNC(Popup1stPayTipPage);
	virtual void Enter(MainFrame*);
	virtual void Execute(MainFrame*);
	virtual void Exit(MainFrame*);
	virtual void load(void);
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
	void showPage(Popup1stPay_Page_Type _type);
	Popup1stPay_Page_Type mType;
private:
	void _refreshPage();
	void readFirstPayInfo();
};
#define  POP_1STPAY_TIP_BOX(mType) dynamic_cast<Popup1stPayTipPage*>(CCBManager::Get()->getPage("Popup1stPayTipPage"))->showPage(mType);