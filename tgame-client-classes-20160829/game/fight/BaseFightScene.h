#ifndef BaseFightScene_h__
#define BaseFightScene_h__

#include "cocos-ext.h"
#include "BaseMapScene.h"
#include "role/Enemy.h"
#include "role/MainHero.h"
#include "FightBossHeadUI.h"
#include "FightHeroHeadUI.h"
#include "MonsterCfgMaster.h"
#include "SceneBuff.h"
#include "SkillEffLayer.h"
#include "MonsterShowCfgMaster.h"
#include "SkillButton.h"
#include "WeaponBuff.h"
#include "SceneTrap.h"
#include "FireButton.h"
#include "FightHitLayout.h"

USING_NS_CC_EXT;

using namespace gui;

class CBossWarning;
class CAutoFightLayer;
class CFightWeaponArrLayout;

const int NEED_UPDATE_NUM = 1;
const unsigned int KILL_INTER = 3;	//连杀间隔
const float BG_RATIO = 0.1f;		//背景层移动的速率
const float FG_RATIO = 1.5f;		//前景层移动的速率
const float MID_RATIO = 0.6f;		//中景层移动的速率
const float MID2_RATIO = 0.25f;		//中景层移动的速率
const float START_BRUSH_DIS = 500;	//刷怪的触发点
const UINT64_t TEMP_WEAPON_GUID = 9999;	//临时枪械GUID

//胜利条件
enum WIN_CONDITIONS{
	WIN_NONE = 0,				//未通过
	WIN_CLEAN_ALL = 1,			//消灭所有
	WIN_DESTROY_ONE = 2,		//消灭某个
	WIN_ALIVE_TIME = 3,			//一定时间不死
	WIN_DEFEND_TIME = 4,		//守护一定时间
	WIN_DESTROY_NUM = 5 		//消灭一定数量
};

enum FIGHT_MODEL{
	NORMAL_FIGHT = 1,			//普通战斗
	ENDLESS_FIGHT = 2			//无尽模式
};

class CBaseFightScene : public CBaseMapScene, public HudLayerDelegate
{
public:
	static const unsigned int HURT_ARM_TAG;		//受伤特效的tag号
	static const unsigned int KILL_EFF_TAG;		//连杀特效的tag号

public:
	CBaseFightScene();
	virtual ~CBaseFightScene();

	virtual void didChangeDirectionTo(HudLayer *hud, CCPoint direction);
	virtual void isHoldingDirection(HudLayer *hud, CCPoint direction);
	virtual void simpleDPadTouchEnded(HudLayer *hud);

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	virtual void update(float delta);

	virtual void initMap();
	virtual void buildMap();

	virtual void pauseFight();			//暂停战斗
	virtual void resumeFight();			//恢复战斗

	virtual void setMainHeroAsTheOne();
	virtual void unSetMainHeroAsTheOne();
	virtual void pauseAllMonsters();
	virtual void resumeAllMonsters();
	virtual void stopAllMonsters();
	virtual void checkTheOneState();

	//猪脚射击
	virtual void mainHeroShoot();
	virtual void mainHeroCeaseShoot();

	virtual CEnemy* initOneMonster(CEnemyData* monsterObj);							//初始化一个怪物
	virtual CEnemy* initOneMonster(CEnemyData* monsterObj, MonsterCfg* monsterCfg);	//初始化一个怪物

	virtual CCPoint checkXYInWalkable(CCPoint pst);

	//技能释放后，地表buff的管理
	virtual CSceneBuff* addBuffBySkill(CSkillEffLayer* pstSkill, CCPoint point);

	virtual CSceneBuff* addFloorBuff(unsigned int floorBuffID, unsigned int roleParty, CCPoint point, bool isDirectAdd = false);
	virtual CWeaponBuff* addWeaponBuff(unsigned int floorBuffID, unsigned int roleParty, CCPoint point, bool isDirectAdd = false);
	virtual void removeFloorBuff(CSceneBuff* pstBuff, bool isRemoveRoleBuff = true);		//isRemoveRoleBuff是否移除角色身上的buff

