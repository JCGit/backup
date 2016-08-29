#ifndef FightGuideMapScene_h__
#define FightGuideMapScene_h__

#include "fight/BaseFightScene.h"

class CFightTipLayout;
class CBossWarning;

class CFightGuideMapScene : public CBaseFightScene
{
public:
	CFightGuideMapScene();
	virtual ~CFightGuideMapScene();

	CREATE_FUNC(CFightGuideMapScene);

	bool init();

	void onEnterTransitionDidFinish();

	void update(float delta);

	void initMap();

	void loadMapBgRes();
	void loadHeroRes();											//角色资源的加载，动态的
	void loadMonsterRes();										//怪物资源的加载，动态
	void loadItemRes();											//场景item资源的加载，动态
	void loadFloorBuffRes();									//场景buff资源加载
	void loadTrapRes();											//陷阱资源加载

protected:
	void setMapRes();											//通过地图类型设置每层的资源名
	void setWalkHight();

	//init相关
	void initSceneItem();										//关卡地图item
	void initFloorBuff();										//地表buff
	void initTrap();											//陷阱
	void initMapUI();											//关卡UI
	void initMainHero();										//猪脚
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	virtual void initFightData();								//初始化战斗相关的数据

	void initSleepMonsters();									//初始化休眠的怪物放在舞台上

	void startFight();											//开始战斗

	//触发事件
	void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//update相关
	void updateInstnaceByAI();									//怪物AI
	void brushMonsterLogic(float dt);							//刷怪逻辑
	void handlePassLogic();										//过关逻辑
	void handleGiveUpLogic(CCObject* pstObj);

	void updateSceneBuffs(float dt);							//地表buff更新

	//视角update
	void setViewpointCenter(const CCPoint& position);			//视角居中
	void updateBgViewCenter(float foreX);						//更新地图前景、远景的视角(根据地图不同，改写)

	void handleGameOver();										//发送结束请求

protected:
	//主角或友军事件
	void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);
	void handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj);	//友军掉血
	void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//主角出场播放完毕

	//怪物事件
	void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡瞬间的数据记录

	//场景物件事件
	void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
	void handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj);	//物件消失

	//胜利特效播放完成
	void handleWinEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	//boss警告播放完成
	void handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);

private:
	void setAttackOpBtnVisible(bool bIsVisible);

	void checkShowGoTip();										//检测是否提示向前走，当舞台上无怪物，但本阶段未过关，提示向前
	void nextStageTip();										//下一阶段提示
	void cancelTip();											//取消提示

	void checkBrushSceneItem();									//检查是否按触发线刷物件 OR BUFF
	void brushSceneItemByTouch(unsigned int touchID);			//根据触发线刷物件
	void brushFloorBuffByTouch(unsigned int touchID);			//根据触发线刷floor buff
	void brushTrapByTouch(unsigned int touchID);				//根据触发线刷陷阱

	void checkShowStage();										//显示当前阶段

	void brushMonster();										//刷怪具体逻辑
	void brushMonsterByTouch(CEnemyData* monsterObj);			//根据触发线刷怪
	void activateMonsterByTouch(CEnemyData* monsterObj);		//根据触发线激活怪物
	void activateCarByTouch(CEnemyData* monsterObj);			//根据触发线触发救护车载着怪物出场			
	void brushFromCar(CCObject* pstObject);						//从救护车上刷出怪物
	void brushFromCarOne(CCObject* pstObject);					//一个个从救护车上刷出怪物
	void carMoveEnd(CCObject* pstObject);						//救护车移除屏幕

	void brushAfterDie(CEnemyData* pstObj);						//当前阶段按死亡后走cd刷怪
	void updateTimeByKey(unsigned int key);						//根据key更新时间

	void showBossWarning();										//boss出现警告

	void handleFailLogic();										//失败逻辑

	void nextStage();											//下一阶段
	void delPreStage();											//删除前一阶段
	void passStageViewChange(float dt);

	//结算逻辑
	void handleSuccessLogic();									//成功逻辑
	bool checkWinLogic(unsigned int cfgID);						//检查通过条件

	CCRect getStageRect() const;								//获取当前阶段的可行走区域

	void moveToDst(CQStageEntity* pstEntity, CCPoint stPos);	//移动到目的点
	void onArrived(const string &evt, CCObject *pSender);	    //到达目的地

	void checkFightTipShow();
protected:
	CFightTipLayout* m_pnextStageTip;							//下一阶段提示
	CBossWarning* m_pBossWarning;								//boss出现警告
	ImageView* m_pTipImg;
protected:
	unsigned int m_uTotalStage;									//总阶数
	unsigned int m_uCurStageIdx;								//当前阶段，从1开始(坑爹的策划说的)

	unsigned int m_uCurStageMonNum;								//当前阶段怪物数量
			
	float m_fStartBrushTime;									//本阶段开始刷怪时间
	bool m_bStartedBrush;										//本阶段是否开始刷怪
	bool m_bPassViewChange;										//通过阶段的视角变化
	bool m_bHasShowStage;										//是否显示过当前阶段

	int m_uPassType;											//通关类型,(0：未通关)

	bool m_bIsIntroduced;										//boss是否已经介绍过

	unsigned int m_uLastKillID;									//胜利的时候杀死的最后一个怪物id
	bool m_bIsPreSuccess;										//等待胜利
	bool m_bIsWinComplete;										//胜利特效、动作是否播放完成

	map<int, vector<CEnemyData*> > m_stIDXToMonsters;			//记录每个阶段的刷怪点，<int, CCArray*>阶段索引对应怪物
	map<int, int> m_stIDXToMonsterNum;							//记录每个阶段的怪物数量
	map<int, vector<CItemObj*> > m_stIDXToItems;				//记录每个阶段的item，<int, CCArray*>阶段索引对应item
	map<int, vector<CFloorBuffObj*> > m_stIDXToFloorBuffs;		//记录每个阶段的floorbuff，<int, CCArray*>阶段索引对应floorbuff
	map<int, vector<CTrapObj*> > m_stIDXToTraps;				//记录每个阶段的陷阱，<int, CCArray*>阶段索引对应陷阱

	vector<CEnemyData*> m_stCurStageMonsters;					//当前阶段刷怪点的信息
	map<unsigned int, int> m_stMonKeyToTime;					//每个阶段每个怪物对应的起始刷怪时间(key有id，x，y合成，value为时间)

	vector<CEnemyData*> m_stBrushAfterDieArr;					//当前阶段按死亡后走cd刷怪的数据
	vector<unsigned int> m_stDieMonsterData;					//死亡怪物的数据

	CC_SYNTHESIZE(CCArray*, _sleepMonsters, SleepMonsters);		//当前阶段休眠中的怪物
	vector<CEnemyData*> m_stCurCarMonsters;						//当前阶段救护车的刷怪点

	CCPoint m_stBuffPos;
};

#endif // FightGuideMapScene_h__
