#ifndef __DUMMY_ENEMY_H__
#define __DUMMY_ENEMY_H__

#include "role/Enemy.h"
#include "BasicTypes.h"

class DummyEnemyHurtEventObj : public CCObject
{
public:
    DummyEnemyHurtEventObj() {
        uEnemyUin = 0;
        iHurtVal = 0;
    }
    bool init(){return true;}
    CREATE_FUNC(DummyEnemyHurtEventObj);


    unsigned int iHurtVal;
    UIN_t uEnemyUin;
};

class DummyEnemy:public CEnemy
{
public:
	static const string ENEMY_DIE_UP_DATA;		//怪物强制一滴血时上报数据

    enum {
        EMPTY_SKILL_ID = 100,
    };

public:
    DummyEnemy();
    ~DummyEnemy();

    CREATE_FUNC(DummyEnemy);
    bool init(){
        CEnemy::init();
        return true;
    }

    void ShowHurt(int fDamage);
    virtual void HurtWithDamage(float fDamage); //重写QStageEntity的方法
    virtual void HurtWithDamage(CHurtDamageObj stObj);

    virtual void update(float dt); //update? update what? 
    virtual void AttackWithSkillAtIdx(int iIdx); //重写一下放技能

    //同步的数据
    void SyncHP(const int iHP); //同服务器同步血量
    void SyncPos(const CCPoint& stPos); //同服务器同步位置

    void setUin(UIN_t uEnemyUin){
        m_uEnemyUin = uEnemyUin;
    }
    UIN_t getUin(){
        return m_uEnemyUin;
    }

    void setActionStat(const int iNewStat, const int xPos);
    int getActionStat();
    

    CC_SYNTHESIZE(int, m_iPosChannel, PosChannel); //怪物在哪条线上 [0-4]
    CC_SYNTHESIZE(bool, m_bIsArrive, IsArriveBase); //是否走到了基地
    CC_SYNTHESIZE(bool, m_bNeedSendSkill, NeedSendSkill); //是否需要上报释放技能
private:
    void doUploadStat(const int iNewStat, const int iPreStat);
private:
    int m_iBulletRoleType; //计算本次伤害时的Bullet的RoleType

    UIN_t m_uEnemyUin;

    int prestat;
    float preStatTime; //保证1.0秒内没有重复的动作上报

    int m_iActionStat;
};

#endif