	//scene item
	virtual void addSceneItem(CItemObj* pstObj);

	//陷阱
	virtual void addSceneTrap(CTrapObj* pstObj, bool isDirectAdd = false);
	virtual void addPunctureTrap(CTrapObj* pstObj, bool isDirectAdd = false);
	virtual void removeTrap(CSceneTrap* pstTrap, bool isRemoveRoleBuff = true);

	//换枪
	virtual void changeWeapon();
	//某些特定子弹的帧事件
	virtual void HandleBulletKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);

	const CCRect getPlayerVisibleSize(); //找到主角可见的行走区域

	virtual CCRect getStageRect() const;		//获取当前阶段的可行走区域

	virtual void setAttackOpBtnVisible(bool bIsVisible);				//设置攻击、技能按钮是否可见

	virtual UILayout* getSkillLayout(){
		return m_pSkillLayout;
	}

	virtual void openModuleEffect(UIWidget* pLayout);
	virtual void handleOpenModuleEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	virtual void brushOneMonster(CEnemyData* pstObj);
	virtual string getEmergeByData(CEnemyData* pstObj);
	virtual CEnemy* addOneMonster(CEnemyData* pstObj, bool isEmerge);
	virtual void handleComEmergeEvent(CCBone *bone, const char* stEvent, int originFrameIndex, int currentFrameIdx);
	virtual void handleComEmergeComplete(CCArmature * armature, MovementEventType type, const char * name);

	virtual void brushOneMonsterWithWalkIn(CEnemyData* pstObj);			//从屏幕外走入
	virtual void moveToDst(CQStageEntity* pstEntity, CCPoint stPos);	//移动到目的点
	virtual void onMonsterArrived(const string &evt, CCObject *pSender);//到达目的地

	virtual void showBossWarning();										//boss出现警告
	virtual void handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//boss警告播放完成

	virtual bool checkMonsterOnStage(CEnemyData* pstObj);				//检查某刷怪点是否有怪在舞台上

	virtual void brushFromCar(CCObject* pstObject);						//从救护车上刷出怪物
	virtual void brushFromCarOne(CCObject* pstObject);					//一个个从救护车上刷出怪物
	virtual void carMoveEnd(CCObject* pstObject);						//救护车移除屏幕

	virtual bool checkPosInWalkable(CCPoint stPos);						//检测点是否在可行走区域

	virtual CQStageEntity* getFriend();									//获取助战好友

	virtual unsigned int getMonsterNumOnStage();									//检查舞台上的怪物数量	

	virtual void handleArmatureRelease(CCArmature* armature, MovementEventType type, const char* name);

	virtual void startShowBulletOutTip();
	virtual void cancelshowBulletOutTip();
	virtual void showBulletOutTip(float dt);

	virtual void autoChangeNextWeapon();								//当前弹药用尽，自动切换到下一阵位的枪械
	virtual bool isAllWeaponOutBullet();								//弹药是否全部用完
	virtual void checkAllWeaponOutBullet();
	//virtual void autoFillAllBullets();									//判断是否有弹药箱，有就自动补充弹药，没有就弹出弹药补充面板
	virtual void fillAllWeaponBullet(bool bIsInvincible = true);			//补充弹药，如果当前枪械为临时枪械，同样补满
