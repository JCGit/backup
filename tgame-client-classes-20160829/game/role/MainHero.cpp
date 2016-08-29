#include "MainHero.h"
#include "FallingItem.h"
#include "UserObj.h"
#include "Macros.pb.h"
#include "CCColorUtil.h"
#include "control/AudioManager.h"
#include "instance/InstanceEventUtils.h"
#include "instance/InstanceUtils.h"
#include "vlib/CVRand.h"

#include "SkillCfgMaster.h"
#include "BuffCfgMaster.h"
#include "BulletCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "HeroCfgMaster.h"
#include "cocos2d.h"
#include "cocos-ext.h"

#include "CfgMaster/VIPLvCfgMaster.h"

USING_NS_CC;
USING_NS_CC_EXT;


const string CMainHero::TRY_KICK_ENIMY_EVENT = "try_kick_enimy_event";
const string CMainHero::HERO_HP_CHANGE_EVENT = "hero_hp_change_event";
const string CMainHero::HERO_EMERGE_COMPLETE_EVENT = "hero_emerge_complete_event";
const string CMainHero::HERO_CAST_SKILL_EVENT = "hero_cast_skill_event";

#define HERO_NAME_TAG 101

CMainHero::CMainHero()
	:m_uWeaponMovementID(0),
	m_pstLeftHand(NULL),
	m_pstRightHand(NULL),
	m_uHeroState(E_STATE_IDLE),
	m_dLastFireTime(0),
	m_pstGunFireAni(NULL),
	m_iGunfireSoundID(0),
	m_uShowID(0),
	m_dFireStateChgTime(0.0),
	m_pTitleImg(NULL)
{
	setRoleParty(ROLE_PARTY_FRIEND);
	m_fBaseScale = 1.0f;
}

void CMainHero::onEnter(){
	CQStageEntity::onEnter();
}

void CMainHero::onExit(){	
	removeAllChildrenWithCleanup(true);
// 	if(m_pstBody_stand){
// 		m_pstBody_stand->autorelease();
// 		m_pstBody_stand = NULL;
// 	}
// 
// 	if(m_pstBody_attack){
// 		m_pstBody_attack->autorelease();
// 		m_pstBody_attack = NULL;
// 	}
// 
// 	if(m_pstBody_skill){
// 		m_pstBody_skill->autorelease();
// 		m_pstBody_skill = NULL;
// 	}


	if (m_pstLeftHand){		
		m_pstLeftHand = NULL;
	}

// 	if (m_pstLeftHand_stand){
// 		m_pstLeftHand_stand->autorelease();
// 		m_pstLeftHand_stand = NULL;
// 	}
// 
// 	if (m_pstLeftHand_skill){
// 		m_pstLeftHand_skill->autorelease();
// 		m_pstLeftHand_skill = NULL;
// 	}
// 
// 	if (m_pstLeftHand_attack){
// 		m_pstLeftHand_attack->autorelease();
// 		m_pstLeftHand_attack = NULL;
// 	}



	if (m_pstRightHand){		
		m_pstRightHand = NULL;
	}

// 	if (m_pstRightHand_attack){
// 		m_pstRightHand_attack->autorelease();
// 		m_pstRightHand_attack = NULL;
// 	}
// 
// 	if (m_pstRightHand_stand){
// 		m_pstRightHand_stand->autorelease();
// 		m_pstRightHand_stand = NULL;
// 	}
// 
// 	if (m_pstRightHand_skill){
// 		m_pstRightHand_skill->autorelease();
// 		m_pstRightHand_skill = NULL;
// 	}

//	setArmature(NULL);
	CQStageEntity::onExit();

	if (m_pstGunFireAni){
		m_pstGunFireAni = NULL;
	}

	
}

void CMainHero::stateChangeByAttack(bool attack)
{
	unsigned int preState = m_uHeroState;
	if(m_uHeroState == E_STATE_IDLE)
	{
		if(attack == true)
		{
			m_uHeroState = E_STATE_SHOOTING;
// 			getArmature()->getAnimation()->setSpeedScale(40);
// 			m_pstLeftHand->getAnimation()->setSpeedScale(40);
// 			m_pstRightHand->getAnimation()->setSpeedScale(40);
			Attack();
		}else{
			Idle();
		}
	}else if(m_uHeroState == E_STATE_WALK){
		if(attack == true)
		{
			if (getArmature()->getAnimation()->getCurrentMovementID() == "run"
				|| getArmature()->getAnimation()->getCurrentMovementID() == "walk_attack"){
				m_uHeroState = E_STATE_WALK_SHOOTING;
				AttackWalkWithDirection(getVelocity());
			}else{
				//m_uHeroState = E_STATE_WALK_SHOOTING;
				AttackCounterMarchWithDirection(getVelocity());
			}
			
		}else{
			WalkWithDirection(getVelocity());
		}
	}else if(m_uHeroState == E_STATE_WALK_SHOOTING)	{
		if(attack == false)
		{
			WalkWithDirection(getVelocity());
			m_uHeroState = E_STATE_WALK;			
		}else{
			AttackWalkWithDirection(getVelocity());
		}
	}else if(m_uHeroState == E_STATE_SHOOTING){
		if (attack == false){
			m_uHeroState = E_STATE_IDLE;
			//CCLOG("2222");
			Idle();
		}else{
			Attack();
		}
	}else if(m_uHeroState == E_STATE_BACK_SHOOTING){
		if (attack == false){
			m_uHeroState = E_STATE_WALK;
            WalkWithDirection(getVelocity());
		}else{
			AttackCounterMarchWithDirection(getVelocity());
		}
		
	}
	if (m_bAttacking != attack){
		setFireStateChgTime(0);
	}
	m_bAttacking = attack;
	//CCLOG("hero state stateChangeByAttack pre state == %d, next state === %d, attack %d", preState, m_uHeroState, attack);
}

void CMainHero::stateChangeByMove(const CCPoint& stDir)
{	
	//CCLOG("hero stateChangeByMove begin %d, dir.x %f", m_uHeroState, stDir.x);
	//CCLOG("time %f hero state stateChangeByMove3 %d, action %s", m_dObjectLife,  m_uHeroState, getArmature()->getAnimation()->getCurrentMovementID().c_str());
	if(m_uHeroState == E_STATE_IDLE)
	{		
		WalkWithDirection(stDir);
		m_uHeroState = E_STATE_WALK;
	}else if (m_uHeroState == E_STATE_SHOOTING){
		//CCLOG("hero state stateChangeByMove1 %d, action %s", m_uHeroState, getArmature()->getAnimation()->getCurrentMovementID().c_str());
		if (stDir.x * getScaleX() >= 0){
			AttackWalkWithDirection(stDir);
			m_uHeroState = E_STATE_WALK_SHOOTING;
		}else{
			AttackCounterMarchWithDirection(stDir);
			m_uHeroState = E_STATE_BACK_SHOOTING;
		}
	}else if(m_uHeroState == E_STATE_WALK_SHOOTING){
		if (stDir.x * getScaleX() >= 0){
			AttackWalkWithDirection(stDir);
			m_uHeroState = E_STATE_WALK_SHOOTING;
		}else{
			AttackCounterMarchWithDirection(stDir);
			m_uHeroState = E_STATE_BACK_SHOOTING;
		}
	}else if (m_uHeroState == E_STATE_WALK){
		//CCLog("action %s, scalex %f, dir.x %f", getArmature()->getAnimation()->getCurrentMovementID().c_str(), getScaleX(), stDir.x);
		WalkWithDirection(stDir);
// 		if (getArmature()->getAnimation()->getCurrentMovementID() == "walk_attack"){
// 			if ((stDir.x >= 0 && getScaleX() < 0)
// 				|| (stDir.x < 0 && getScaleX() > 0)){
// 				//getArmature()->getAnimation()->play("back_attack");
// 				CCPoint stNDir = ccpNormalize(stDir);
// 				setVelocity(ccp(getWalkSpeed() * stNDir.x, getWalkSpeed() * stNDir.y));
// 			}
// 		}else if (getArmature()->getAnimation()->getCurrentMovementID() == "back_attack"){
// 			if ((stDir.x > 0 && getScaleX() > 0)
// 				|| (stDir.x < 0 && getScaleX() < 0)){
// 					//getArmature()->getAnimation()->play("walk_attack");
// 					CCPoint stNDir = ccpNormalize(stDir);
// 					setVelocity(ccp(getWalkSpeed() * stNDir.x, getWalkSpeed() * stNDir.y));
// 			}
// 		}
	}else if (m_uHeroState == E_STATE_BACK_SHOOTING){
		if (stDir.x * getScaleX() >= 0){
			AttackWalkWithDirection(stDir);
			m_uHeroState = E_STATE_WALK_SHOOTING;
		}else{
			AttackCounterMarchWithDirection(stDir);
			m_uHeroState = E_STATE_BACK_SHOOTING;
		}
	}else{
		//CCLOG("hero state stateChangeByMove %d, action %s", m_uHeroState, getArmature()->getAnimation()->getCurrentMovementID().c_str());
		WalkWithDirection(stDir);
		m_uHeroState = E_STATE_WALK;
	}

	//CCLOG("hero state stateChangeByMove %d, action %s", m_uHeroState, getArmature()->getAnimation()->getCurrentMovementID().c_str());
}

