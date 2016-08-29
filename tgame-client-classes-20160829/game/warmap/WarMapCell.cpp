#include "WarMapCell.h"

#include "defines.h"

WarMapCell::WarMapCell()
:m_uWarMapID(0)
,m_uWarStat(WAR_STAT_LOCKED)
,m_pWarImg(NULL)
,m_pStar1Img(NULL)
,m_pStar2Img(NULL)
,m_pStar3Img(NULL)
{
}

WarMapCell::~WarMapCell()
{
}

WarMapCell* WarMapCell::create() {
	WarMapCell *pRet = new WarMapCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WarMapCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UIWidget::init());

		ignoreContentAdaptWithSize(false);

		m_pWarImg = UIImageView::create();
		addChild(m_pWarImg);

		m_pStar1Img = UIImageView::create();
		addChild(m_pStar1Img);

		m_pStar2Img = UIImageView::create();
		addChild(m_pStar2Img);

		m_pStar3Img = UIImageView::create();
		addChild(m_pStar3Img);

		setStarNum(0);

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int WarMapCell::getWarMapID() {
	return m_uWarMapID;
}

void WarMapCell::setWarMapID(unsigned int uWarMapID) {
	m_uWarMapID = uWarMapID;
}

unsigned int WarMapCell::getWarStat() {
	return m_uWarStat;
}

void WarMapCell::setWarStat(WarStat stat) {
	changeWarStatLogic(stat);
}

void WarMapCell::setStarNum(unsigned int num) {
	m_pStar1Img->setVisible(true);
	m_pStar2Img->setVisible(true);
	m_pStar3Img->setVisible(true);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/war_map.plist");

	if(num == 3){
		m_pStar1Img->loadTexture("war_map/little_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/little_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/little_star_light.png", UI_TEX_TYPE_PLIST);
	}else if(num == 2){
		m_pStar1Img->loadTexture("war_map/little_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/little_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/little_star_dark.png", UI_TEX_TYPE_PLIST);
	}else if(num == 1){
		m_pStar1Img->loadTexture("war_map/little_star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/little_star_dark.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/little_star_dark.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pStar1Img->setVisible(false);
		m_pStar2Img->setVisible(false);
		m_pStar3Img->setVisible(false);
	}

	CCSize size = getSize();
	float dx = size.width / 4;
	float dy = size.height / 2;
	m_pStar1Img->setPosition(ccp(-1.2f * dx, -dy));
	m_pStar2Img->setPosition(ccp(0, -dy - m_pStar2Img->getSize().height / 4));
	m_pStar3Img->setPosition(ccp(1.2f * dx, -dy));
}

void WarMapCell::changeWarStatLogic(WarStat stat) {
	m_uWarStat = stat;

	if(m_uWarStat == WAR_STAT_LOCKED) {
		m_pWarImg->loadTexture("war_map/war_cell_locked.png", UI_TEX_TYPE_PLIST);
	} else if(m_uWarStat == WAR_STAT_OPENED) {
		m_pWarImg->loadTexture("war_map/war_cell_opened.png", UI_TEX_TYPE_PLIST);
	} else if(m_uWarStat == WAR_STAT_PASSED) {
		m_pWarImg->loadTexture("war_map/war_cell_passed.png", UI_TEX_TYPE_PLIST);
	}
	
	setSize(m_pWarImg->getSize());
	setTouchEnabled(m_uWarStat != WAR_STAT_LOCKED);
}

StageWarMapCell::StageWarMapCell()
{
}

StageWarMapCell::~StageWarMapCell()
{
}

StageWarMapCell* StageWarMapCell::create() {
	StageWarMapCell *pRet = new StageWarMapCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool StageWarMapCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!WarMapCell::init());

		bRet = true;
	} while(0);

	return bRet;
}

void StageWarMapCell::setStarNum(unsigned int num) {
	m_pStar1Img->setVisible(true);
	m_pStar2Img->setVisible(true);
	m_pStar3Img->setVisible(true);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/war_map.plist");

	if(num == 3){
		m_pStar1Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
	}else if(num == 2){
		m_pStar1Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/star_dark.png", UI_TEX_TYPE_PLIST);
	}else if(num == 1){
		m_pStar1Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/star_dark.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/star_dark.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pStar1Img->setVisible(false);
		m_pStar2Img->setVisible(false);
		m_pStar3Img->setVisible(false);
	}

	CCSize size = getSize();
	float dx = size.width / 4;
	float dy = size.height / 2;
	m_pStar1Img->setPosition(ccp(-1.2f * dx, -dy));
	m_pStar2Img->setPosition(ccp(0, -dy - m_pStar2Img->getSize().height / 4));
	m_pStar3Img->setPosition(ccp(1.2f * dx, -dy));
}

void StageWarMapCell::changeWarStatLogic(WarStat stat) {
	m_uWarStat = stat;

	if(m_uWarStat == WAR_STAT_LOCKED) {
		string url = string("key_stages/stage_") + intToString(m_uWarMapID) + string("_locked.png");
		m_pWarImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	} else if(m_uWarStat == WAR_STAT_OPENED) {
		string url = string("key_stages/stage_") + intToString(m_uWarMapID) + string("_opened.png");
		m_pWarImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	} else if(m_uWarStat == WAR_STAT_PASSED) {
		string url = string("key_stages/stage_") + intToString(m_uWarMapID) + string("_opened.png");
		m_pWarImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	}

	setTouchEnabled(m_uWarStat != WAR_STAT_LOCKED);
	setSize(m_pWarImg->getSize());

}

EliteWarMapCell::EliteWarMapCell()
{
}

EliteWarMapCell::~EliteWarMapCell()
{
}

EliteWarMapCell* EliteWarMapCell::create() {
	EliteWarMapCell *pRet = new EliteWarMapCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EliteWarMapCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!WarMapCell::init());

		m_pElitePassImg = UIImageView::create();
		addChild(m_pElitePassImg, 5);
		m_pElitePassImg->loadTexture("war_map/stamp_passed.png", UI_TEX_TYPE_PLIST);
		m_pElitePassImg->setRotation(10);
		m_pElitePassImg->setPosition(ccp(2, -45));
		m_pElitePassImg->setVisible(false);

		bRet = true;
	} while(0);

	return bRet;
}

void EliteWarMapCell::changeWarStatLogic(WarStat stat) {
	m_uWarStat = stat;

	m_pElitePassImg->setVisible(false);
	if(m_uWarStat == WAR_STAT_LOCKED) {
		string url = string("elite_stages/stage_") + intToString(m_uWarMapID % 5 == 0 ? 5 : m_uWarMapID % 5) + string("_locked.png");
		m_pWarImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	} else if(m_uWarStat == WAR_STAT_OPENED) {
		string url = string("elite_stages/stage_") + intToString(m_uWarMapID % 5 == 0 ? 5 : m_uWarMapID % 5) + string("_opened.png");
		m_pWarImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	} else if(m_uWarStat == WAR_STAT_PASSED) {
		m_pElitePassImg->setVisible(true);
		string url = string("elite_stages/stage_") + intToString(m_uWarMapID % 5 == 0 ? 5 : m_uWarMapID % 5) + string("_opened.png");
		m_pWarImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	}

	setTouchEnabled(m_uWarStat == WAR_STAT_OPENED);
	setSize(m_pWarImg->getSize());
}
