#include "ActiveCodePage.h"
#include "MessageBoxPage.h"
#include "libPlatform.h"
USING_NS_CC;

ActiveCodePage::ActiveCodePage(void)
{
	load();
	libOS::getInstance()->registerListener(this);
	CCB_FUNC(this,"mTitle",CCLabelBMFont,setString(Language::Get()->getString("@ActiveCodeTitle").c_str()) );
}


ActiveCodePage::~ActiveCodePage(void)
{
	libOS::getInstance()->removeListener(this);
}

void ActiveCodePage::load( void )
{
	loadCcbiFile("PopActiveCode.ccbi");
}

void ActiveCodePage::onInputboxEnter(const std::string& content)
{
	inputMethodCallBack(content);
}

void ActiveCodePage::inputMethodCallBack(const std::string& content)
{
	mInputStr=content;
	CCB_FUNC(this,"mMessage",CCLabelBMFont,setString(mInputStr.c_str()) );
}

void ActiveCodePage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{

	if(itemName=="onSen")
	{

		if(mInputStr.length()>13)
		{
			//暂时注释掉激活码功能， by zhenhui 2014/7/30
			/*OPActiveCode msg;
			msg.set_uid(libPlatformManager::getPlatform()->loginUin());
			msg.set_activecode(mInputStr);
			PacketManager::Get()->sendPakcet(OPCODE_ACTIVE_CODE_C,&msg);*/
		}
		else
		{
			CCMessageBox(Language::Get()->getString("@ActiveCodeLengthError").c_str(),Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		}

	}
	else if(itemName=="onTouchInput")
	{
		libOS::getInstance()->showInputbox(true);
	}
}

