#ifndef NihilityFightScene_h__
#define NihilityFightScene_h__

#include "fight/BaseFightScene.h"

class CNextStageTipLayer;
class CBossWarning;
class CopterEffLayer;
class CFightEndlessScoreUI;

class CNihilityFightScene : public CBaseFightScene
{
public:
	CNihilityFightScene();
	virtual ~CNihilityFightScene();

	CREATE_FUNC(CNihilityFightScene);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//地图背景资源的加载，动态
	void loadHeroRes();											//角色资源的加载，动态的
	void loadMonsterRes();										//怪物资源的加载，动态
	void loadBrushRes();										//直升特效需要的资源
	void loadItemRes();											//场景item资源的加载，动态
	void loadFloorBuffRes();									//场景buff资源加载
	void loadTrapRes();											//陷阱资源加载

	void initMap();

	CEnemy* initOneMonster(CEnemyData* monsterObj);						

	void updateCoin();
	void updateScore();
	void updatePackage(bool bIsShowEff = false);

	void setAtkBtnEnable(bool isEnable);

	string getEmergeByData(CEnemyData* pstObj);
protected:
	void startUpdate();											//开始更新

	//init相关
	void initSceneItem();										//关卡地图item
	void initFloorBuff();										//地表buff
	void initTrap();											//陷阱
	void initMapUI();											//关卡UI
	void initMainHero();										//猪脚
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	void initFightData();										//初始化战斗相关的数据

	void setMapRes();											//通过地图类型设置每层的资源名
	void setWalkHight();										//设置可行走区域

	void brushMonsterLogic(float dt);							//刷怪逻辑

	//update相关
	void handlePassLogic();										//过关逻辑

	//放弃战斗
	void handleGiveUpLogic(CCObject* pstObj);					//中途放弃退出关卡

	void handleGameOver();										//发送结束请求

	void handleCoinEndEvent();

	void autoFightHandle();
protected:
	void handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj);

	//主角死亡，未复活战斗失败
	void doHeroDieFail();

	//主角事件
	void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//主角出场播放完毕

	//怪物事件
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡瞬间的数据记录
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);

	//场景物件事件
	void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
	void handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj);	//物件消失

	//boss警告播放完成
	void handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);

	//void btnTouchEvent(CCObject *pSender, TouchEventType type);
private:
	void startNihility();
	void startCopterEff();
	void handleCopterEffOverEvent(const string& stEvent, CCObject* pstEventObj);
	void startCopterFly();
	void copterFlyOut(CCArmature* armature);
	void copterFlyEndFun(CCObject* pstObject);
	void copterEffCallFun(CCArmature* armature, MovementEventType type, const char* nameStr);

	void packEndFun(CCNode* pNode);
	void handlePackageCallBack(CCArmature* armature, MovementEventType type, const char* nameStr);

	void countRushAward();										//计算扫荡的奖励

	void prepAllStageRes();										//准备所有阶段的资源

	void setMonsterNum();										//设置当前阶段怪物总数
	void brushMonsters();

	void initSleepMonsters();									//休眠的怪物
	void brushMonsterByTouch(CEnemyData* monsterObj);			//根据触发线刷怪

	void checkBrushSceneItem();									//检查是否按触发线刷物件 OR BUFF
	void brushSceneItemByTouch(unsigned int touchID);			//根据触发线刷物件
	void brushFloorBuffByTouch(unsigned int touchID);			//根据触发线刷floor buff
	void brushTrapByTouch(unsigned int touchID);				//根据触发线刷陷阱

	void showBossWarning();										//boss出现警告

	bool checkWinLogic(unsigned int cfgID);						//判断是否通关

	void addNextStageTip();										//出现下一阶段提示
	void removeStageTip();										//移除传送点
	void checkArrivedEdge();									//检查是否到下一阶段

	unsigned int getStageMapID(unsigned int uStageIdx);			//获得当前阶段的地图ID

	void resetStage();											//重置阶段
	void clearPreStage();										//清除上一阶段
	void nextStage();											//下一阶段

	void handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj);
	void handleTransitionComplete(const string& stEvent, CCObject* pstEventObj);

	int rollPackage(unsigned int uLv);							//随机掉落
private:
	//数据
	float m_fObjectTime;					//记录时间,总时间
	float m_fStageTime;						//每阶段的时间，换阶段时重置
	float m_fStartWalkTime;					//过阶段时主角开始行走时间

	unsigned int m_uCurLevel;				//当前层数
	unsigned int m_uCurStage;				//当前阶段数，从1开始，有3个阶段
	unsigned int m_uCurMonsterLev;			//怪物等级
	unsigned int m_uCurMonsterNum;			//当前阶段怪物总数 

	//每层3个关卡的id
	unsigned int m_uNormalID;
	unsigned int m_uEliteID;
	unsigned int m_uBossID;

	bool m_bIsArrivedEdge;					//是否在到达最右边
	bool m_bIsAutoWalking;					//是否自动走向下一阶段

	CCPoint m_stInitPos;
	CCArray* m_stSleepMonsters;

	CBossWarning* m_pBossWarning;			//boss出现警告
	CNextStageTipLayer* m_pTipLayer;		//下一阶段提示
	CFightEndlessScoreUI* m_pScoreUI;
	CopterEffLayer* m_pCopterEff;
};

#endif // NihilityFightScene_h__
