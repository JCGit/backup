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
	static const string ENEMY_DIE_UP_DATA;		//����ǿ��һ��Ѫʱ�ϱ�����

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
    virtual void HurtWithDamage(float fDamage); //��дQStageEntity�ķ���
    virtual void HurtWithDamage(CHurtDamageObj stObj);

    virtual void update(float dt); //update? update what? 
    virtual void AttackWithSkillAtIdx(int iIdx); //��дһ�·ż���

    //ͬ��������
    void SyncHP(const int iHP); //ͬ������ͬ��Ѫ��
    void SyncPos(const CCPoint& stPos); //ͬ������ͬ��λ��

    void setUin(UIN_t uEnemyUin){
        m_uEnemyUin = uEnemyUin;
    }
    UIN_t getUin(){
        return m_uEnemyUin;
    }

    void setActionStat(const int iNewStat, const int xPos);
    int getActionStat();
    

    CC_SYNTHESIZE(int, m_iPosChannel, PosChannel); //�������������� [0-4]
    CC_SYNTHESIZE(bool, m_bIsArrive, IsArriveBase); //�Ƿ��ߵ��˻���
    CC_SYNTHESIZE(bool, m_bNeedSendSkill, NeedSendSkill); //�Ƿ���Ҫ�ϱ��ͷż���
private:
    void doUploadStat(const int iNewStat, const int iPreStat);
private:
    int m_iBulletRoleType; //���㱾���˺�ʱ��Bullet��RoleType

    UIN_t m_uEnemyUin;

    int prestat;
    float preStatTime; //��֤1.0����û���ظ��Ķ����ϱ�

    int m_iActionStat;
};

#endif

