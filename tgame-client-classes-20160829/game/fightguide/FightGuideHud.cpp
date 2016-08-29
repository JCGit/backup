#include "FightGuideHud.h"
#include "FightGuideControl.h"
#include "FightGuideScene.h"
#include "QEnvSet.h"

CFightGuideHud::CFightGuideHud(){
	m_iDirection = -1;
	m_bIsActive = false;
	m_bIsMove = false;
	m_stPos = CCPointZero;
}

CFightGuideHud::~CFightGuideHud(){

}

bool CFightGuideHud::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!UILayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CFightGuideHud::initUI(){
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/city.plist");

	m_pBgImg = ImageView::create();
	m_pBgImg->loadTexture("city/bg_rocker_out.png", UI_TEX_TYPE_PLIST);

	m_pClickImg = FireButton::create();
	m_pClickImg->loadTextures("ui_v2/kong.png", "ui_v2/kong.png", "ui_v2/kong.png");
	m_pClickImg->setTouchPassedEnable(true);
	m_pClickImg->setTouchEnabled(true);
	m_pClickImg->setSize(CCSize(233, 233));
	m_pBgImg->addChild(m_pClickImg);

	m_pHudImg = ImageView::create();
	m_pHudImg->loadTexture("city/btn_rocker.png", UI_TEX_TYPE_PLIST);

	addChild(m_pBgImg);
	m_pBgImg->addChild(m_pHudImg);

	setSize(m_pBgImg->getSize());

	m_pClickImg->addTouchEventListener(this, toucheventselector(CFightGuideHud::touchEventHandler));

	setDirect(UP_TYPE);

	if(QEnvSet::beck()){
		m_bIsMove = true;
	}
}

void CFightGuideHud::updatePos(float delta){
	CCPoint stPos = m_stPos - getPosition();
	float xPos = stPos.x;
	float yPos = stPos.y;
	if(m_iDirection == UP_TYPE){
		xPos = 0;
		if(stPos.y < 0){
			yPos = 0;
		}else if(stPos.y > m_pBgImg->getSize().height/2){
			yPos = m_pBgImg->getSize().height/2;
		}
	}else if(m_iDirection == DOWN_TYPE){
		xPos = 0;
		if(stPos.y > 0){
			yPos = 0;
		}else if(stPos.y < -(m_pBgImg->getSize().height/2)){
			yPos = -(m_pBgImg->getSize().height/2);
		}
	}
	m_pHudImg->setPosition(ccp(xPos, yPos));

	//CCLOG("yyy =========== %f", stPos.y);
}

void CFightGuideHud::setDirect(int direct){
	if(!QEnvSet::beck()){
		m_bIsMove = false;
	}
	m_iDirection = direct;

	//if(direct == UP_TYPE){
	//	m_pClickImg->setPosition(ccp(0, m_pClickImg->getSize().height/2));
	//}else if(direct == DOWN_TYPE){
	//	m_pClickImg->setPosition(ccp(0, -m_pClickImg->getSize().height/2));
	//}
}

void CFightGuideHud::setActive(bool isActive){
	m_bIsActive = isActive;
	m_pHudImg->setPosition(CCPointZero);
	this->setVisible(isActive);
	m_pClickImg->setTouchEnabled(isActive);
}

void CFightGuideHud::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_BEGAN:
		this->schedule(schedule_selector(CFightGuideHud::updatePos));
		m_stPos = m_pClickImg->getTouchStartPos();
		m_bIsMove = true;
		startMove();
		//if(checkInRect(m_stPos - getPosition())){
		//	m_bIsMove = true;
		//	startMove();
		//}else{
		//	m_bIsMove = false;
		//}
		break;
	case TOUCH_EVENT_MOVED:
		m_stPos = m_pClickImg->getTouchMovePos();
		//if(!checkInRect(m_stPos - getPosition())){
		//	endTouch();
		//}else{
		//	if(!m_bIsMove){
		//		m_bIsMove = true;
		//		startMove();
		//	}
		//}
		break;
	case TOUCH_EVENT_ENDED:
		endTouch();
		this->unschedule(schedule_selector(CFightGuideHud::updatePos));
		break;
	case TOUCH_EVENT_CANCELED:
		endTouch();
		this->unschedule(schedule_selector(CFightGuideHud::updatePos));
		break;
	default:
		break;
	}
}

void CFightGuideHud::startMove(){
	if(QEnvSet::beck()){
		//P_FIGHT_GUIDE_CTRL->getGuideScene()->mainHeroShoot();
	}

	P_FIGHT_GUIDE_CTRL->getGuideScene()->moveHero();
}

bool CFightGuideHud::checkInRect(CCPoint stPos){
	bool bRet = true; 

	if(m_iDirection == UP_TYPE){
		if(stPos.y > 0 && stPos.y < m_pBgImg->getSize().height/2){
			bRet = true;
		}else{
			bRet = false;
		}
	}else if(m_iDirection == DOWN_TYPE){
		if(stPos.y < 0 && stPos.y < -(m_pBgImg->getSize().height/2)){
			bRet = true;
		}else{
			bRet = false;
		}
	}

	return bRet;
}

void CFightGuideHud::endTouch(){
	m_stPos = _touchEndPos;
	resetPos();
	m_bIsMove = false;

	if(m_iDirection == UP_TYPE){
		P_FIGHT_GUIDE_CTRL->getGuideScene()->showMoveFireGuide(m_iDirection);
	}else if(m_iDirection == DOWN_TYPE){
		P_FIGHT_GUIDE_CTRL->getGuideScene()->showMoveFireGuide(m_iDirection);
	}
}

void CFightGuideHud::resetPos(){
	m_pHudImg->setPosition(CCPointZero);
}


