#ifndef GuideCallFunc_h__
#define GuideCallFunc_h__

#include "cocos2d.h"

USING_NS_CC;

class GuideCallFunc: public CCObject
{
public:
	GuideCallFunc();
	virtual ~GuideCallFunc();

	static GuideCallFunc* create();

	void checkGuideNodeExit(float dt);

private:

};

#endif // GuideCallFunc_h__