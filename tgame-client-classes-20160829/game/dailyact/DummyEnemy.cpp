#include "DummyEnemy.h"
#include "DailyActControl.h"
#include "city/CityControl.h"
#include "city/CityProxy.h"

const string DummyEnemy::ENEMY_DIE_UP_DATA = "enemy_die_up_data";		//怪物强制一滴血时上报数据

DummyEnemy::DummyEnemy(){
    prestat = 0;
    preStatTime = 0;
    m_iPosChannel = -1;
    m_bIsArrive = false;
    m_iActionStat = vmsg::MONSTER_ACTION_SCENE_WALK;
    m_bNeedSendSkill = false;
}

DummyEnemy::~DummyEnemy(){

}



void DummyEnemy::SyncHP(const int iHP) {
    if(IsDead()){
		return;
	}

    _hp = iHP; 
    if(checkIsDied()){
		ForceIdle();
        Die();
    }
}

void DummyEnemy::SyncPos(const CCPoint& stPos){
    if(checkIsDied()){
        if(stPos.x != getPositionX() && stPos.y != getPositionY()) {
            setPositionX(stPos.x);
            setPositionY(stPos.y);
        }
        return;
    }
    WalkTo(stPos);
}


void DummyEnemy::HurtWithDamage(CHurtDamageObj stObj) 
{
    m_iBulletRoleType = stObj.iBulletRoleType;

    CEnemy::HurtWithDamage(stObj);
}


void DummyEnemy::HurtWithDamage(float fDamage)
{
    ShowBloodNum(int(fDamage), false);
    //自己看到的队友被打照样扣血，只是不死亡，定期同步血量
    if(ROLE_TYPE_FRIEND == m_iBulletRoleType) {

    } else {
        _hp -= (int)fDamage;  
        _hp = (_hp <= 0) ? 1: _hp; //留一滴血吧，等同步血量时判断是否死亡

		if(_hp == 1){   //强制一滴血时，立马上报
			DispatchEvent(ENEMY_DIE_UP_DATA, this);
		}
    }
    

    ShowHurt((int)fDamage);
    HPChange();
    if (_state != kActionStateHurt){
        _preState = _state;
    }

    Hurt();
}



void DummyEnemy::ShowHurt(int fDamage)
{ 
    if(fDamage > this->getHP()){
        //fDamage = this->getHP();
    }
    DispatchEvent(ENEMY_HURT_EVENT, this); //发出受伤时的事件，展示界面上怪物血条

    //只记录自己的伤害吧
    if(ROLE_TYPE_PLAYER == m_iBulletRoleType && fDamage > 0) {
        DummyEnemyHurtEventObj* hurtObj = DummyEnemyHurtEventObj::create();
        hurtObj->uEnemyUin = m_uEnemyUin;
        hurtObj->iHurtVal = fDamage;
        
        DispatchEvent(HERO_HARMVALUE_CHANGE, hurtObj);		//怪物受伤的数值上报，计入总伤害 
        //CCLog("-->%lu,%d", m_uEnemyUin, fDamage);
    }
    
}


void DummyEnemy::update(float dt) {
    CEnemy::update(dt);

    int newstat = getMyState();
    const int stat = getMyState();
    switch(stat){
    case kActionStateAttack:
    case kActionStateIdle:
    case kActionStateNone:
        m_iActionStat = vmsg::MONSTER_ACTION_SCENE_HIT;
    default:
        m_iActionStat = vmsg::MONSTER_ACTION_SCENE_WALK;
    }

    if(GetIsControlledByAI() && prestat != newstat && preStatTime >= 1.0) {
        //上报怪物状态信息
        //CCLog("[trace]DummyEnemy::update, uin %lu, new stat %d, %f, %f",m_uEnemyUin, newstat, dt, preStatTime);
        //上报
        //doUploadStat(newstat, prestat);

        prestat = newstat;
        preStatTime = 0.0;
        return;
    }
    preStatTime += dt;
}



void DummyEnemy::doUploadStat(const int iNewStat, const int iPreStat) {
    /*
    CCPoint direct(this->getScaleX(), 0);
    if(kActionStateAttack == iPreStat) {
        //发送停止攻击
        P_CITY_CTRL->getProxy()->sendPlayerAtkRqst(false, direct, getPosition());
        //return; //暂时注释
    }
    switch(iNewStat) {
    case kActionStateIdle:
        P_DAILYACT_CTRL->getProxy()->sendMonsterSetPosRqst(direct, this->getPosition(), m_uEnemyUin);
        break;
    case kActionStateAttack:
        P_CITY_CTRL->getProxy()->sendPlayerAtkRqst(true, direct, getPosition());
        break;
    case kActionStateWalk:
        P_DAILYACT_CTRL->getProxy()->sendMonsterMoveRqst(direct, this->getPosition(), m_uEnemyUin);
        break;
    default:
        CCLOG("[trace]DummyEnemy::doUploadStat, stat %d, do nothing", iNewStat);
    }*/
}


int DummyEnemy::getActionStat(){
    return m_iActionStat;
}


void DummyEnemy::setActionStat(const int iNewStat, const int xPos){
    int deltaX = getPositionX() - xPos;
    if(abs(deltaX) >= 20) {
        setPositionX(xPos);
    }
    m_iActionStat = iNewStat;
    /*
    if(vmsg::MONSTER_ACTION_SCENE_WALK == iNewStat) {
        setIsFollowActivate(true);
        WalkWithDirection(ccp(-1, 0));
    } else if(vmsg::MONSTER_ACTION_SCENE_HIT == iNewStat) {
        //Attack();
        getArmature()->stopAllActions();
        getArmature()->getAnimation()->play("attack", -1, -1, 1);
        setMyState(kActionStateAttack);
    }*/
}


void DummyEnemy::AttackWithSkillAtIdx(int iIdx) {
    CEnemy::AttackWithSkillAtIdx(iIdx);
    CCLOG("---DummyEnemy,skill idx %d,total %d", iIdx, _skillArray->count());
    if(!m_bNeedSendSkill) {
        return;
    }

    if(iIdx == (_skillArray->count() - 1)) {
        return; //最后一个技能是普通攻击
    }
    //上报释放技能
    CCPoint direct(getScaleX(), 0);
    

    vmsg::CSCVZMonsterInfo stInfoRqst;
    stInfoRqst.set_monsterid(m_uEnemyUin);
    stInfoRqst.set_action(vmsg::MONSTER_ACTION_SCENE_HIT);
    stInfoRqst.mutable_pos()->set_xpos(getPositionX());
    stInfoRqst.mutable_pos()->set_ypos(getPositionY());
    stInfoRqst.set_wave(1);
    stInfoRqst.set_lv(1);
    stInfoRqst.set_skillid(iIdx);

    P_DAILYACT_CTRL->getProxy()->sendSubmitMonsterSkillRqst(stInfoRqst);
}

