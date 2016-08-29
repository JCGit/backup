#include "HudLayer.h"


USING_NS_CC;
USING_NS_CC_EXT;
const string HudLayer::TOUCH_EVENT = "touche_hud_event";

HudLayer::HudLayer(void)
{
	_delegate = NULL;
	jsSprite = NULL;
	jsBgSprite = NULL;
}

HudLayer::~HudLayer(void)
{
    if(_delegate) {
        CCLOG("[trace]del HudLayer,delegate %s", typeid(*_delegate).name() );
    } else {
        CCLOG("[trace]del HudLayer" );
    }
}

void HudLayer::updatePos(float dt){
    if(!_delegate) {
        Inactive();
        return;
    }

	jsSprite->setPosition(ccpAdd(jsSprite->getPosition(),ccpMult(ccpSub(currentPoint, jsSprite->getPosition()),0.5)));
	if (_isHeld)
	{
		_delegate->isHoldingDirection(this, _direction);
	}
}

//启动摇杆
void HudLayer::Active()
{
	if (!active) {
		active=true;
		schedule(schedule_selector(HudLayer::updatePos));//添加刷新函数
		CCDirector *pDirector = CCDirector::sharedDirector();
		m_bTouchEnabled = true;
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, 100 - 2, false);//添加触摸委托
	}else {
	}
}

//解除摇杆
void HudLayer::Inactive()
{
	if(active) {
		active=false;
		_direction = CCPointZero;
		CCDirector *pDirector = CCDirector::sharedDirector();
		m_bTouchEnabled = false;
		pDirector->getTouchDispatcher()->removeDelegate(this);//删除委托
		this->unschedule(schedule_selector(HudLayer::updatePos));//删除刷新

		CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("city/bg_rocker_out.png");
		if(pFrame){
			jsBgSprite->setDisplayFrame(pFrame);
		}

		CCSpriteFrame *pJsFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("city/btn_rocker.png");
		if(pJsFrame){
			jsSprite->setDisplayFrame(pJsFrame);
		}

		currentPoint = jsBgSprite->getPosition();
		jsSprite->setPosition(currentPoint);
	}
}

//摇杆方位
CCPoint HudLayer::getDirection()
{
	return ccpNormalize(ccpSub(centerPoint, currentPoint));
}
//摇杆力度
float HudLayer::getVelocity()
{
	return ccpDistance(centerPoint, currentPoint);
}

//第一个参数aPoint：摇杆中心点的坐标；
//第二个参数aRadius: 摇杆的半径
//第三个参数aJsSprite :摇杆的图片资源精灵
//第四个参数aJsBg: 摇杆背景图片资源精灵
//第五个参数isFollowRole：是否让摇杆永远跟随用户触屏点（Himi新添加的功能）
HudLayer* HudLayer::HudLayerWithCenter(CCPoint aPoint, float aRadius, bool _isFollowRole){
	HudLayer *jstick = HudLayer::create();
	jstick->initWithCenter(aPoint, aRadius, _isFollowRole);
	return jstick;
}

bool HudLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (!active){
		return false;
	}

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();

	CCPoint location = touch->getLocation();

	CCRect touchRect = CCRectMake(screenOrigin.x, screenOrigin.y, touchSize.width, touchSize.height);

	if(!touchRect.containsPoint(location)) {
		return false;
	}

	
	float distanceSQ = ccpDistanceSQ(location, this->getPosition());
	if (distanceSQ <= _radius * _radius)
	{
		this->updateDirectionForTouchLocation(location);
		_isHeld = true;
	}

	CCPoint touchPoint = touch->getLocationInView();
	touchPoint = CCDirector:: sharedDirector()->convertToGL(touchPoint);
	if(!isFollowRole){
		if (ccpDistance(touchPoint, centerPoint) > radius){
			//CCLog("hud began false, dist to long at point (%f, %f)", touchPoint.x, touchPoint.y);
			return false;
		}
	}
	
	if(isFollowRole){
		if(ccpDistance(touchPoint, centerPoint) > radius){
			if(touchPoint.x < touchSize.width && touchPoint.y < touchSize.height){   //触摸点在左半屏幕
				currentPoint = touchPoint;
				centerPoint = ccpAdd(getDirection()*_radius, touchPoint);
				
				if(centerPoint.x < initPoint.x){
					centerPoint.x = initPoint.x;
				}

				if(centerPoint.y < initPoint.y){
					centerPoint.y = initPoint.y;
				}
				jsBgSprite->setPosition(centerPoint);

				updateDirectionForTouchLocation(touchPoint);
			}else{
				return false;
			}
		}
	}else{
		currentPoint = touchPoint;
	}

	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("city/bg_rocker_down.png");
	if(pFrame){
		jsBgSprite->setDisplayFrame(pFrame);
	}

	CCSpriteFrame *pJsFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("city/btn_rocker_down.png");
	if(pJsFrame){
		jsSprite->setDisplayFrame(pJsFrame);
	}

