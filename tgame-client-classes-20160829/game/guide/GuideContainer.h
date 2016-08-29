#ifndef GuideContainer_h__
#define GuideContainer_h__

#include "cocos2d.h"

USING_NS_CC;

class GuideContainer
{
public:
	GuideContainer() {};
	virtual ~GuideContainer() {};

	virtual CCNode* getGuideNodeByName(const std::string &name) = 0;

};

#endif // GuideContainer_h__