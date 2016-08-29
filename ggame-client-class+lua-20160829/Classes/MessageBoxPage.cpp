
#include "stdafx.h"

#include "MessageBoxPage.h"
#include "ServerDateManager.h"
#include "StringConverter.h"
#include "DataTableManager.h"
#include "BlackBoard.h"
#include "CCBManager.h"
#include "Language.h"
#include "GameMessages.h"

USING_NS_CC;
MessageBoxPage::MessageBoxPage(void)
{

}

MessageBoxPage::~MessageBoxPage(void)
{
}

void MessageBoxPage::load( void )
{
	loadCcbiFile("MessageBox.ccbi");
}

void MessageBoxPage::showMsg( std::string msgString )
{
	bool quietRestart = ServerDateManager::Get()->getServerWillRestart();
	if (quietRestart)
	{
		CCLOG("@MessageBoxPage::showMsg -- quiet restart");
		return;
	}
	load();
	CCLabelBMFont* message=dynamic_cast<CCLabelBMFont* >(getVariable("mMsg"));
	if(message)
	{
		message->setString(Language::Get()->getString(msgString).c_str());
	}

	MsgMainFrameMSGShow msg;
	MessageManager::Get()->sendMessage(&msg);
}

void MessageBoxPage::Enter( MainFrame* )
{
	this->stopAllActions();
	this->setPosition(0, 0);
	//this->runAction(CCSequence::create(CCShow::create(),
	//	CCMoveBy::create(0.3f, CCPoint(0,20)), 
	//	CCDelayTime::create(1.0f),  
	//	CCHide::create(), 
	//	NULL));
	/*this->runAction(CCSequence::create(CCShow::create(),
		CCDelayTime::create(1.0f),  
		CCHide::create(),
		NULL));*/

	CCNode* root = dynamic_cast<CCNode*>(getVariable("mRootNode"));
	if (root)
	{
		CCArray* array = root->getChildren();
		for (unsigned int i = 0;i < array->count(); ++i)
		{
			cocos2d::extension::CCScale9Sprite* child = dynamic_cast<cocos2d::extension::CCScale9Sprite*>(array->objectAtIndex(i));
			if (child)
			{
				child->stopAllActions();
				child->setOpacity(255);
				child->runAction(CCSequence::create(CCDelayTime::create(1.0f),  
					CCFadeOut::create(0.3f),
					NULL));
				continue;
			}
		}
	}
	CCLabelBMFont* msg = dynamic_cast<CCLabelBMFont*>(getVariable("mMsg"));
	if (msg)
	{
		msg->stopAllActions();
		msg->setOpacity(255);
		msg->runAction(CCSequence::create(CCDelayTime::create(1.0f),  
			CCFadeOut::create(0.3f),
			NULL));
	}	
}

void MessageBoxPage::Execute( MainFrame* )
{

}

void MessageBoxPage::Exit( MainFrame* )
{

}

void MessageBoxPage::Msg_Box(std::string msgString)
{
	MessageBoxPage::Get()->showMsg(msgString);
}

void MessageBoxPage::Msg_Box_Lan( std::string msgString )
{
	MessageBoxPage::Get()->showMsg(Language::Get()->getString(msgString));
}

void MessageBoxPage::showMsgByErrCode(unsigned int errId)
{
	std::string msg; 
	const ErrMsgItem* item = ErrMsgTableManager::Get()->getErrMsgItemByID(errId);
	if (item)
	{
		msg = item->msgContent;
	}
	MessageBoxPage::Get()->showMsg(msg);
}
