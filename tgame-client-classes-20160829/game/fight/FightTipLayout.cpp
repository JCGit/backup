#include "FightTipLayout.h"

CFightTipLayout::CFightTipLayout(){
	_isActing = false;
}

CFightTipLayout::~CFightTipLayout(){

}

bool CFightTipLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui/fight_next_tip_ui.json");
		addChild(m_pRootLayout);

		m_pGoImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("go_img"));
		m_pArrowImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("tip_arrow"));

		bRet = true;
	} while(0);

	return bRet;
}

void CFightTipLayout::startAct(){
	stopAct();

	float duration = 0.5;

	m_pArrowImg->setScale(2.0f);
	m_pArrowImg->setOpacity(0x00);
	CCActionInterval *arrowPop = CCFadeIn::create(duration);
	CCActionInterval* arrowScaleTo = CCScaleTo::create(duration, duration);
	CCFiniteTimeAction* arrowSpawn = CCSpawn::create(arrowPop, arrowScaleTo, NULL);
	CCFiniteTimeAction* arrowCallback = CCCallFunc::create(this, callfunc_selector(CFightTipLayout::moveFun));
	CCSequence* stSequence = CCSequence::create(arrowSpawn, arrowCallback, NULL);
	m_pArrowImg->getVirtualRenderer()->runAction(stSequence);

	m_pGoImg->setScale(2.0f);
	m_pGoImg->setOpacity(0x00);
	CCActionInterval *pOptionAction = CCFadeIn::create(duration);
	CCActionInterval* scaleTo = CCScaleTo::create(duration, duration);
	CCFiniteTimeAction* spawn = CCSpawn::create(pOptionAction, scaleTo, NULL);
	CCFiniteTimeAction* goCallback = CCCallFunc::create(this, callfunc_selector(CFightTipLayout::scaleFun));

	CCSequence* stGoSequence = CCSequence::create(spawn, goCallback, NULL);
	m_pGoImg->getVirtualRenderer()->runAction(stGoSequence);

	_isActing = true;
}

void CFightTipLayout::scaleFun(){
	float duration = 0.5;
	CCActionInterval* scale = CCScaleBy::create(duration, 1.25f);
	CCSequence* stGoSequence = CCSequence::create(scale, scale->reverse(), NULL);
	m_pGoImg->runAction(CCRepeatForever::create(stGoSequence));
}

void CFightTipLayout::moveFun(){
	float duration = 0.5;
	CCActionInterval* move = CCMoveBy::create(duration, ccp(15, 0));
	CCSequence* stSequence = CCSequence::create(CCShow::create(), move, move->reverse(), NULL);
	m_pArrowImg->runAction(CCRepeatForever::create(stSequence));
}

void CFightTipLayout::stopAct(){
	if(m_pGoImg->numberOfRunningActions() > 0){
		m_pGoImg->stopAllActions();
	}

	if(m_pGoImg->getVirtualRenderer()->numberOfRunningActions() > 0){
		m_pGoImg->getVirtualRenderer()->stopAllActions();
	}

	if(m_pArrowImg->numberOfRunningActions() > 0){
		m_pArrowImg->stopAllActions();
	}

	if(m_pArrowImg->getVirtualRenderer()->numberOfRunningActions() > 0){
		m_pArrowImg->getVirtualRenderer()->stopAllActions();
	}

	_isActing = false;
}