//	CCLog("hud began true at point (%f, %f), this %p", touchPoint.x, touchPoint.y, this);
	return true;
}

void  HudLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint location = touch->getLocation();
	this->updateDirectionForTouchLocation(location);

	CCPoint touchPoint = touch->getLocationInView();
	touchPoint = CCDirector:: sharedDirector()->convertToGL(touchPoint);

	CCDirector *pDirector = CCDirector::sharedDirector();

	if(!isFollowRole){
		if (ccpDistance(touchPoint, centerPoint) > radius){
			currentPoint =ccpAdd(centerPoint,ccpMult(ccpNormalize(ccpSub(touchPoint, centerPoint)), radius));
		}else{
			currentPoint = touchPoint;
		}
		return;
	}

	if (ccpDistance(touchPoint, centerPoint) > radius)  //移动出摇杆背景范围
	{
		if(touchPoint.x < touchSize.width && touchPoint.y < touchSize.height){
			currentPoint =ccpAdd(centerPoint,ccpSub(touchPoint, centerPoint));

			centerPoint = ccpAdd(currentPoint,ccpMult(ccpNormalize(ccpSub(centerPoint, currentPoint)), radius));
			if(centerPoint.x < initPoint.x){
				centerPoint.x = initPoint.x;
			}

			if(centerPoint.y < initPoint.y){
				centerPoint.y = initPoint.y;
			}

			currentPoint = ccpAdd(centerPoint,ccpMult(ccpNormalize(ccpSub(currentPoint, centerPoint)), radius));
			jsBgSprite->setPosition(centerPoint);
		}else{
			CCPoint curPos = touchPoint;
			if(touchPoint.x >= touchSize.width){
				curPos.x = touchSize.width;
			}

			if(touchPoint.y >= touchSize.height){
				curPos.y = touchSize.height;
			}

			currentPoint =ccpAdd(centerPoint,ccpSub(curPos, centerPoint));

			centerPoint = ccpAdd(currentPoint,ccpMult(ccpNormalize(ccpSub(centerPoint, currentPoint)), radius));
			if(centerPoint.x < initPoint.x){
				centerPoint.x = initPoint.x;
			}else if(centerPoint.x >= touchSize.width){
				centerPoint.x = touchSize.width;
			}

			if(centerPoint.y < initPoint.y){
				centerPoint.y = initPoint.y;
			}else if(centerPoint.y >= touchSize.height){
				centerPoint.y = touchSize.height;
			}

			currentPoint = ccpAdd(centerPoint,ccpMult(ccpNormalize(ccpSub(currentPoint, centerPoint)), radius));
			jsBgSprite->setPosition(centerPoint);
		}
			
	}else {
		currentPoint = touchPoint;
	}

	//CCLog("hud move at point (%f, %f)", touchPoint.x, touchPoint.y);
}

void  HudLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if(!_delegate) {
        Inactive();
        return;
    }

	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("city/bg_rocker_out.png");
	if(pFrame){
		jsBgSprite->setDisplayFrame(pFrame);
	}

	CCSpriteFrame *pJsFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("city/btn_rocker.png");
	if(pJsFrame){
		jsSprite->setDisplayFrame(pJsFrame);
	}

	_direction = CCPointZero;
	_isHeld = false;
	_delegate->simpleDPadTouchEnded(this);

	currentPoint = centerPoint;
	if(isFollowRole){
		//this->setVisible(false);
	}
	_isHeld = false;
}

HudLayer* HudLayer::initWithCenter(CCPoint aPoint, float aRadius, bool _isFollowRole){
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/city.plist");

	//滑动区域
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCSize visibleSize = pDirector->getVisibleSize();

	touchSize = CCSize(visibleSize.width/3, visibleSize.height/2);

	isFollowRole =_isFollowRole;
	active = false;
	radius = aRadius;
	initPoint = aPoint;
	if(!_isFollowRole){
		centerPoint = aPoint;
	}else{
		//centerPoint = ccp(0,0);
		centerPoint = aPoint;
	}
	_radius = radius;
	_direction = CCPointZero;
	_isHeld = false;
	currentPoint = centerPoint;
	jsSprite = CCSprite::create();//摇杆
	jsSprite->initWithSpriteFrameName("city/btn_rocker.png");
	jsSprite->setPosition(centerPoint);
	jsBgSprite = CCSprite::create();//摇杆
	jsBgSprite->initWithSpriteFrameName("city/bg_rocker_out.png");
	jsBgSprite->setPosition(centerPoint);
	this->addChild(jsBgSprite);
	this->addChild(jsSprite);
	if(isFollowRole){
		//this->setVisible(false);
	}

	this->Active();//激活摇杆
	return this;
}

