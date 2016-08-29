#include "FallingItem.h"
#include "CfgMaster/ItemCfgMaster.h"
#include "vlib/CVRand.h"
#include "GlobalTime.h"
USING_NS_CC;

FallingItem::FallingItem()
{
}

FallingItem::~FallingItem()
{
}

FallingItem* FallingItem::fall(unsigned int itemID, CCNode* pHero, CCPoint pos)
{
	return FallingItem::fall(itemID, pHero, pos, NULL);
}

FallingItem* FallingItem::fall(unsigned int itemID, CCNode* pHero, CCPoint pos, SEL_CallFuncO selector){
	FallingItem* pItem = FallingItem::create();
	pItem->initUI(itemID, pos);
	pItem->setHero(pHero);
	pHero->getParent()->addChild(pItem);
	pItem->onPickup(pHero, selector);
	pItem->scheduleUpdate();
	return pItem;
}

bool FallingItem::init()
{
	if(!CCSprite::init()) {
		return false;
	}

	m_fScopeWidth = 200;
	m_fScopeYScale = 0.5;
	m_fSpeed = 4;
	m_bIsFlying = false;

	return true;
}

void FallingItem::initUI(unsigned int itemID, CCPoint &pos){
	const ItemCfg* itemCfg = ITEM_CFG_MASTER->GetItemCfgByID(itemID);
	setName(itemCfg->itemname().c_str());
	srand((unsigned int)GlobalTime::getTime());
	//float disX = 1 + (2*getContentSize().width)*rand()/(RAND_MAX + 1);
    float disX = CVLib::CRandom::GetRandHaveMax( (2*getContentSize().width) );
    //float disY = 1 + (2*getContentSize().height)*rand()/(RAND_MAX + 1);
    float disY = CVLib::CRandom::GetRandHaveMax( (2*getContentSize().height) );
	setPosition(ccpAdd(pos, ccp(disX, disY)));
}

void FallingItem::setHero(CCNode* pHero)
{
	pHero->retain();
	m_pHero = pHero;
}

const char* FallingItem::getName()
{
	return m_strName;
}

void FallingItem::setName(const char* name)
{
	m_strName = name;
}

void FallingItem::update(float delta)
{
	CCPoint itemPos, heroPos;
	itemPos = this->getPosition();
	heroPos = m_pHero->getPosition();
	
	if(!m_bIsFlying && m_pHero != NULL && this->checkInScope(itemPos, heroPos)) {
		m_bIsFlying = true;
	}

	if(m_bIsFlying) {
		flyByFrame(m_pHero->getPosition());
	}

	if(itemPos.getDistance(heroPos) < 20) {
		this->unscheduleUpdate();
		m_bIsFlying = false;
		this->setVisible(false);

		if(m_pListener != NULL && m_pfnHandler != NULL) {
			(m_pListener->*m_pfnHandler)(this);
		}
		this->removeFromParentAndCleanup(true);
		m_pHero->release();
	}
}

void FallingItem::flyByFrame(const CCPoint &destPos)
{
	CCPoint itemPos = this->getPosition();
	CCPoint dv = ccpNormalize(destPos - itemPos);
	this->setPosition(itemPos + dv * m_fSpeed);
}

bool FallingItem::checkInScope(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos)
{
	return checkInScopeByRect(itemPos, heroPos);
	//return checkInScopeByCircle(itemPos, heroPos);
}

bool FallingItem::checkInScopeByRect(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos)
{
	CCRect scopeRect = CCRectMake(heroPos.x - m_fScopeWidth, heroPos.y - m_fScopeWidth * m_fScopeYScale, 2 * m_fScopeWidth, 2 * m_fScopeWidth * m_fScopeYScale);
	return scopeRect.containsPoint(itemPos);
}

bool FallingItem::checkInScopeByCircle(const cocos2d::CCPoint &itemPos, const cocos2d::CCPoint &heroPos)
{
	float dx = (itemPos.x - heroPos.x);
	float dy = (itemPos.y - heroPos.y) / m_fScopeYScale;
	return dx * dx + dy * dy < 2 * m_fScopeWidth * m_fScopeWidth;
}

void FallingItem::onPickup(CCObject* pListener, SEL_CallFuncO pfnHandler)
{
	m_pListener = pListener;
	m_pfnHandler = pfnHandler;
}