protected:
	CC_SYNTHESIZE(CMainHero*, _mainHero, MainHero);						//猪脚
	CC_SYNTHESIZE(CCArray*, _stageEntities, StageEntities);				//当前舞台上所有目标
	CC_SYNTHESIZE(CCArray*, _sceneItems, SceneItems)					//当前舞台上的item
	CC_SYNTHESIZE(CCArray*, _skills, Skills);							//当前舞台上的技能     
	CC_SYNTHESIZE(CCArray*, _hero_bullets, HeroBullets);				//友方子弹
	CC_SYNTHESIZE(CCSize, _winSize, WinSize);
	CC_SYNTHESIZE(CCArray*, _sceneBuffs, SceneBuffs);					//地表buff的数组,注意每个阶段需要清空
	CC_SYNTHESIZE(CCArray*, _sceneTraps, SceneTraps);					//地表陷阱的数组,注意每个阶段需要清空
	
	CC_SYNTHESIZE(CCArray*, _sleepMonsters, SleepMonsters);				//当前阶段休眠中的怪物
	CC_SYNTHESIZE(CCArray*, _moveEntities, MoveEntities);				//舞台上位移的角色

	CC_SYNTHESIZE(bool, m_bIsAniEffOn, bIsAniEffOn);					//是否可以在怪物身上加特效开关
	CC_SYNTHESIZE(int, m_uCurAniTotalNum, CurAniTotalNum);				//场景中所有怪物身上特效(受伤、死亡、papa)总数
	CC_SYNTHESIZE(int, m_uBulletSkillBuffNum, BulletSkillBuffNum);		//场景中子弹、技能添加buff总数

	CC_SYNTHESIZE(bool, m_bIsShowBulletOut, IsShowBulletOut);			//是否在显示弹药用尽的提示

	float m_fLastAniTime;												//上次在怪物身上加特效的时间
	unsigned int m_uAniMaxNum;											//战斗中受伤、死亡特效数量上限
	float m_fAniInter;													//战斗中受伤、死亡特效间隔时间

	float m_fMidPos;
	bool m_bIsPreOver;													//是否已经准备结束,还在播放胜利、死亡等
	bool m_bIsGameOver;													//game over

	bool m_bIsInTheOneState;											//释放技能状态
	unsigned int m_uTheOneBeginSec;										//释放技能开始时间

	bool m_bDisturbShoot;												//是否打断射击
	bool m_bShootAfterSkill;											//在技能释放后是否立即射击 

	bool m_bIsMainHeroShooting;											//主角是否在射击

	CCArray* m_stRightArrowArr;											//怪物右边指引箭头数组
	CCArray* m_stLeftArrowArr;											//怪物左边指引箭头数组

	CCPoint m_stTargetPos;												//技能释放时目标位置
protected:
	CCLayer* m_pGameLayer;
	UILayer* m_pWidgeLayer;
	UILayer *m_pUiLayer;
	HudLayer* _hudLayer;
	CFightHeroHeadUI* m_pHeroHeadUI;		//猪脚血条
	CFightBossHeadUI* m_pBossHPUI;			//boss血条
	CFightHitLayout* m_pHitLayout;			//连击面板
	CFightWeaponArrLayout* m_pArrLayout;	//枪械阵位
	UILayout *m_pSkillLayout;				//技能layout
	UILayout* m_pPauseLayout;
	FireButton* fireBtn;
	UIButton* m_pAutoBtn;					//托管按钮
	UIButton* pauseBtn;						//暂停按钮
	UIButton* m_pChangeWeaponBtn;			//换枪按钮

	SkillButton* m_trickBtn;				//枪械技能按钮
	SkillButton* m_skillBtn;				//进化技能按钮
	SkillButton* m_dodgeBtn;				//闪避技能按钮

	CCSprite* m_pstBlackScreen;				//黑屏
	CCArmature* m_pstSanguang;				//闪光特效

	CBossWarning* m_pBossWarning;			//boss出现警告

	CAutoFightLayer* m_pAutoLayer;			//托管状态
