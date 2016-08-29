#pragma once
#include "cocos2d.h"

class CSimpleDPad;

class CSimpleDPadDelegate
{
public:
	virtual void didChangeDirectionTo(CSimpleDPad *SimpleDPad, cocos2d::CCPoint direction) = 0;
	virtual void isHoldingDirection(CSimpleDPad *SimpleDPad, cocos2d::CCPoint direction) = 0;
	virtual void simpleDPadTouchEnded(CSimpleDPad *SimpleDPad) = 0;
};

class CSimpleDPad : public cocos2d::CCSprite, public cocos2d::CCTargetedTouchDelegate
{
public:
	CSimpleDPad(void);
	~CSimpleDPad(void);

	static CSimpleDPad* dPadWithFile(cocos2d::CCString *fileName, float radius);
	bool initWithFile(cocos2d::CCString *filename, float radius);

	void onEnterTransitionDidFinish();
	void onExit();
	void update(float dt);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	void updateDirectionForTouchLocation(const cocos2d::CCPoint& location);

	const cocos2d::CCPoint& getDirection()const{
		return _direction;
	}

	CC_SYNTHESIZE(CSimpleDPadDelegate*, _delegate, Delegate);
	CC_SYNTHESIZE(bool, _isHeld, IsHeld);
	

protected:
	float _radius;
	
	cocos2d::CCPoint _direction;
};