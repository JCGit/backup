#include "FightHeroHeadUI.h"
#include "defines.h"
#include "UserObj.h"
#include "Resources.h"

#include "HeroCfgMaster.h"

CFightHeroHeadUI::CFightHeroHeadUI()
:hpBar(NULL)
,epBar(NULL)
,hpTxt(NULL)
,epTxt(NULL)
{

}

CFightHeroHeadUI::~CFightHeroHeadUI(){

}

bool CFightHeroHeadUI::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/fight_hero_head_ui.ExportJson");
		addChild(m_pRootLayout);

		hpBar = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("hero_hp"));
		epBar = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("hero_ep"));
		hpTxt = dynamic_cast<UILabel*>(hpBar->getChildByName("hp_txt"));
		epTxt = dynamic_cast<UILabel*>(epBar->getChildByName("ep_txt"));
		hpBar->setPercent(100);
		epBar->setPercent(100);

		m_pHeadImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("head_img"));
		m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lv_txt"));

		m_pNumLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("num_layout"));
		m_pTimeLayou = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("time_layout"));
		m_pNumImg = dynamic_cast<ImageView*>(m_pNumLayout->getChildByName("num_img"));
		m_pMinTxt = dynamic_cast<LabelAtlas*>(m_pTimeLayou->getChildByName("minute_txt"));
		m_pSceTxt = dynamic_cast<LabelAtlas*>(m_pTimeLayou->getChildByName("second_txt"));

		m_pNumTxt = ImageNumber::create(10);
		m_pNumTxt->setNum("40", -1);
		float xx = m_pNumImg->getPositionX() + 80;
		m_pNumTxt->setPosition(ccp(xx, m_pNumTxt->getContentSize().height/2));
		m_pNumLayout->addNode(m_pNumTxt);

		updateUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CFightHeroHeadUI::updateUI(){
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();

	m_pLvTxt->setText(intToString(pUserInfo->level));

	unsigned int uAvatarID = P_USER_OBJ->getUserInfo()->showID;
	std::string url = HEAD_ICON_PATH + HERO_CFG_MASTER->GetHeroCfgByID(uAvatarID)->headicon() + PNG_TYPE_NAME;
	m_pHeadImg->loadTexture(url.c_str());
}

void CFightHeroHeadUI::setHPPercent(int nowHP, int maxHP){
	int percent = 100.0*nowHP/maxHP;
	hpBar->setPercent(percent);
	
	string hpStr = to_string(nowHP) + "/" + to_string(maxHP);
	hpTxt->setText(hpStr);
}

void CFightHeroHeadUI::setEPPercent(int nowEP, int maxEP){
	int percent = 100.0*nowEP/maxEP;
	epBar->setPercent(percent);

	string epStr = to_string(nowEP) + "/" + to_string(maxEP);
	epTxt->setText(epStr);
}

const CCPoint& CFightHeroHeadUI::getHeadPos(){
	return m_pHeadImg->getPosition();
}