protected:
	//校验数据
	float m_fFightTime;						//关卡时间

	unsigned int m_uTotalMonsterHp;			//怪物总血量
	unsigned int m_uTotalHarmValue;			//伤害总值
	unsigned int m_uTotalEnemyKilled;		//杀怪总数
	unsigned int m_uTotalLoseValue;			//损失总值
	unsigned int m_uMaxKillValue;			//最高连杀
	unsigned int m_uCurKillValue;			//当前连杀
	unsigned int m_uKillTime;				//杀上一个怪的时间 
	unsigned int m_uScorePercent;			//评分比(万分比)
	unsigned int m_uMark;					//评分
	unsigned int m_uScore;					//星级 (失败为0)
	unsigned int m_uBossID;					//bossID (未杀死为0)
	unsigned int m_uEvolutionCnt;			//进化技使用次数
	unsigned int m_uLimiteTime;				//限制时间
	unsigned int m_uWarLevel;				//关卡等级
	unsigned int m_uDropCoin;				//掉落金币
	unsigned int m_uUseMedicalCnt;			//使用医药包数量
	unsigned int m_uUseReliveCnt;			//使用复活币数量
	unsigned int m_uUseBulletCnt;			//使用弹药箱数量

	//胜利数值
	unsigned int m_uWinType;
	unsigned int m_uWinValue1;
	unsigned int m_uWinValue2;		

protected:
	virtual void setWalkHight();										//设置可行走区域

	virtual void loadHeroRes();											//角色资源的加载，动态的，自己的资源已加载，友方的资源需要每个场景自己重写

	virtual void loadItemRes();

	virtual void loadFloorBuffRes();

	virtual void loadTrapRes();

	virtual void loadHeroResByShowID(unsigned int showID, unsigned int weaponCfgID, bool isPreLoad = true);		//通过showid加载角色资源, isPreLoad是否为预加载

	virtual void loadWeaponeResByCfgID(unsigned int cfgID, bool isPreLoad = true);		//通关武器cfgid加载武器相关资源， isPreLoad是否为预加载

	//预加载技能资源
	virtual void loadSkillRes(unsigned int skillID, bool isPreLoad = true);	

	virtual void loadItemResByID(unsigned int itemID, unsigned int dropType, unsigned int dropID, bool isPreLoad = true);	//加载单个的场景物件相关资源

	virtual void loadFloorBuffByID(unsigned int id, bool isPreLoad = true);		//加载地表buff相关资源

	virtual void loadWeaponStarUpRes(unsigned int weaponCfgID, bool isPreLoad = true);					//加载枪械升星相关的资源

	//预加载陷阱资源
	virtual void loadTrapResByID(unsigned int cfgId, bool isPreLoad = true);

	//预加载buff资源
	virtual void loadBuffRes(unsigned int uBuffID, bool isPreLoad = true);

	//预加载buff资源
	virtual void loadBuffRes(const string& stBuffRes, bool isPreLoad = true);

	//预加载怪物资源, num:怪物数量，isPreLoad:是否为预加载
	virtual void loadMonsterResByCfg(MonsterCfg* stCfg, int num, bool isPreLoad = true);

	virtual void loadMonsterResByCfg(const MonsterShowCfg* stCfg, int num, bool isPreLoad = true);

	//init相关
	virtual void initSceneItem();										//关卡地图item
	virtual void initFloorBuff();										//地表buff
	virtual void initTrap();											//陷阱
	virtual void initMapUI();											//关卡UI
	virtual void initMainHero();										//猪脚
	virtual void initFriend();											//友军
	virtual void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	virtual void initFightData();										//初始化战斗相关的数据

	virtual void startFight();											//开始战斗
	virtual void startUpdate();											//开始更新

	//update相关
	virtual void brushMonsterLogic(float dt);							//刷怪逻辑
	virtual void handlePassLogic();										//过关逻辑

	virtual void updateMainHeroPosition();								//猪脚位置更新

	//地面buff更新
	virtual void updateSceneBuffs(float dt);							//地表buff更新

	//地表陷阱更新
	virtual void updateSceneTraps(float dt);

	//按钮更新
	virtual void updateBtnState();										//按钮更新

	virtual void updateAniEffNum(float dt);								//更新当前怪物身上特效总数

	//怪物AI
	virtual void updateInstnaceByAI();

	//放弃战斗
	virtual void handleGiveUpLogic(CCObject* pstObj);					//中途放弃退出关卡

	virtual bool needTurnBack();										//检测是否翻转方向，只有在射击状态下，正面没有敌人了，背面才有敌人的情况下，才需要翻转
	virtual bool needTurnBackForSkill();
	//怪物指引箭头更新
	virtual void updateMonsterArrow();									//更新舞台上怪物指引箭头
	virtual bool checkArrowCondition(CQStageEntity* pstEntity);			//怪物指引箭头的条件

	//更新舞台上移动，计算伤害
	virtual void updateMoveEntities();									//更新舞台上移动，计算伤害

	virtual void handleGameOver();										//发送结束请求

	virtual void clearAllSceneItems();									//清楚item
	virtual void clearAllBuffs();										//清除buff相关
	virtual void clearAllTraps();										//清除陷阱

	virtual void clearAllSkills();										//结束口清楚所有技能
	virtual void clearAllBullet();										//结束口清楚所有子弹
	virtual void clearAllArrows();										//关卡结束时，清除所有箭头

	virtual void clearAllMonsters();									//关卡胜利时，清除活着的怪物
	virtual void forceOhterMonstersDie();								//关卡胜利时，强制其他怪物死亡

	virtual void updateHit();

	virtual void playKillEff();

	virtual void removeEntityBlock(CQStageEntity* pstEntity);

	virtual void updateMainHeroBulletTime(float dt);

