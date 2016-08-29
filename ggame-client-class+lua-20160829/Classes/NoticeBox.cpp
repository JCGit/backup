#include "NoticeBox.h"
#include "GameMessages.h"
#include "MessageManager.h"

REGISTER_PAGE("NoticeBox", NoticeBox);

NoticeBox::NoticeBox(void)
{
	_clear();
}

NoticeBox::~NoticeBox(void)
{
	mCb = NULL;
}

void NoticeBox::Enter(MainFrame*)
{
	CCB_FUNC(this, "mTitle", CCLabelTTF, setString(mTitle.c_str()));
	CCB_FUNC(this, "mPopMsg1", CCLabelBMFont, setString(mMsg1.c_str()));
	CCB_FUNC(this, "mPopMsg2", CCLabelBMFont, setString(mMsg2.c_str()));
	std::string color = VaribleManager::Get()->getSetting("colorPurple");
	CCB_FUNC(this,"mPopMsg2",CCLabelBMFont,setColor(StringConverter::parseColor3B(color)));
	CCB_FUNC(this, "mPopMsg3", CCLabelBMFont, setString(mMsg3.c_str()));
	CCB_FUNC(this, "mConfirm", CCLabelTTF, setString(mBtnText.c_str()));
}

void NoticeBox::Exit(MainFrame*)
{
	_clear();
}

void NoticeBox::load()
{
	loadCcbiFile("RebateCouponPopUp.ccbi");
	CCB_FUNC(this, "mClose", CCMenuItem, setVisible(false));
}

void NoticeBox::onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender)
{
	if ( itemName == "onClose")
	{
		_popSelf();
		_clear();
	}
	else if( itemName == "onButton")
	{
		_popSelf();
		if ( mCb ) 
			mCb();
		_clear();
	}
}

void NoticeBox::showPage(std::string noticeMsg1,std::string noticeMsg2,std::string noticeMsg3, NoticeCallback cb, const std::string btnLang/* ="@Confirm" */, std::string title/* ="" */)
{
	MsgMainFramePushPage gameMsg;
	gameMsg.pageName = "NoticeBox";
	MessageManager::Get()->sendMessage(&gameMsg);

	NoticeBox* page = dynamic_cast<NoticeBox* >(CCBManager::Get()->getPage("NoticeBox"));
	if ( page )
	{
		page->mTitle = title;
		page->mMsg1 = noticeMsg1;
		page->mMsg2 = noticeMsg2;
		page->mMsg3 = noticeMsg3;
		page->mBtnText = Language::Get()->getString(btnLang);
		page->mCb = cb;
	}
}

void NoticeBox::_clear()
{
	mTitle = "";
	mMsg1 = "";
	mMsg2 = "";
	mMsg3 = "";
	mBtnText = "";
	mCb = NULL;
}

void NoticeBox::_popSelf()
{
	MsgMainFramePopPage msg;
	msg.pageName = "NoticeBox";
	MessageManager::Get()->sendMessage(&msg);
}