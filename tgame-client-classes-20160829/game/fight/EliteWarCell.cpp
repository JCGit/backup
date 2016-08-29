#include "EliteWarCell.h"

#include "CCColorUtil.h"

EliteWarCell::EliteWarCell()
:m_pEliteWarLockedImg(NULL)
,m_pEliteWarOpenedImg(NULL)
,m_pEliteWarPassedImg(NULL)
,m_pEliteWarBossImg(NULL)
,m_pStar1Img(NULL)
,m_pStar2Img(NULL)
,m_pStar3Img(NULL)
,m_pStampImg(NULL)
,m_uWarMapID(0)
,m_uStat(ELITE_WAR_LOCKED)
{
}

EliteWarCell::~EliteWarCell()
{
}

EliteWarCell* EliteWarCell::create() {
	EliteWarCell *pRet = new EliteWarCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EliteWarCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/EliteWarCellUI.ExportJson");
		addChild(m_pRootLayout);

		m_pEliteWarLockedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("elite_war_locked_img"));

		m_pEliteWarOpenedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("elite_war_opened_img"));

		m_pEliteWarPassedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("elite_war_passed_img"));

		m_pEliteWarBossImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("boss_img"));

		m_pStar1Img = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("star_1_img"));

		m_pStar2Img = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("star_2_img"));

		m_pStar3Img = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("star_3_img"));

		m_pStampImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("stamp_img"));

		setStat(ELITE_WAR_LOCKED);
		setStarNum(0);

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int EliteWarCell::getWarMapID() {
	return m_uWarMapID;
}

void EliteWarCell::setWarMapID(unsigned int uWarMapID) {
	m_uWarMapID = uWarMapID;
}

void EliteWarCell::setStat(EliteWarStat stat) {
	m_uStat = stat;

	setTouchEnabled(m_uStat == ELITE_WAR_OPENED);

	m_pEliteWarLockedImg->setVisible(m_uStat == ELITE_WAR_LOCKED);
	m_pEliteWarOpenedImg->setVisible(m_uStat == ELITE_WAR_OPENED);
	m_pEliteWarPassedImg->setVisible(m_uStat == ELITE_WAR_PASSED);
	m_pStampImg->setVisible(m_uStat == ELITE_WAR_PASSED);

	m_pStar1Img->setVisible(m_uStat == ELITE_WAR_PASSED);
	m_pStar2Img->setVisible(m_uStat == ELITE_WAR_PASSED);
	m_pStar3Img->setVisible(m_uStat == ELITE_WAR_PASSED);

	if(m_uStat == ELITE_WAR_LOCKED) {
		CCColorUtil::addGray(m_pEliteWarBossImg->getVirtualRenderer());
	} else {
		CCColorUtil::removeGray(m_pEliteWarBossImg->getVirtualRenderer());
	}
}

void EliteWarCell::setStarNum(unsigned int num) {
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/level_panel_ui.plist");
	if(num == 3){
		m_pStar1Img->loadTexture("level_panel_ui/pass_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("level_panel_ui/pass_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("level_panel_ui/pass_star_light.png", UI_TEX_TYPE_PLIST);
	} else if(num == 2){
		m_pStar1Img->loadTexture("level_panel_ui/pass_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("level_panel_ui/pass_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("level_panel_ui/pass_star_dark.png", UI_TEX_TYPE_PLIST);
	} else if(num == 1){
		m_pStar1Img->loadTexture("level_panel_ui/pass_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("level_panel_ui/pass_star_dark.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("level_panel_ui/pass_star_dark.png", UI_TEX_TYPE_PLIST);
	} else {
		m_pStar1Img->loadTexture("level_panel_ui/pass_star_dark.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("level_panel_ui/pass_star_dark.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("level_panel_ui/pass_star_dark.png", UI_TEX_TYPE_PLIST);
	}
}