protected:
	//触发事件
	virtual void autoBtnEvent(CCObject *pSender, TouchEventType type);
	virtual void btnTouchEvent(CCObject *pSender, TouchEventType type);

	//主角或友军事件
	virtual void handleHeroHurtValueEvent(const string& stEvent, CCObject* pstEventObj); //掉血
	virtual void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);//主角或友军死亡
	virtual void handleHeroEPCostEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroHPChangeEvent(const string& stEvent, CCObject* pstEventObj);	//主角血量变化
	virtual void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//主角出场播放完毕
	virtual void handleHeroSkillCastEvent(const string& stEvent, CCObject* pstEventObj);		//主角释放技能事件

	//怪物事件
	virtual void handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroEPAddEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void activeEnemyEvent(const string& stEvent, CCObject* pstEventObj);		//怪物从休眠中激活完成事件
	virtual void handleEnemySkillActionEvent(const string& stEvent, CCObject* pstEventObj);	//怪物技能释放时，记录一些数据
	virtual void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡瞬间的数据记录
	virtual void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡动作完成事件
	virtual void handleEnemyDashCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//怪物冲撞结束时间
	virtual void handleEnemyDieEffectEvent(const string& stEvent, CCObject* pstEventObj);	//boss死亡动作中特效的音效事件
	virtual void handleAniEffCountEvent(const string& stEvent, CCObject* pstEventObj);		//记录怪物身上特效数量

	//关键帧事件
	virtual void handleHeroTheOneBeginEvent(const string& stEvent, CCObject* pstEventObj); //主角放大招，设置其为唯一显示
	virtual void handleHeroTheOneEndEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleSkillKeyFrameEvent(const string& stEvent, CCObject* pstEventObj); //放技能关键帧事件，计算伤害等信息
    virtual void handleFireABulletEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleCloseAtkKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);		
	virtual void handleDieEffKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);		//死亡特效帧事件
	virtual void handleSkillBtnStateEvent(const string& stEvent, CCObject* pstEventObj);		//技能按钮状态改变，释放中其他按钮不可点
	virtual void handleHeroDashCompleteEvent(const string& stEvent, CCObject* pstEventObj);		//主角闪避完成事件，加入无敌buff

	//场景物件事件
	virtual void handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj);	//物件消失

	//地表buff事件
	virtual void handleSceneBuffDisappearEvent(const string& stEvent, CCObject* pstEventObj);

	//地表陷阱
	virtual void handleSceneTrapDisappearEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSceneTrapHitKeyEvent(const string& stEvent, CCObject* pstEventObj);

	//技能按钮事件
	virtual void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);

	//换枪
	virtual void handleHeroWeaponChange(const string& stEvent, CCObject* pstEventObj);

	//检查是否需要踢翻对方
	virtual void handleTryKickEvent(const string& stEvent, CCObject* pstEventObj);
	
	//处理踢人事件
	virtual void handleKickEvent(const string& stEvent, CCObject* pstEventObj);

	//胜利动作播放完成
	virtual void hanldeWinCompleteFun(const string& stEvent, CCObject* pstEventObj);

	//连击超时事件
	virtual void handleHitTimeOutEvent(const string& stEvent, CCObject* pstEventObj);

	//金币掉落
	virtual void addCoinEff(CQStageEntity* pEntity, unsigned int uNum);
	virtual void updateCoinPos(float dt);								//掉落金币的位置更新
	const string getCoinActName(unsigned int uNum);
	virtual void handleCoinActComplete(CCArmature* armature, MovementEventType type, const char* name);
	virtual void handleCoinEndEvent();

	//自动战斗
	virtual void addAutoFightLayer();
	virtual void handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void removeAutoFightLayer();
	virtual void autoFightHandle();

	//玩家死亡
	virtual void doHeroDieEvent();	
	virtual void doHeroDieFail();				//玩家未复活，战斗失败
	virtual void handleHeroReliveDoneEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleHeroCancelReliveEvent(const string& stEvent, CCObject* pstEventObj);

	//弹药用尽
	virtual void doAllBulletOut();
	virtual void handleFillBulletEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleCancelFillBulletEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void doCancelHeroAutoFight();

	//医药箱用尽
	virtual void doMedicalBtnClick();
	virtual void doMedicalEff();				//医药包作用函数,固定回血50%(仅限主角)
	virtual void doMedicalOut();
	virtual void handleBuyMedicalEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleCancelBuyMedicalEvent(const string& stEvent, CCObject* pstEventObj);