void CMainHero::stateChangeByReleaseHud(){
	
	if (m_uHeroState == E_STATE_BACK_SHOOTING 
		|| m_uHeroState == E_STATE_SHOOTING
		|| m_uHeroState == E_STATE_WALK_SHOOTING){
		//这里可能要处理一下， 看看射击动作是否就是站立动作
// 		getArmature()->getAnimation()->setSpeedScale(0.5);
// 		m_pstLeftHand->getAnimation()->setSpeedScale(0.5);
// 		m_pstRightHand->getAnimation()->setSpeedScale(0.5);
		Attack();
		m_uHeroState = E_STATE_SHOOTING;
	}else{
		//这里怎么处理双击的问题？暂时不管么。。。
		if (!IsActionInterruptable() || !IsWalkable()){
			return;
		}

		setDstDirection(_velocity);
		ForceIdle();
		m_uHeroState = E_STATE_IDLE;
	}
	//CCLOG("hero state %d", m_uHeroState);
}


void CMainHero::turnBackShooting(const CCPoint& stDir){
	if (m_uHeroState == E_STATE_WALK_SHOOTING){
		AttackCounterMarchWithDirection(stDir);
		m_uHeroState = E_STATE_BACK_SHOOTING;
	}else if (m_uHeroState != E_STATE_IDLE && m_uHeroState != E_STATE_WALK){
		AttackWalkWithDirection(stDir);
		m_uHeroState = E_STATE_WALK_SHOOTING;
	}else{
		this->setScaleX(-1 * this->getScaleX());
	}
}


void CMainHero::turnBackShooting(){
	this->setScaleX(-1 * this->getScaleX());
	if (getArmature()->getAnimation()->getCurrentMovementID() == "run"
		 || getArmature()->getAnimation()->getCurrentMovementID()== "walk_attack"){
		AttackCounterMarchWithDirection(getVelocity());
		m_uHeroState = E_STATE_BACK_SHOOTING;
	}else if (getArmature()->getAnimation()->getCurrentMovementID() == "back_attack"){
		AttackWalkWithDirection(getVelocity());
		m_uHeroState = E_STATE_WALK_SHOOTING;
	}else if (m_uHeroState == E_STATE_IDLE){
		Attack();
		m_uHeroState = E_STATE_SHOOTING;
		
	}
	//CCLOG("hero state %d, action %s", m_uHeroState, getArmature()->getAnimation()->getCurrentMovementID());
}

void CMainHero::CastSkill(int iSkillIdx){
	CQSkillCell* stSkill = (CQSkillCell*)_skillArray->objectAtIndex(iSkillIdx);
	if (!stSkill){
		return;
	}
	const SkillCfg* skillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(stSkill->m_uSkillID);
	unsigned int costEp = skillCfg->epcost();
	if(_ep < costEp){  //能量值不够
		CCLOG("ep is not enough!!!");
		return;
	}

	_ep -= costEp;
	AttackWithSkillAtIdx(iSkillIdx);

	CCString* strValue = CCString::createWithFormat("%d", costEp);
	DispatchEvent(HERO_EPVALUE_COST_EVENT, strValue);		//猪脚能量值变化
}

void CMainHero::CastSkillWithSkillID(int iSkillID){
	const SkillCfg* skillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(iSkillID);
	unsigned int costEp = skillCfg->epcost();
	if(_ep < costEp){  //能量值不够
		return;
	}

	_ep -= costEp;
	AttackWithSkill(iSkillID);

	CCString* strValue = CCString::createWithFormat("%d", costEp);
	DispatchEvent(HERO_EPVALUE_COST_EVENT, strValue);		//猪脚能量值变化
}

void CMainHero::HandleMovementEvent(CCArmature * armature, MovementEventType type, const char * name)  
{  
	static int atkCnt = 2;
	if (type == COMPLETE && 
		(strcmp(name, "hurt") == 0 || strcmp(name, "skill_attack")== 0 || strcmp(name, "skill_attack1") == 0))  
	{  
		if(m_uHeroState == E_STATE_IDLE)
		{
			if(m_bAttacking == true)
			{
				m_uHeroState = E_STATE_SHOOTING;
				Attack();
			}else{
				Idle();
			}
		}else if(m_uHeroState == E_STATE_WALK){
			if(m_bAttacking == true)
			{
				m_uHeroState = E_STATE_WALK_SHOOTING;
				AttackWalkWithDirection(getVelocity());
			}else{
				WalkWithDirection(getVelocity());
				m_uHeroState = E_STATE_WALK;
			}
		}else if(m_uHeroState == E_STATE_WALK_SHOOTING)	{
			if(m_bAttacking == false)
			{
				WalkWithDirection(getVelocity());
				m_uHeroState = E_STATE_WALK;
			}else{
				m_uHeroState = E_STATE_WALK_SHOOTING;
				AttackWalkWithDirection(getVelocity());
			}
		}else if(m_uHeroState == E_STATE_SHOOTING){
			if (m_bAttacking == false){
				m_uHeroState = E_STATE_IDLE;
				Idle();
			}else{
				m_uHeroState = E_STATE_SHOOTING;
				Attack();
			}
		}else if(m_uHeroState == E_STATE_BACK_SHOOTING){
			if (m_bAttacking == false){
				m_uHeroState = E_STATE_WALK;
				WalkWithDirection(getVelocity());
			}else{
				AttackCounterMarchWithDirection(getVelocity());
				m_uHeroState = E_STATE_BACK_SHOOTING;
			}
		}
	}  else if (type == COMPLETE &&	(strcmp(name, "hurt") == 0)){
		m_uHeroState = E_STATE_DIE;
	} else if (type == COMPLETE && strcmp(name, "attack") == 0){
		Attack();	
	}else if ((m_uHeroState == E_STATE_WALK_SHOOTING || m_uHeroState == E_STATE_BACK_SHOOTING) && type == LOOP_COMPLETE){
		
	}else if (m_uHeroState == E_STATE_IDLE && type == COMPLETE){
		//CCLog("should idle here, stage engity state %d", getMyState());
		Idle();
	}

	if(type == COMPLETE){
		string actStr = name;
		if(actStr.find("stand") != string::npos){
			if(getMyState() == kActionStateIdle || getMyState() == kActionStateDizzy){
				RandStand();
			}
		}
	}
	//CCLOG("hero state HandleMovementEvent %d, action old %s, action now %s, attack %d",
	//	m_uHeroState, 
	//	name,
	//	getArmature()->getAnimation()->getCurrentMovementID().c_str(),
	//	m_bAttacking);
	//CCLog("action %s, type %d, state is %d, stageeintity state %d", name, type, m_uHeroState, getMyState());
} 

void CMainHero::pickupItem(CCObject *pItem) 
{
	// play sound effect
	AudioManager::playEffect("sound/pick.mp3");
	
	// float text
	this->FloatText(dynamic_cast<FallingItem *>(pItem)->getName());
}

