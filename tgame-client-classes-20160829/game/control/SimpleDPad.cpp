#include "SimpleDPad.h"
using namespace cocos2d;

CSimpleDPad::CSimpleDPad(void)
{
	_delegate = NULL;
}

CSimpleDPad::~CSimpleDPad(void)
{
}

CSimpleDPad* CSimpleDPad::dPadWithFile(CCString *fileName, float radius)
{
	CSimpleDPad *pRet = new CSimpleDPad();
	if (pRet && pRet->initWithFile(fileName, radius))
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CSimpleDPad::initWithFile(CCString *filename, float radius)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCSprite::initWithFile(filename->getCString()));

		_radius = radius;
		_direction = CCPointZero;
		_isHeld = false;
		

		bRet = true;
	} while (0);

	return bRet;
}

void CSimpleDPad::onEnterTransitionDidFinish()
{
    this->scheduleUpdate();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void CSimpleDPad::onExit()
{
    this->unscheduleUpdate();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void CSimpleDPad::update(float dt)
{
	if (_isHeld)
	{
		_delegate->isHoldingDirection(this, _direction);
	}
}

bool CSimpleDPad::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocation();

	float distanceSQ = ccpDistanceSQ(location, this->getPosition());
	if (distanceSQ <= _radius * _radius)
	{
		this->updateDirectionForTouchLocation(location);
		_isHeld = true;
		return true;
	}
	return false;
}

void CSimpleDPad::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocation();
	this->updateDirectionForTouchLocation(location);
}

void CSimpleDPad::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	_direction = CCPointZero;
	_isHeld = false;
	_delegate->simpleDPadTouchEnded(this);
}

void CSimpleDPad::updateDirectionForTouchLocation(const CCPoint& location)
{
	//float radians = ccpToAngle(ccpSub(location, this->getPosition()));
	//float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);

	/*
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
	*/
	//直接全方位控制
	_direction = ccpNormalize(ccpSub(location, this->getPosition())) ;
	_delegate->didChangeDirectionTo(this, _direction);
}