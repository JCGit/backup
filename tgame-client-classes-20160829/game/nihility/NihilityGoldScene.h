#ifndef NihilityGoldScene_h__
#define NihilityGoldScene_h__

#include "fight/BaseFightScene.h"
#include "fightendless/FightEndlessScoreUI.h"

class CNextStageTipLayer;

class CNihilityGoldScene : public CBaseFightScene
{
public:
	CNihilityGoldScene();
	virtual ~CNihilityGoldScene();

	CREATE_FUNC(CNihilityGoldScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadHeroRes();											//角色资源的加载，动态的
	void loadMonsterRes();										//怪物资源的加载，动态

	void initMap();

	CEnemy* initOneMonster(CEnemyData* monsterObj);		

	void updateCoin();
	void updateScore();
	void updatePackage();

	void setAtkBtnEnable(bool isEnable);
protected:
	void startUpdate();											//开始更新

	//init相关
	void initMapUI();											//关卡UI
	void initMainHero();										//猪脚
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	void initFightData();										//初始化战斗相关的数据

	void brushMonsterLogic(float dt);							//刷怪逻辑

	//放弃战斗
	void handleGiveUpLogic(CCObject* pstObj);					//中途放弃退出关卡

	//update相关
	void handlePassLogic();										//过关逻辑

	void checkMonsterOutStage();								//检测怪物是否超出屏幕，超出就移除

	void handleGameOver();										//发送结束请求

	void handleCoinEndEvent();

	void autoFightHandle();
protected:
	void handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj);

	//主角死亡，未复活战斗失败
	void doHeroDieFail();

	//怪物事件
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡瞬间的数据记录
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

private:
	void initMonsterAddToStage(CEnemyData* pstObj);

	void addNextStageTip();										//出现下一阶段提示
	void removeStageTip();										//移除传送点
	void checkArrivedEdge();									//检查是否到下一阶段

private:
	//数据
	float m_fObjectTime;					//记录时间,总时间
	float m_fStartWalkTime;					//过阶段时主角开始行走时间

	unsigned int m_uCurMonsterLev;			//怪物等级
	unsigned int m_uCurMonsterNum;			//当前阶段怪物总数 
	unsigned int m_uCurLeftMonsterNum;		//当前阶段还剩多少只怪要刷

	bool m_bIsArrivedEdge;					//是否在到达最右边
	bool m_bIsAutoWalking;					//是否自动走向下一阶段

	map<unsigned int, int> m_stMonKeyToTime;//每个阶段每个怪物对应的起始刷怪时间(key有id，x，y合成，value为时间)

	CNextStageTipLayer* m_pTipLayer;		//下一阶段提示
	CFightEndlessScoreUI* m_pScoreUI;
};

#endif // NihilityGoldScene_h__
