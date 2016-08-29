#include "LoadingLayer.h"
#include "defines.h"
#include <climits>
using namespace std;
CLoadingLayer::CLoadingLayer(){
    m_stSpAnimate = NULL;
    m_stAnimate = NULL;
	m_stBgSprite = NULL;
	m_bIsInLoading = false;

	m_pTarget = NULL;
	m_pEventSelector = NULL;
}

CLoadingLayer::~CLoadingLayer(){
    CC_SAFE_RELEASE_NULL(m_stAnimate);
}

bool CLoadingLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCLayer::init());
        
        setAccelerometerEnabled(false);

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/loading_ui.plist");

        CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
        setTouchEnabled(true);

		m_stBgSprite = CCSprite::create();
        m_stBgSprite->initWithSpriteFrameName("loading_ui/loading_tip.png");
        m_stBgSprite->setPosition(visibleOrigin + ccp(size.width/2, size.height/2));
        addChild(m_stBgSprite, 100);
        
        //动画
        m_stSpAnimate = CCSprite::create();
        m_stSpAnimate->initWithSpriteFrameName("loading_ui/1.png");
        m_stSpAnimate->setPosition(ccp(m_stBgSprite->getContentSize().width/2 - 100, m_stBgSprite->getContentSize().height/2));

        CCAnimation* animation = CCAnimation::create();
        string str;
        for( int i = 0; i < 12; i++)  {  
            str = "loading_ui/" + to_string(i + 1) + ".png";
            //animation->addSpriteFrameWithFileName(str.c_str()); //加载动画的帧  
            animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str.c_str()));
        }  
        animation->setDelayPerUnit((float)0.06);
        animation->setLoops(-1); 

        m_stAnimate = CCAnimate::create(animation);
        m_stAnimate->retain();


        m_stBgSprite->addChild(m_stSpAnimate);

		bRet = true;
	} while(0);

	return bRet;
}

void CLoadingLayer::startCountdown(){
     
	timeFlag = 10; //10秒钟倒计时
	this->schedule(schedule_selector(CLoadingLayer::updateTime), 1);
}

void CLoadingLayer::showLoading(){
    CCScene *pScene = dynamic_cast<CCScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(NULL == pScene) {
        CCLOG("[error]CLoadingLayer::showLoading fail,running scene is null.panel=%s", typeid(*this).name());
        return;
    }
    CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    m_stBgSprite->setPosition(visibleOrigin + ccp(size.width/2, size.height/2));
    m_stBgSprite->stopAllActions();

	m_bIsInLoading = true;
    m_stSpAnimate->runAction(m_stAnimate);
    pScene->addChild(this, 1000);
    startCountdown();
}

void CLoadingLayer::showLoading(CCObject* target, SEL_CallFunc callbackFun){
	m_pTarget = target;
	m_pEventSelector = callbackFun;

	showLoading();
}

void CLoadingLayer::doLoadingDone(){
    m_stBgSprite->stopAllActions();
    m_stSpAnimate->stopAction(m_stAnimate);
    this->removeFromParent();
	m_bIsInLoading = false;
}


void CLoadingLayer::updateTime(float dt){
	if (timeFlag > 1) { 
		timeFlag--;
	} else {
		continueGame();//这是关键5 返回场景
		this->unschedule(schedule_selector(CLoadingLayer::updateTime));
	}
}

void CLoadingLayer::continueGame(){
	doLoadingDone();
	//DispatchEvent(TIME_UP, this);	

	if(m_pTarget && m_pEventSelector){
		(m_pTarget->*m_pEventSelector)();
	} 
}

void CLoadingLayer::registerWithTouchDispatcher(){  
    //使用int最小值做最高优先级,并且吞掉事件true  
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);
}

bool CLoadingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) { 
	CCPoint pointL = m_stBgSprite->getPosition();
	CCPoint pointR = m_stBgSprite->getPosition();
	pointL = ccpAdd(pointL, ccp(-3, -3));
	pointR = ccpAdd(pointR, ccp(3, 3));
	CCMoveTo* moveLeft = CCMoveTo::create(0.08f, pointL);
	CCMoveTo* moveRight = CCMoveTo::create(0.08f, pointR);
	CCFiniteTimeAction* action = CCSequence::create(moveLeft,moveRight,NULL);
	CCActionInterval* actionShake = CCRepeat::create((CCActionInterval*)action, 5);

	if(m_stBgSprite->numberOfRunningActions() > 0){
		m_stBgSprite->stopAllActions();
	}

	m_stBgSprite->runAction(actionShake);
	return true;  
}  

void CLoadingLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {  
}  


void CLoadingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {  
}  