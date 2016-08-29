#ifndef DummyBoss_h__
#define DummyBoss_h__

#include "role/Enemy.h"
#include "BasicTypes.h"

class CDummyBoss : public CEnemy
{
public:
	static const string BOSS_DIE_UP_DATA;		//怪物强制一滴血时上报数据

	CDummyBoss();
	virtual ~CDummyBoss();

	CREATE_FUNC(CDummyBoss);
	bool init(){
		CEnemy::init();
		return true;
	}

	virtual void HurtWithDamage(float fDamage); //重写QStageEntity的方法

	//同步的数据
	void SyncHP(UINT64_t iHP); //同服务器同步血量

protected:
private:
};

#endif // DummyBoss_h__
