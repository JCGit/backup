#ifndef __DUMMY_HERO_H__
#define __DUMMY_HERO_H__

#include "role/MainHero.h"

class DummyHero:public CMainHero
{
public:
    DummyHero();
    ~DummyHero();

    CREATE_FUNC(DummyHero);
    bool init(){
        CMainHero::init();
        return true;
    }

    virtual void HurtWithDamage(float fDamage); //��дQStageEntity�ķ���

    //ͬ��������
    void SyncHP(const int iHP); //ͬ������ͬ��Ѫ��
    void SyncPos(const CCPoint& stPos); //ͬ������ͬ��λ��
};

#endif

