#pragma once
#include "cocos2d.h"

class NumberChangeAction : public cocos2d::CCActionInterval
{
public:

	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* pZone);
	virtual void startWithTarget(cocos2d::CCNode *pTarget);
	virtual CCActionInterval* reverse(void);
	virtual void update(float time);

public:
	/** creates the action */
	static NumberChangeAction* create(float duration, float targetNumber, int decimalCount = 0);
protected:
	float mStartNumber;
	float mTargetNumber;
	int mDecimalCount;
};