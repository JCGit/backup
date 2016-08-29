#pragma once
#include "CustomPage.h"
#include "MainFrame.h"

typedef void (* NoticeCallback)();

class NoticeBox
	: public CustomPage
	, public State<MainFrame>
{
public:
	NoticeBox(void);
	~NoticeBox(void);

	virtual PAGE_TYPE getPageType() {return PT_CONTENT;}

	CREATE_FUNC(NoticeBox);
	virtual void Enter(MainFrame*);
	virtual void Execute(MainFrame*) {};
	virtual void Exit(MainFrame*);
	virtual void load(void);
	void showPage(std::string noticeMsg1,std::string noticeMsg2,std::string noticeMsg3, NoticeCallback cb, const std::string btnLang = "@Confirm", std::string title = "");

	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
private:
	void _clear();
	void _popSelf();

	std::string mTitle;
	std::string mMsg1;
	std::string mMsg2;
	std::string mMsg3;
	std::string mBtnText;
	NoticeCallback mCb;
};

#define PopNoticeBox(msg1,msg2,msg3, cbFunc, btnLang, title) dynamic_cast<NoticeBox* >(CCBManager::Get()->getPage("NoticeBox"))->showPage(msg1,msg2,msg3, cbFunc, btnLang, title);