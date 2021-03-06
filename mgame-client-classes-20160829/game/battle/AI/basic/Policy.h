/*
* jacey
* AI 决策类
*/
#pragma once

#include "Constant.h"
#include "AIState.h"

#include "cocos2d.h"
USING_NS_CC;

class AI;
class Camp;
class Policy
{
public:
	Policy();
	~Policy();

	bool init();
	void update(float dt);
	eCampType getPolicyCamp();

	void	registerState(eStateTag eTag);
	void	unregisterState(eStateTag eTag);


	bool getBuildCanUpgrade(int buildKey);
	




protected:

	// 获取决策对象所属阵营
	Camp*	getTargetCamp();




protected:

	CC_SYNTHESIZE(AI*, mAITarget, AITarget);

	StateList	mStateList;
	float	mPolicyInterval;
};

