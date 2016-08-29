#include "TransitionLayer.h"

const string CTransitionLayer::TRANSITION_MID_EVENT = "transition_mid_event";
const string CTransitionLayer::TRANSITION_COMPLETE = "transition_complete";

CTransitionLayer::CTransitionLayer(){

}

CTransitionLayer::~CTransitionLayer(){

}

bool CTransitionLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(size.width + largeOffset * 2, size.height + largeOffset * 2);

		m_pLayer = UILayout::create();
		m_pLayer->setPosition(point);
		addChild(m_pLayer);

		ccColor3B c = {25, 0, 0};
		m_pLayer->setBackGroundColor(c);
		m_pLayer->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pLayer->setBackGroundColorOpacity(255);
		m_pLayer->setTouchEnabled(true);
		m_pLayer->setSize(largeSize);

		bRet = true;
	} while(0);

	return bRet;
}

void CTransitionLayer::startAct(){
	float duration = 1.0f;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
	float aspect = size.width / size.height;
	int x = (int)(12 * aspect);
	int y = 12;
	CCActionInterval* outAct = CCFadeOutBLTiles::create(duration, CCSizeMake(x,y));
	CCActionInterval* action = CCFadeOutTRTiles::create(duration, CCSizeMake(x,y));
	CCCallFunc* midFun =  CCCallFunc::create(this, callfunc_selector(CTransitionLayer::midAct));
	CCCallFunc* finishFun =  CCCallFunc::create(this, callfunc_selector(CTransitionLayer::finishAct));
	CCActionInterval* seqAct = CCSequence::create(outAct->reverse(), midFun, action, finishFun, CCStopGrid::create(), NULL);
	m_pLayer->getVirtualRenderer()->runAction(seqAct);
}

void CTransitionLayer::midAct(){
	DispatchEvent(TRANSITION_MID_EVENT, this);
}

void CTransitionLayer::finishAct(){
	DispatchEvent(TRANSITION_COMPLETE, this);
	this->removeFromParentAndCleanup(true);
}