void CMainHero::WalkToDstDoneCallback() {
	CQStageEntity::WalkToDstDoneCallback();
	stateChangeByReleaseHud();
	DispatchEvent("walk_done", this);
}

unsigned int CMainHero::GetSkillNextActSec(int iIdx){
	CQSkillCell* pstSkill = dynamic_cast<CQSkillCell*>(_skillArray->objectAtIndex(iIdx));
	return pstSkill->m_stCfg.cd() + pstSkill->m_uLastCastTime;

}

void CMainHero::stateChangeByKeyTouch(const HudTouchEvent& stTouch){
	if (stTouch.touchType == E_TOUCH_TYPE_SINGLE){
		//单击的，啥都不管

	}else{
		//双击的
		CCPoint stDirection;
		switch(stTouch.touchKey){
			case E_TOUCH_CENTER:
				stDirection = ccp(0,0);
				break;
			case E_TOUCH_DOWN:
				stDirection = ccp(0, -1);
				break;
			case E_TOUCH_LEFT:
				stDirection = ccp(-1, 0);
				break;
			case E_TOUCH_RIGHT:
				stDirection = ccp(1, 0);
				break;
			case E_TOUCH_UP:
				stDirection = ccp(0, 1);
				break;
		}

		if (stTouch.touchKey == E_TOUCH_CENTER){
			Jump();
		}else{
			//check if it's ready to dash now, maybe in CD

			DashWithDirection(stDirection);
		}
	}
}

void CMainHero::ShowHurt(int fDamage){
    if(fDamage > 0) {
        CCString* strValue = CCString::createWithFormat("%d", fDamage);
        DispatchEvent(HERO_LOSEVALUE_CHANGE, strValue);		//猪脚受伤的数值上报，计入损失血量总值 
		DispatchEvent(FRIEND_HURT_EVENT, this);			
    }
	
}

void CMainHero::update(float dt){
	CQStageEntity::update(dt);
	//检查是否应该发射子弹
	setFireStateChgTime(getFireStateChgTime() + dt);
	CheckShooting(dt);

// 	//debug：打印当前正在播放的帧
// 	CCLog("body frame idx %d", mArmature->getAnimation()->getCurrentFrameIndex());
// 	CCLog("left hand frame idx %d", m_pstLeftHand->getAnimation()->getCurrentFrameIndex());
// 	CCLog("right hand frame idx %d", m_pstRightHand->getAnimation()->getCurrentFrameIndex());
// 	CCLOG("---------------------------------");
    if(!mArmature || !mArmature->getParent()) {
        CCLOG("[error]CMainHero::update, get animation fail.");
        return;
    }
    std::string stMoveID = mArmature->getAnimation()->getCurrentMovementID();

// 	if (!strcmp(stMoveID.c_str(), "run") 
// 		|| !strcmp(stMoveID.c_str(), "stand")
// 		|| !strcmp(stMoveID.c_str(), "attack")){
// 			int iFrameIdx = mArmature->getAnimation()->getCurrentFrameIndex();
// 			if (iFrameIdx !=  m_pstLeftHand->getAnimation()->getCurrentFrameIndex()){	
// 				/*CCLog("current movement %s, body movement %s", 
// 					m_pstLeftHand->getAnimation()->getCurrentMovementID().c_str(),
// 					mArmature->getAnimation()->getCurrentMovementID().c_str());
//                     */
// 				m_pstLeftHand->getAnimation()->gotoAndPlay(iFrameIdx);
// 			}
// 
// 			if (iFrameIdx !=  m_pstRightHand->getAnimation()->getCurrentFrameIndex()){
// 				m_pstRightHand->getAnimation()->gotoAndPlay(iFrameIdx);
// 			}
// 	}
	
	
	
}

void CMainHero::CheckShooting(double dt){
// 	if (m_pstGunFireAni && m_pstGunFireAni->isVisible() && isShooting() && !m_pstRightHand->getAnimation()->getIsComplete()){	
// 				
// 		string stMovementID = m_pstRightHand->getAnimation()->getCurrentMovementID();
// 		int iFrameIdx = m_pstRightHand->getAnimation()->getCurrentFrameIndex();
// 		if (iFrameIdx < m_pstRightHand->getAnimation()->getAnimationData()->getMovement(stMovementID.c_str())->duration){
// 			CCPoint stPos;
// 			if (m_pstRightHand->getAnimation()->getAnimationData()->getMovement(stMovementID.c_str())->getMovementBoneData(m_stFirePointStr.c_str())
// 				&& m_pstRightHand->getAnimation()->getAnimationData()->getMovement(stMovementID.c_str())->getMovementBoneData(m_stFirePointStr.c_str())->duration){
// 				iFrameIdx = iFrameIdx / 5; //m_pstRightHand->getAnimation()->getAnimationData()->getMovement(stMovementID.c_str())->getMovementBoneData(m_stFirePointStr.c_str())->duration;
// 				iFrameIdx = iFrameIdx % m_pstRightHand->getAnimation()->getAnimationData()->getMovement(stMovementID.c_str())->getMovementBoneData(m_stFirePointStr.c_str())->frameList.count();
// 				const CCFrameData* pstFrame = m_pstRightHand->getAnimation()->getAnimationData()->getMovement(stMovementID.c_str())->getMovementBoneData(m_stFirePointStr.c_str())->getFrameData(iFrameIdx);
// 				stPos = ccp(pstFrame->x + m_stFirePointPos.x, pstFrame->y + m_stFirePointPos.y);
// 			}else{
// 				stPos = m_stFirePointPos;
// 			}
// 			
// 			//CCLog("gun fire ani pos %f, %f, frame idx %i", stPos.x, stPos.y, iFrameIdx);
// 			m_pstGunFireAni->setPosition(stPos);
// 		}		
// 	}
	
	double dShootInt = GetShootInterval();
	if ((m_dObjectLife - m_dLastFireTime > dShootInt)		
		&& (getArmature()->getAnimation()->getCurrentMovementID() == "attack"
			|| getArmature()->getAnimation()->getCurrentMovementID() == "walk_attack"
			|| getArmature()->getAnimation()->getCurrentMovementID() == "back_attack")){
				//CCLog("current %s", getArmature()->getAnimation()->getCurrentMovementID().c_str());
// 		int iCnt = int ((m_dObjectLife - m_dLastFireTime) / dShootInt);
// 		while (iCnt > 0){
// 			DispatchEvents(FIRE_EVENT, this);
// 			iCnt --;
// 		}
		DispatchEvents(FIRE_EVENT, this);
		m_dLastFireTime = m_dObjectLife;
	}
}

bool CMainHero::IsShootable() const{
	if (!IsAttackable()){
		return false;
	}

	return ((m_dObjectLife - m_dLastFireTime) >= GetShootInterval() 
		|| m_dLastFireTime == 0
		|| isShooting()) ;
}

unsigned int CMainHero::GetArmorType() const{
	return _armorType;
}

unsigned int CMainHero::GetAtkMagicType()const{
	return _attackMagicType;
}

unsigned int CMainHero::GetAtkType() const{
	return _attackType;
}

void CMainHero::HPChange(){
	//猪脚血条变化
	DispatchEvents(HERO_HP_CHANGE_EVENT, this);
}

void CMainHero::HandleEntityDeath(){
	DispatchEvent(HERO_KILLED_EVENT, this);
}

void CMainHero::OtherWalk(){
	char acName[32];
	sprintf(&acName[0], "run%02d", m_uWeaponMovementID);

	//CCLog("run run run");
	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0],  -1, -1, 1);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0],  -1, -1, 1);
	}
	/*CCLog("play run ....");*/

	ShowHands();
	HideGunFire();
	m_uHeroState = E_STATE_WALK;

	walkActWithSpeed(_walkSpeed);
}

void CMainHero::OtherAttackWalk(){
	;
	char acName[32];
	sprintf(&acName[0], "walk_attack%02d", m_uWeaponMovementID);

	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0], -1, -1, 1);
	}

	
	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0],  -1, -1, 1);
	}

	ShowHands();
	ShowGunFire();
	CCLog("OtherAttackWalk %d", m_uHeroState);
	m_uHeroState = E_STATE_WALK_SHOOTING;

	
}