void HudLayer::setIsFollowRole(bool var){
	if(isFollowRole == var){
		return;
	}

	isFollowRole = var;
	
	centerPoint = initPoint;
	currentPoint = centerPoint;
	_isHeld = false;
	jsBgSprite->setPosition(currentPoint);
	jsSprite->setPosition(centerPoint);
}

void HudLayer::updateDirectionForTouchLocation(CCPoint location)
{
    if(NULL == _delegate) {
        CCLOG("[warn]HudLayer::updateDirectionForTouchLocation, delegate is null.");
        Inactive();
        return;
    }
 	float radians = ccpToAngle(ccpSub(location, centerPoint));
 	float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);
 
 	if (degrees <= 22.5 && degrees >= -22.5) 
 	{
 		//right
 		_direction = ccp(1.0, 0.0);
 	}
 	else if (degrees > 22.5 && degrees < 67.5)
 	{
 		//bottomright
 		_direction = ccp(1.0, -1.0);
 	}
 	else if (degrees >= 67.5 && degrees <= 112.5)
 	{
 		//bottom
 		_direction = ccp(0.0, -1.0);
 	}
 	else if (degrees > 112.5 && degrees < 157.5)
 	{
 		//bottomleft
 		_direction = ccp(-1.0, -1.0);
 	}
 	else if (degrees >= 157.5 || degrees <= -157.5)
 	{
 		//left
 		_direction = ccp(-1.0, 0.0);
 	}
 	else if (degrees < -22.5 && degrees > -67.5)
 	{
 		//topright
 		_direction = ccp(1.0, 1.0);
 	}
 	else if (degrees <= -67.5 && degrees >= -112.5)
 	{
 		//top
 		_direction = ccp(0.0, 1.0);
 	}
 	else if (degrees < -112.5 && degrees > -157.5)
 	{
 		//topleft
 		_direction = ccp(-1.0, 1.0);
 	}
	//_direction = ccpNormalize(ccpSub(location, centerPoint));
    
	_delegate->didChangeDirectionTo(this, _direction);
}

void HudLayer::touchEvent(CCObject *pSender, TouchEventType type)
{
    if(!_delegate) {
        Inactive();
        return;
    }
	CHudTouches stTouch;
	stTouch.touchTime = 0; //通过全局接口去获取时间
	if (_pDownBtn && _pDownBtn->m_uID == pSender->m_uID){
		stTouch.touchKey = E_TOUCH_DOWN;
	}else if (_pLeftBtn && _pLeftBtn->m_uID == pSender->m_uID){
		stTouch.touchKey = E_TOUCH_LEFT;
	}else if (_pRightBtn && _pRightBtn->m_uID == pSender->m_uID){
		stTouch.touchKey = E_TOUCH_RIGHT;
	}else if (_pUpBtn && _pUpBtn->m_uID == pSender->m_uID){
		stTouch.touchKey = E_TOUCH_UP;
	}else if (_pCenterBtn && _pCenterBtn->m_uID == pSender->m_uID){
		stTouch.touchKey = E_TOUCH_CENTER;
	}

	if(stTouch.touchKey != 0){
		_stTouches.push_back(stTouch);
		//check if there is any touch events should be sent
	}

	int count = _stTouches.size();
	HudTouchEvent stEvent;
	stEvent.touchType = E_TOUCH_TYPE_SINGLE;
	if (count >= 2 && stTouch.touchKey != 0){
		if (_stTouches[count - 1].touchKey == _stTouches[count - 2].touchKey 
			&& _stTouches[count - 1].touchTime - _stTouches[count - 2].touchKey <= 0.5){		
			
			//double click
			
			stEvent.touchKey = stTouch.touchKey;
			stEvent.touchType = E_TOUCH_TYPE_DOUBLE;
			DispatchEvent(TOUCH_EVENT, &stEvent);
		}else{
			
			
			stEvent.touchKey = stTouch.touchKey;
			DispatchEvent(TOUCH_EVENT, &stEvent);
		}
	}else{
		
		stEvent.touchKey = stTouch.touchKey;
		DispatchEvent(TOUCH_EVENT, &stEvent);
	}

	_delegate->keyIsTouched(this, stEvent);

	if (count > 2){
		//remove old touches
		_stTouches.erase(_stTouches.begin(), _stTouches.begin()+ count - 2);
	}
}


void HudLayer::setDelegate(HudLayerDelegate* pDele) {
    _delegate = pDele;
    if(NULL == _delegate) {
        CCLOG("[trace]HudLayer,setDelegate null");
    } else {
        CCLOG("[trace]HudLayer,setDelegate=%s", typeid(*_delegate).name() );
    }
}

