#include "ConfirmPage.h"
#include "GameMessages.h"
#include "MessageManager.h"

REGISTER_PAGE("ConfirmPage", ConfirmPage);

ConfirmPage::ConfirmPage(void)
{
	_clear();
}

ConfirmPage::~ConfirmPage(void)
{
}

void ConfirmPage::Enter(MainFrame*)
{
	CCB_FUNC(this, "mDecisionTex", CCLabelBMFont, setString(mMsg.c_str()));
	CCB_FUNC(this, "mTitle", CCLabelBMFont ,setString( mTitle.c_str() ));
}

void ConfirmPage::Exit(MainFrame*)
{
	_clear();
}

void ConfirmPage::load()
{
	loadCcbiFile("GeneralDecisionPopUp.ccbi");
}

void ConfirmPage::onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender)
{
	if ( itemName == "onConfirmation" || itemName == "onCancel" || itemName == "onClose")
	{
		_popSelf();
		if ( mCb ) 
			mCb(itemName == "onConfirmation");
		_clear();
	}
}

void ConfirmPage::showPage(std::string confirmMsg,std::string confirmTitle, ConfirmCallback cb)
{
	MsgMainFramePushPage gameMsg;
	gameMsg.pageName = "ConfirmPage";
	MessageManager::Get()->sendMessage(&gameMsg);

	ConfirmPage* page = dynamic_cast<ConfirmPage* >(CCBManager::Get()->getPage("ConfirmPage"));
	if ( page )
	{
		page->mMsg = confirmMsg;
		page->mTitle = confirmTitle;
		page->mCb = cb;
	}
}

void ConfirmPage::_clear()
{
	mMsg = "";
	mCb = NULL;
}

void ConfirmPage::_popSelf()
{
	MsgMainFramePopPage msg;
	msg.pageName = "ConfirmPage";
	MessageManager::Get()->sendMessage(&msg);
}