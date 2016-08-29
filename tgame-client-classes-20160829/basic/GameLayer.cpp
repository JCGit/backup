#include "GameLayer.h"

#define VALID_HIT_DISTANCE 5.0f

GameLayer::GameLayer()
:m_pHitEventListener(NULL)
,m_pfnHitEventSelector(NULL)
{
}

GameLayer::~GameLayer()
{
}

GameLayer* GameLayer::create() {
	GameLayer* pRet = new GameLayer();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GameLayer::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCLayer::init());

		bRet = true;
	} while(0);

	return bRet;
}

void GameLayer::onEnter() {
	setTouchMode(kCCTouchesOneByOne);
	setTouchEnabled(true);
	CCLayer::onEnter();
}

void GameLayer::onExit() {
	setTouchEnabled(false);
	CCLayer::onExit();
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	return true;
}

void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {

}

void GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
	CCPoint startPos = pTouch->getStartLocation();
	CCPoint endPos = pTouch->getLocation();

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();

	CCRect rockerRect = CCRectMake(screenOrigin.x, screenOrigin.y, screenSize.width / 3 + 20.0f, screenSize.height / 2 + 20.0f);

	if(!rockerRect.containsPoint(startPos) && !rockerRect.containsPoint(endPos)) {
		if(endPos.getDistance(startPos) < VALID_HIT_DISTANCE) { // ÓÐÐ§µã»÷
			if(m_pHitEventListener != NULL && m_pfnHitEventSelector != NULL) {
				(m_pHitEventListener->*m_pfnHitEventSelector)(this, convertToNodeSpace(endPos));
			}
		}
	}
	
}

void GameLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {

}

void GameLayer::addHitEventListener(CCObject *pListener, SEL_HitEvent pfnSelector) {
	m_pHitEventListener = pListener;
	m_pfnHitEventSelector = pfnSelector;
}
