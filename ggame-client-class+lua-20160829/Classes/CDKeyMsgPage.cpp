
#include "stdafx.h"

#include "CDKeyMsgPage.h"
#include "MessageBoxPage.h"
#include "libOS.h"
REGISTER_PAGE("CDKeyMsgPage",CDKeyMsgPage);
USING_NS_CC;

CDKeyMsgPage::CDKeyMsgPage(void)
{
}


CDKeyMsgPage::~CDKeyMsgPage(void)
{
	libOS::getInstance()->removeListener(this);
	MessageManager::Get()->removeMessageHandler(this);
}

void CDKeyMsgPage::Enter(MainFrame*)
{
	libOS::getInstance()->registerListener(this);
	CCObject* obj=getVariable("mTitle");
	if(!obj)
	{
		char errMsg[256];
		sprintf(errMsg,"Failed to find variable:mTitle");
		CCMessageBox(errMsg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
	}
	mTitle =  dynamic_cast<cocos2d::CCLabelBMFont* >(obj);
	if(mTitle)
	{
		mTitle->setString(Language::Get()->getString("@CDKeyTitle").c_str());
	}
	mMessage =  dynamic_cast<cocos2d::CCLabelTTF* >(getVariable("mMessage"));
}

void CDKeyMsgPage::Exit( MainFrame* frame)
{
	libOS::getInstance()->removeListener(this);
	MessageManager::Get()->removeMessageHandler(this);
}

void CDKeyMsgPage::Execute( MainFrame* )
{

}

void CDKeyMsgPage::load( void )
{
	loadCcbiFile("LeaveMsgPage.ccbi");
}

void CDKeyMsgPage::onInputboxEnter(const std::string& content)
{
	inputMethodCallBack(content);
}

void CDKeyMsgPage::inputMethodCallBack(const std::string& content)
{
	mInputStr=content;
	//modify by dai 20140120 添加cdkey单行字数限制
	mMessage->setString(mInputStr.c_str());
	int lines;
	GameMaths::stringAutoReturn(content,mInputStr,StringConverter::parseInt(VaribleManager::Get()->getSetting("CDKeyLineWordCount"),19),lines);
	CCB_FUNC(this,"mMessage",CCLabelTTF,setString(mInputStr.c_str()));
}

void CDKeyMsgPage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{
	if(itemName=="onXClose")
	{
		MsgMainFramePopPage msg;
		msg.pageName="CDKeyMsgPage";
		MessageManager::Get()->sendMessage(&msg);

		libOS::getInstance()->removeListener(this);
	}
	else if(itemName=="onClose")
	{
		MsgMainFramePopPage msg;
		msg.pageName="CDKeyMsgPage";
		MessageManager::Get()->sendMessage(&msg);

		libOS::getInstance()->removeListener(this);
	}
	else if(itemName=="onCloseButton")
	{
		MsgMainFramePopPage msg;
		msg.pageName="CDKeyMsgPage";
		MessageManager::Get()->sendMessage(&msg);

		libOS::getInstance()->removeListener(this);
	}
	else if(itemName=="onSendMsgButton")
	{
		
		if(mInputStr.length()>13)
		{
			//CDkey 兑换，暂时注释掉CDKey兑换逻辑，后期可能会添加 by zhenhui 2014/7/30
			/*OPCdkeyReward msg;
			msg.set_cdkey(mInputStr);
			PacketManager::Get()->sendPakcet(OPCODE_CDKEY_REWARD_C,&msg);*/
		}
		else
		{
			MSG_BOX(Language::Get()->getString("@CDKeyLengthError"));
		}
		
	}
	else if(itemName=="onTouchInput")
	{
		libOS::getInstance()->showInputbox(true);
	}
}

void CDKeyMsgPage::onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender, int tag)
{

}


void CDKeyMsgPage::onReceiveMassage( const GameMessage * message )
{
	if(message->getTypeId() == MSG_SEVERINFO_UPDATE)
	{
		const MsgSeverInfoUpdate * msg = dynamic_cast<const MsgSeverInfoUpdate*>(message);
		/*if(msg->opcode==OPCODE_CDKEY_REWARDRET_S)
		{

		}*/
	}
}