void CMainHero::OtherDash(){
	char acName[32];
	sprintf(&acName[0], "dash%02d", m_uWeaponMovementID);

	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0]);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0]);
	}	
	ShowHands();
	HideGunFire();
}


void CMainHero::OtherAttackDash(){
	char acName[32];
	sprintf(&acName[0], "attackdash%02d", m_uWeaponMovementID);

	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0]);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0]);
	}
	ShowHands();
	ShowGunFire();
}

void CMainHero::OtherAttackCounterMarch(){
	char acName[32];
	sprintf(&acName[0], "back_attack%02d", m_uWeaponMovementID);

	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0],  -1, -1, 1);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0],  -1, -1, 1);
	}	
	ShowHands();
	ShowGunFire();
	m_uHeroState = E_STATE_BACK_SHOOTING;
}


void CMainHero::OtherAttack(){
	char acName[32];
	sprintf(&acName[0], "attack%02d", m_uWeaponMovementID);

	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0], -1, -1, 1);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0], -1, -1, 1);
	}

	


	ShowGunFire();
	m_uHeroState = E_STATE_SHOOTING;
	ShowHands();
}
void CMainHero::OtherIdle(){
	char acName[32];
	sprintf(&acName[0], "stand%02d", m_uWeaponMovementID);

	//CCLOG("other idle action name is %s", acName);

	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0],  -1, -1, 1);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0],  -1, -1, 1);
	}
	
	HideGunFire();
	ShowHands();
	m_uHeroState = E_STATE_IDLE;
}

void CMainHero::ShowGunFire(){
	const WeaponCfg* pstCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(_weaponCfgID);
	if (!pstCfg){
		CCLOGERROR("no cfg is found for weapon %u", _weaponCfgID);
		return;
	}

	if (!m_pstGunFireAni){

		m_uWeaponMovementID = pstCfg->weapondisp();
		m_uWeaponGuesture = pstCfg->guesture();

		unsigned int uiBullteID = pstCfg->bulletid();
		const BulletCfg* pstBulletCfg = BULLET_CFG_MASTER->GetBulletCfgByID(uiBullteID);
		if (!pstBulletCfg){
			CCLOGERROR("no bullet cfg is found for bullet %u", uiBullteID);
			return;
		}

		if (pstBulletCfg->shootanimation().length() && strcmp(pstBulletCfg->shootanimation().c_str(), "null")){
			if (m_pstGunFireAni){
				m_pstGunFireAni->removeFromParentAndCleanup(true);
				/*m_pstRightHand->autorelease();*/
				m_pstGunFireAni = NULL;
			}

			m_pstGunFireAni = CCArmature::create(pstBulletCfg->shootanimation().c_str());
			getArmature()->addChild(m_pstGunFireAni, 1000);
			//addChild(m_pstGunFireAni, RIGHT_HAND_ZORDER + 1);
			m_pstGunFireAni->setPosition(GetGunRelativePos());
			m_pstGunFireAni->getAnimation()->play("fly", -1, -1, 1);
		}

		if (m_iGunfireSoundID == 0 && pstCfg->firesoundtype() == FIRE_SOUND_CYCLING_TYPE){
			m_iGunfireSoundID = AudioManager::playEffect(pstCfg->firesound().c_str(), true);
		}

	}else{
		if(!m_pstGunFireAni->getAnimation()->getIsPlaying()){
			m_pstGunFireAni->getAnimation()->play("fly", -1, -1, 1);
		}
		m_pstGunFireAni->setVisible(true);
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeEffect(m_iGunfireSoundID);
		AudioManager::resumeEffect(m_iGunfireSoundID);
		m_pstGunFireAni->setPosition(GetGunRelativePos());
	}

	if (m_iGunfireSoundID == 0 && pstCfg->firesoundtype() == FIRE_SOUND_CYCLING_TYPE){
		m_iGunfireSoundID = AudioManager::playEffect(pstCfg->firesound().c_str(), true);
	}
}

void CMainHero::HideGunFire(){
	if (m_pstGunFireAni && m_pstGunFireAni->isVisible()){
		m_pstGunFireAni->setVisible(false);
		m_pstGunFireAni->getAnimation()->pause();
	}

	if (m_iGunfireSoundID){
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(m_iGunfireSoundID);
		AudioManager::stopEffect(m_iGunfireSoundID);
		m_iGunfireSoundID = 0;
	}
}

void CMainHero::OtherDie(){
	if (m_pstLeftHand){
		m_pstLeftHand->setVisible(false);
	}

	if (m_pstRightHand){
		m_pstRightHand->setVisible(false);
	}
	HideGunFire();
	return;

	char acName[32];
	sprintf(&acName[0], "die%02d", m_uWeaponMovementID);
	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0],  -1, -1, 0);
	}else{
		m_pstLeftHand->setVisible(false);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0],  -1, -1, 0);
	}else{
		m_pstRightHand->setVisible(false);
	}


}
void CMainHero::OtherHurt(){
	char acName[32];
	sprintf(&acName[0], "hurt%02d", m_uWeaponMovementID);

	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0],  -1, -1, 0);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0],  -1, -1, 0);
	}	
	HideGunFire();
	ShowHands();
	//m_uHeroState = E_STATE_NULL;
}

void CMainHero::OtherCounterMarch(){
	char acName[32];
	sprintf(&acName[0], "backrun%02d", m_uWeaponMovementID);
	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0]);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0]);
	}	
	ShowHands();
	HideGunFire();
}

void CMainHero::OtherAttackWithSkill(const CQSkillCell* stSkill){
	char acName[32];
	
	if (strcmp(stSkill->m_stCfg.castaction().c_str(), "null")){
			//大招，且有大招动作，就放大招动作		
			//getArmature()->getAnimation()->play(stSkill->m_stCfg.castaction().c_str(), -1, -1, 0);
		sprintf(&acName[0], "%s_%02d",stSkill->m_stCfg.castaction().c_str(), m_uWeaponMovementID);
		if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
			m_pstLeftHand->getAnimation()->play(&acName[0], -1, -1, 0);
		}

		if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
			m_pstRightHand->getAnimation()->play(&acName[0], -1, -1, 0);
		}
			
	}else{
		sprintf(&acName[0], "attack%d", m_uWeaponMovementID);
		if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
			m_pstLeftHand->getAnimation()->play(&acName[0], -1, -1, 0);
		}

		if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
			m_pstRightHand->getAnimation()->play(&acName[0], -1, -1, 0);
		}
	}

	ShowHands();
	HideGunFire();

	CCLOG("the MainHero is playing skill === %u", stSkill->m_stCfg.skillid());
}

void CMainHero::OtherDizzy(){
	OtherIdle();
}

void CMainHero::KnockDown(){
	CQStageEntity::KnockDown();
	OtherDie();
}


void CMainHero::OtherBackDash(){
	char acName[32];
	sprintf(&acName[0], "back_dash%02d", m_uWeaponMovementID);
	if (m_pstLeftHand && m_pstLeftHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstLeftHand->getAnimation()->play(&acName[0], -1, -1, 1);
	}

	if (m_pstRightHand && m_pstRightHand->getAnimation()->getAnimationData()->getMovement(&acName[0])){
		m_pstRightHand->getAnimation()->play(&acName[0], -1, -1, 1);
	}
	HideGunFire();
	ShowHands();
}

