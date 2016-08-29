#include "DummyHero.h"
#include "city/CityControl.h"
#include "city/CityProxy.h"

DummyHero::DummyHero(){

}

DummyHero::~DummyHero(){

}


void DummyHero::HurtWithDamage(float fDamage)
{
    if(checkIsDied()){		
        return;
    }

    if(fDamage > _hp){
        fDamage = _hp;
    }

    ShowBloodNum(int(fDamage), false);
    //�Լ������Ķ��ѱ���������Ѫ��ֻ�ǲ�����������ͬ��Ѫ��
    _hp -= (int)fDamage;  
    _hp = (_hp <= 0) ? 1: _hp; //��һ��Ѫ�ɣ���ͬ��Ѫ��ʱ�ж��Ƿ�����

    ShowHurt((int)fDamage);
    HPChange();
    if (_state != kActionStateHurt){
        _preState = _state;
    }

    if(!checkIsDied() 
        && IsActionInterruptable()
        && _state != kActionStateHurt
        && getArmature()
        && getNextHurtActionTime() <= 0)
    {
        SwitchRes("hurt");
        getArmature()->stopAllActions();
        getArmature()->getAnimation()->play("hurt", -1, -1, 0);
        _preState = _state;
        m_stPreVocity = _velocity;
        _state = kActionStateHurt;
        OtherHurt();
        _velocity = ccp(0, 0);
        setNextHurtActionTime(m_iEffectHurtActionCD);

    }else if(checkIsDied()){
        //Die();
    }
}


void DummyHero::SyncHP(const int iHP) {
    if(iHP > _hp){
        //do?
    }
    _hp = iHP; 
    if(checkIsDied()){
        Die();
    }
}

void DummyHero::SyncPos(const CCPoint& stPos){
    if(checkIsDied()){
        if(stPos.x != getPositionX() && stPos.y != getPositionY()) {
            setPositionX(stPos.x);
            setPositionY(stPos.y);
        }
        return;
    }
    WalkTo(stPos);
}
