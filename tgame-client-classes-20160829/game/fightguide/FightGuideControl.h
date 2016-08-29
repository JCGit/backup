#ifndef FightGuideControl_h__
#define FightGuideControl_h__

#include "cocos2d.h"

USING_NS_CC;

class CFightGuideDialog;
class CFightGuideLayout;
class CFightGuideScene;

#define FIGHT_GUIDE_SCENE_ID 90001
#define FIGHT_GUIDE_NORMAL_SCENE_ID 90002

enum FIGHT_GUIDE_TYPE{
	DIALOG_TYPE = 1,			//对话
	MONSTER_EMERGE_TYPE = 2,	//怪物出现
	FIRE_CLEAN_TYPE = 3,		//原地子弹消灭怪物
	UP_FIRE_CLEAN_TYPE = 4,		//向上移动射击消灭
	DOWN_FIRE_CLEAN_TYPE = 5,	//向下移动射击消灭
	MOVE_AHEAD_TYPE = 6,		//引导向前
	SKILL_CLEAN_TYPE = 7,		//绝招消灭怪物
	END_GUIDE_TYPE = 8,			//结束指引
};

class CFightGuideControl
{
public:
	CFightGuideControl();
	virtual ~CFightGuideControl();

	void enterFightGuideScene();

	void enterGuideNormalScene();

	void startGuide();
	void init();

	void endGuide();

	void showDialog();
	void hideDialog();

	void addGuide();
	void removeGuide();

	CFightGuideLayout* getGuideLayout();
	CFightGuideScene* getGuideScene();

	void nextStep();

protected:
private:
	CC_SYNTHESIZE(int, _stepID, StepID);
	CC_SYNTHESIZE(bool, m_bIsInGuide, IsInGuide);		//是否在指引中

	bool m_bInitialized;
	CC_SYNTHESIZE(bool, m_bIsShowDialog, IsShowDialog);
	CC_SYNTHESIZE(bool, m_bIsShowForceGuide, IsShowForceGuide);

	CC_SYNTHESIZE(int, _direct, Direct);

	CFightGuideDialog *m_pDialogLayout;
	CFightGuideLayout* m_pGuideLayout;
};

#endif // FightGuideControl_h__

#define P_FIGHT_GUIDE_CTRL CVLib::CVSingleton<CFightGuideControl>::Instance()
