#include "ReviveLayer.h"
#include "defines.h"
#include <climits>
CReviveLayer::CReviveLayer(){

}

CReviveLayer::~CReviveLayer(){

}

bool CReviveLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCLayer::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		m_pLayer = UILayout::create();
		addChild(m_pLayer);

		m_pLayer->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pLayer->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pLayer->setBackGroundColorOpacity(0x40);
		setTouchEnabled(true);
		m_pLayer->setSize(size);

		m_pTxt = UILabel::create();
		m_pTxt->setFontName("fonts/tgamefont.ttf");
		m_pTxt->setFontSize(80);
		m_pTxt->setColor(ccc3(139, 252, 255));
		m_pTxt->setTouchEnabled(false);
		m_pTxt->setAnchorPoint(ccp(0.5, 0.5));
		m_pTxt->setPosition(ccp(size.width/2, size.height/2));
		m_pLayer->addChild(m_pTxt);

		bRet = true;
	} while(0);

	return bRet;
}

void CReviveLayer::startCountdown(){
	//m_pBeforeTargetSets = CCDirector::sharedDirector()->getScheduler()->pauseAllTargets();
	//m_pBeforeTargetSets->retain();

	timeFlag = 5;
	m_pTxt->setText("5");
	this->schedule(schedule_selector(CReviveLayer::updateTime),1);
}

void CReviveLayer::showWaiting(){
    m_pTxt->setText("waiting for room...");
}

void CReviveLayer::updateTime(float dt){
	if (timeFlag > 1){ 
		timeFlag--;
		m_pTxt->setText(to_string(timeFlag));
	}else{
		continueGame();//这是关键5 返回场景
		this->unschedule(schedule_selector(CReviveLayer::updateTime));
	}
}

void CReviveLayer::continueGame(){
	//CCDirector::sharedDirector()->getScheduler()->resumeTargets(m_pBeforeTargetSets);
	this->removeFromParentAndCleanup(true);

	DispatchEvent(REVIVE_DONE, this);	
}

void CReviveLayer::doWaitingDone(){
    this->removeFromParentAndCleanup(true);
}

void CReviveLayer::registerWithTouchDispatcher(){  
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);//使用int最小值做最高优先级,并且吞掉事件true  
}

bool CReviveLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){     
	return true;  
}  

void CReviveLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){  


}  


void CReviveLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){  

}  