CCPoint CMainHero::GetFirePosition(){
	//CCBone* tmpBone = m_pstRightHand->getBone("firepoint");
// 	char acBuf[32];
// 	sprintf(&acBuf[0], "firepoint%02d", m_uWeaponMovementID);
 
// 	CCBoneData* pstBoneData;
// 	if (m_pstRightHand->getBone(&acBuf[0])){
// 		pstBoneData = m_pstRightHand->getBone(&acBuf[0])->getBoneData();		
// 	}else{
// 		pstBoneData = m_pstRightHand->getBone("firepoint")->getBoneData();		
// 	}
// 
// 	


// 	
	float x, y;
	if (m_pstGunFireAni){
		x = m_pstGunFireAni->getPositionX() * getArmature()->getScaleX() + getPositionX();
		y = m_pstGunFireAni->getPositionY() * getArmature()->getScaleY() + getPositionY();
	}else{
		x = m_stFirePointPos.x * getArmature()->getScaleX() + getPositionX();
		y = m_stFirePointPos.y * getArmature()->getScaleY() + getPositionY();
	}
	return	ccp(x, y)*m_fBaseScale;

// 	float x, y;
//  	x = m_stFirePointPos.x * getScaleX() + getPositionX();
//   	y = m_stFirePointPos.y * getScaleY() + getPositionY();
//  	return	ccp(x, y);

}

CCPoint CMainHero::GetGunRelativePos(){
	float x, y;
	string stCurAction = getArmature()->getAnimation()->getCurrentMovementID();
	CCMovementBoneData* pstBone = getArmature()->getAnimation()->getAnimationData()->getMovement(stCurAction.c_str())->getMovementBoneData("firepoint");
	if (pstBone){
		x = (m_stFirePointPos.x  + pstBone->getFrameData(0)->x) ;
		y = (m_stFirePointPos.y + pstBone->getFrameData(0)->y);
		
	}else{
		x = m_stFirePointPos.x * getArmature()->getScaleX();
		y = m_stFirePointPos.y * getArmature()->getScaleY();
	}
	
	return	ccp(x, y)*m_fBaseScale;
}



CCPoint CMainHero::GetSkillGunPoint(const string& stSkillAction){
	char acBuf[32];
	char acDefultGunpoint[32];
	float x, y;

	int iSkillActionIdx = 0;
	if (stSkillAction.find("1") != string::npos){
		iSkillActionIdx = 1;
	}else if (stSkillAction.find("2") != string::npos){
		iSkillActionIdx = 2;
	}else if (stSkillAction.find("3") != string::npos){
		iSkillActionIdx = 3;

	}

	string stDefaultGunpoint;
	if (iSkillActionIdx != 0){
		sprintf(&acBuf[0], "skill%d_gunpoint%02d",iSkillActionIdx, m_uWeaponMovementID);
		sprintf(&acDefultGunpoint[0], "skill%d_gunpoint",iSkillActionIdx);
	}else{
		sprintf(&acBuf[0], "skill_gunpoint%02d", m_uWeaponMovementID);
		sprintf(&acDefultGunpoint[0], "skill_gunpoint");
	}
	
	string stBoneName;
	CCBoneData* pstBoneData;
	if (m_pstRightHand && m_pstRightHand->getBone(&acBuf[0])){
		pstBoneData = m_pstRightHand->getBone(&acBuf[0])->getBoneData();
		stBoneName = acBuf;
	}else if (m_pstRightHand && m_pstRightHand->getBone(&acDefultGunpoint[0])){
		pstBoneData = m_pstRightHand->getBone(&acDefultGunpoint[0])->getBoneData();
		stBoneName = acDefultGunpoint;
	}else if (m_pstRightHand){
		pstBoneData = m_pstRightHand->getBone("skill_gunpoint")->getBoneData();
		stBoneName = "skill_gunpoint";
	}else{
		pstBoneData = getArmature()->getBone("skill_gunpoint")->getBoneData();
		stBoneName = "skill_gunpoint";
	}

	string stCurAction = getArmature()->getAnimation()->getCurrentMovementID();
    CCMovementData* pstMovData = getArmature()->getAnimation()->getAnimationData()->getMovement(stCurAction.c_str());
	CCMovementBoneData* stBone = NULL;
    if(pstMovData) {
        stBone = pstMovData->getMovementBoneData(stBoneName.c_str());
    }
	if(stBone ){
		x = (pstBoneData->x + stBone->getFrameData(0)->x) * getScaleX();// + getPositionX();
		y = (pstBoneData->y + stBone->getFrameData(0)->y) * getScaleY();// + getPositionY();
	}else{
		x = (pstBoneData->x) * getScaleX();// + getPositionX();
		y = (pstBoneData->y) * getScaleY();// + getPositionY();
	}
	
	//CCLOG("the skill gun point is x === %f, y === %f", x, y);
	return	ccp(x, y)*m_fBaseScale;
}

void CMainHero::LoadOtherRes(){
// 	if (m_stBodyRes.length() && strcmp(m_stBodyRes.c_str(), "null")){
// 		if (getArmature()){
// 			getArmature()->removeFromParentAndCleanup(true);
// 			getArmature()->autorelease();
// 		}
// 
// 		if (m_pstBody_attack){
// 			m_pstBody_attack->removeFromParentAndCleanup(true);
// 			m_pstBody_attack->autorelease();
// 		}
// 		string stPath = m_stBodyRes;
// 		m_pstBody_attack = CCArmature::create(stPath.append("_attack1").c_str());
// 		m_pstBody_attack->retain();
// 
// 		if (m_pstBody_stand){
// 			m_pstBody_stand->removeFromParentAndCleanup(true);
// 			m_pstBody_stand->autorelease();
// 		}
// 
// 		stPath = m_stBodyRes;
// 		m_pstBody_stand = CCArmature::create(stPath.append("_s_h1").c_str());
// 		m_pstBody_stand->retain();
// 		setArmature(m_pstBody_stand);
// 
// 
// 		if (m_pstBody_skill){
// 			m_pstBody_skill->removeFromParentAndCleanup(true);
// 			m_pstBody_skill->autorelease();
// 		}
// 
// 		stPath = m_stBodyRes;
// 		m_pstBody_skill = CCArmature::create(stPath.append("_skill1").c_str());
// 		m_pstBody_skill->retain();
// 	}
	ReloadWeaponRes();
	
	
}

