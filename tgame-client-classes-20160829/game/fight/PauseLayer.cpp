#include "PauseLayer.h"
#include "defines.h"
#include <climits>
using namespace std;

CPauseLayer::CPauseLayer(){

}

CPauseLayer::~CPauseLayer(){

}

bool CPauseLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCLayer::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();
		m_pLayer = UILayout::create();
		addChild(m_pLayer);

		m_pLayer->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pLayer->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pLayer->setBackGroundColorOpacity(0x40);
		setTouchEnabled(true);
		m_pLayer->setSize(size);

		m_pNumImg = ImageView::create();
		m_pNumImg->setAnchorPoint(ccp(0.5, 0.5));
		m_pNumImg->setPosition(ccp(size.width/2, size.height/2));
		m_pLayer->addChild(m_pNumImg);

		bRet = true;
	} while(0);

	return bRet;
}

void CPauseLayer::startCountdown(){
	//m_pBeforeTargetSets = CCDirector::sharedDirector()->getScheduler()->pauseAllTargets();
	//m_pBeforeTargetSets->retain();

	//CCDirector::sharedDirector()->pause();

	timeFlag = 3;
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/basic.plist");
	m_pNumImg->loadTexture("share/count_down_3.png", UI_TEX_TYPE_PLIST);
	this->schedule(schedule_selector(CPauseLayer::updateTime),1);
}

void CPauseLayer::updateTime(float dt){
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/basic.plist");
	if (timeFlag > 1){ 
		timeFlag--;
		string str = "share/count_down_" + intToString(timeFlag) + ".png";
		m_pNumImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
	}else{
		continueGame();//这是关键5 返回场景
		this->unschedule(schedule_selector(CPauseLayer::updateTime));
	}
}

void CPauseLayer::continueGame(){
	//CCDirector::sharedDirector()->getScheduler()->resumeTargets(m_pBeforeTargetSets);
	//CC_SAFE_RELEASE_NULL(m_pBeforeTargetSets);

	//CCDirector::sharedDirector()->resume();

	DispatchEvent(RESUME_GAME, this);	
	this->removeFromParentAndCleanup(true);
}

void CPauseLayer::registerWithTouchDispatcher(){  
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);//使用int最小值做最高优先级,并且吞掉事件true  
}

bool CPauseLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){     
	return true;  
}  

void CPauseLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){  


}  


void CPauseLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){  

}  