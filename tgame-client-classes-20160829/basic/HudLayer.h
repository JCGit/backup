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
	{//Ĭ��ʵ���ﲻ����˫���¼�

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

	//��ʼ�� aPoint��ҡ������ aRadius��ҡ�˰뾶 aJsSprite��ҡ�˿��Ƶ� aJsBg��ҡ�˱���
	static HudLayer*  HudLayerWithCenter(CCPoint aPoint,float aRadius, bool _isFollowRole);

	//����ҡ��
	void Active();

	//���ҡ��
	void Inactive();

	HudLayer* initWithCenter(CCPoint aPoint,float aRadius,bool _isFollowRole);

	CCPoint initPoint;	//��ʼ���ĵ�
	CCPoint centerPoint;//ҡ������
	CCPoint currentPoint;//ҡ�˵�ǰλ��
	bool active;//�Ƿ񼤻�ҡ��
	float radius;//ҡ�˰뾶
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
	bool isFollowRole;//�Ƿ�����û����

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