void CMainHero::ReloadWeaponRes(){
	
	//m_stLeftHandRes.assign("");
	//m_stRightHandRes.assign("");
	//if (m_stLeftHandRes.length() && strcmp(m_stLeftHandRes.c_str(), "null")){
	//	if (m_pstLeftHand){
	//		m_pstLeftHand->removeFromParentAndCleanup(true);
	//	}
	//	
	//	m_pstLeftHand = CCArmature::create(m_stLeftHandRes.c_str());
	//	m_pstLeftHand->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CQStageEntity::HandleFrameEvent));
	//	addChild(m_pstLeftHand, LEFT_HAND_ZORDER);
	//}

	//if (m_stRightHandRes.length() && strcmp(m_stRightHandRes.c_str(), "null")){
	//	if (m_pstRightHand){
	//		m_pstRightHand->removeFromParentAndCleanup(true);
	//	}
	//
	//	m_pstRightHand = CCArmature::create(m_stRightHandRes.c_str());
	//	m_pstRightHand->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CQStageEntity::HandleFrameEvent));
	//	addChild(m_pstRightHand, RIGHT_HAND_ZORDER);
	//}

    //m_pstLeftHand->setScale(0.8); //TODO:NEILWU
    //m_pstRightHand->setScale(0.8); //TODO:NEILWU


	
	

	const WeaponCfg* pstCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(this->getWeaponCfgID());
	if (!pstCfg){
		CCLOGERROR("[warn]CMainHero::ReloadWeaponRes,no cfg is found for weapon %u", this->getWeaponCfgID());
		return;
	}

	m_uWeaponMovementID = pstCfg->weapondisp();
	m_uWeaponGuesture = pstCfg->guesture();
	unsigned int uiBullteID = pstCfg->bulletid();
	const BulletCfg* pstBulletCfg = BULLET_CFG_MASTER->GetBulletCfgByID(uiBullteID);
	if (!pstBulletCfg){
		CCLOGERROR("[warn]CMainHero::ReloadWeaponRes,no bullet cfg is found for bullet %u", uiBullteID);
		return;
	}

	if (pstBulletCfg->shootanimation().length() && strcmp(pstBulletCfg->shootanimation().c_str(), "null")){
		if (m_pstGunFireAni){
			m_pstGunFireAni->removeFromParentAndCleanup(true);
			/*m_pstRightHand->autorelease();*/
			m_pstGunFireAni = NULL;
		}

		
	}

	char acBuf[32];
	snprintf(acBuf, 32, "firepoint%02d", m_uWeaponMovementID);

	CCBoneData* pstBoneData = NULL;
	if (m_pstRightHand && m_pstRightHand->getBone(&acBuf[0])){
		pstBoneData = m_pstRightHand->getBone(&acBuf[0])->getBoneData();		
	}else if (m_pstRightHand){
		snprintf(acBuf, 32, "firepoint");
		pstBoneData = m_pstRightHand->getBone("firepoint")->getBoneData();		
	}else{
        if(getArmature()) {
            pstBoneData = getArmature()->getBone("firepoint")->getBoneData();
        } else {
            CCLOG("[error]CMainHero::ReloadWeaponRes, fail to get my armature.");
        }
			
	}
    if(NULL == pstBoneData) {
        CCLOG("[error]CMainHero::ReloadWeaponRes, bone data is null!");
        return;
    }
	m_stFirePointStr.assign(&acBuf[0]);
	m_stFirePointPos = ccp(pstBoneData->x, pstBoneData->y);



	CCDictElement* pstPair;
	CCDictionary* plistDic = mArmature->getBoneDic();
	CCDICT_FOREACH(plistDic, pstPair){
		if (!pstPair){
			break;
		}
		string stCurBoneName(pstPair->getStrKey());

		size_t szPos = stCurBoneName.find("qiang");
		if (szPos != std::string::npos){
			//这一层是枪，要替换

			CCBone* pstCurBone = dynamic_cast<CCBone*>(pstPair->getObject());
			if (!pstCurBone){
				continue;
			}

			//pstCurBone->removeDisplay(0);
			CCSkin* pstSkin;
			
			pstSkin = CCSkin::createWithSpriteFrameName(pstCfg->weaponpic().c_str());
			pstCurBone->addDisplay(pstSkin, 0);
			pstCurBone->changeDisplayWithIndex(0, true);
			
		}

// 		if (stCurBoneName.find("Rxiaobi") != std::string::npos){
// 			//这一层是枪，要替换
// 
// 			CCBone* pstCurBone = dynamic_cast<CCBone*>(pstPair->getObject());
// 			if (!pstCurBone){
// 				continue;
// 			}
// 
// 			//pstCurBone->removeDisplay(0);
// 			CCSkin* pstSkin = CCSkin::createWithSpriteFrameName(pstCfg->weaponpic().c_str());
// 			pstCurBone->addDisplay(pstSkin, 0);
// 			pstCurBone->changeDisplayWithIndex(0, true);
// 
// 		}

	}



}

void CMainHero::SwitchRes(const char* pstActionName){	
// 	if (m_pstLeftHand && m_pstLeftHand->getParent()){
// 		m_pstLeftHand->getAnimation()->stop();
// 		removeChild(m_pstLeftHand);
// 
// 	}
// 
// 	if (m_pstRightHand && m_pstRightHand->getParent()){
// 		m_pstRightHand->getAnimation()->stop();
// 		removeChild(m_pstRightHand);
// 
// 	}
// 
// 	if (getArmature() && getArmature()->getParent()){
// 		getArmature()->getAnimation()->stop();
// 		removeChild(getArmature());
// 
// 	}
// 
// 	if (strcmp(pstActionName, "stand") == 0 
// 		|| strcmp(pstActionName, "hurt") == 0
// 		|| strcmp(pstActionName, "win") == 0
// 		|| strcmp(pstActionName, "die") == 0){
// 		setArmature(m_pstBody_stand);
// 		m_pstLeftHand = m_pstLeftHand_stand;
// 		m_pstRightHand = m_pstRightHand_stand;		
// 	}else if (strcmp(pstActionName, "backattack") == 0
// 		|| strcmp(pstActionName, "attack") == 0
// 		|| strcmp(pstActionName, "run") == 0
// 		|| strcmp(pstActionName, "attackrun")){		
// 		setArmature(m_pstBody_attack);
// 		m_pstLeftHand = m_pstLeftHand_attack;
// 		m_pstRightHand = m_pstRightHand_attack;	
// 	}else if (strcmp(pstActionName, "defend") == 0 
// 		|| strcmp(pstActionName, "dash") == 0
// 		|| strcmp(pstActionName, "backdash") == 0){
// 			setArmature(m_pstBody_skill);
// 			m_pstLeftHand = m_pstLeftHand_skill;
// 			m_pstRightHand = m_pstRightHand_skill;	
// 	}
// 	addChild(m_pstLeftHand, -8);
// 	addChild(m_pstRightHand, 1);
// 	addChild(getArmature(), 0);
}

void CMainHero::PauseMovement(){
	setIsPlayingMovement(false);
}

void CMainHero::ResumeMovement(){
	setIsPlayingMovement(true);
}


int CMainHero::SwitchWeapon(unsigned int uiWeaponID, bool isNeedupdateData){
	
	//数据改变,战斗中使用
	if(isNeedupdateData){
		ChangeWeapon();
	}

	unsigned int uShowID = getShowID();

	//const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	string stBodyName;
	int iRet = GetResourceNames(uiWeaponID, uShowID, stBodyName);
	if (iRet){
		return iRet;
	}

	const HeroCfg* heroCfg = HERO_CFG_MASTER->GetHeroCfgByID(uShowID);
	//切换枪械音效
	if (heroCfg->switchweaponvoice().length() && strcmp(heroCfg->switchweaponvoice().c_str(), "null")){
		AudioManager::playEffect(heroCfg->switchweaponvoice().c_str());
	}

	if (m_pstGunFireAni){
		m_pstGunFireAni->removeFromParentAndCleanup(true);
		m_pstGunFireAni = NULL;
	}

	mArmature->removeFromParentAndCleanup(true);
	//mArmature->release();	
	mArmature = CCArmature::create(stBodyName.c_str());
	addChild(mArmature);
	mArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CQStageEntity::HandleFrameEvent));
	mArmature->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CQStageEntity::MovementCallback));

	setResPath(stBodyName);

	const WeaponCfg* pstCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(uiWeaponID);
	if (!pstCfg){
		CCLOGERROR("no cfg is found for weapon %u", uiWeaponID);
		return -1;
	}
	m_uWeaponGuesture = pstCfg->guesture();
	m_uWeaponMovementID = pstCfg->weapondisp();
	setMyState(kActionStateNone);

	// 	m_pstRightHand->getAnimation()->setSpeedScale(0.2);
	// 	m_pstLeftHand->getAnimation()->setSpeedScale(0.2);
	// 	mArmature->getAnimation()->setSpeedScale(0.2);
	

	char acBuf[32];
	sprintf(&acBuf[0], "firepoint%02d", m_uWeaponMovementID);

