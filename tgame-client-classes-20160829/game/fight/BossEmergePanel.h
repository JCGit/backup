#ifndef BossEmergePanel_h__
#define BossEmergePanel_h__

#include "BasePanel.h"
using namespace std;

enum {
	SHOW_STATE_INIT,			//展示初始化
	SHOW_STATE_ACTING,			//逐步展示中
	SHOW_STATE_END				//展示完成
};

class CBossEmergePanel : public BasePanel
{
public:
	static const CCPoint initRolePos;
public:
	CBossEmergePanel();
	virtual ~CBossEmergePanel();

	CREATE_FUNC(CBossEmergePanel);

	bool init();

	void openWithCallFun(CCObject* target, SEL_CallFunc callbackFun);
	void open();
	void close();

	void loadArmature();

private:
	void initUI();

	void startAction();

	//逐字显示boss的介绍
	void showDescByStep();				
	void stepUpdate(float dt);		

	//显示boss伤害位置
	void setHurtPos(const CCPoint& point);
	void updateHurtArmaturePos();
	void showHurtLocation();
	void showHurtArrow();
	void showHurtDesc();
	void showEnd();

	void showAllImmediately();

	void closeCountDown(float dt);

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void reset();

	CCActionManager* createNewCScheduler(); 
private:
	UIPanel* m_pBossDescLayer;
	UIPanel* m_pTouchLayer;
	UILabel* m_pBossNameTxt;
	UILabel* m_pBossDescTxt;
	ImageView* m_pHurtImg;
	ImageView* m_pHurtDescImg;
	ImageView* m_pLine0;
	ImageView* m_pLine1;
	ImageView* m_pLine2;
	UIPanel* m_pHurtLayer;

	CCArmature* m_pArmature;
	CCArmature* m_pHurtArmature;			//攻击区域指示特效
private:
	CC_PROPERTY(unsigned int, _cfgID, CfgID);

	string m_stResPath;

	string m_stDesc;					//描述文件
	unsigned int m_uIndex;

	unsigned int m_uShowState;			//展示的状态

	CCPoint m_stDescPos;				//boss介绍的位置
	CCPoint m_stHurtPos;				//boss伤害介绍位置

	unsigned int m_uTime;

	CCObject*    m_pTarget;
	SEL_CallFunc m_pEventSelector;
};

#endif // BossEmergePanel_h__
