#ifndef __GUARD_ACT_SCENE_h__
#define __GUARD_ACT_SCENE_h__


#include "fight/BaseFightScene.h"
#include "OSDSceneInf.h"
#include "BasicTypes.h"

using namespace vmsg;

class CGuardActRankUI;
class CGuardActPrizeUI;
class DummyHero;
class DummyEnemy;
class CEnemyData;
class CReviveLayer;

class GuardActScene : public CBaseFightScene, public OSDSceneInf
{
public:
    //定义一些常量
    enum {
        PATH_LINE_NUM       = 5, //怪物按五条路线行走
        PATH_LINE_HEIGHT    = 50, //一条路线的高度
        ENEMY_UIN_MAX       = 1000, //怪物Uin的最大值
        BASE_STAT_LOSE      = 0, //活动失败
        BASE_STAT_DIE       = 1, //基地挂了
        BASE_STAT_LIVE      = 2, //基地还在
        SCREEN_NUM          = 2, //
    };

public:
    GuardActScene();
    virtual ~GuardActScene();

    CREATE_FUNC(GuardActScene);
    bool init();
    virtual void onEnterTransitionDidFinish();
    void update(float delta);
    virtual void cleanup();

    void initMap();
    void buildMap();

    void loadMonsterRes();										//怪物资源的加载，动态
    void loadItemRes();											//场景item资源的加载，动态

    void brushMonsterLogic(float dt); //在BaseFightScene帧的update里触发

    //osd
	void addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo);
    void addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction); // 添加一个角色到场景中
    void removeRole(UIN_t uin); // 从场景中移除一个角色
    void setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction); // 设置一个角色的位置，移动到指定位置
    void moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction); // 通过方向移动角色
    void moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction); // 移动角色到目的地
    virtual void setPlayerStartAtkStat(const UIN_t uin, const vmsg::CSPlayerStartAtk& stStartAtk); // 设置角色射击状态
    virtual void setPlayerStopAtkStat(const UIN_t uin, const vmsg::CSPlayerStopAtk& stStopAtk); // 设置角色停止射击状态
    virtual void addMonster(const vmsg::CSOSDMonstersInfo& monsterInfo); // 添加一个怪物到场景中
    virtual void setPlayerSkillInScene(const UIN_t uin, const vmsg::CSPlayerSkillInScene& stSkillInScene); // 设置角色释放技能

    // hud layer delegate
    virtual void didChangeDirectionTo(HudLayer *hud, CCPoint direction);
    virtual void isHoldingDirection(HudLayer *hud, CCPoint direction);
    virtual void simpleDPadTouchEnded(HudLayer *hud);

    virtual void mainHeroShoot(); //重写射击
    virtual void mainHeroCeaseShoot();

    //处理协议
    void setRoomReady();
    void doControlNotify(const CSCVZControlNotify& stCtrlNotify); //更新控制怪物信息
    void doHitMonsterNotify(const CSCVZHitMonsterNotify& stHitMonsterNotify); //同步怪物血量
    void doHitedNotify(const CSCVZHitedNotify& stHitedNotify); //Role被攻击
    void doDestroyBaseNotify(const CSCVZDestroyBaseNotify& stDestroyBaseNotify); //摧毁基地
    void doFail(); //活动失败
    void doMonstersInfoNotify(const CSCVZMonsters& stMonstersInfo); //更新怪物状态和位置
    void doRankInfoNotify(const CSCVZRankInfo& stMonstersInfo); //更新排行
    void doWaveTimeNotify(const int iTime); //更新刷怪时间显示
    void doActOver(); //活动结束
    void doTotalContNotify(const int iTotalCont); //我的贡献值通知
    void doMyContAwardCoinNotify(const int iCoin); //我的Coin通知

protected:
    void updateInstnaceByAI();
    virtual void setWalkHight();								//设置可行走区域
	virtual void handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj);	//主角出场播放完毕
    virtual void handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj); //友军掉血
    virtual void handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleEnemyDieCompleteEvent(const std::string& stEvent, CCObject* pstEventObj); //怪物死亡
	virtual void handleEnemyUpDataEvent(const std::string& stEvent, CCObject* pstEventObj);		//怪物强制一滴血时上报数据

    virtual void initMapUI();											//关卡UI
	virtual void initMonster();
    virtual void handlePassLogic();										//过关逻辑
    void doGiveUP();													//点了退出按钮

    virtual void setMainHeroAsTheOne() {
    }

    //技能按钮事件
    virtual void handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj);
    virtual void handleGiveUpLogic(CCObject* pstObj);

	virtual void btnTouchEvent(CCObject *pSender, TouchEventType type);
private:
    DummyHero * initOneDummyHero(const UIN_t uin, const CCPoint& pos, unsigned int showID, int weaponCfgID); //初始化一个队友数据
    DummyEnemy* initOneDummyEnemy(const UIN_t uin, const CCPoint& pos, const int level, const int wave); //初始化一个怪物数据
    int genMonsterCfg(int level, int iWave, MonsterCfg& cfg);

    void doUploadHitMonster(); //上报我的杀怪数据
    void doUploadMonsterInfo(); //上报我控制的怪物位置数据
     
    inline bool isEnemyUin(const UIN_t uUin) {
        return (uUin <= ENEMY_UIN_MAX);
    }
    CQStageEntity* getStageEntity(const UIN_t uUin);


    void showReviveLayer();
    void onReviveDone(const std::string& stEvent, CCObject* pstEventObj);


    void showWaitingRoomLayer(); //显示等待匹配房间

    CCPoint randMonsterPos(const int iChannel); //roll 一个位置

	void haneldBossArmCallBack(CCArmature* armature, MovementEventType type, const char* stName);
	void addBossByIdx(int iIdx);	
	void bossArmBoom(int iIdx);  //基地爆炸
	void clearBossArm(CCArmature* armature);
private:
    CGuardActRankUI *m_pRankUI; //排名UI
    CGuardActPrizeUI *m_pPrizeUI; //奖励UI

    CCDictionary *m_pRolesDict; // 场景中的队友角色列表 <UIN_t, DummyHero>
    CCDictionary *m_pEnemysDict; // 场景中的怪物列表 <UIN_t, DummyEnemy>

    CCArray* m_stHeroRes;			//人物资源
    CCPoint m_stLastDirection;

    std::map<UIN_t, unsigned int> m_stPlayerHurtData; //<怪物UIN, 伤害值>，记录本次上报间隔期间主角对各个怪物的伤害

    bool m_bIsRoomReady; //是否匹配好了房间

    CCPoint m_stMainHeroInitPos; //主角的初始位置（复活时重置）
    CReviveLayer* m_pReviveLayer;

    std::set<UIN_t> m_stMyCtrlEnemy; //由我AI驱动的怪物
    int m_stBaseStat[PATH_LINE_NUM]; //0表示被摧毁，1表示还在

	CCArmature* m_stBossArm[PATH_LINE_NUM];		//5个主基地

    bool m_bIsActOver; //活动是否结束
	
	map<int, CCPoint> m_stID2Point;		//id对应位置
};


#endif // __GUARD_ACT_SCENE_h__
