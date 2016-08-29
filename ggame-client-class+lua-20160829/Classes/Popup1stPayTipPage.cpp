
#include "stdafx.h"

#include "Popup1stPayTipPage.h"
#include "GameMessages.h"
#include "DataTableManager.h"
#include "GameMaths.h"
#include "json/json.h"
#include "GamePlatform.h"
REGISTER_PAGE("Popup1stPayTipPage",Popup1stPayTipPage);
USING_NS_CC;

Popup1stPayTipPage::Popup1stPayTipPage(void)
{
}


Popup1stPayTipPage::~Popup1stPayTipPage(void)
{
}
void Popup1stPayTipPage::load( void )
{
	loadCcbiFile("PopUp1stPay.ccbi");
}

void Popup1stPayTipPage::showPage(Popup1stPay_Page_Type _type)
{
	BasePage* obj = CCBManager::Get()->getPage("Popup1stPayTipPage");
	Popup1stPayTipPage* page = dynamic_cast<Popup1stPayTipPage*>(obj);
	if(page)
	{
		page->mType=_type;
	}
	MsgMainFramePushPage msg;
	msg.pageName="Popup1stPayTipPage";
	MessageManager::Get()->sendMessage(&msg);
	
}
void Popup1stPayTipPage::_refreshPage()
{
	std::string outstr;
	//int lines;
	int width = StringConverter::parseInt(VaribleManager::Get()->getSetting("PopupPage_Msg"));
	std::list<std::string> strList;
	if(mType==GoldNotEnough)
	{
		strList.push_back(Language::Get()->getString("@ZuanShi"));
		CCB_FUNC(this,"mTitle",CCLabelTTF,setString(Language::Get()->getString("@BUYNotEnoughRMB").c_str()));
	}
	else if(mType==SilverNotEnough)
	{
		strList.push_back(Language::Get()->getString("@BeiLi"));
		CCB_FUNC(this,"mTitle",CCLabelTTF,setString(Language::Get()->getString("@BUYNotEnoughBL").c_str()));
	}
	/*GameMaths::stringAutoReturn(GameMaths::replaceStringWithStringList(Language::Get()->getString("@FstPayRewardNote1"),&strList),outstr,width,lines);*/
	CCB_FUNC(this,"mPopMsg",CCLabelBMFont,setString(Language::Get()->getString("@FstPayRewardNote1").c_str()));
	CCB_FUNC(this,"mPopMsg2",CCLabelBMFont,setString(Language::Get()->getString("@FstPayRewardNote2").c_str()));

	CCB_FUNC(this,"mFunction1",CCLabelTTF,setString(Language::Get()->getString("@Close").c_str()));

	CCB_FUNC(this,"mFunction",CCLabelTTF,setString(Language::Get()->getString("@Recharge").c_str()));
	//modify by dylan at 20131116 
	strList.clear();
	std::string _rechargeMaxLimit="10000";
#ifdef WIN32
#else
	const PlatformRoleItem* item=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
	if(item)
	{
		_rechargeMaxLimit=item->rechargeMaxLimit;
	}
#endif
	strList.push_back(_rechargeMaxLimit);
	CCB_FUNC(this,"mRewardFor1stPay",CCLabelBMFont,setString(GameMaths::replaceStringWithStringList(Language::Get()->getString("@RewardNote1"),&strList).c_str()));
	readFirstPayInfo();

}
void Popup1stPayTipPage::Enter( MainFrame* )
{
	_refreshPage();
}
void Popup1stPayTipPage::Execute( MainFrame* )
{

}

void Popup1stPayTipPage::Exit( MainFrame* )
{
}

void Popup1stPayTipPage::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{
	if(itemName=="onFunction2")
	{
		MsgMainFramePushPage msg;
		msg.pageName = "RechargePage";
		MessageManager::Get()->sendMessage(&msg);
	}
	MsgMainFramePopPage msg;
	msg.pageName="Popup1stPayTipPage";
	MessageManager::Get()->sendMessage(&msg);
}

void Popup1stPayTipPage::readFirstPayInfo()
{
	Json::Reader jreader;
	Json::Value data;
	unsigned long filesize;
	char* pBuffer = (char*)getFileData(
		cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("pay1st.cfg").c_str(),
		"rt",&filesize);

	if(!pBuffer)
	{
		char msg[256];
		sprintf(msg,"Failed open file: pay1st.cfg !!");
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
	}
	else
	{
		jreader.parse(pBuffer,data,false);
		CC_SAFE_DELETE_ARRAY(pBuffer);

		if(data["version"].asInt()==1)
		{
			CCB_FUNC(this,"mItem1Pic",CCSprite,setTexture(data["icon1"].asString().c_str()));
			CCB_FUNC(this,"mItem2Pic",CCSprite,setTexture(data["icon2"].asString().c_str()));
			CCB_FUNC(this,"mItem3Pic",CCSprite,setTexture(data["icon3"].asString().c_str()));
			CCB_FUNC(this,"mItem4Pic",CCSprite,setTexture(data["icon4"].asString().c_str()));
			std::string str = data["description1"].asString();
			CCB_FUNC(this,"mReward1",CCLabelBMFont,setString(str.c_str()));
			str = data["description2"].asString();
			CCB_FUNC(this,"mReward2",CCLabelBMFont,setString(str.c_str()));
			str = data["description3"].asString();
			CCB_FUNC(this,"mReward3",CCLabelBMFont,setString(str.c_str()));
			str = data["description4"].asString();
			CCB_FUNC(this,"mReward4",CCLabelBMFont,setString(str.c_str()));
			if(data["quality1"].asInt()>0)
			{
				CCB_FUNC(this,"mItem1Frame",CCSprite,setColor(::getFrameColor(data["quality1"].asInt())));
			}
			if(data["quality2"].asInt()>0)
			{
				CCB_FUNC(this,"mItem2Frame",CCSprite,setColor(::getFrameColor(data["quality2"].asInt())));
			}
			if(data["quality3"].asInt()>0)
			{
				CCB_FUNC(this,"mItem3Frame",CCSprite,setColor(::getFrameColor(data["quality3"].asInt())));
			}
			if(data["quality4"].asInt()>0)
			{
				CCB_FUNC(this,"mItem4Frame",CCSprite,setColor(::getFrameColor(data["quality4"].asInt())));
			}

			CC_SAFE_DELETE_ARRAY(pBuffer);
		}
	}
	CC_SAFE_DELETE_ARRAY(pBuffer);
}