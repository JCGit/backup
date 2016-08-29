#ifndef CopterEffLayer_h__
#define CopterEffLayer_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "vlib/QEventDispatcher.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class CopterEffLayer : public CCLayer, public CQEventDispatcher
{
public:
	static const string COPTER_FLY_OUT_EVENT;
public:
	CopterEffLayer();
	virtual ~CopterEffLayer();

	CREATE_FUNC(CopterEffLayer);

	bool init();

	void onEnter();
	void onExit();

	void start();

	void update(float delta);

	void setViewpointCenter();

	void fly(float dt);

	void initUI();

private:
	void analyseInfo();
	void initMonsters();

	void checkMonsterDie();
	void handleMonsterDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);

	void checkCopterOut();
private:
	CCArmature* m_pCopterArm;
	CCSprite* m_pMapBg;

	CC_SYNTHESIZE(CCArray*, m_stMonsterData, MonsterData);		//怪物信息

	CCArray* m_stStageMonsters;

	CCSize m_stBgSize;
	float m_fSpeed;

	bool m_bIsCopterOut;					//是否在飞出
};

#endif // CopterEffLayer_h__