//  换枪的逻辑要变化，同一中持枪姿势的，只用换图片
// 	if (m_pstRightHand){
// 		m_pstRightHand->removeFromParentAndCleanup(true);
// 		//mArmature->release();
// 		m_pstRightHand = CCArmature::create(stRightHandName.c_str());
// 		addChild(m_pstRightHand, RIGHT_HAND_ZORDER);
// 		m_pstRightHand->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CQStageEntity::HandleFrameEvent));
// 		m_stRightHandRes.assign(stRightHandName);
// 	}
// 
// 	if (m_pstLeftHand){
// 		m_pstLeftHand->removeFromParentAndCleanup(true);
// 		//mArmature->release();
// 		m_pstLeftHand = CCArmature::create(stLeftHandName.c_str());
// 		addChild(m_pstLeftHand, LEFT_HAND_ZORDER);
// 
// 		m_pstLeftHand->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CQStageEntity::HandleFrameEvent));
// 		m_stLeftHandRes.assign(stLeftHandName);
// 	}
	

	//切换当前枪支的图片

	CCDictElement* pstPair;
	CCDictionary* plistDic = mArmature->getBoneDic();
	CCDICT_FOREACH(plistDic, pstPair){
		if (!pstPair){
			break;
		}
		string stCurBoneName(pstPair->getStrKey());
		
		size_t szPos = stCurBoneName.find("qiang");
		if (szPos != std::string::npos){
			//这一层是枪，要替换

			CCBone* pstCurBone = dynamic_cast<CCBone*>(pstPair->getObject());
			if (!pstCurBone){
				continue;
			}

			//pstCurBone->removeDisplay(0);
			CCSkin* pstSkin = CCSkin::createWithSpriteFrameName(pstCfg->weaponpic().c_str());
			pstCurBone->addDisplay(pstSkin, 0);
			pstCurBone->changeDisplayWithIndex(0, false);			
		}

	}
	

	

	CCBoneData* pstBoneData;
	if (m_pstRightHand){
		if (m_pstRightHand->getBone(&acBuf[0])){
			pstBoneData = m_pstRightHand->getBone(&acBuf[0])->getBoneData();		
		}else{
			sprintf(&acBuf[0], "firepoint");
			pstBoneData = m_pstRightHand->getBone("firepoint")->getBoneData();		
		}
	}else{
		pstBoneData = getArmature()->getBone("firepoint")->getBoneData();	
	}
	
	
	m_stFirePointStr.assign(&acBuf[0]);
	m_stFirePointPos = ccp(pstBoneData->x, pstBoneData->y);


	//切换枪口特效

	

	_weaponCfgID = uiWeaponID;

	//把子弹的伤害宽度改一下。
	unsigned int uiBulletID = pstCfg->bulletid();
	const BulletCfg* pstBulletCfg = BULLET_CFG_MASTER->GetBulletCfgByID(uiBulletID);
	m_iBulletDamageRangeHight = 50;
	 
	if (pstCfg->shotrange() == 0){
		m_iBulletDamageRangeWidth = 1000;
	}else{
		m_iBulletDamageRangeWidth = pstCfg->shotrange();
	}
	

	ForceIdle();   //后退闪避过程中，换枪会出现2把枪
	return 0;

}

int CMainHero::GetResourceNames(unsigned int uiWeaponID, unsigned int uShowID, string& stBodyName){
	const WeaponCfg* pstCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(uiWeaponID);
	if (!pstCfg){
		CCLOGERROR("no cfg is found for weapon %u", uiWeaponID);
		return -1;
	}
	//找到这个配置了，把武器匹配上去
	
	const string* pHeroStr = HERO_CFG_MASTER->GetResName(uShowID);
	if(NULL == pHeroStr) {
		CCLOG("[error]CBaseFightScene::initMainHero, get hero res name fail! cfg id %d", uShowID );
		return -1;
	}

	char acMajorGroup[16];
	sprintf(&acMajorGroup[0], "_%02d", pstCfg->weaponid());
	string stGuesture;
	switch (pstCfg->guesture())
	{
	case GUESTURE_PISTOLS:
		stGuesture.assign("sq");
		break;
	case GUESTURE_RIFLES:
		stGuesture.assign("cq");
		break;
	case GUESTURE_RPG:
		stGuesture.assign("hj");
		break;
	case GUESTURE_MACHINE_GUN:
		stGuesture.assign("jtl");
		break;
	default:
		break;
	}

	stBodyName.assign(*pHeroStr).append("_body_");
	stBodyName.append(stGuesture);

	return 0;
}

//int CMainHero::GetResourceNames(unsigned int uiWeaponID, string& stBodyName, string& stLeftHandName, string& stRightHandName){
//	
//	//找到这个配置了，把武器匹配上去
//	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
//	
//	return GetResourceNames(uiWeaponID, pUserInfo->showID, stBodyName, stLeftHandName, stRightHandName);
//
//}

void CMainHero::ChangeWeapon(){
	//移除buff的加成
	for (int iIdx = 0; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
		if (m_stBuffs[iIdx] == NULL){
			break; 
		}		
		const BuffCfg* pstConfig = BUFF_CFG_MASTER->GetBuffCfgByID(m_stBuffs[iIdx]->m_uiBuffID);

		for (int k = 0; k < BUFF_MAX_EFFECTS_IN_ONE; k++){
			if (m_stBuffs[iIdx]->m_uiEffType[k] != 0){
				CInstanceUtils::RemoveEffect(this, m_stBuffs[iIdx]->m_uiEffType[k], m_stBuffs[iIdx]->m_uiVal[k], 
					m_stBuffs[iIdx]->m_uiMagicType[k], m_stBuffs[iIdx]->m_uiAddOrDec == BUFF_ADD_EFF);
			}				
		}
	}

	_obj->setInfoByUserInfo(P_USER_OBJ->getUserInfo());

	//只改变相关的属性
	_walkSpeed = _obj->walkSpeed;
	_backSpeed = _obj->backSpeed;
	m_uEffAtk = _obj->atk;
	_toatlAtkSpeed = _obj->totalAtkSpeed;
	_hit = _obj->hit;
	_attackType = _obj->atkType;
	_attackMagicType = _obj->atkAttr.attrtype();
	SetMagicAtkRatio(_obj->stMagicAtkRatio);

	if (_obj->weaponCfgID){
		m_pstWeaponCfg = WEAPON_CFG_MASTER->GetMutableWeaponCfgByID(_obj->weaponCfgID);

		//枪械上的技能
		CQSkillCell* pstSkill = dynamic_cast<CQSkillCell*>(getSkillArray()->objectAtIndex(0));
		pstSkill->m_uSkillID = WEAPON_CFG_MASTER->GetWeaponCfgByID(_obj->weaponCfgID)->weaponsuperskill();	
		pstSkill->m_uLastCastTime = 0;
		if (!m_pstWeaponCfg){
			assert(m_pstWeaponCfg != NULL);
		}
		
		const BulletCfg* pstBulletConfig = BULLET_CFG_MASTER->GetBulletCfgByID(m_pstWeaponCfg->bulletid());
		m_uiBullteTrailType = pstBulletConfig->bullettype();
		m_uiBulletBurstCnt =  pstBulletConfig->burstcnt();
		m_fBulletTime = m_pstWeaponCfg->bulletval()*1.0f/1000.0f;
		m_uiBulletCurCnt = 0;
		InitSkills();
	}

	//重新计算buff
	for (int iIdx = 0; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
		if (m_stBuffs[iIdx] == NULL){
			break; 
		}		
		const BuffCfg* pstConfig = BUFF_CFG_MASTER->GetBuffCfgByID(m_stBuffs[iIdx]->m_uiBuffID);
		for (int j = 0; j < pstConfig->effects_size(); j++){
			//添加效果
			CInstanceUtils::AddEffect(this, pstConfig->effects(j).effecttype(), pstConfig->effects(j).value(), pstConfig->effects(j).magictype(), pstConfig->addordec() == BUFF_ADD_EFF);
			m_stBuffs[iIdx]->m_uiVal[j] = pstConfig->effects(j).value();
			m_stBuffs[iIdx]->m_uiEffType[j] = pstConfig->effects(j).effecttype();
			m_stBuffs[iIdx]->m_uiMagicType[j] = pstConfig->effects(j).magictype();
		}
	}
}

void CMainHero::showName(string nick, bool isSelf){
	if(mArmature == NULL){
		return;
	}

	if(_blood != NULL){
		_blood->setVisible(false);
	}

	//CCLabelTTF* m_pNameTxt = CCLabelTTF::create(nick.c_str(), "fonts/DroidSansFallback.ttf", 28);
    UILabel* m_pNameTxt = UILabel::create();
	m_pNameTxt->setScale(1/m_fBaseScale);
    m_pNameTxt->setFontName("fonts/tgamefont.ttf");
    m_pNameTxt->setFontSize(28);

    //m_pNameTxt->setColor(ccc3(0xFE, 0xF8, 0x97));
	if(isSelf){
		m_pNameTxt->setColor(ccc3(0xFF, 0xFF, 0x00));
	}else{
		m_pNameTxt->setColor(ccc3(0xFF, 0xFF, 0xFF));
	}

	m_pNameTxt->setPosition(ccp(0, getContentSize().height + 10));
    //CCColorUtil::addStroke(m_pNameTxt, 1.0f, ccc3(0xFE, 0xF8, 0x97));
    CCColorUtil::addStroke(m_pNameTxt, 0.8f, ccc3(0xFF, 0x00, 0x00));
    CCColorUtil::addShadow(m_pNameTxt);
    m_pNameTxt->setText(nick);
	addChild(m_pNameTxt, 2, HERO_NAME_TAG);
}

