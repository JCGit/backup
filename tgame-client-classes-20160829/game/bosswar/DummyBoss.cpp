#include "DummyBoss.h"

const string CDummyBoss::BOSS_DIE_UP_DATA = "boss_die_up_data";		//怪物强制一滴血时上报数据

CDummyBoss::CDummyBoss(){

}

CDummyBoss::~CDummyBoss(){

}

void CDummyBoss::HurtWithDamage(float fDamage){
	if(checkIsDied()){		
		return;
	}

	if(fDamage > _hp){
		fDamage = _hp;
	}

	ShowBloodNum(UINT64_t(fDamage), false);
	_hp -= (UINT64_t)fDamage;

	bool isForeHp = (_hp <= 0); 

	if(isForeHp){  //留一滴血吧，等同步血量时判断是否死亡
		_hp = 1;
		DispatchEvent(BOSS_DIE_UP_DATA, this);			//强制一滴血时，立马上报
	}

	ShowHurt((int)fDamage);
	HPChange();
	if (_state != kActionStateHurt){
		_preState = _state;
	}

	Hurt();
}

void CDummyBoss::SyncHP(UINT64_t iHP) {
	if(IsDead()){
		return;
	}

	_hp = iHP; 

	if(checkIsDied()){
		ForceIdle();
		Die();
	}
}

