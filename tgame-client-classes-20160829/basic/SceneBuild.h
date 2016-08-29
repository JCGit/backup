#ifndef SceneBuild_h__
#define SceneBuild_h__

#include "cocos2d.h"

USING_NS_CC;

class CSceneBuild : public CCNode
{
public:
	CSceneBuild();
	~CSceneBuild();

	CREATE_FUNC(CSceneBuild);

	bool init();

	void createWithTexture(CCTexture2D *pTexture);
protected:
private:
	CC_SYNTHESIZE(int, _cfgID, CfgID);

	CCSprite* picture;

};

#endif // SceneBuild_h__
