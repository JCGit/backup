#ifndef BloodBar_h__
#define BloodBar_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class CBloodBar : public CCNode
{
public:
	CBloodBar();
	virtual ~CBloodBar();

	CREATE_FUNC(CBloodBar);

	bool init();

	static CBloodBar* createWithImage(const string progressName = "share/fight_hp.png", 
        const string progressBgName = "share/fight_hp_bg.png");

	void initWithImage(const string progressName, const string progressBgName);

	void setPercentage(float fPercentage);

	float getPercentage();
protected:
private:
	CCProgressTimer* progress;		//ÑªÌõ
	CCSprite* progressBg;			//ÑªÌõbg
};

#endif // BloodBar_h__
