#pragma once

#include "cocos2d.h"
USING_NS_CC;

// �����ָ���¼
class PowerRecord
{
public:
	PowerRecord() :mTimeNode(-1), mIfGetted(false)
	{

	}

	~PowerRecord()
	{
	}

protected:

	// ʱ�̵㣬0-0�� / 1-12��		eRecoveryTime
	CC_SYNTHESIZE(int, mTimeNode, TimeNode);
	// �����Ƿ�����ȡ
	CC_SYNTHESIZE(int, mIfGetted, IfGetted);
};


// ���¼�¼
class BadgeItem
{
public:
	BadgeItem() : mType(0), mTime(0){}
	~BadgeItem(){}

	void setType(unsigned int val) { mType = val; }
	unsigned int getType() const { return mType; }

	void setTime(unsigned int val) { mTime = val; }
	unsigned int getTime() const { return mTime; }
	
protected:
	unsigned int mType;
	unsigned int mTime;
};