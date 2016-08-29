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

    virtual void HurtWithDamage(float fDamage); //重写QStageEntity的方法

    //同步的数据
    void SyncHP(const int iHP); //同服务器同步血量
    void SyncPos(const CCPoint& stPos); //同服务器同步位置
};

#endif

