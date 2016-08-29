#include "EvolutionCell.h"

#include "defines.h"
#include "Resources.h"

#include "CfgMaster/SkillCfgMaster.h"

#include "basic/CCColorUtil.h"

EvolutionCell::EvolutionCell()
:m_eStat(CELL_DARK)
{
}

EvolutionCell::~EvolutionCell()
{
}

void EvolutionCell::setStat(CellStat stat) {
	m_eStat = stat;
}

EvolutionStarCell::EvolutionStarCell()
:m_pStarImg(NULL)
,m_uColor(STAR_GREEN)
{

}

EvolutionStarCell::~EvolutionStarCell()
{

}

//EvolutionStarCell* EvolutionStarCell::create(UILayout *pRefLayout) {
//	EvolutionStarCell *pRet = new EvolutionStarCell();
//
//	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
//		pRet->autorelease();
//	} else {
//		CC_SAFE_DELETE(pRet);
//	}
//
//	return pRet;
//}
//
//bool EvolutionStarCell::initWithLayout(UILayout *pRefLayout) {
//	bool bRet = false;
//
//	do {
//		CC_BREAK_IF(!BaseLayout::init());
//
//		initLayoutWithUI(pRefLayout);
//
//		m_pStarImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("dark_img"));
//
//		setStat(m_eStat);
//
//		bRet = true;
//	} while(0);
//
//	return bRet;
//}

EvolutionStarCell* EvolutionStarCell::create() {
	EvolutionStarCell *pRet = new EvolutionStarCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EvolutionStarCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UIWidget::init());

		m_pStarImg = UIImageView::create();
		addChild(m_pStarImg);

		setStat(m_eStat);

		bRet = true;
	} while(0);

	return bRet;
}

void EvolutionStarCell::setStat(CellStat stat) {
	EvolutionCell::setStat(stat);

	string url = "";

	switch(m_uColor) {
		case STAR_GREEN:
			url = stat == CELL_LIGHT ? "evolution/star_green_light.png" : "evolution/star_green_dark.png";
			break;
		case STAR_BLUE:
			url = stat == CELL_LIGHT ? "evolution/star_blue_light.png" : "evolution/star_blue_dark.png";
			break;
		case STAR_PURPLE:
			url = stat == CELL_LIGHT ? "evolution/star_purple_light.png" : "evolution/star_purple_dark.png";
			break;
		case STAR_YELLOW:
			url = stat == CELL_LIGHT ? "evolution/star_yellow_light.png" : "evolution/star_yellow_dark.png";
			break;
		case STAR_RED:
			url = stat == CELL_LIGHT ? "evolution/star_red_light.png" : "evolution/star_red_dark.png";
			break;
		default:;
	}

	m_pStarImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	ignoreContentAdaptWithSize(false);
	setSize(m_pStarImg->getSize());
}

void EvolutionStarCell::setColor(StarColor color) {
	m_uColor = color;
}

EvolutionSkillCell::EvolutionSkillCell()
:m_pBgIconImg(NULL)
,m_pSkillIconImg(NULL)
{

}

EvolutionSkillCell::~EvolutionSkillCell() 
{

}

//EvolutionSkillCell* EvolutionSkillCell::create(UILayout *pRefLayout) {
//	EvolutionSkillCell *pRet = new EvolutionSkillCell();
//
//	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
//		pRet->autorelease();
//	} else {
//		CC_SAFE_DELETE(pRet);
//	}
//
//	return pRet;
//}
//
//bool EvolutionSkillCell::initWithLayout(UILayout *pRefLayout) {
//	bool bRet = false;
//
//	do {
//		CC_BREAK_IF(!BaseLayout::init());
//
//		initLayoutWithUI(pRefLayout);
//
//		m_pBgIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_icon_img"));
//
//		m_pSkillIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("skill_icon_img"));
//
//		setStat(m_eStat);
//
//		bRet = true;
//	} while(0);
//
//	return bRet;
//}

EvolutionSkillCell* EvolutionSkillCell::create() {
	EvolutionSkillCell *pRet = new EvolutionSkillCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EvolutionSkillCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UIWidget::init());

		if(CCTextureCache::sharedTextureCache()->textureForKey("ui_v2/fight.png") == NULL) {
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist", "ui_v2/fight.png");
		}
		
		m_pBgIconImg = UIImageView::create();
		addChild(m_pBgIconImg);
		m_pBgIconImg->loadTexture("fight/fight_skill_btn_bg.png", UI_TEX_TYPE_PLIST);
		ignoreContentAdaptWithSize(false);
		setSize(m_pBgIconImg->getSize());

		m_pSkillIconImg = UIImageView::create();
		m_pSkillIconImg->ignoreContentAdaptWithSize(false);
		m_pSkillIconImg->setSize(CCSizeMake(118, 118));
		addChild(m_pSkillIconImg);

		setStat(m_eStat);

		bRet = true;
	} while(0);

	return bRet;
}

void EvolutionSkillCell::setStat(CellStat stat) {
	EvolutionCell::setStat(stat);

	m_pBgIconImg->setVisible(true);

	if(stat == CELL_LIGHT) {
		CCColorUtil::removeGray(m_pBgIconImg->getVirtualRenderer());
		CCColorUtil::removeGray(m_pSkillIconImg->getVirtualRenderer());
	} else {
		CCColorUtil::addGray(m_pBgIconImg->getVirtualRenderer());
		CCColorUtil::addGray(m_pSkillIconImg->getVirtualRenderer());
	}
	
}

void EvolutionSkillCell::setSkill(unsigned int uSkillID) {
	const SkillCfg *pSkillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(uSkillID);

	if(pSkillCfg != NULL) {
		std::string path = SKILLICON_RES_PATH + pSkillCfg->skillicon() + ".png";
		m_pSkillIconImg->loadTexture(path.c_str());
		setStat(m_eStat);
	}
	
}
