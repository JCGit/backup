#ifndef FightGuideScene_h__
#define FightGuideScene_h__

#include "fight/BaseFightScene.h"

class CFightTipLayout;
class CFightGuideHud;

class CFightGuideScene : public CBaseFightScene
{
public:
	CFightGuideScene();
	virtual ~CFightGuideScene();

	CREATE_FUNC(CFightGuideScene);

	bool init();

	void onEnterTransitionDidFinish();

	void update(float delta);

	void initMap();

	void loadMonsterRes();										//怪物资源的加载，动态

	CEnemy* initOneMonster(CEnemyData* monsterObj);				//初始化一个怪物

	void setAttackOpBtnVisible(bool bIsVisible);				//设置攻击、技能按钮是否可见

	void showFireGuide();										//指引战斗

	void showMoveFireGuide(int direct);

	void removeGuide();

	void removeFireBtns();

	FireButton* getFireBtn(){
		return fireBtn;
	}

	SkillButton* getSkillBtn(){
		return m_trickBtn;
	}

	bool checkInFireGuide();		

	bool checkInMoveFireGuide();

	void showMoveToGuide();
	void moveHero();

	void showSkillGuide();

	void moveToEnd();
	void checkArrivedEdge();									//检查是否到下一阶段

	void unSetMainHeroAsTheOne();
protected:
	void initMapUI();											//关卡UI
	void initMainHero();										//猪脚
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	void initFightData();										//初始化战斗相关的数据

	void updateMonsterArrow();									//更新舞台上怪物指引箭头
	void brushMonsterLogic(float dt);							//刷怪逻辑

	void handleGiveUpLogic(CCObject* pstObj);					

	//触发事件
	void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//视角update
	void updateBgViewCenter(float foreX);						//更新地图前景、远景的视角(根据地图不同，改写)

protected:
	//怪物事件
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡瞬间的数据记录

	//技能按钮事件
	void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);
private:
	void nextStageTip();										//下一阶段提示
	void cancelTip();											//取消提示

	void moveToDst(CQStageEntity* pstEntity, CCPoint stPos);	//移动到目的点
	void onArrived(const string &evt, CCObject *pSender);	    //到达目的地

	unsigned int getNowStepType();

	void handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj);
	void handleTransitionComplete(const string& stEvent, CCObject* pstEventObj);
private: 
	CFightTipLayout* m_pNextStageTip;							//下一阶段提示
	CFightGuideHud* m_pGuideHud;
	ImageView* m_pTipImg;

	float m_fObjectTime;										//时间

	unsigned int m_uGuideStep;									//指引步骤
	unsigned int m_uArriveNum;									//怪物移动到目的点的数量
	unsigned int m_uCurStageMonNum;								//当前阶段怪物数量
	unsigned int m_uBrushNum;
	unsigned int m_uLastMonUID;

	bool m_bIsArrivedEdge;
	CC_SYNTHESIZE(bool, m_bCanBrushMonster, CanBrushMonster);	//是否可以刷怪

	vector<CEnemyData*> m_stCurStageMonsters;					//当前阶段刷怪点的信息
};

#endif // FightGuideScene_h__
