#ifndef FightMapScene_h__
#define FightMapScene_h__

#include "BaseFightScene.h"
#include "role/Enemy.h"
#include "FightTipLayout.h"

class CBossWarning;
class CNextStageTipLayer;

class CFightMapScene : public CBaseFightScene
{
public:
    virtual void release(void);
    virtual void retain(void);
    virtual CCObject* autorelease(void);

	CFightMapScene();
	virtual ~CFightMapScene();

	CREATE_FUNC(CFightMapScene);

    bool init();
    void onExit();

	virtual void onEnterTransitionDidFinish();

	void update(float delta);

	void initMap();

	void loadMapBgRes();
	void loadHeroRes();											//角色资源的加载，动态的
	void loadMonsterRes();										//怪物资源的加载，动态
	void loadItemRes();											//场景item资源的加载，动态
	void loadFloorBuffRes();									//场景buff资源加载
	void loadTrapRes();											//陷阱资源加载

protected:
	//init相关
	void initSceneItem();										//关卡地图item
	void initFloorBuff();										//地表buff
	void initTrap();											//陷阱
	void initMapUI();											//关卡UI
	void initFriend();											//友军
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	virtual void initFightData();								//初始化战斗相关的数据

	void initSleepMonsters();									//初始化休眠的怪物放在舞台上

	//触发事件
	void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//update相关
	void updateInstnaceByAI();									//怪物AI
	void brushMonsterLogic(float dt);							//刷怪逻辑
	void handlePassLogic();										//过关逻辑
	void handleGiveUpLogic(CCObject* pstObj);

	//视角update
	void setViewpointCenter(const CCPoint& position);			//视角居中
	void updateBgViewCenter(float foreX);						//更新地图前景、远景的视角(根据地图不同，改写)

	void handleGameOver();										//发送结束请求

protected:
	//主角死亡，未复活战斗失败
	void doHeroDieFail();		

	//主角或友军事件
	void handleFriendDieEvent(const string& stEvent, CCObject* pstEventObj);		//友军死亡
	void handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj);	//友军掉血
	void hanldeWinCompleteFun(const string& stEvent, CCObject* pstEventObj);		//胜利动作播放完成

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

private:
	void checkShowGoTip();										//检测是否提示向前走，当舞台上无怪物，但本阶段未过关，提示向前
	void showGoTip();											//前进提示
	void cancelGoTip();											//取消提示

	void checkBrushSceneItem();									//检查是否按触发线刷物件 OR BUFF
	void brushSceneItemByTouch(unsigned int touchID);			//根据触发线刷物件
	void brushFloorBuffByTouch(unsigned int touchID);			//根据触发线刷floor buff
	void brushTrapByTouch(unsigned int touchID);				//根据触发线刷陷阱

	void checkShowStage();										//显示当前阶段

	void brushMonster();										//刷怪具体逻辑

	void handleFailLogic();										//失败逻辑

	void doNormalWarHideAward();								//普通关卡隐藏奖励判断
	bool IsAchieveHideAward(unsigned int uType, unsigned int uValue);	//检测是否达到隐藏条件

	void doDailyInstanceOver();

	void setAtkBtnEnable(bool isEnable);
	void checkNextStage();										
	void checkArrivedEdge();
	void walkToNextStage();
	void removeStageTip();
	void nextStage();											//下一阶段
	void setNextStagePos();
	void handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj);
	void handleTransitionComplete(const string& stEvent, CCObject* pstEventObj);

	void passStageViewChange(float dt);

	//结算逻辑
	void handleSuccessLogic();									//成功逻辑
	bool checkWinLogic(unsigned int cfgID);						//检查通过条件

	//关卡boss出现在视野中时，弹出boss介绍面板
	void checkBossInView();
	void showBossEmergePanel();

	CCRect getStageRect() const;								//获取当前阶段的可行走区域

protected:
	CFightTipLayout* m_pnextStageTip;							//下一阶段提示
	CNextStageTipLayer* m_pTipLayer;							//下一阶段提示

protected:
	unsigned int m_uTotalStage;									//总阶数
	unsigned int m_uCurStageIdx;								//当前阶段，从1开始(坑爹的策划说的)

	unsigned int m_uCurStageMonNum;								//当前阶段怪物数量

	float m_fStageStartTime;									//每阶段开始的时间

	float m_fStartWalkTime;										//过阶段时主角开始行走时间

	bool m_bStartedBrush;										//本阶段是否开始刷怪
	bool m_bHasShowStage;										//是否显示过当前阶段

	bool m_bIsAutoWalking;		
	bool m_bIsArrivedEdge;

	bool m_bIsIntroduced;										//boss是否已经介绍过

	unsigned int m_uLastKillID;									//胜利的时候杀死的最后一个怪物id
	bool m_bIsWinComplete;										//胜利特效、动作是否播放完成

	map<int, vector<CEnemyData*> > m_stIDXToMonsters;			//记录每个阶段的刷怪点，<int, CCArray*>阶段索引对应怪物
	map<int, int> m_stIDXToMonsterNum;							//记录每个阶段的怪物数量
	map<int, vector<CItemObj*> > m_stIDXToItems;				//记录每个阶段的item，<int, CCArray*>阶段索引对应item
	map<int, vector<CFloorBuffObj*> > m_stIDXToFloorBuffs;		//记录每个阶段的floorbuff，<int, CCArray*>阶段索引对应floorbuff
	map<int, vector<CTrapObj*> > m_stIDXToTraps;				//记录每个阶段的陷阱，<int, CCArray*>阶段索引对应陷阱

	vector<CEnemyData*> m_stCurStageMonsters;					//当前阶段刷怪点的信息

	vector<CEnemyData*> m_stBrushAfterDieArr;					//当前阶段按死亡后走cd刷怪的数据
	vector<unsigned int> m_stDieMonsterData;					//死亡怪物的数据
};

#endif // FightMapScene_h__
