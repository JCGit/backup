#ifndef ArenaFightScene_h__
#define ArenaFightScene_h__

#include "fight/BaseFightScene.h"
#include "vlib/CVArray.h"

#include "ArenaMapLibCfgMaster.h"

class CArenaFightInfoLayout;

class CArenaFightScene : public CBaseFightScene
{
public:
	CArenaFightScene();
	virtual ~CArenaFightScene();

	CREATE_FUNC(CArenaFightScene);

	void onEnterTransitionDidFinish();

	void update(float delta);

	void loadHeroRes();
	void loadMonsterRes();
	void loadItemRes();

	void initMap();

	CEnemy* addOneMonster(CEnemyData* pstObj, bool isEmerge);
	CEnemy* initOneMonster(CEnemyData* monsterObj);

	void stopAllMonsters();
protected:
	//init相关
	void initSceneItem();										//关卡地图item
	void initMapUI();											//关卡UI
	void initMainHero();										//猪脚
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	void initOpponent();										//初始化对手
	void initFightData();										//初始化战斗相关的数据

	void setMapRes();											//通过地图类型设置每层的资源名
	void setWalkHight();										//设置可行走区域

	void brushMonsterLogic(float dt);							//刷怪逻辑
	void brushMonsterSpecial(CEnemyData* pstObj);				//竞技场特殊刷怪

	void brushOneMonster(CEnemyData* pstObj);

	//视角update
	void setViewpointCenter(const CCPoint& position);

	//放弃战斗
	void handleGiveUpLogic(CCObject* pstObj);					//中途放弃退出关卡

	void handleGameOver();										//发送结束请求

	bool checkMonsterOnStage(CEnemyData* pstObj);				//检查某刷怪点是否有怪在舞台上

	bool checkArrowCondition(CQStageEntity* pstEntity);			//怪物指引箭头的条件

protected:
	//主角死亡，未复活战斗失败
	void doHeroDieFail();

	//怪物事件
	void handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡瞬间的数据记录
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

	//对手事件
	void handleOpponentEPAddEvent(const string& stEvent, CCObject* pstEventObj);
	void handleOpponentHurtValueEvent(const string& stEvent, CCObject* pstEventObj);	//对手掉血
	void handleOpponentKilledEvent(const string& stEvent, CCObject* pstEventObj);		//对手死亡

	//胜利特效播放完成
	void handleWinEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	//击杀特效
	void handleKillEffComplete(CCArmature * armature, MovementEventType type, const char * name);
private:
	void startPvp(const string& stEvent, CCObject* pstEventObj);

	void setCurBossAI();

	void rectifyPos(CEnemyData* pstObj);						//矫正刷怪点的位置
	void initStage();

	void checkLimitTime();										//检测是否超时
	void setCurStageBoss();
	CCPoint getOpponentPos();									//对手出生位置
	bool checkWinLogic(unsigned int cfgID);						//检测是否通关
	void doDieMovement(unsigned int cfgID, unsigned int uID);
	void doWinLogic(unsigned int uID);
	void doWin();

	void nextStage(unsigned int uID);							//下一阶段
	void delBrushByStage();										//通过阶段后，将本阶段的刷怪点废弃

	void checkOppInRevive();									//判断对手是否复活
	void doReviveOpp();
	float getReiviveTime();										//获得复活时间

	void initPowerGrid();										//添加电网
	void addBossTipEff(CEnemy* pEnemy);							//在关卡boss建筑物头上加上特效
	void addOpTipEff();											//对手身上添加特效

	void playKillHeroEff();

	void doHeroReplyHp();										//主角血量、ep回复		
private:
	CArenaFightInfoLayout* m_pInfoLayout;
	CMainHero* m_pOpponentHero;									//对手英雄

private:
	unsigned int m_uTotalStage;									//总阶段数
	unsigned int m_uStageIdx;									//阶段
	unsigned int m_uArenaLv;									//竞技场军衔
	unsigned int m_uMonsterLev;									//怪物等级
	unsigned int m_uLastKillID;									//胜利的时候杀死的最后一个怪物id

	unsigned int m_uOppDieNum;									//对手复活次数

	unsigned int m_uKillBossNum;								//已经杀死boss数量
	unsigned int m_uBossTotalNum;								//通关boss总数

	float m_fLimitTime;
	float m_fOppDieTime;										//对手死亡时间
	float m_fStageW;

	bool m_bIsWin;	
	bool m_bIsWinComplete;
	bool m_bIsOppDying;											//对手复活中
	bool m_bIsBossHited;										//boss建筑是否被攻击

	bool m_bIsFirstEnter;										//是否第一次进入

	ArenaMapLibCfg m_stMapCfg;

	vector<CCPoint> m_stPosVec;									//对手3个阶段的刷怪位置
	CCArray* m_stAllBossData;									//所有boss数据
	CVArray<unsigned int, 4> m_stBossIDs;						//当前阶段bossIDs
	CCArray* m_stPowerGrids;									//电网，依次存放

	unsigned int m_uOpRandWeaponID;								//对手随机的枪
};

#endif // ArenaFightScene_h__
