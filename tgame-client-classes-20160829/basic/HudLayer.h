#ifndef HudLayer_h__
#define HudLayer_h__
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "vlib/QEventDispatcher.h"
using namespace CVLib;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

class HudLayer;

const int HUD = 100;
class HudTouchEvent: public CCObject{
public:
	unsigned int touchKey;
	unsigned int touchType;

	HudTouchEvent(){
		touchType = 0;

	}
};

class HudLayerDelegate
{
public:
	virtual void didChangeDirectionTo(HudLayer *hud, CCPoint direction) = 0;
	virtual void isHoldingDirection(HudLayer *hud, CCPoint direction) = 0;
	virtual void simpleDPadTouchEnded(HudLayer *hud) = 0;
	virtual void keyIsTouched(HudLayer *hud, HudTouchEvent stTouch)
	{//默认实现里不处理双击事件

	}
};

enum{
	E_TOUCH_LEFT = 1,
	E_TOUCH_UP = 2,
	E_TOUCH_RIGHT = 3,
	E_TOUCH_DOWN = 4,
	E_TOUCH_CENTER = 5,
	
};

enum{
	E_TOUCH_TYPE_SINGLE = 0,
	E_TOUCH_TYPE_DOUBLE = 1,
};

class CHudTouches{
public:
	double touchTime;
	unsigned int touchKey;
	CHudTouches(){
		touchKey = 0;
		touchTime = 0.0;
	}
};



class HudLayer : public CCLayer, public CQEventDispatcher
{
public:
	static const string TOUCH_EVENT;	

	
	HudLayer(void);
	virtual ~HudLayer(void);

	//初始化 aPoint是摇杆中心 aRadius是摇杆半径 aJsSprite是摇杆控制点 aJsBg是摇杆背景
	static HudLayer*  HudLayerWithCenter(CCPoint aPoint,float aRadius, bool _isFollowRole);

	//启动摇杆
	void Active();

	//解除摇杆
	void Inactive();

	HudLayer* initWithCenter(CCPoint aPoint,float aRadius,bool _isFollowRole);

	CCPoint initPoint;	//初始中心点
	CCPoint centerPoint;//摇杆中心
	CCPoint currentPoint;//摇杆当前位置
	bool active;//是否激活摇杆
	float radius;//摇杆半径
	CCSprite* jsSprite;
	CCSprite* jsBgSprite;

	CCPoint getDirection();
	float getVelocity();
	void  updatePos(float dt);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void touchEvent(CCObject *pSender, TouchEventType type);   

	void updateDirectionForTouchLocation(CCPoint location);

	CREATE_FUNC(HudLayer);
	//CC_SYNTHESIZE(HudLayerDelegate*, _delegate, Delegate);
    HudLayerDelegate* getDelegate() {
        return _delegate;
    }
    void setDelegate(HudLayerDelegate* pDele);
	CC_SYNTHESIZE(bool, _isHeld, IsHeld);

	void setIsFollowRole(bool var);

	bool getIsFollowRole(){
		return isFollowRole;
	}
protected:
	float _radius;
	bool isFollowRole;//是否跟随用户点击

	CCPoint _direction;
	UIButton* _pUpBtn;
	UIButton* _pLeftBtn;
	UIButton* _pRightBtn;
	UIButton* _pDownBtn;
	UIButton* _pCenterBtn;
	vector<CHudTouches> _stTouches;
	
	CCSize touchSize;
    HudLayerDelegate* _delegate;
};

#endif // HudLayer_h__
