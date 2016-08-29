#include "FightGuideDialog.h"
#include "FightGuideControl.h"
#include "UserObj.h"
#include "Resources.h"
#include "FightGuideControl.h"

#include "HeroCfgMaster.h"
#include "FightGuideCfgMaster.h"

CFightGuideDialog::CFightGuideDialog(){
	m_pBgLayout = NULL;
}

CFightGuideDialog::~CFightGuideDialog(){

}

bool CFightGuideDialog::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCPoint screenOrigin = pDirector->getVisibleOrigin(); // 屏幕原点
		CCSize screenSize = pDirector->getVisibleSize(); // 屏幕大小

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(screenSize.width + largeOffset * 2, screenSize.height + largeOffset * 2);

		m_pBgLayout = UILayout::create();
		m_pBgLayout->setSize(largeSize);
		m_pBgLayout->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pBgLayout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pBgLayout->setBackGroundColorOpacity(0x40);
		addChild(m_pBgLayout);

		//setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		//setBackGroundColorType(LAYOUT_COLOR_SOLID);
		//setBackGroundColorOpacity(0x40);

		setSize(screenSize);
		setPosition(screenOrigin);

		initLayoutFromJsonFile("ui_v2/fight_guide_dialog_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pBgWordsImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_words_img"));
		m_pBgWordsImg->setTouchEnabled(true);
		m_pBgWordsImg->addTouchEventListener(this, toucheventselector(CFightGuideDialog::touchEventHandler));

		m_pNameBgImg = dynamic_cast<UIImageView*>(m_pBgWordsImg->getChildByName("name_bg"));
		m_pNameBgImg->setPosition(ccp(242, 231));

		m_pLightImg = dynamic_cast<UIImageView*>(m_pBgWordsImg->getChildByName("light_img"));
		m_pLightImg->setSize(CCSizeMake(m_pBgWordsImg->getSize().width - 235 ,35));
		m_pLightImg->setPosition(ccp(149, 18));

		m_pRoleHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_head_img"));
		m_pRoleHeadImg->setPosition(ccp(-m_pRoleHeadImg->getSize().width, 0));

		m_pBgWordsImg->setSize(CCSizeMake(screenSize.width - 320, 256));
		m_pBgWordsImg->setPosition(ccp(getSize().width, 0));

		m_pRoleNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("role_name_txt"));
		m_pRoleNameTxt->setPosition(ccp(562, 230));

		m_pWordsTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("words_txt"));
		m_pWordsTxt->setSize(CCSizeMake(m_pBgWordsImg->getSize().width - 140, 128));
		m_pWordsTxt->setPosition(ccp(423, 183));

		m_pTipBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("tip_bg"));
		m_pTipBg->setPosition(ccp(m_pWordsTxt->getPositionX() + m_pWordsTxt->getSize().width/2, m_pTipBg->getPositionY()));

		setName("dialog_layout");
		//m_pRootLayout->setTouchEnabled(true);
		addTouchEventListener(this, toucheventselector(CFightGuideDialog::touchEventHandler));

		bRet = true; 
	} while(0);

	return bRet;
}

void CFightGuideDialog::showDialog(){
	unsigned int uStepID = P_FIGHT_GUIDE_CTRL->getStepID();
	const FightGuideCfg* stCfg = FIGHT_GUIDE_CFG->GetFightGuideCfgByID(uStepID);

	string url = "";
	if(stCfg->isplayer() > 0){  //主角
		unsigned int uAvatarID = P_USER_OBJ->getUserInfo()->showID;
		url = BIGHEAD_ICON_PATH + HERO_CFG_MASTER->GetHeroCfgByID(uAvatarID)->headicon() + ".png";
		m_pRoleNameTxt->setText(P_USER_OBJ->getUserInfo()->nick);

		
		m_pBgWordsImg->setScaleX(1.0f);
		m_pBgWordsImg->setPositionX(getSize().width - m_pBgWordsImg->getSize().width);
		m_pRoleHeadImg->setPositionX(0.0f);
		m_pNameBgImg->setPositionX(242);
		m_pWordsTxt->setPositionX(423);
		m_pRoleNameTxt->setPositionX(562);
	}else{
		url = BIGHEAD_ICON_PATH + "fight_guide.png";
		m_pRoleNameTxt->setText(stCfg->monname());

		m_pBgWordsImg->setScaleX(-1.0f);
		m_pBgWordsImg->setPositionX(m_pBgWordsImg->getSize().width);
		m_pRoleHeadImg->setPositionX(getSize().width - m_pRoleHeadImg->getSize().width);
		m_pNameBgImg->setPositionX(780);
		m_pWordsTxt->setPositionX(38);
		m_pRoleNameTxt->setPositionX(179);
	}

	m_pRoleHeadImg->loadTexture(url.c_str());
	m_pWordsTxt->setText(stCfg->desc());
	m_pTipBg->setPosition(ccp(m_pWordsTxt->getPositionX() + m_pWordsTxt->getSize().width/2, m_pTipBg->getPositionY()));
}

void CFightGuideDialog::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		P_FIGHT_GUIDE_CTRL->nextStep();
		break;
	default:
		break;
	}
}