private:
	void updateSkillEffPos();
	void updateBullets(float dt);

	void changeTempWeapon(unsigned int weaponBuffID);					//更换临时枪械
	void weaponUpStar(unsigned int buffType);							//枪械升星buff

	void handleSanguangActionEvent(CCArmature * armature, MovementEventType type, const char * name);
	void handleDieEffActionEvent(CCArmature * armature, MovementEventType type, const char * name);
	void handleMainHeroHurtComplete(CCArmature * armature, MovementEventType type, const char * name);

/************************************************************************/
/*                       boss技能                                       */
/************************************************************************/
public:
	//预警播放完成事件
	virtual void handleWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj);

	//技能事件
	virtual void handleSkillEffEvent(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSkillEffAtkFrame(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSkillEffSpawnFrame(const string& stEvent, CCObject* pstEventObj);
	virtual void handleSceenShakeEvent(const string& stEvent, CCObject* pstEventObj);		//震屏帧事件
private:
	unsigned int m_uSkillEffCnt;				//boss的一个技能中特效重复次数
	unsigned int m_uCurSKillCnt;				//boss当前技能特效已经重复次数
	CQStageEntity* m_pSkillAtker;				//技能释放者
	CQSkillCell* m_pSkillCell;					//技能数据
	vector<CCPoint> m_stSkillPoints;			//boss一个技能的多个特效的位置

protected:
	CCArray* m_stAddCoins;						//需要添加的金币
	CCArray* m_stCoinEffs;						//金币特效集合

	bool m_bIsPlayKillEff;						//是否播放连杀特效
	bool m_bIsAddCoinEff;						//是否播放金币掉落

	bool m_bIsGamePause;						//战斗是否暂停
	bool m_bIsWalkNextStage;					//是否正在通过阶段
public:
	//boss相关技能
	void skillRandFlyToGroud(CQStageEntity* pstAtker, CQSkillCell* pstSkill);
	void skillStepUpdate(float dt);
	void getSkillAttackPos(CQSkillCell* pstSkill);				//获取技能打击的位置，其中一个为主角位置，其他在主角周围随机
};

#endif // BaseFightScene_h__