void CMainHero::setScale(float fScale){
	CQStageEntity::setScale(fScale);

	if(m_pstLeftHand){
		m_pstLeftHand->setScale(fScale);
	}

	if(m_pstRightHand){
		m_pstRightHand->setScale(fScale);
	}
}

void CMainHero::setScaleX(float fScaleX){
	CQStageEntity::setScaleX(fScaleX);

	if(m_pstLeftHand){
		m_pstLeftHand->setScaleX(fScaleX);
	}

	if(m_pstRightHand){
		m_pstRightHand->setScaleX(fScaleX);
	}
}
void CMainHero::setScaleY(float fScaleX){
	CQStageEntity::setScaleY(fScaleX);

	if(m_pstLeftHand){
		m_pstLeftHand->setScaleX(fScaleX);
	}

	if(m_pstRightHand){
		m_pstRightHand->setScaleX(fScaleX);
	}
}

float CMainHero::getScaleX(){
	return CQStageEntity::getScaleX();
}

float CMainHero::getScaleY(){
	return CQStageEntity::getScaleY();
}


void CMainHero::OtherKick(){
	HideGunFire();
}

void CMainHero::walkActWithSpeed(unsigned int speed){
	if(!mArmature){
		return;
	}

	CCMovementData* movData = mArmature->getAnimation()->getAnimationData()->getMovement("run");
	if(!movData){
		return;
	}

	float fInterval = CCDirector::sharedDirector()->getAnimationInterval();
	unsigned int runDis = HERO_CFG_MASTER->GetHeroCfgByID(P_USER_OBJ->getUserInfo()->showID)->rundis();
	unsigned int runFrames = movData->duration;
	float speedScale = (runFrames*speed*1.0f*fInterval)/(movData->scale*runDis);
	mArmature->getAnimation()->setSpeedScale(speedScale);
}

void CMainHero::setVIPLv(unsigned int uVIPLv) {
	 if(uVIPLv > 0) {
		 const VIPLvCfg *pVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(uVIPLv);

		 UILabel *pNameTxt = dynamic_cast<UILabel*>(getChildByTag(HERO_NAME_TAG));

		 if(m_pTitleImg == NULL) {
			 m_pTitleImg = UIImageView::create();
			 addChild(m_pTitleImg, 2);
		 }

		 string iconURL = string("title/") + pVIPLvCfg->titleicon() + ".png";
		 m_pTitleImg->loadTexture(iconURL.c_str());
		 m_pTitleImg->setScale(1 / m_fBaseScale);
		 m_pTitleImg->setPosition(pNameTxt->getPosition() + ccp(0, (pNameTxt->getSize().height + m_pTitleImg->getSize().height) * 0.5 / m_fBaseScale));
	 } else {
		 if(m_pTitleImg != NULL) {
			 m_pTitleImg->removeFromParent();
			 m_pTitleImg = NULL;
		 }
	 }
}

void CMainHero::EmergeComplete(){
	DispatchEvent(HERO_EMERGE_COMPLETE_EVENT, this);
}

void CMainHero::Idle(){
	m_stPathPtLst.clear();
	getArmature()->getAnimation()->setSpeedScale(1.0f);
	if (!IsActionInterruptable() || IsDead()){
		return;
	}

	if (getArmature()->getAnimation()->getCurrentMovementID().find("attack") != string::npos){

	}else if (_state != kActionStateIdle){

		if(mArmature->getAnimation()->getCurrentMovementID().find("stand") == string::npos){  //当前播放的不是stand
			if(getArmature()->numberOfRunningActions() > 0) {
				getArmature()->stopAllActions();
			}

			FirstRand();
		}

		SwitchRes("stand");
		
		OtherIdle();
	}
	setMyState(kActionStateIdle);
	_pstCurSkillCell = NULL;
	_curSkill = 0;
	_velocity = ccp(0, 0);
	//CCLOG("main hero idle");
}

void CMainHero::ForceIdle(){
	m_stPathPtLst.clear();
	getArmature()->getAnimation()->setSpeedScale(1.0f);
	if (!IsDead()){

		if(mArmature->getAnimation()->getCurrentMovementID().find("stand") == string::npos){  //当前播放的不是stand
			if(getArmature()->numberOfRunningActions() > 0) {
				getArmature()->stopAllActions();
			}

			FirstRand();
		}

		SwitchRes("stand");
		OtherIdle();
		setMyState(kActionStateIdle);
	}

	_pstCurSkillCell = NULL;
	_curSkill = 0;
	_velocity = ccp(0, 0);

	CCString* strValue = CCString::createWithFormat("%d", 0);
	DispatchEvent(SKILL_CAST_IN_ACTION, strValue);		//技能释放动作播放完了
	//CCLOG("main hero force idle");
}

void CMainHero::winAct(bool isLoop){
	setMyState(kActionStateWin);
	if(getArmature()->numberOfRunningActions() > 0) {
		getArmature()->stopAllActions();
	}

	HideGunFire();
	m_fTransformScale = 1.0;

	if(mArmature && mArmature->getChildByTag(SKILL_GUN_EFF_TAG)){
		mArmature->removeChildByTag(SKILL_GUN_EFF_TAG);
	}

	unsigned int showID = m_uShowID;
	if(showID == 0){
		showID = P_USER_OBJ->getUserInfo()->showID;
	}
	unsigned int actNum = HERO_CFG_MASTER->GetHeroCfgByID(showID)->actnum();  //动作的数量
	unsigned int randIdx = CRandom::GetRand(actNum);  //随机一组动作
	string actStr = "win_" + intToString(randIdx + 1);

	if(getArmature()->getAnimation()->getAnimationData()->getMovement(actStr.c_str())){
		if(isLoop){
			getArmature()->getAnimation()->play(actStr.c_str(), -1, -1, -1);
		}else{
			getArmature()->getAnimation()->play(actStr.c_str(), -1, -1, 0);
		}
	}else if(getArmature()->getAnimation()->getAnimationData()->getMovement("win")){
		if(isLoop){
			getArmature()->getAnimation()->play("win", -1, -1, -1);
		}else{
			getArmature()->getAnimation()->play("win", -1, -1, 0);
		}
	}
}

void CMainHero::Dizzy(){
	m_stPathPtLst.clear();
	if (!IsActionInterruptable()){
		return;
	}
	if (_state != kActionStateDizzy && !IsDead()){

		if(mArmature->getAnimation()->getCurrentMovementID().find("stand") == string::npos){  //当前播放的不是stand
			getArmature()->stopAllActions();
			FirstRand();
		}

		SwitchRes("stand");

		OtherDizzy();
		setMyState(kActionStateDizzy);
	}

	//_pstCurSkillCell = NULL;
	_curSkill = 0;
	_velocity = ccp(0, 0);
}

void CMainHero::FirstRand(){
	if(getArmature()->getAnimation()->getAnimationData()->getMovement("stand_1")){
		getArmature()->getAnimation()->play("stand_1", -1, -1, 0);
	}else{
		getArmature()->getAnimation()->play("stand", -1, -1, 0);
	}
}

void CMainHero::RandStand(){
	unsigned int showID = m_uShowID;
	if(showID == 0){
		showID = P_USER_OBJ->getUserInfo()->showID;
	}
	unsigned int actNum = HERO_CFG_MASTER->GetHeroCfgByID(showID)->actnum();  //动作的数量
	unsigned int randIdx = CRandom::GetRand(actNum);  //随机一组动作
	string actStr = "stand_" + intToString(randIdx + 1);

	if(getArmature()->getAnimation()->getAnimationData()->getMovement(actStr.c_str())){
		getArmature()->getAnimation()->play(actStr.c_str(), -1, -1, 0);
	}else if(getArmature()->getAnimation()->getAnimationData()->getMovement("stand")){
		getArmature()->getAnimation()->play("stand", -1, -1, 0);
	}
}
