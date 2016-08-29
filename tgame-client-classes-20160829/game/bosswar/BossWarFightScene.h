#ifndef BossWarFightScene_h__
#define BossWarFightScene_h__

#include "fight/BaseFightScene.h"
#include "DummyBoss.h"
#include "CSBossChallengeMsg.pb.h"

class CBossWarWaitLayer;
class CHeroReviveLayer;
class CBossWarInfoLayout;

using namespace vmsg;

class CBossWarFightScene : public CBaseFightScene
{
public:
	enum {
		WORLD_BOSS_SCENE_TYPE = 100001,
	};

public:
	CBossWarFightScene();
	virtual ~CBossWarFightScene();

	CREATE_FUNC(CBossWarFightScene);

	bool init();

	void onEnterTransitionDidFinish();

	void update(float delta);

	void loadMapBgRes();										//地图背景资源的加载，动态
	void loadHeroRes();											//角色资源的加载，动态的
	void loadMonsterRes();										//怪物资源的加载，动态
	void loadItemRes();											//场景item资源的加载，动态
	void loadFloorBuffRes();									//场景buff资源加载
	void loadTrapRes();											//陷阱资源加载

	void initMap();

	//协议处理
	void onBuyReviveCallBack();									//复活协议返回处理
	void doDamageNotify(const CSBossChallengeDamageNotification& stRsp);	//伤害同步

	void doWarOver();

protected:
	void setWalkHight();										//设置可行走区域
	void initSceneItem();										//关卡地图item
	void initFloorBuff();										//地表buff
	void initTrap();											//陷阱
	void initMapUI();											//关卡UI
	void initMainHero();
	void initMonster();											//初始化当前阶段的怪物，预加载下一阶段的怪物资源
	void initFightData();										//初始化战斗相关的数据

	void startUpdate();											//开始更新

	void brushMonsterLogic(float dt);							//刷怪逻辑
	void handlePassLogic();										//过关逻辑
	void handleGiveUpLogic(CCObject* pstObj);

	void setViewpointCenter(const CCPoint& position);			//视角居中

	void handleGameOver();										//发送结束请求

	void btnTouchEvent(CCObject *pSender, TouchEventType type);
protected:
	//主角或友军事件
	void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//主角出场播放完毕

	//怪物事件
	void handleEnemyUpDataEvent(const string& stEvent, CCObject* pstEventObj);		//怪物强制一滴血时上报数据
	void handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj);	//怪物死亡瞬间的数据记录

	void handleCloseAtkKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleSkillKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleFireABulletEvent(const string& stEvent, CCObject* pstEventObj);
	void handleDieEffKeyFrameEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemySkillActionEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDashCompleteEvent(const string& stEvent, CCObject* pstEventObj);
	void handleEnemyDieEffectEvent(const string& stEvent, CCObject* pstEventObj);
	void handleHeroEPAddEvent(const string& stEvent, CCObject* pstEventObj);

	//技能
	void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);
private:
	void doUploadHitMonster();		//上报我的伤害

	CDummyBoss* initOneDummyBoss(unsigned int uCfgID, unsigned int uLv); //初始化一个怪物数据

	//显示准备
	void showWaitLayer();
	void onWaitDone(const string& stEvent, CCObject* pstEventObj);

	//角色复活
	void showReviveLayer();
	void onReviveDone(const string& stEvent, CCObject* pstEventObj);

	void showDamageEff(UINT64_t uDamge);
	void showDamageEffCallBack(CCObject* pstObj);

private:
	CDummyBoss* m_pBoss;

	CBossWarWaitLayer* m_pWaitLayer;
	CHeroReviveLayer* m_pReviveLayer;
	CBossWarInfoLayout* m_pWarInfoLayout;

private:
	CCPoint m_stBossInitPos;		//boss位置
	CCPoint m_stMainHeroInitPos;	//主角的初始位置（复活时重置）

	bool m_bIsShowWait;				//是否显示准备时间

	unsigned int m_uBossShowID;

	bool m_bIsUpSkillID;			//是否上报使用绝招
	unsigned int m_uUpHarmValue;	//上报伤害值

	UINT64_t m_uInitBossHp;
	UINT64_t m_uPreBossHp;		
};

#endif // BossWarFightScene_h__
