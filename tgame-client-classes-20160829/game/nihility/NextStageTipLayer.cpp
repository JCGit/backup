#include "NextStageTipLayer.h"
#include "fight/FightTipLayout.h"

CNextStageTipLayer::CNextStageTipLayer(){
	m_pLayer = NULL;
}

CNextStageTipLayer::~CNextStageTipLayer(){

}

bool CNextStageTipLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(size.width + largeOffset * 2, size.height + largeOffset * 2);

		m_pLayer = UILayout::create();
		m_pLayer->setTouchEnabled(true);
		m_pLayer->setSize(largeSize);
		addChild(m_pLayer);
		
		m_pNextStageTip = CFightTipLayout::create();
		m_pNextStageTip->setPosition(ccp(size.width/2 - m_pNextStageTip->getSize().width/2, size.height*3/5));
		m_pLayer->addChild(m_pNextStageTip);

		bRet = true;
	} while(0);

	return bRet;
}

void CNextStageTipLayer::startAct(){
	m_pNextStageTip->startAct();
}

