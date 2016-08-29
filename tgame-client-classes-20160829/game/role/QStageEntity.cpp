#include "QStageEntity.h"
#include "cocoa/CCObject.h"
#include "control/AudioManager.h"
#include "SkillCfgMaster.h"
#include "instance/InstanceUtils.h"
#include "instance/InstanceEventUtils.h"
#include "instance/AnimationUtils.h"
#include "UserObj.h"
#include "GlobalTime.h"
#include "defines.h"
#include "ImageNumber.h"
#include "ToolUtils.h"
#include "QEnvSet.h"
#include "vlib/CVRand.h"
#include "ConstHelper.h"

#include "BuffCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "PlayerInitCfgMaster.h"
#include "HeroCfgMaster.h"
#include "Global.h"
#include "BulletCfgMaster.h"

USING_NS_CC_EXT;
USING_NS_CC;

#define SHADOW_TAG 2000
#define HURT_COLOR_TIME 0.1f

const string CQStageEntity::ATK_ACTION_KEY_FRAME = "atk_key";				//近战普通攻击
const string CQStageEntity::EXCLUSIVE_ONE_BEGIN_FRAME = "begin_the_one";	//播放技能黑屏，只有猪脚动
const string CQStageEntity::EXCLUSIVE_ONE_END_FRAME = "end_the_one";		//播放完技能取消黑屏
const string CQStageEntity::SKILL_CAST_SOUND_FRAME = "skill_cast_sound";	//技能音效
const string CQStageEntity::SKILL_CAST_FRAME = "skill_cast_eff";			//技能特效
const string CQStageEntity::FIRE_EVENT = "fire_event";                      //告诉上层，这里要发射子弹了
const string CQStageEntity::DIE_EFF_FRAME = "die_eff_frame";                //角色死亡播放死亡特效的帧
const string CQStageEntity::ENLARGE_FRAME = "enlarge_frame";
const string CQStageEntity::SHRINK_FRAME = "shrink_frame";
const string CQStageEntity::SKILL_CAST_IN_ACTION = "skill_cast_action";
const string CQStageEntity::KICK_EVENT = "kick_frame"; 
const string CQStageEntity::BEGIN_SLIDE_FRAME = "begin_slide_frame"; 
const string CQStageEntity::CEASE_SLIDE_FRAME = "cease_slide_frame"; 
const string CQStageEntity::GUN_SMOKE_FRAME = "zhujue_yanchen";
const string CQStageEntity::GUN_BULLET_SHADE_FRAME = "zhujie_danmu";
const string CQStageEntity::GUN_HIT_GROUND_FRAME = "zhujue_dadi";
const string CQStageEntity::ACTIVE_COMPLETE_EVENT = "active_complete";		//从休眠中激活完成事件
const string CQStageEntity::WIN_COMPLETE_EVENT = "win_complete_event";
const string CQStageEntity::DIE_MOMENT_EVENT = "die_moment_event";			//死亡瞬间的数据记录
const string CQStageEntity::DIE_COMPLETE_EVENT = "die_complete_event";		//死亡动作完成事件 
const string CQStageEntity::DASH_COMPLETE_EVENT = "dash_complete_event";	//冲撞技能完成事件
const string CQStageEntity::DIE_EFF_SOUND_EVENT = "die_eff_sound_event";	//boss死亡动作中特效的音效事件
const string CQStageEntity::JUMP_ATK_END_KET_FRAME = "jump_end_key";		//跳跃攻击结束帧
const string CQStageEntity::GUN_POINT_EFF_KEY_FRAME = "gun_point_eff_key";	//枪口特效帧
const string CQStageEntity::ADD_ANI_EFF_EVENT = "add_ani_eff_event";		//角色身上特效增加事件
const string CQStageEntity::HERO_DASH_COMPLETE_EVENT = "hero_dash_complete_event";	//角色闪避完成事件

CQStageEntity::CQStageEntity()
	:m_bIsInvincible(false),
	_pstCurSkillCell(NULL),
	_blood(NULL),
	mArmature(NULL),
	_state(kActionStateNone),
	_hurtSuspendAction(true),
	_vision(-1),
	_isTheOne(false),
	m_bIsCheckOutSize(true),
	m_bIsFollowActivate(false),
	m_stFollowDstPos(CCPointZero),
	m_stFollowDirection(CCPointZero),
	m_dObjectLife(0),
	m_dLastCheckTime(0),
	m_uBulletDepthRatio(1),
	_attakRange(0),
	_damageRange(0),
	m_pstWeaponCfg(NULL),
	m_bIsControlledByAI(false),
	m_fAITime(0),
	m_bIsNeedPath(true),
	m_iNextHurtActionTime(0),
	m_fTransformScale(1.0),
	m_bIsPlayingMovement(true),
	m_AIType(AI_TYPE_FIRE_AT_WILL),
	m_uDefAdjustRatio(1),
	m_uAtkAdjustRatio(1),
	_obj(NULL),
	m_fOrigScale(1),
	_patrolArea(0),
	m_iBulletDamageAdj(0),
	m_fPreTime(0),
	_isEmerge(false),
	m_fHurtTime(0),
	m_bIsHurtColor(false),
	_hurtColor(0),
	_toatlAtkSpeed(0),
	_hit(0),
	m_fNextDashableTime(0),
	_isBlock(false),
	m_bIsPlayDieVoice(true),
	m_iCurAniEffNum(0),
	_stEmergeAct(""),
	_dieResPath(""),
	_blockSize(CCSizeZero),
	m_stJumpAtkDstPos(CCPointZero),
	m_dStartWalkTime(0.0),
	m_uRandData(0),
	m_bIsBulletLimit(false),
	m_fBulletTime(0)
{
	_skillArray = CCArray::create();
	_skillArray->retain();	

	for (int i=0; i < MAX_MAGIC_CNT; i++){
		m_stMagicAtkRatio[i] = 1;
		m_stMagicDefRatio[i] = 1;
	}

	for (int i = 0; i < ROLE_MAX_BUFF_CNT; i++){
		m_stBuffs[i] = NULL;
	}

	m_iKnockCount = random_range(1,3);
    _weaponCfgID = 0; //init

	//随机得到加密数据
	m_uRandData = 9527 + CRandom::GetRandHaveMax(EGAME_DENOM);
	setBaseScale(1.5);
}


CQStageEntity::~CQStageEntity(){
    /*
	if (_skillArray){
		_skillArray->removeAllObjects();
		_skillArray->release();
		_skillArray = NULL;
	}*/

    CC_SAFE_RELEASE_NULL(_skillArray);
    CC_SAFE_RELEASE_NULL(_obj);

	this->removeAllChildrenWithCleanup(true);

	for (int i = 0; i < ROLE_MAX_BUFF_CNT; i++){
		CC_SAFE_RELEASE_NULL(m_stBuffs[i]);
	}

	//CC_SAFE_RELEASE_NULL(mArmature);	
}



void CQStageEntity::onEnter(){
	CCNode::onEnter();
	scheduleUpdate();
	CCNode::setScale(getBaseScale());
}

void CQStageEntity::onExit(){
	unscheduleUpdate();
	CCNode::onExit();
	this->removeAllChildrenWithCleanup(true);
	
}


bool CQStageEntity::init()
{
    CCNode::init();

	_state = kActionStateNone;
	mArmature = NULL;	
	m_uInitScale = 0; 
	_velocity.x = 0;
	_velocity.y = 0;
	return true;
}

void CQStageEntity::setScaleX(float fScaleX){
	//CCNode::setScale(1.5);
	if(mArmature){
		mArmature->setScaleX(fScaleX);
	}
}

void CQStageEntity::setScaleY(float fScaleX){
	//CCNode::setScale(1.5);
	if(mArmature){
		mArmature->setScaleY(fScaleX);
	}
}

void CQStageEntity::setScale(float fScale){
	//CCNode::setScale(1.5);
	if(mArmature){
		mArmature->setScale(fScale);
	}

	m_fTransformScale = fScale;
}

float CQStageEntity::getScale(){
	return mArmature->getScaleY();
}

float CQStageEntity::getScaleX(){
	return mArmature->getScaleX();
}

float CQStageEntity::getScaleY(){
	return mArmature->getScaleY();
}

void CQStageEntity::SleepEntity(){

}

void CQStageEntity::Active(){

}

void CQStageEntity::Jump(){
	if (getMyState()!= kActionStateJump && IsActionInterruptable()){
		this->stopAllActions();
		getArmature()->getAnimation()->play("jump");
		setMyState(kActionStateJump);
	}
}

void CQStageEntity::WanderWithDirection(const CCPoint& stDir){
	if (getMyState() == kActionStateWander || _state == kActionStateWalkToDst){
		setMyState(kActionStateWalk);
	}	
	//CCLog("wonder with dir %f:%f, state is %d\n", stDir.x, stDir.y, _state);
	WalkWithDirection(stDir);
	setMyState(kActionStateWander);
}

void CQStageEntity::WalkTo(const CCPoint& stDst){
//	_dstPoint = stDst;
    m_stPathPtLst.clear();
    m_stPathPtLst.push_back(stDst);
	
	WalkWithDirection(ccpNormalize(ccpSub(stDst, getPosition())));
	
	setMyState(kActionStateWalkToDst);

	
}

void CQStageEntity::WalkWithDirection(const CCPoint& stDir1)
{
	//CCLog("hehe");
	if (!IsActionInterruptable() || !IsWalkable()){
		return;
	}

	if (!getArmature()){
		return;
	}

	CCPoint stDir = ccpNormalize(stDir1);

	//CCLog("walk with dir %f:%f, state is %d, skill %p, id %d\n", stDir.x, stDir.y, _state, GetCurSkillCell(), m_uID);
	if (_state != kActionStateWalk
		&& _state != kActionStateWander
		&& _state != kActionStateWalkToDst
		&& _state != kActionStateAttackAndWalk
		&& _state != kActionStateAtkCounterMarch)
	{
		this->stopAllActions();
		SwitchRes("run");
		getArmature()->getAnimation()->play("run", -1, -1, 1);
		OtherWalk();
		setMyState(kActionStateWalk);
	}

	_velocity = ccp(stDir.x * _walkSpeed, stDir.y * _walkSpeed);

	if ((getArmature()->getAnimation()->getCurrentMovementID() == "walk_attack"
			|| getArmature()->getAnimation()->getCurrentMovementID() == "back_attack")
		&&  fabs(m_dStartWalkTime + 1.0) < 0.01){
			setMyState(kActionStateWalk);
			m_dStartWalkTime = m_dObjectLife;
	}
	if (m_dStartWalkTime > 0 && (m_dStartWalkTime > m_dObjectLife - 0.14) 
		&& (getArmature()->getAnimation()->getCurrentMovementID() == "walk_attack"	|| getArmature()->getAnimation()->getCurrentMovementID() == "back_attack")){
		//调整转向
			if (_velocity.x > 0)
			{
				this->setScaleX(1.0);
			} else if (_velocity.x < 0){
				this->setScaleX(-1.0);
			}
	}

	if (_state == kActionStateAttackAndWalk
		|| _state == kActionStateAtkCounterMarch){
		setMyState(kActionStateWalk);
		//m_dStartWalkTime = m_dObjectLife;
		
	}else if (getArmature()->getAnimation()->getCurrentMovementID() == "run")
	{
		float nowSpeed = _walkSpeed;
		if(_state == kActionStateAttackAndWalk){
			nowSpeed = _backSpeed;
			
		}else{
			nowSpeed = _walkSpeed;
		}

		if(getIsFollowActivate()) { // 若已激活追踪，正确位置和方向映射到虚拟对象
			setFollowDirection(ccp(stDir.x * nowSpeed, stDir.y * nowSpeed));
		} else {
			
			if(_state == kActionStateWalk)
			{
				if (_velocity.x > 0)
				{
					this->setScaleX(1.0);
				} else if (_velocity.x < 0){
					this->setScaleX(-1.0);
				}
			}
		}
	}

	
}

void CQStageEntity::AttackWalkWithDirection(const CCPoint& stDir1)
{
	if (!IsActionInterruptable() || !IsWalkable()){
		return;
	}

	CCPoint stDir = ccpNormalize(stDir1);

	if (mArmature->getAnimation()->getCurrentMovementID() != "walk_attack"){
		this->stopAllActions();
		SwitchRes("walk_attack");
		if (mArmature->getAnimation()->getAnimationData()->getMovement("walk_attack")){
			getArmature()->getAnimation()->play("walk_attack", -1, -1, 1);
		}else{
			getArmature()->getAnimation()->play("run");
		}		
		OtherAttackWalk();
		_state = kActionStateAttackAndWalk;
	}

	_state = kActionStateAttackAndWalk;

	if (_state == kActionStateAttackAndWalk){
		_velocity = ccp(stDir.x * _backSpeed, stDir.y * _backSpeed);
		if (_velocity.x > 0){
			this->setScaleX(1.0);
		} else if (_velocity.x < 0){
			this->setScaleX(-1.0);
		}
	}

	m_dStartWalkTime = -1.0;
}

bool CQStageEntity::IsActionInterruptable() {
	if (_state == kActionStateAttack && _pstCurSkillCell
		&& _pstCurSkillCell->m_uSkillID == _curSkill){
			//CCLog("not interruptable!\n");
			//CCLog("id %d not interruptable, state %d, skill %d", m_uID, getMyState(), _curSkill);
			return false; 
			//释放一个不可被打断的技能之中
	}

	if (_state == kActionStateBackDash || _state == kActionStateWin 
		|| _state == kActionStateForwardDash || _state == kActionStateJumpAtk){
		return false;
	}
	//CCLog("id %d interruptable, state %d, skill %d", m_uID, getMyState(), _curSkill);
	
	return true;
}

void CQStageEntity::AttackCounterMarchWithDirection(const CCPoint& stDir1)
{
	m_stPathPtLst.clear();
	if (!IsActionInterruptable() || !IsWalkable()){
		return;
	}

	CCPoint stDir = ccpNormalize(stDir1);
	if (getArmature()->getAnimation()->getCurrentMovementID() != "back_attack"){
		this->stopAllActions();
		SwitchRes("back_attack");
		getArmature()->getAnimation()->play("back_attack");		
		
		OtherAttackCounterMarch();
		if (stDir1.x > 0){
			setScaleX(-1);
		}else{
			setScaleX(1);
		}
	}

	_state = kActionStateAtkCounterMarch;
	
	_velocity = ccp(stDir.x * _backSpeed, stDir.y * _backSpeed);
	
	m_dStartWalkTime = -1.0;
	
}


void CQStageEntity::CounterMarchWithDirection(const CCPoint& stDir1){
	if (!IsActionInterruptable()){
		return;
	}

	CCPoint stDir = ccpNormalize(stDir1);
	//CCLog("counter march with dir %f:%f, state is %d\n", stDir.x, stDir.y, _state);
	if (_state != kActionStateWalk
		&& _state != kActionStateWander
		&& _state != kActionStateWalkToDst
		&& _state != kActionStateAtkCounterMarch){
		
		this->stopAllActions();
		SwitchRes("run");
		getArmature()->getAnimation()->play("run");
		setMyState(kActionStateWalk);

	}
	if (_state == kActionStateWalk 
		|| _state == kActionStateAttackAndWalk
		|| _state == kActionStateWalkToDst
		|| _state == kActionStateAtkCounterMarch){
		_velocity = ccp(stDir.x * _backSpeed, stDir.y * _backSpeed);

		if (_velocity.x > 0){
			this->setScaleX(-1.0);
		} else if (_velocity.x < 0){
			this->setScaleX(1.0);
		}
	}
	setMyState(kActionStateAtkCounterMarch);
	OtherCounterMarch();
}



void CQStageEntity::DashWithDirection(const CCPoint& stDir){
	if (_state == kActionStateDash){
		this->stopAllActions();
		SwitchRes("dash");
		if(mArmature->getAnimation()->getAnimationData()->getMovement("dash")){
			getArmature()->getAnimation()->play("dash");
		}else{
			getArmature()->getAnimation()->play("run");
		}
		
		_state = kActionStateDash;
		OtherDash();
	}

	if (_state == kActionStateDash){
		_velocity = ccp(stDir.x * _dashSpeed, stDir.y * _dashSpeed);
		if (_velocity.x > 0){
			this->setScaleX(1.0);
		} else if (_velocity.x < 0){
			this->setScaleX(-1.0);
		}
	}
}

void CQStageEntity::AttackDashWithDirection(const CCPoint& stDir){
	if (_state == kActionStateIdle){
		this->stopAllActions();
		SwitchRes("attackdash");
		getArmature()->getAnimation()->play("attakdash");
		_state = kActionStateWalk;
		OtherAttackDash();
	}

	if (_state == kActionStateWalk){
		_velocity = ccp(stDir.x * _dashSpeed, stDir.y * _dashSpeed);
		if (_velocity.x > 0){
			this->setScaleX(1.0);
		}else if (_velocity.x < 0){
			this->setScaleX(-1.0);
		}
	}
}

void CQStageEntity::AttackWithSkill(unsigned int uSkillID){
	for (unsigned int iIdx = 0; iIdx < _skillArray->count(); iIdx++){
		CQSkillCell* stSkill = (CQSkillCell*)_skillArray->objectAtIndex(iIdx);
		if (!stSkill){
			return;
		}
		if (stSkill->m_uSkillID == uSkillID){
			return AttackWithSkillAtIdx(iIdx);
		}
	}
}

void CQStageEntity::AttackWithSkillAtIdx(int iIdx){
	if (!IsAttackable()){
		return;
	}

	CQSkillCell* stSkill = (CQSkillCell*)_skillArray->objectAtIndex(iIdx);
	if (!stSkill){
		return;
	}

	stSkill->m_stIDToDamgeCnt.clear();  //重置伤害次数

	if (stSkill->m_uLastCastTime && stSkill->m_uLastCastTime + stSkill->m_stCfg.cd() > GetEntityTime()){
		return;
	}
	_pstCurSkillCell = stSkill;
	_pstCurSkillCell->m_uLastCastTime = GetEntityTime();
	setCurSkill(stSkill->m_uSkillID);
	_pstCurSkillCell = stSkill;
	setMyState(kActionStateAttack);
	getArmature()->getAnimation()->stop();	
	SwitchRes("attack");
	if (strcmp(stSkill->m_stCfg.castaction().c_str(), "null") 
		&& mArmature->getAnimation()->getAnimationData()->getMovement(stSkill->m_stCfg.castaction().c_str())){
		//大招，且有大招动作，就放大招动作		
		getArmature()->getAnimation()->play(stSkill->m_stCfg.castaction().c_str(), -1, -1, 0);
	}else if (mArmature->getAnimation()->getAnimationData()->getMovement("skill_attack")){
		getArmature()->getAnimation()->play("skill_attack", -1, -1, 0);
		//CCLog("Attck skill, interruptable %d", _pstCurSkillCell->bIsInteruptable);
	}else{
		getArmature()->getAnimation()->play("attack", -1, -1, 0);

	}
	
	OtherAttackWithSkill(stSkill);
	getArmature()->getAnimation()->setSpeedScale(1.0);

	CCString* strSkillValue = CCString::createWithFormat("%d", stSkill->m_uSkillID);
	DispatchEvent(CMainHero::HERO_CAST_SKILL_EVENT, strSkillValue);

	CCString* strValue = CCString::createWithFormat("%d", 1);
	DispatchEvent(SKILL_CAST_IN_ACTION, strValue);		//开始播放技能释放动作
	//CCLog("time %f id %d attack with skill %d", m_dObjectLife, m_uID, stSkill->m_uSkillID);
}

void CQStageEntity::CastSkillGunAnimation(const CQSkillCell* pstSkill){
	//播放音效，只需要播放cast 部分？
	if (pstSkill->m_stCfg.castsound().length() && strcmp(pstSkill->m_stCfg.castsound().c_str(), "null")){
		AudioManager::playEffect(pstSkill->m_stCfg.castsound().c_str());
	}

	if (pstSkill->m_stCfg.gunanimation().length() && strcmp(pstSkill->m_stCfg.gunanimation().c_str(), "null")){
		CCArmature* stGunAni = CCArmature::create(pstSkill->m_stCfg.gunanimation().c_str());
		if (stGunAni){
			stGunAni->getAnimation()->setMovementEventCallFunc((CQStageEntity*)this,movementEvent_selector(CQStageEntity::MovementCallbackToRelease));
			stGunAni->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(CQStageEntity::HandleFrameEvent));
			stGunAni->getAnimation()->play("fly", -1, -1, 0);
			CCPoint stPoint = GetSkillGunPoint(pstSkill->m_stCfg.castaction());
			stGunAni->setPosition(ccp(stPoint.x*getScaleX(), stPoint.y));			
			getArmature()->addChild(stGunAni, 100, SKILL_GUN_EFF_TAG);

		}
	}
}

void CQStageEntity::AttackWithAction(const string& strName, const string& strSoundName){
	if (!IsAttackable()){
		return;
	}

	if(strName.length() <= 0 || strName == "null"){
		return;
	}

	if (mArmature->getAnimation()->getAnimationData()->getMovement(strName.c_str())){
		getArmature()->getAnimation()->play(strName.c_str(), -1, -1, 0);
	}else{
		CCLOG("AttackWithAction Error: there is no %s action", strName.c_str());
	}

	//播放音效
	if(strSoundName.length() && strcmp(strSoundName.c_str(), "null")){
		AudioManager::playEffect(strSoundName.c_str());
	}

	_state = kActionStateAttack;
}

const CQSkillCell* CQStageEntity::GetSkillByIdx(int iIdx){
	CQSkillCell* stSkill = (CQSkillCell*)_skillArray->objectAtIndex(iIdx);
	return stSkill;
}

void CQStageEntity::Attack()
{
	m_stPathPtLst.clear();
	if (!IsActionInterruptable() || !IsAttackable() || getArmature()->getAnimation()->getCurrentMovementID() == "kick"){
		return;
	}
	SwitchRes("attack");
	getArmature()->stopAllActions();
	getArmature()->getAnimation()->play("attack", -1, -1, 0);
	setMyState(kActionStateAttack);
	OtherAttack();

	//CCLOG("attack %p", this);
}

void CQStageEntity::Idle()
{
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

			getArmature()->getAnimation()->play("stand", -1, -1, 1);
		}
		
		SwitchRes("stand");
		OtherIdle();
		
	}
	setMyState(kActionStateIdle);
	_pstCurSkillCell = NULL;
	_curSkill = 0;
	_velocity = ccp(0, 0);
	//CCLOG("idle %d id", m_uID);
}

void CQStageEntity::ForceIdle(){
	if (_state != kActionStateIdle && !IsDead()){
		SwitchRes("stand");

		if(mArmature->getAnimation()->getCurrentMovementID().find("stand") == string::npos){  //当前播放的不是stand
			if(getArmature()->numberOfRunningActions() > 0) {
				getArmature()->stopAllActions();
			}

			getArmature()->getAnimation()->play("stand");
		}
		OtherIdle();
		setMyState(kActionStateIdle);
	}
	

	_pstCurSkillCell = NULL;
	_curSkill = 0;
	_velocity = ccp(0, 0);
	//CCLOG("id %d force idle", m_uID);
}

void CQStageEntity::ForceWalk(){
	if (!IsDead()){	

		SwitchRes("run");

		getArmature()->getAnimation()->play("run");
		OtherWalk();

		setMyState(kActionStateWalk);
		if (getVelocity().x > 0){
			setScaleX(fabs(getScaleX()));
		}else {
			setScaleX(fabs(getScaleX()) * -1.0);
		}
	}
	
	//CCLOG("idle %d force walk", m_uID);
	
}


void CQStageEntity::Dizzy(){
	m_stPathPtLst.clear();
	if (!IsActionInterruptable()){
		return;
	}
	if (_state != kActionStateDizzy && !IsDead()){

		if(mArmature->getAnimation()->getCurrentMovementID().find("stand") == string::npos){  //当前播放的不是stand
			getArmature()->stopAllActions();
			getArmature()->getAnimation()->play("stand", -1, -1, 1);

		}

		SwitchRes("stand");
		OtherDizzy();
		setMyState(kActionStateDizzy);
	}

	//_pstCurSkillCell = NULL;
	_curSkill = 0;
	_velocity = ccp(0, 0);

}


void CQStageEntity::Die(){
	m_stPathPtLst.clear();
	if (_state == kActionStateDying && mArmature && mArmature->getAnimation()->getCurrentMovementID().find("die") != string::npos){
		return;
	}

	if (mArmature->getAnimation()->getCurrentMovementID() == "die_1" || mArmature->getAnimation()->getCurrentMovementID() == "die_2"){
// 		CCLOG("die event %u, res name %s, action name %s, status %d", 
// 			this->m_uID,
// 			getArmature()->getName().c_str(),
// 			getArmature()->getAnimation()->getCurrentMovementID().c_str(),
// 			getMyState());
	}

// 	CCLOG("die event %u, res name %s, action name %s, status %d", 
// 		this->m_uID,
// 		getArmature()->getName().c_str(),
// 		getArmature()->getAnimation()->getCurrentMovementID().c_str(),
// 		getMyState());


	if (_dieResPath != "" && _dieResPath != "null" && m_iCurAniEffNum < (int)ConstHelper(FIGHT_ANI_EFF_MAX)){
		CCArmature* dieArm = CCArmature::create(_dieResPath.c_str());
		if (dieArm){
			dieArm->getAnimation()->setMovementEventCallFunc((CQStageEntity*)this,movementEvent_selector(CQStageEntity::MovementCallbackToRelease));
			dieArm->getAnimation()->play("fly", -1, -1, 0);
			dieArm->setPosition(ccp(0, getCenterToBottom()));			
			getArmature()->addChild(dieArm, 100);

			this->DispatchEvent(ADD_ANI_EFF_EVENT, this);
			m_iCurAniEffNum++;
		}
	}


	SwitchRes("die");
	if (mArmature && mArmature->getAnimation()->getAnimationData()->getMovement("circle") && m_iKnockCount > 0){
		mArmature->getAnimation()->play("circle", -1, -1, 1);
	}else if (mArmature && mArmature->getAnimation()->getAnimationData()->getMovement("die")){
		mArmature->getAnimation()->play("die", -1, -1, 0);
		OtherDie();
	}
	

	//如果有死亡音效，也在这里播放
	if (getDieVoice().length() && strcmp(getDieVoice().c_str(), "null") && m_bIsPlayDieVoice){
		AudioManager::playEffect(getDieVoice().c_str());
	}
	setMyState(kActionStateDying);
	setShadowVisiable(false);
	_blood->setVisible(false);
	this->DispatchEvent(DIE_MOMENT_EVENT, this);
// 	CCLOG("dispatch event %u, res name %s, action name %s, status %d", 
// 		this->m_uID,
// 		getArmature()->getName().c_str(),
// 		getArmature()->getAnimation()->getCurrentMovementID().c_str(),
// 		getMyState());

    _velocity = ccp(0, 0);
}

void CQStageEntity::DieWithAction(const char* stDieAction){
	if (mArmature && mArmature->getAnimation()->getAnimationData()->getMovement(stDieAction)
		&&  mArmature->getAnimation()->getCurrentMovementID().find(stDieAction) == string::npos){
		mArmature->getAnimation()->play(stDieAction, -1, -1, 0);
		OtherDie();
	}
}

void CQStageEntity::HurtWithDamage(float fDamage){
	if(checkIsDied()){		
		return;
	}

	//数据校验
	if(!checkData(_hp, m_uHpMd5)){
		resumeData(_hp, m_uHpMd5);
	}

	if(fDamage > _hp){
		fDamage = _hp;
	}

	ShowBloodNum(UINT64_t(fDamage), false);
	_hp -= (UINT64_t)fDamage;

	updateData(_hp, m_uHpMd5);

	ShowHurt((int)fDamage);
	HPChange();
	if (_state != kActionStateHurt){
		_preState = _state;
	}
	
	//CCLog("state is %d, %d", _state, IsActionInterruptable());
	
	Hurt();
}

void CQStageEntity::Hurt(){
	if(m_fHurtTime != 0){  //已经受伤变红
		resetColor();
	}

	if(!checkIsDied() 
		&& _state != kActionStateHurt
		&& getArmature()
		&& getNextHurtActionTime() <= 0){
		//策划说不播放受伤动作，2014-9-18
		//SwitchRes("hurt");
		//getArmature()->stopAllActions();
		//getArmature()->getAnimation()->play("hurt", -1, -1, 0);
		//_preState = _state;
		//m_stPreVocity = _velocity;
		//_state = kActionStateHurt;
		//OtherHurt();
		//_velocity = ccp(0, 0);
		//if(random_range(0, 100) > 50){
		//	//KnockDown();
		//}
		//setNextHurtActionTime(m_iEffectHurtActionCD);

		setNextHurtActionTime(m_iEffectHurtActionCD);
		getArmature()->setColor(CToolUtils::parseColorToCC3(_hurtColor));
		m_bIsHurtColor = true;
	}else if(checkIsDied()){
		Die();
	}
}

void CQStageEntity::resetColor(){
	getArmature()->setColor(ccc3(255, 255, 255));
	m_bIsHurtColor = false;
	m_fHurtTime = 0;
}

void CQStageEntity::HurtWithDamage(float fDamage, const CCPoint& stHitBackDist){
	if (fDamage < 0){
		CCLog("damage < 0");
	}
	HurtWithDamage(fDamage);
	if (_state == kActionStateHurt && getNextHurtActionTime() == m_iEffectHurtActionCD){
		_velocity = ccpNormalize(stHitBackDist);
		_velocity.x  *= 300; 
		_velocity.y  *= 300; 

		m_stHitBackDest = ccpAdd(getPosition(), stHitBackDist);
		//重置速度，准备后移
	}
}



void CQStageEntity::HurtWithDamage(CHurtDamageObj stObj) {
	if (IsDead()){
		return;
	}
	int iNewCD = (1 - (stObj.iBulletOwnerLevel - getLevel()) * 13 / 400) * getHurtActionCD();
	if (iNewCD < 0){
		m_iEffectHurtActionCD = 0;
	}else{
		m_iEffectHurtActionCD = iNewCD;
	}
	HurtWithDamage(stObj.fDamage, stObj.stHitBackPos);
	bool bIsPlayAni = true;
	m_iCurAniEffNum = stObj.iCurAniEffNum;
	if (strcmp(stObj.stHurtAnimation.c_str(), "null") && stObj.stHurtAnimation.length()){
		CCArray* pstChildren = getChildren();
		CCObject* pstObj;
		int iCnt = 0;
		CCARRAY_FOREACH(pstChildren, pstObj){
			CCArmature* hurtAnimation = dynamic_cast<CCArmature*>(pstObj);
			if (hurtAnimation && strcmp(hurtAnimation->getName().c_str(), stObj.stHurtAnimation.c_str()) == 0){
				iCnt ++;
			}
			
		}

		if (iCnt > 2 || (m_iCurAniEffNum >= (int)ConstHelper(FIGHT_ANI_EFF_MAX))){ //同一个子弹的受击特效超过3个，就不加了，再加太好资源了，这个数字还可以继续调整
			bIsPlayAni = false;;
		}

		CCArmature* hurtAnimation = CCArmature::create(stObj.stHurtAnimation.c_str());
		if (hurtAnimation && bIsPlayAni){
			getArmature()->addChild(hurtAnimation, 100);		
			hurtAnimation->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CQStageEntity::HandleBeatenEffActionEvent));
			if (hurtAnimation->getAnimation()->getAnimationData()->getMovement("fly") == NULL){
				CCLOG("error!!! animation %s has no action fly!", hurtAnimation->getName().c_str());
			}
			hurtAnimation->getAnimation()->play("fly", -1, -1, 0);
			hurtAnimation->setPosition(ccp(0, stObj.stBulletPos.y - this->getPositionY()));

			this->DispatchEvent(ADD_ANI_EFF_EVENT, this);
			m_iCurAniEffNum++;	
			hurtAnimation->setScale(1 / m_fBaseScale);
		}
	}


	bIsPlayAni = true;
	if ((m_iCurAniEffNum >= (int)ConstHelper(FIGHT_ANI_EFF_MAX))){ 
		bIsPlayAni = false;;
	}

	if (stObj.stPapaAnimation != "null" && stObj.stPapaAnimation.length() && stObj.fDamage > 0 && bIsPlayAni){
		CCArmature* papaAnimation = CCArmature::create(stObj.stPapaAnimation.c_str());
		if (papaAnimation){
			addChild(papaAnimation, 100);		
			papaAnimation->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CQStageEntity::HandleBeatenEffActionEvent));
			papaAnimation->getAnimation()->play("fly", -1, -1, 0);
			papaAnimation->setPosition(ccp(0, stObj.stBulletPos.y - this->getPositionY()));

			int randAngle = random_range(-30, 30);
			papaAnimation->setRotation(randAngle);

			this->DispatchEvent(ADD_ANI_EFF_EVENT, this);
			m_iCurAniEffNum++;
		}
	}

	if (stObj.fDamage > 0 && stObj.pstSkillCfg && stObj.pstSkillCfg->dieaction() != "null" && IsDead()){
		//死了，并且有死亡的动作要求
		if (getArmature()->getAnimation()->getAnimationData()->getMovement(stObj.pstSkillCfg->dieaction().c_str())){
			if (stObj.stSkillPos.x > this->getPositionX()){
				this->setScaleX(-1);
			}else{
				this->setScaleX(1);
			}
			DieWithAction(stObj.pstSkillCfg->dieaction().c_str());
		}
	}else if (stObj.fDamage > 0 && stObj.stBulletPos.x != 0 && stObj.stBulletPos.y != 0 && IsDead()){
		//死了，并且有死亡的动作要求
		if (getArmature()->getAnimation()->getAnimationData()->getMovement("die_1")){
			if (stObj.stBulletPos.x > this->getPositionX()){
				this->setScaleX(1);
			}else{
				this->setScaleX(-1);
			}
			DieWithAction("die_1");
		}
	}

	if(stObj.fDamage > 0){  //伤害大雨0才计算
		if(stObj.pstSkillCfg == NULL && _roleParty == ROLE_PARTY_HOSTILE && stObj.uBulletParty == BULLET_PARTY_HERO){  //怪物受伤 && 猪脚的子弹
			DispatchEvent(HERO_EPVALUE_ADD_EVENT, this);
		}

		if(stObj.pstSkillCfg == NULL && _roleParty == ROLE_PARTY_FRIEND && stObj.uBulletParty == BULLET_PARTY_ENEMY){  //自己受伤 && 对方的子弹
			DispatchEvent(OPPONENT_EPVALUE_ADD_EVENT, this);
		}
	}
}

void CQStageEntity::ShowHurt(int fDamage)
{

}

void CQStageEntity::KnockDown(){
	if(!checkIsDied() 
		&& getArmature()){
		getArmature()->getAnimation()->play("die", -1, -1, 0);
		_velocity = ccp(400 * getScaleX() * -1, 0);
		setMyState(kActionStateKnockDown);
	}

}


void CQStageEntity::ShowBeatenAnimation(const string& stBeatenAni){
	if (strcmp(stBeatenAni.c_str(), "null") == 0){
		return;
	}

	CCArmature* hurtAnimation = CCArmature::create(stBeatenAni.c_str());
	if (!hurtAnimation){
		return;
	}

	hurtAnimation->setPosition(ccp(0, getCenterToBottom()));
	getArmature()->addChild(hurtAnimation, 100);		
	hurtAnimation->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CQStageEntity::HandleBeatenEffActionEvent));
	hurtAnimation->getAnimation()->play("fly", -1, -1, 0);

}

void CQStageEntity::ShowBloodNum(UINT64_t num, bool isAdd){
	if(mArmature == NULL || num <= 0){
		return;
	}

	float delayT = 0;
	if(m_fPreTime == 0){
		delayT = 0;
	}else{
		if(m_dObjectLife - m_fPreTime <= 0){
			delayT = 0.5f;
		}else{
			if(m_dObjectLife - m_fPreTime > 0.5){
				delayT = 0;
			}else{
				delayT = 0.5 - (m_dObjectLife - m_fPreTime);
			}
		}
	}

	float hight = mArmature->getContentSize().height;
	if(_blood->isVisible()){
		hight += 50;
	}else{
		hight += 10;
	}

	unsigned int numIdx = 14;
	if(isAdd){
		numIdx = 13;
	}else{
		numIdx = 14;
	}

	ImageNumber* attackNum = ImageNumber::create(numIdx);
	attackNum->setNum(intToString(randDamage(num)), -4);
	attackNum->setScale(1/m_fBaseScale);
	attackNum->setPosition(ccp(0, hight));
	addChild(attackNum, 1, 12);
	attackNum->setVisible(false);

	//飘字动画
	CCActionInterval* actionTo = CCMoveTo::create(0.3f, ccp(0, hight + 30));
	//设置动画弹出放大和缩小
	CCActionInterval* scale_act = CCScaleTo::create(0.05f,1.5f/m_fBaseScale, 1.5f/m_fBaseScale);
	CCActionInterval* scale_back = CCScaleTo::create(0.05f, 1.0f/m_fBaseScale, 1.0f/m_fBaseScale);
	//设置动画淡出淡入（开始显示，最后隐藏）
	CCActionInterval* fade_actby = CCFadeIn::create(0.05f);
	CCActionInterval* fade_actby2 = CCFadeOut::create(0.05f);
	CCCallFuncO *actbackfun = CCCallFuncO::create(this, callfuncO_selector(CQStageEntity::ShowNumberCallBack), attackNum);
	attackNum->runAction(CCSequence::create(CCDelayTime::create(delayT), 
		CCShow::create(),
		fade_actby, scale_act, 
		actionTo, scale_back, fade_actby2, actbackfun, NULL));

	m_fPreTime = m_dObjectLife;   //记录上一次飘雪时间
}

int CQStageEntity::randDamage(int iRealNum){
	if(iRealNum < 10){   //小于10不浮动
		return iRealNum;
	}

	unsigned int randNum = CRandom::GetRandHaveMax(10);
	int numDis = randNum - 5;
	int iNum = iRealNum;
	if(iRealNum + numDis < 0){
		iNum = iRealNum;
	}else{
		iNum = iRealNum + numDis;
	}

	return iNum;
}

void CQStageEntity::ShowMiss(){
	float hight = mArmature->getContentSize().height;
	if(_blood->isVisible()){
		hight += 50;
	}else{
		hight += 10;
	}

	CCSprite* pMissSp = CCSprite::create("ui_v2/numbers/miss.png");
	pMissSp->setScale(1/m_fBaseScale);
	pMissSp->setPosition(ccp(0, hight));
	addChild(pMissSp, 1, 12);
	pMissSp->setVisible(false);

	//飘字动画
	CCActionInterval* actionTo = CCMoveTo::create(0.3f, ccp(0, hight + 30));
	//设置动画弹出放大和缩小
	CCActionInterval* scale_act = CCScaleTo::create(0.05f,1.5f/m_fBaseScale, 1.5f/m_fBaseScale);
	CCActionInterval* scale_back = CCScaleTo::create(0.05f, 1.0f/m_fBaseScale, 1.0f/m_fBaseScale);
	//设置动画淡出淡入（开始显示，最后隐藏）
	CCActionInterval* fade_actby = CCFadeIn::create(0.05f);
	CCActionInterval* fade_actby2 = CCFadeOut::create(0.05f);
	CCCallFuncO *actbackfun = CCCallFuncO::create(this, callfuncO_selector(CQStageEntity::ShowNumberCallBack), pMissSp);
	pMissSp->runAction(CCSequence::create(CCShow::create(), fade_actby, scale_act, 
		actionTo, scale_back, fade_actby2, actbackfun, NULL));
}

void CQStageEntity::FloatText(const char *str){
	CCLabelTTF *txt = CCLabelTTF::create(str, "Arial", 16);
	txt->setPosition(ccp(-txt->getContentSize().width/2, 130));
	txt->setScale(1/m_fBaseScale);
	txt->setColor(ccc3(0xFF, 0xFF, 0));
	addChild(txt, 1, 18);

	//飘字动画
	CCActionInterval* actionTo = CCMoveTo::create(0.3f, ccp(-txt->getContentSize().width/2, 160));
	//设置动画弹出放大和缩小
	CCActionInterval* scale_act = CCScaleTo::create(0.05f,1.5f, 1.5f);
	CCActionInterval* scale_back = CCScaleTo::create(0.05f, 1.0f, 1.0f);
	//设置动画淡出淡入（开始显示，最后隐藏）
	CCActionInterval* fade_actby = CCFadeIn::create(0.05f);
	CCActionInterval* fade_actby2 = CCFadeOut::create(0.05f);
	CCCallFunc *actbackfun = CCCallFunc::create(this, callfunc_selector(CQStageEntity::FloatTextCallBack));
	txt->runAction(CCSequence::create(fade_actby, scale_act, actionTo, scale_back, fade_actby2, actbackfun, NULL));
}

void CQStageEntity::ShowNumberCallBack(CCObject* pstObj){
	CCNode* pImg = dynamic_cast<CCNode*>(pstObj);
	if(pImg){
		pImg->removeFromParentAndCleanup(true);
	}
}

void CQStageEntity::FloatTextCallBack(){
	if(!checkIsDied()){
		removeChildByTag(18);
	}
}

void CQStageEntity::updatePosition(float dt){
	if(!getArmature()){
		return;
	}

	string stMovement = getArmature()->getAnimation()->getCurrentMovementID();
	if (_state == kActionStateWalk 
		|| (_state == kActionStateAttackAndWalk && !GetIsControlledByAI() && strcmp("walk_attack", stMovement.c_str()) == 0)
		|| _state == kActionStateWander
		/*|| _state == kActionStateWalkToDst*/
		|| (_state == kActionStateAtkCounterMarch && strcmp("back_attack", stMovement.c_str()) == 0)
		|| _state == kActionStateDash
		|| _state == kActionStateJump
		|| _state == kActionStateHurt
		|| _state == kActionStateBackDash
		|| _state == kActionStateJumpAtk
		|| _state == kActionStateForwardDash
		|| _state == kActionStateDying
		|| _state == kActionStateKnockDown
		|| (_state == kActionStateKick && _velocity.x != 0)){
		if(getIsFollowActivate()){
			float nowSpeed = _walkSpeed;
			if(_state == kActionStateAttackAndWalk){
				nowSpeed = _backSpeed;
			}else if(_state == kActionStateDash){
				nowSpeed = _dashSpeed;
			}else{
				nowSpeed = _walkSpeed;
			}

			setFollowDstPos(ccpAdd(getFollowDstPos(), ccpMult(getFollowDirection(), dt)));

			// 开始追踪
			CCPoint dv = ccpNormalize(getFollowDstPos() - this->getPosition());
			_velocity = ccp(dv.x * nowSpeed, dv.y * nowSpeed);
			this->setPosition(ccpAdd(this->getPosition(), ccpMult(_velocity, dt)));

			if (_velocity.x > 0){
				this->setScaleX(1.0);
			} else if (_velocity.x < 0){
				this->setScaleX(-1.0);
			}

			if(getFollowDstPos().getDistance(this->getPosition()) < 1) { // 追踪完成
				setIsFollowActivate(false);
				this->setPosition(getFollowDstPos());
				_velocity = getFollowDirection();
			}
		} else{
			_desiredPosition = ccpAdd(this->getPosition(), ccpMult(_velocity, dt));
			m_stPrePos = this->getPosition();

			if (_state == kActionStateHurt && fabs(this->getPositionX() - m_stHitBackDest.x) < 20){
				_desiredPosition = this->getPosition();
			}else if(_state == kActionStateBackDash && fabs(this->getPositionX() - m_stDashDstPos.x) < 20){
				//CCString* strValue = CCString::createWithFormat("%d", 0);
				//DispatchEvent(SKILL_CAST_IN_ACTION, strValue);	
				Idle();
			}else if(_state == kActionStateForwardDash && fabs(this->getPositionX() - m_stForDashDstPos.x) < 20){
				DispatchEvent(DASH_COMPLETE_EVENT, this);	
				ForceIdle();
			}else if(_state == kActionStateJumpAtk && fabs(this->getPositionX() - m_stJumpAtkDstPos.x) < 20){
				//ForceIdle();
			}else{
				setPosition(_desiredPosition);
			}
		}
	}

	if (_state == kActionStateWalkToDst 
		|| (_state == kActionStateAttackAndWalk && GetIsControlledByAI())){
        int ii = m_stPathPtLst.size();
        if (m_stPathPtLst.size() <= 0){
            //没有路径点了，到达
            WalkToDstDoneCallback();
            Idle(); // _velocity to 0,0
        }else{
            const CCPoint& stDestPoint = m_stPathPtLst[m_stPathPtLst.size() - 1]; 
			const CCPoint stDistPoint = ccpSub(stDestPoint, this->getPosition());
			
            CCPoint dv = ccpNormalize(stDistPoint);
            _velocity = ccp(dv.x * _walkSpeed, dv.y * _walkSpeed);
			if (fabs(stDistPoint.x) < 2 && fabs(stDistPoint.y) < 2){
				_velocity.x = 0;
				_velocity.y = 0;
			}
            //计算这帧的目的点坐标
            _desiredPosition = ccpAdd(this->getPosition(), ccpMult(_velocity, dt));
			m_stPrePos = this->getPosition();
            setPosition(_desiredPosition);
			if (getRoleType() == ROLE_TYPE_PLAYER){
// 				CCLOG("pre pos (%f, %f), desire pos (%f, %f), next dst (%f, %f);", 
// 					m_stPrePos.x, m_stPrePos.y,
// 					_desiredPosition.x, _desiredPosition.y,
// 					stDestPoint.x, stDestPoint.y);
			}

            if(stDestPoint.getDistance(getPosition()) < 6){
                //走到了
				//CCLog("reach point (%f, %f)", stDestPoint.x, stDestPoint.y);
                this->setPosition(stDestPoint);

                m_stPathPtLst.pop_back(); //这个点走到了，取出来
                if (m_stPathPtLst.size() <= 0){
                    //没有路径点了，停下来
                    WalkToDstDoneCallback();
                    ForceIdle(); // _velocity to 0,0
                }else{
					const CCPoint stDestPoint2 = m_stPathPtLst[m_stPathPtLst.size() - 1]; 
					const CCPoint stDistPoint2 = ccpSub(stDestPoint2, this->getPosition());

					CCPoint dv2 = ccpNormalize(stDistPoint2);
					_velocity = ccp(dv.x * _walkSpeed, dv.y * _walkSpeed);
					if (fabs(stDistPoint2.x) < 2 && fabs(stDistPoint2.y) < 2){
						_velocity.x = 0;
						_velocity.y = 0;
					}
				}
            }

			if (_velocity.x > 0){
				this->setScaleX(1.0);
			} else if (_velocity.x < 0){
				this->setScaleX(-1.0);
			}
        } 
	}
}

void CQStageEntity::update(float dt){
	//CCLog("begin scale %f, uid %d", getScaleX(), m_uID);
	m_dObjectLife += dt;
	m_fAITime += dt;
	m_iNextHurtActionTime -= int(dt * 1000);
	if (m_iNextHurtActionTime < 0){
		m_iNextHurtActionTime = 0;
	}

	if(getArmature() == NULL){
		return;
	}
	
	if (getIsPlayingMovement()){
		updatePosition(dt);
	}

	if (_isTheOne){  //最顶层目标，不用做任何zorder设置
		this->getParent()->reorderChild(this, ROLE_Z_ORDER);
		//CCLog("my zorder is %d", getZOrder());
	}else{
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, this->getPositionY());
		if (getZOrder() != iNewZorder){
			this->getParent()->reorderChild(this, iNewZorder);
		}
	}
	
	//处理变大或者变小的问题
	if (fabs(getScaleY() - m_fTransformScale) > 0.05){
		float fTargetScale = getScaleY()  + (m_fTransformScale - m_fOrigScale) / 0.5 * dt;
		if ((getScaleY() - m_fTransformScale) * (fTargetScale - m_fTransformScale) <= 0){
			fTargetScale = m_fTransformScale;						
		}	
		setScaleY(fTargetScale);
		if (getScaleX() > 0){
			setScaleX(fTargetScale);
		}else{
			setScaleX(fTargetScale * -1);
		}
	}
	
	//更新buff
	RefreshBuffs();

	//更新cd
	m_fNextDashableTime -= dt;
	if (m_fNextDashableTime < 0){
		m_fNextDashableTime = 0;
	}
	
	//更新受伤变红时间
	if(m_fHurtTime >= HURT_COLOR_TIME){
		resetColor();
	}else{
		if(m_bIsHurtColor){
			m_fHurtTime += dt;
		}
	}
}

void CQStageEntity::SetEnityPosition(const CCPoint& stPos){
	setPosition(stPos);
}

void CQStageEntity::setPosition(const CCPoint &position){
	CCNode::setPosition(position);
}

void CQStageEntity::MovementCallback(CCArmature * armature, MovementEventType type, const char * org_name){ 
	//CCLog("name = %s, type = %d\n", name, type);
	//CCLOG("movement event %d, movement %s", type, org_name);
	char name[128];
	string nameStr = org_name;
	sprintf(&name[0], "%s", org_name);
	if (type == COMPLETE && armature->m_uID == getArmature()->m_uID){  
		if (_pstCurSkillCell && nameStr != "dash"){
			//CCLOG("time %f, id %d, action %s complete", m_dObjectLife, m_uID, org_name);
			_pstCurSkillCell = NULL;
			_curSkill = 0;
			CCString* strValue = CCString::createWithFormat("%d", 0);
			DispatchEvent(SKILL_CAST_IN_ACTION, strValue);		//技能释放动作播放完了
		}
		
		if (getMyState() == kActionStateIdle){
			ForceIdle();
			return;
		}else if (getMyState() == kActionStateWalk){
			ForceWalk();
			return;
		}

		if (_state == kActionStateDie){
			HandleEntityDeath();
			return;
		}

		if (strcmp(name,"attack") == 0 ){  
			if (_pstCurSkillCell){
				Idle();
				_curSkill = 0;
				_pstCurSkillCell = NULL;
			}else if (getRoleType() != ROLE_TYPE_PLAYER){
				Idle();
			}
		}else if(strcmp(name, "dash") == 0 && _state == kActionStateIdle){
			Idle();
		}else if (strcmp(name, "hurt") ==0 ){
			Idle();
			if (_preState == kActionStateWalk){
				WalkWithDirection(m_stPreVocity);
			}else if(_preState == kActionStateAtkCounterMarch){
				AttackCounterMarchWithDirection(m_stPreVocity);

			}else if (_preState == kActionStateAttack){
				Attack();
			}else if (_preState == kActionStateAttackAndWalk){
				AttackWalkWithDirection(m_stPreVocity);
			}else{
				Idle();
			}
		}else if (nameStr.find("emerge") != string::npos){
			EmergeComplete();
			Idle(); //某系角色有第一次出现在场景中的特定动作和特效
		}else if (nameStr.find("die") != string::npos && IsDead()){
			setMyState(kActionStateDie);
			HandleEntityDeath();
		}else if (strcmp(name, "run") == 0){
			int iPreState = getMyState();
			setMyState(kActionStateIdle);
			WalkWithDirection(_velocity);
			setMyState(iPreState);
			//CCLOG("run to idle id %d", m_uID);
		}else if (strcmp("back_dash", name) == 0 || strcmp("walk_dash", name) == 0){
			CCString* strValue = CCString::createWithFormat("%d", 0);
			DispatchEvent(SKILL_CAST_IN_ACTION, strValue);	
			DispatchEvent(HERO_DASH_COMPLETE_EVENT, this);
			ForceIdle();
		}else if (strcmp("kick", name) == 0){

		}else if (nameStr.find("win") != string::npos){
			DispatchEvent(WIN_COMPLETE_EVENT, this);	
		}else if (nameStr.find("jump_attack") != string::npos){
			ForceIdle();
		}else{
			Idle();
		}
	}else if (type == LOOP_COMPLETE && armature->m_uID == getArmature()->m_uID){
		//CCLog("action %s, state %d", nameStr.c_str(), getMyState());
		if (getMyState() == kActionStateIdle){
			ForceIdle();
		}else if (getMyState() == kActionStateWalk){
			ForceWalk();
		}
	}
	HandleMovementEvent(armature, type, name);
}  

void CQStageEntity::HandleMovementEvent(CCArmature * armature, MovementEventType type, const char * name){

}

void CQStageEntity::LoadActionRes(bool isEmerge/* = true*/){
	_isEmerge = isEmerge;

	if (mArmature == NULL && strcmp(getResPath().c_str(), "null") != 0){
		mArmature = CCArmature::create(_resPath.c_str());
	}

	LoadOtherRes();
	if (mArmature){
		mArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CQStageEntity::HandleFrameEvent));
	
		getArmature()->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CQStageEntity::MovementCallback));
		setContentSize(mArmature->getContentSize());
		addChild(mArmature);

		//todo
		//mArmature->setScale(1.5f);

		if(getIsBlock()){  //阻挡
			_blockSize = CAnimationUtils::getBlockRect(mArmature).size;
		}

		_centerToBottom = getContentSize().height / 2;
		_centerToSides = getContentSize().width / 2;
		InitSkills();

		_blood = CBloodBar::createWithImage();
		_blood->setScale(1/m_fBaseScale);
		_blood->setPosition(ccp(0, GetBloodPosY()));
		addChild(_blood, -9);

		if(_roleType == ROLE_TYPE_NPC || _roleType == ROLE_TYPE_JUNIOR_BOSS || _roleType == ROLE_TYPE_SENIOR_BOSS || _roleType == ROLE_TYPE_PLAYER){
			_blood->setVisible(false);
		}

		if(_isEmerge){
			Emerge();
		}else{
			Idle();
		}

		//脚下价格阴影
		CCSprite *shadow = CCSprite::create();
		shadow->initWithSpriteFrameName("share/shadow.png");
		shadow->setScaleX(getCenterToSides() / (shadow->getContentSize().width));
		addChild(shadow, -10, SHADOW_TAG);
	}
}

float CQStageEntity::GetBloodPosY(){
	if (!mArmature->getBone("blood")){
		return getContentSize().height + 30;
	}else{
		return mArmature->getBone("blood")->getBoneData()->y;
	}
}

void CQStageEntity::Emerge(){
	//只有主角有多个动作工程，这里就偷个懒了。。。希望不要出问题
	if (mArmature->getAnimation()->getAnimationData()->getMovement("emerge")){
		mArmature->getAnimation()->play("emerge", -1, -1, 0);
		_state = kActionStateEmerge;
	}else{
		Idle();
	}

	if (getEmergeVoice().length() && strcmp(getEmergeVoice().c_str(), "null")){
		AudioManager::playEffect(getEmergeVoice().c_str());
	}

	_isEmerge = true;
}

void CQStageEntity::EmergeComplete(){

}

void CQStageEntity::HandleEntityDeath(){

}

void CQStageEntity::HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx){
	string strEventStr;
	strEventStr.assign(stEvent);
	size_t offset = 0;
	size_t pos = 0;
	while((pos = strEventStr.find(";", offset)) != string::npos){
		string singleEvent = strEventStr.substr(offset, pos - offset);
		HandleSingleEvent(singleEvent);
		offset = pos + 1;
	}

	string singleEvent = strEventStr.substr(offset, strEventStr.length() - offset);
	if (singleEvent.length()){
		HandleSingleEvent(singleEvent);
	}
}

void CQStageEntity::WalkToDstDoneCallback() {
	// 同屏用的
}

void CQStageEntity::checkWalk() {
	float posX = 0.0f;
	float posY = 0.0f;

	const CCRect &walkableRect = getWalkableRect();

	if(this->getPositionX() - this->getCenterToSides() <= walkableRect.getMinX()) {
		posX = walkableRect.getMinX() + this->getCenterToSides();
	} else if(this->getPositionX() + this->getCenterToSides() >= walkableRect.getMaxX()) {
		posX = walkableRect.getMaxX() - this->getCenterToSides();
	} else {
		posX = this->getPositionX();
	}

	if(this->getPositionY() <= walkableRect.getMinY()) {
		posY = walkableRect.getMinY();
	} else if(this->getPositionY() >= walkableRect.getMaxY()) {
		posY = walkableRect.getMaxY();
	} else {
		posY = this->getPositionY();
	}

	this->setPosition(ccp(posX, posY));
}

void CQStageEntity::HandleSingleEvent(const string& stSingleEvent){
	if (stSingleEvent.c_str() == BEGIN_SLIDE_FRAME){
		_velocity.x = -240 * getArmature()->getScaleX();
	}else if(stSingleEvent.c_str() == CEASE_SLIDE_FRAME){
		_velocity.x = 0;
	}else if (strcmp(stSingleEvent.c_str(), ENLARGE_FRAME.c_str()) == 0){
		m_fTransformScale = 1.5;
		m_fOrigScale = getScaleY();
	}else if (strcmp(stSingleEvent.c_str(), SHRINK_FRAME.c_str()) == 0){
		m_fTransformScale = 1.0;
		m_fOrigScale = getScaleY();
	}else if (strcmp(stSingleEvent.c_str(), DIE_EFF_FRAME.c_str()) == 0
		&& !IsDead()){
			//用死亡动作来做被击倒
			//Idle();
			return;
	}else if (stSingleEvent == GUN_POINT_EFF_KEY_FRAME){
		CastSkillGunAnimation(GetCurSkillCell());
	}

	DispatchEvent(stSingleEvent, this);
}


CQSkillCell* CQStageEntity::GetCurSkillCell(){
	for (unsigned int iIdx = 0; iIdx < _skillArray->count(); iIdx++){
		CQSkillCell* pstSkill = (CQSkillCell*)_skillArray->objectAtIndex(iIdx);
		if (pstSkill->m_uSkillID == _curSkill){
			return pstSkill;
		}
	}

	return NULL;
}

int CQStageEntity::GetAReadySkill(){
	for (unsigned int iIdx = 0; iIdx < _skillArray->count(); iIdx++){
		CQSkillCell* pstSkill = (CQSkillCell*)_skillArray->objectAtIndex(iIdx);
		if (pstSkill->m_uLastCastTime + pstSkill->m_stCfg.cd() < GetEntityTime()){
			return iIdx;
		}
	}

	return -1;
}

bool CQStageEntity::checkIsDied(){
	if(_hp <= 0){
		return true;	
	}else{
		return false;
	}
}

void CQStageEntity::HandleBeatenEffActionEvent(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}	
}

void CQStageEntity::Pause(){
	if (getArmature()){
		getArmature()->getAnimation()->pause();
		pauseSchedulerAndActions();
	}
	
}

void CQStageEntity::Resume(){
	if (getArmature()){
		getArmature()->getAnimation()->resume();		
		resumeSchedulerAndActions();
	}
}

void CQStageEntity::Stop(){
	if (getArmature()){
		getArmature()->getAnimation()->stop();		
		resumeSchedulerAndActions();
	}
}

int CQStageEntity::InitSkills(){
	_damageRange = 1000;
	_attakRange = 2000;
	CCObject* pstObj;
	CCARRAY_FOREACH(_skillArray, pstObj){
		
		CQSkillCell* pstSkill = dynamic_cast<CQSkillCell*>(pstObj);
		const SkillCfg* pstCfg = SKILL_CFG_MASTER->GetSkillCfgByID(pstSkill->m_uSkillID);
		if (pstCfg){
			pstSkill->m_stCfg = *pstCfg;
			pstSkill->m_stIDToDamgeCnt.clear();

			if (_attakRange > pstCfg->castdist()){
				_attakRange = pstCfg->castdist();
			}

			if (_damageRange > pstCfg->damagezone()){
				_damageRange = pstCfg->damagezone();
			}
		}else{
			CCLOG("unknown skill id %d\n", pstSkill->m_uSkillID);
			return -1;
		}
	}

	return 0;
}


bool CQStageEntity::IsEnimy(unsigned int uMyParty){
	return (uMyParty != getRoleParty() && getRoleParty() != ROLE_PARTY_NEUTRAL);
}

bool CQStageEntity::IsFriend(unsigned int uMyParty){
	return (uMyParty == getRoleParty() && getRoleParty() != ROLE_PARTY_NEUTRAL);
}

unsigned int CQStageEntity::GetArmorType() const{

	return _armorType;
}

unsigned int CQStageEntity::GetAtkMagicType()const{
	return _attackMagicType;
}

unsigned int CQStageEntity::GetAtkType() const{
	return _attackType;
}

unsigned int CQStageEntity::GetMagicAtkRatio(unsigned int uMagicType) const{
	if (uMagicType >= MAX_MAGIC_CNT){
		return 1;
	}

	return m_stMagicAtkRatio[uMagicType];
}

unsigned int CQStageEntity::GetMagicDefRatio(unsigned int uMagicType) const{
	if (uMagicType >= MAX_MAGIC_CNT){
		return 1;
	}

	return m_stMagicDefRatio[uMagicType];
}

void CQStageEntity::SetMagicDefRatio(const uint32 def[]){
	for (int iIdx = 0; iIdx < MAX_MAGIC_CNT; iIdx++){
		m_stMagicDefRatio[iIdx] = def[iIdx];
	}
}

void CQStageEntity::SetMagicAtkRatio(const uint32 atk[]){
	for (int iIdx = 0; iIdx < MAX_MAGIC_CNT; iIdx++){
		m_stMagicAtkRatio[iIdx] = atk[iIdx];
	}
}

void CQStageEntity::AddMagicAtkRatio(unsigned int uMagic, unsigned int uVal){
	if (uMagic >= MAX_MAGIC_CNT){
		return;
	}

	m_stMagicAtkRatio[uMagic] = int((RANDOM_DOMINATOR + uVal * 1.0)* m_stMagicAtkRatio[uMagic] / RANDOM_DOMINATOR);
	if (m_stMagicAtkRatio[uMagic] > 200 * RANDOM_DOMINATOR){
		m_stMagicAtkRatio[uMagic] = 200 * RANDOM_DOMINATOR;
	}
}

void CQStageEntity::DecMagicAtkRatio(unsigned int uMagic, unsigned int uVal){
	m_stMagicAtkRatio[uMagic] = int((RANDOM_DOMINATOR - uVal * 1.0)* m_stMagicAtkRatio[uMagic] / RANDOM_DOMINATOR);
	if (m_stMagicAtkRatio[uMagic] < 0){
		m_stMagicAtkRatio[uMagic] = 0;
	}
}

void CQStageEntity::AddMagicDefRatio(unsigned int uMagic, unsigned int uVal){
	if (uMagic >= MAX_MAGIC_CNT){
		return;
	}

	m_stMagicDefRatio[uMagic] = int((RANDOM_DOMINATOR + uVal * 1.0)* m_stMagicDefRatio[uMagic] / RANDOM_DOMINATOR);
	if (m_stMagicDefRatio[uMagic] > 200 * RANDOM_DOMINATOR){
		m_stMagicDefRatio[uMagic] = 200 * RANDOM_DOMINATOR;
	}
}

void CQStageEntity::DecMagicDefRatio(unsigned int uMagic, unsigned int uVal){
	m_stMagicDefRatio[uMagic] = int((RANDOM_DOMINATOR - uVal * 1.0)* m_stMagicDefRatio[uMagic] / RANDOM_DOMINATOR);
	if (m_stMagicDefRatio[uMagic] < 0){
		m_stMagicDefRatio[uMagic] = 0;
	}
}

void CQStageEntity::AddAtkRatio(unsigned int uVal){
	m_uEffAtk = int((RANDOM_DOMINATOR + uVal * 1.0) * m_uEffAtk / RANDOM_DOMINATOR);
}

void CQStageEntity::DecAtkRatio(unsigned int uVal){
	m_uEffAtk = uint64((RANDOM_DOMINATOR - uVal * 1.0) * m_uEffAtk / RANDOM_DOMINATOR);
	if (m_uEffAtk < 0){
		m_uEffAtk = 0;
	}
}

void CQStageEntity::AddDefRatio(unsigned int uVal){
	m_uEffDef = int((RANDOM_DOMINATOR + uVal * 1.0) * m_uEffDef / RANDOM_DOMINATOR);
}

void CQStageEntity::DecDefRatio(unsigned int uVal){
	m_uEffDef = int((RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR * m_uEffDef);
	if (m_uEffDef < 0){
		m_uEffDef = 0;
	}
}

void CQStageEntity::AddHpMaxByRatio(unsigned int uVal){
	_maxHp = (UINT64_t)(_maxHp * (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR);
}

void CQStageEntity::DecHpMaxByRatio(unsigned int uVal){
	_maxHp = (UINT64_t)(_maxHp * (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR);
}

void CQStageEntity::AddDashSpeedByRatio(unsigned int uVal){
	_dashSpeed = (int)(_dashSpeed * (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR);
	if (getMyState() == kActionStateBackDash || getMyState() == kActionStateDash){
		_velocity.x *= (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR;
		_velocity.y *= (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR;
	}
}

void CQStageEntity::DecDashSpeedByRatio(unsigned int uVal){
	_dashSpeed = (int)(_dashSpeed * (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR);
	if (getMyState() == kActionStateBackDash || getMyState() == kActionStateDash){
		_velocity.x *= (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR;
		_velocity.y *= (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR;
	}
}

void CQStageEntity::AddWalkSpeedByRatio(unsigned int uVal){
	_walkSpeed = (int)(_walkSpeed * (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR + 0.5);
	_backSpeed = (int)(_backSpeed * (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR + 0.5);
	if (getMyState() != kActionStateBackDash
		&& getMyState() != kActionStateDash){
		_velocity.x *= (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR;
		_velocity.y *= (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR;
	}
}

void CQStageEntity::DecWalkSpeedByRatio(unsigned int uVal){
	_walkSpeed = (int)(_walkSpeed * (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR + 0.5);
	_backSpeed = (int)(_backSpeed * (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR + 0.5);
	if (getMyState() != kActionStateBackDash
		&& getMyState() != kActionStateDash){
		_velocity.x *= (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR;
		_velocity.y *= (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR;
	}
}

void CQStageEntity::AddBulletDepth(unsigned int uVal){
	m_uBulletDepthRatio = (int)(m_uBulletDepthRatio * (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR);
}

void CQStageEntity::DecBulletDepth(unsigned int uVal){
	m_uBulletDepthRatio = (int)(m_uBulletDepthRatio * (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR);
}

void CQStageEntity::AddBulletLight(unsigned int uVal){
	m_uBulletLightRatio = (int)(m_uBulletLightRatio * (RANDOM_DOMINATOR + uVal * 1.0) / RANDOM_DOMINATOR);
}

void CQStageEntity::DecBulletLight(unsigned int uVal){
	m_uBulletLightRatio = (int)(m_uBulletLightRatio * (RANDOM_DOMINATOR - uVal * 1.0) / RANDOM_DOMINATOR);
}

void CQStageEntity::AddBuff(unsigned int uiBuffID, unsigned int uOwnerUID, bool bIsAddArm/* = true*/){
	//替换规则
	const BuffCfg* pstConfig = BUFF_CFG_MASTER->GetBuffCfgByID(uiBuffID);
	if (!pstConfig){
		CV_ERROR(("invalid buffid %d, owner id === %u", uiBuffID, uOwnerUID));
		return;
	}

	bool bIsInvicible = false;
	for (int i = 0; i < pstConfig->effects_size(); i++){
		if (pstConfig->effects(i).effecttype() == BUFF_INVINCIBLE){
			bIsInvicible = true;
			break;
		}
	}
	int iIdx = 0;

	//检查是否能被加上或者顶替原有的某个buff
	if (!bIsInvicible){
		for (; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
			if (m_stBuffs[iIdx] == NULL){
				break; 
			}		

			bool bIsTheSameType = true;
			int iCmpCnt = 0;
			const BuffCfg* pstOrigBufCfg = BUFF_CFG_MASTER->GetBuffCfgByID(m_stBuffs[iIdx]->m_uiBuffID);

			for (int j = 0; j < pstConfig->effects_size(); j++){
				if (m_stBuffs[iIdx]->m_uiEffType[j] != pstConfig->effects(j).effecttype()){
					bIsTheSameType = false;
					break;
				}
				iCmpCnt ++;
			}

			if (iCmpCnt == pstOrigBufCfg->effects_size() && bIsTheSameType){
				bIsTheSameType = true;
			}else{
				bIsTheSameType = false;
			}

			if (bIsTheSameType){
				for (int j = 0; j < pstConfig->effects_size(); j++){
					//添加效果
					CInstanceUtils::RemoveEffect(this, pstConfig->effects(j).effecttype(), pstConfig->effects(j).value(), 
						pstConfig->effects(j).magictype(), pstConfig->addordec() == BUFF_ADD_EFF);
					
				}
				m_stBuffs[iIdx]->removeFromParentAndCleanup(true);
				m_stBuffs[iIdx] = NULL;
				break;
			}

		}
	}else{
		for (; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
			if (m_stBuffs[iIdx] == NULL){
				break; 
			}			

			//无敌buff顶替
			if (!m_stBuffs[iIdx]->IsPositiveBuff() || m_stBuffs[iIdx]->IsInvincible()){
				RemoveBuff(m_stBuffs[iIdx]->m_uiBuffID, m_stBuffs[iIdx]->getOwnerID());				
				iIdx --;
			}
		}

		for (iIdx = 0; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
			if (m_stBuffs[iIdx] == NULL || m_stBuffs[iIdx]->IsInvincible()){
				break; 
			}
		}

		if (iIdx == ROLE_MAX_BUFF_CNT){
			iIdx = 0;
		}
	}
	

	if (iIdx < ROLE_MAX_BUFF_CNT){
		
		CBuff* pstBuff = CBuff::create(uOwnerUID);
		pstBuff->retain();
		m_stBuffs[iIdx] = pstBuff;
		m_stBuffs[iIdx]->m_uiBuffID = uiBuffID;
		
		m_stBuffs[iIdx]->m_uiLastCheckTime = 0;
		m_stBuffs[iIdx]->m_uiBuffDeadline = pstConfig->livespan() + m_dObjectLife;
		
		m_stBuffs[iIdx]->m_uiAddOrDec = pstConfig->addordec();
		if (strcmp(pstConfig->animation().c_str(), "null") && pstConfig->animation().length()){
            //是否添加buff动画
			if(bIsAddArm){
				CCArmature* pArm = CCArmature::create(pstConfig->animation().c_str());
				if(pArm) {
					m_stBuffs[iIdx]->SetArmature(pArm);
				} else {
					CCLOG("[error]CQStageEntity::AddBuff, fail to create armature %s", pstConfig->animation().c_str());
				}
			}
		}else{
			CCLOG("[error]CQStageEntity::AddBuff, the armature resource is null");
		}
		
		m_stBuffs[iIdx]->m_uiBuffEffDisplayPos = pstConfig->displaypos();
		for (int j = 0; j < pstConfig->effects_size(); j++){
			//添加效果
			CInstanceUtils::AddEffect(this, pstConfig->effects(j).effecttype(), pstConfig->effects(j).value(), 
				pstConfig->effects(j).magictype(), pstConfig->addordec() == BUFF_ADD_EFF);
			m_stBuffs[iIdx]->m_uiVal[j] = pstConfig->effects(j).value();
			m_stBuffs[iIdx]->m_uiEffType[j] = pstConfig->effects(j).effecttype();
		}

		if (m_stBuffs[iIdx]->GetArmature()){
			 //把这个buff加在角色身上，以100n 为tag，便于快速索引
			//m_stBuffs[iIdx].GetArmature()->setVisible(false);
			m_stBuffs[iIdx]->Play();
			switch (m_stBuffs[iIdx]->m_uiBuffEffDisplayPos)
			{
			case BUFF_POS_HEAD:
				m_stBuffs[iIdx]->setPosition(ccp(0, getCenterToBottom() * 2));
				addChild(m_stBuffs[iIdx], 1000);
				break;
			case BUFF_POS_CENTER:
				m_stBuffs[iIdx]->setPosition(ccp(0, getCenterToBottom()));
				addChild(m_stBuffs[iIdx], 1000);
				break;
			case BUFF_POS_FOOT:
				m_stBuffs[iIdx]->setPosition(CCPointZero);
				addChild(m_stBuffs[iIdx], 200);			//策划说改到身前的，有问题找策划 2014-8-14
				break;
			default:
				break;
			}			
		}
		

		//在显示对象上添加buff特效？
	}

	//CCLOG("add buff %u to owner id === %u", uiBuffID, uOwnerUID);
}

void CQStageEntity::RemoveBuff(unsigned int uiBuffID, unsigned int uOwnerUID){
	for (int iIdx = 0; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
		if (m_stBuffs[iIdx] == NULL){
			return;
		}

		if (m_stBuffs[iIdx]->m_uiBuffID == uiBuffID && m_stBuffs[iIdx]->getOwnerID() == uOwnerUID){
			
			for (int k = 0; k < BUFF_MAX_EFFECTS_IN_ONE; k++){
				if (m_stBuffs[iIdx]->m_uiEffType[k] != 0){
					//CCLog("value ===== remove buff value %d from owner id === %u", uiBuffID, uOwnerUID);
					CInstanceUtils::RemoveEffect(this, m_stBuffs[iIdx]->m_uiEffType[k], m_stBuffs[iIdx]->m_uiVal[k], 
						m_stBuffs[iIdx]->m_uiMagicType[k], m_stBuffs[iIdx]->m_uiAddOrDec == BUFF_ADD_EFF);
				}else{
					break;
				}		
			}
			
			//消除显示对象上的特效
			if (m_stBuffs[iIdx]->GetArmature()){
				//m_stBuffs[iIdx]->StopPlay();
				//CCLog("effect ==== remove buff effect %d from owner id === %u", uiBuffID, uOwnerUID);
				m_stBuffs[iIdx]->removeFromParentAndCleanup(true);				
			}

			//CCLog("remove buff %d from %p", uiBuffID, this);
			CC_SAFE_RELEASE_NULL(m_stBuffs[iIdx]);

			int j = iIdx;
			for (; j < ROLE_MAX_BUFF_CNT - 1; j++){
				if (m_stBuffs[j + 1] == NULL){
					break;
				}
				m_stBuffs[j] = m_stBuffs[j + 1];
				m_stBuffs[j + 1] = NULL;
			}
			
			//CCLOG("remove buff %u to owner id === %u", uiBuffID, uOwnerUID);
			return;
		}
	}
}

bool CQStageEntity::CheckHasBuff(unsigned int uiBuffID, unsigned int uOwnerUID){
	bool hasBuff = false;
	

	for (int iIdx = 0; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
		if (m_stBuffs[iIdx] == NULL){
			return false;
		}
		if (m_stBuffs[iIdx]->m_uiBuffID == uiBuffID && m_stBuffs[iIdx]->getOwnerID() == uOwnerUID){
			hasBuff = true;
			break;
		}
	}

	return hasBuff;
}

void CQStageEntity::RefreshBuffs(){
	//根据当前时间的尾数，来设置应该显示哪个buff
	
	if (m_dLastCheckTime > m_dObjectLife - 0.1){
		return;
	}

	time_t tNow = GlobalTime::getTime();
	int iBuffCnt = 0;
	for (int iIdx = 0; iIdx < ROLE_MAX_BUFF_CNT; iIdx++){
		if (m_stBuffs[iIdx] == NULL || m_stBuffs[iIdx]->m_uiBuffID == 0){
			break;;
		}
		

		if (m_stBuffs[iIdx]->m_uiBuffDeadline <= m_dObjectLife){
			RemoveBuff(m_stBuffs[iIdx]->m_uiBuffID, m_stBuffs[iIdx]->getOwnerID());
			iIdx --;
			continue;
		}

		if (m_stBuffs[iIdx]->m_uiLastCheckTime < m_dObjectLife){
			bool bIsAdd = m_stBuffs[iIdx]->m_uiAddOrDec == BUFF_ADD_EFF;
			for (int k = 0; k < BUFF_MAX_EFFECTS_IN_ONE; k++){
				if (m_stBuffs[iIdx]->m_uiEffType[k] != 0){
					if (m_stBuffs[iIdx]->m_uiEffType[k] == BUFF_CONT_CUR_HP_BY_RATIO){
						AddHP(UINT64_t(_hp * m_stBuffs[iIdx]->m_uiVal[k] * 1.0 / RANDOM_DOMINATOR), bIsAdd);
					}else if (m_stBuffs[iIdx]->m_uiEffType[k] == BUFF_CONT_MAX_HP_BY_RATIO){
						AddHP(UINT64_t(_maxHp * m_stBuffs[iIdx]->m_uiVal[k] * 1.0 / RANDOM_DOMINATOR), bIsAdd);
					}else if (m_stBuffs[iIdx]->m_uiEffType[k] == BUFF_CONT_CUR_HP_BY_ABS){
						AddHP(m_stBuffs[iIdx]->m_uiVal[k], bIsAdd);
					}
				}
			}
			
			m_stBuffs[iIdx]->m_uiLastCheckTime = ceil(m_dObjectLife);
		}
		iBuffCnt ++;
	}

	for (int iIdx = 0; iIdx < iBuffCnt; iIdx++){
// 		int iMod = ((int)(m_dObjectLife * 10)) % iBuffCnt;
// 		if (iMod == iIdx){
// 			if (m_stBuffs[iIdx].GetArmature()){
// 				m_stBuffs[iIdx].GetArmature()->setVisible(true);
// 			}
// 			
// 		}else{
// 			if (m_stBuffs[iIdx].GetArmature()){
// 				m_stBuffs[iIdx].GetArmature()->setVisible(false);
// 			}
// 		}
	}

	m_dLastCheckTime = m_dObjectLife;
}


void CQStageEntity::AddHP(UINT64_t uiHP, bool bIsAdd){
	if (bIsAdd){
		UINT64_t HPUp = uiHP;
		if((_hp + uiHP) > _maxHp){
			HPUp = _maxHp - _hp;
		}

		_hp += uiHP;

		ShowBloodNum(HPUp, true);
	}else{
		HurtWithDamage(uiHP);
	}

	if (_hp > _maxHp){
		_hp = _maxHp;
	}

	if (_hp < 0){
		_hp = 0;
	}

	//调用加血的演示特效


	HPChange();
}

double CQStageEntity::GetShootInterval() const{
	if (m_pstWeaponCfg == NULL){
		return 0.2;
	}else if (m_uiBullteTrailType == TRA_CON_HORI_LINE || m_uiBullteTrailType == TRA_JTL_LINE){
		return 0.0;
	}else{
		return 1270.0 / (m_pstWeaponCfg->shotspeed() + _roleAtkSpeed);
	}
}
unsigned int CQStageEntity::GetBulletID()		//子弹ID
{
	if (m_pstWeaponCfg == NULL){
		return 1;
	}else{
		return m_pstWeaponCfg->bulletid();
	}
}
unsigned int CQStageEntity::GetShootSpeed()		//子弹射速
{
	if (m_pstWeaponCfg == NULL){
		return 10;
	}else{
		return m_pstWeaponCfg->shotspeed();
	}
	
}
unsigned int CQStageEntity::GetBulletShootDistance() //子弹射程
{
	if (m_pstWeaponCfg == NULL){
		return 100;
	}else{
		return m_pstWeaponCfg->shotrange();
	}
}
CCPoint CQStageEntity::GetBulletPosition() //子弹起始位置
{
	return GetFirePosition();
}
double CQStageEntity::GetZAlias()   //获得子弹的Z轴
{
	return GetFootPos().y;
}
bool CQStageEntity::IsInThrash()    //当前角色是否在暴走状态
{
	return false;
}

bool CQStageEntity::isShooting() const{
	return false;
}

void CQStageEntity::WalkByPath(const vector<CCPoint>& stPathLst)
{
    if (stPathLst.size() <= 0 || !IsWalkable())
    {
        return;
    }
    m_stPathPtLst = stPathLst;
    setMyState(kActionStateWalkToDst);
	if (getArmature()->getAnimation()->getCurrentMovementID() != "run"){

		getArmature()->getAnimation()->play("run");
		OtherWalk();
	}
}

void CQStageEntity::HPChange(){

}


void CQStageEntity::setObj(CQStageEntityObj* var){
	_obj = var;
	_obj->retain();
	_hp = _obj->hp;
	_maxHp = _obj->hp;
	if(QEnvSet::beck()){
		_ep = 10000;
	}else{
		_ep = 0;  //_obj->ep; //策划让修改为0
	}
	_maxEp = _obj->ep; 
	_walkSpeed = _obj->walkSpeed;
	_dashSpeed = _obj->runSpeed;
	_backSpeed = _obj->backSpeed;
	m_uEffAtk = _obj->atk;
	_roleAtkSpeed = _obj->atkSpeed;
	_toatlAtkSpeed = _obj->totalAtkSpeed;
	m_uEffDef = _obj->def;
	_attackType = _obj->atkType;
	_armorType = _obj->defType;
	_roleParty = _obj->roleParty;
	_roleType = _obj->roleType;
	_dieResPath = _obj->dieResPath;
	_stEmergeVoice = _obj->emergeVoice;
	_stHurtVoice = _obj->hurtVoice;
	_stDieVoice = _obj->dieVoice;
	m_iBackDashDist = _obj->dodgeDist;
	m_iBackDashCD = _obj->dodgeCD;
	_hurtColor = _obj->hurtColor;
	_hit = _obj->hit;
	m_uInitScale = _obj->initScale;
	_isBlock = _obj->isBlock;
	_attackMagicType = _obj->atkAttr.attrtype();
	SetMagicAtkRatio(_obj->stMagicAtkRatio);
	SetMagicDefRatio(_obj->stMagicDefRatio);

	//动作名
	m_stMovements = _obj->stMovements;

	if (_obj->weaponCfgID){
		_weaponCfgID = _obj->weaponCfgID;
		m_pstWeaponCfg = WEAPON_CFG_MASTER->GetMutableWeaponCfgByID(_obj->weaponCfgID);
		if (!m_pstWeaponCfg){
			assert(m_pstWeaponCfg != NULL);
		}
		const BulletCfg* pstBulletConfig = BULLET_CFG_MASTER->GetBulletCfgByID(m_pstWeaponCfg->bulletid());
		m_uiBullteTrailType = pstBulletConfig->bullettype();
		m_uiBulletBurstCnt =  pstBulletConfig->burstcnt();
		m_fBulletTime = m_pstWeaponCfg->bulletval()*1.0f/1000.0f;
		m_uiBulletCurCnt = 0;
	}

	updateData(_hp, m_uHpMd5);
}

CQStageEntityObj* CQStageEntity::getObj(){
	return _obj;
}

void CQStageEntity::setHP(UINT64_t var){
	_hp = var;

	updateData(_hp, m_uHpMd5);
}

UINT64_t CQStageEntity::getHP(){
	return _hp;
}

bool CQStageEntity::checkData(UINT64_t uPreData, UINT64_t uMd5Data){
	if(uPreData + m_uRandData == uMd5Data){
		return true;
	}

	return false;
}

void CQStageEntity::updateData(UINT64_t uPreData, UINT64_t& uMd5Data){
	uMd5Data = uPreData + m_uRandData;
}

void CQStageEntity::resumeData(UINT64_t& uPreData, UINT64_t uMd5Data){
	uPreData = uMd5Data - m_uRandData;
}

int CQStageEntity::GetBulletPenatrate(){
	if (m_pstWeaponCfg){
		return m_pstWeaponCfg->penetrateval();
	}
	return 1;
}

int CQStageEntity::GetBulletAttenuation(){
	
	return RANDOM_DOMINATOR;
}

void CQStageEntity::RollBackPosition(bool bIsIdle){
	setPosition(m_stPrePos);
	if(!bIsIdle){
		return;
	}

	if (getMyState() == kActionStateWalk || getMyState() == kActionStateWander || getMyState() == kActionStateWalkToDst){
		Idle();
	}
}


void CQStageEntity::OtherAttack(){

}

void CQStageEntity::OtherAttackCounterMarch(){

}

void CQStageEntity::OtherAttackDash(){

}

void CQStageEntity::OtherAttackWalk(){

}

void CQStageEntity::OtherAttackWithSkill(const CQSkillCell* pstSkill){

}

void CQStageEntity::OtherCounterMarch(){

}

void CQStageEntity::OtherDash(){

}

void CQStageEntity::OtherDie(){

}

void CQStageEntity::OtherHurt(){

}

void CQStageEntity::OtherIdle(){

}


void CQStageEntity::OtherWalk(){

}

void CQStageEntity::LoadOtherRes(){

}

void CQStageEntity::SwitchRes(const char* pstActionName){

}

void CQStageEntity::OtherDizzy(){

}

CCPoint CQStageEntity::GetFirePosition() {
	if (!mArmature->getBone("firepoint")){
		return GetCenterPos();
	}else{
		float x = mArmature->getBone("firepoint")->getBoneData()->x * mArmature->getScaleX() + getPositionX();
		float y = mArmature->getBone("firepoint")->getBoneData()->y * mArmature->getScaleY() + getPositionY();
		return ccp(x, y);
	}
}

void CQStageEntity::BackDash(){
	getArmature()->getAnimation()->setSpeedScale(1.0);
	int iFrameCnt = 0;
	if(getMyState() != kActionStateBackDash){
		setMyState(kActionStateBackDash);

		float fAngel = ccpToAngle(_velocity);
		float xBackDist = abs(m_iBackDashDist*cos(fAngel));
		float yBackDist = abs(m_iBackDashDist*sin(fAngel));

		float fSymbolic = 1.0f;
		if(_velocity.x < 0){  //向左 or 没有水平位移
			fSymbolic = -1.0f;
		}else if(_velocity.x > 0){
			fSymbolic = 1.0f;
		}else{
			if(getScaleX() > 0){
				fSymbolic = -1.0f;
			}else{
				fSymbolic = 1.0f;
			}
		}

		string bachStr = "back_dash";
		if(fSymbolic*getScaleX() < 0){
			bachStr = "back_dash";
		}else{
			bachStr = "walk_dash";
		}


		float fDashTime = 0.5;
		//float fInterval = CCDirector::sharedDirector()->getAnimationInterval();
		float fInterval = 1/60.0f;

		if(mArmature->getAnimation()->getAnimationData()->getMovement(bachStr.c_str())){
			mArmature->getAnimation()->play(bachStr.c_str(), -1, -1, 0);
			iFrameCnt = mArmature->getAnimation()->getRawDuration();
			fDashTime = mArmature->getAnimation()->getAnimationData()->getMovement(bachStr.c_str())->duration*fInterval / (mArmature->getAnimation()->getAnimationData()->getMovement(bachStr.c_str())->scale);
		}else{
			if(mArmature->getAnimation()->getAnimationData()->getMovement("back_dash")){
				mArmature->getAnimation()->play("back_dash", -1, -1, 0);
				iFrameCnt = mArmature->getAnimation()->getRawDuration();
				fDashTime = mArmature->getAnimation()->getAnimationData()->getMovement("back_dash")->duration*fInterval / (mArmature->getAnimation()->getAnimationData()->getMovement("back_dash")->scale);
			}else{
				return;
			}
		}

		//CCLOG("before main hero back dash the x === %f", getPositionX());

		float x = getPositionX();
		x += fSymbolic*xBackDist;
		_velocity.x = (fSymbolic * xBackDist) / fDashTime;

		if(_velocity.y < 0){ //向下
			fSymbolic = -1.0f;
		}else if(_velocity.y > 0){
			fSymbolic = 1.0f;
		}else{
			fSymbolic = 0;
		}

		float y = getPositionY();
		y += fSymbolic*yBackDist;
		_velocity.y = (fSymbolic * yBackDist) / fDashTime;

		m_stDashDstPos = ccp(x, y);
	}

	OtherBackDash();

	//播放音效
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	const HeroCfg* stHeroCfg = HERO_CFG_MASTER->GetHeroCfgByID(pUserInfo->showID);
	if (stHeroCfg->dodgesound().length() && strcmp(stHeroCfg->dodgesound().c_str(), "null")){
		AudioManager::playEffect(stHeroCfg->dodgesound().c_str());
	}

	m_fNextDashableTime = m_iBackDashCD;
	CCString* strValue = CCString::createWithFormat("%d", 1);
	DispatchEvent(SKILL_CAST_IN_ACTION, strValue);
}

void CQStageEntity::OtherBackDash(){

}

void CQStageEntity::ForwardDash(float distance){
	CCLOG("CQStageEntity::ForwardDash the dis === %f, the pos ==== %f, and the armature is %s", distance, this->GetFootPos().x, mArmature->getName().c_str());

	getArmature()->getAnimation()->setSpeedScale(1.0);
	if(getMyState() != kActionStateForwardDash){
		setMyState(kActionStateForwardDash);
		this->stopAllActions();
		if (mArmature->getAnimation()->getAnimationData()->getMovement("dash")){
			mArmature->getAnimation()->play("dash", -1, -1, 1);
		}

		float fSymbolic = 1.0f;
		if(getScaleX() > 0){
			fSymbolic = 1.0f;
		}else{
			fSymbolic = -1.0f;
		}

		float x = getPositionX();
		x += fSymbolic*distance;
		m_stForDashDstPos = ccp(x, getPositionY());

		_velocity.x = 3*fSymbolic*_walkSpeed;
		_velocity.y = 0;
	}
}

void CQStageEntity::CheckRollBack(const CCRect& stWalkRect){
	bool bIsBack = false;
	float backX = getPositionX();
	float backY = getPositionY();
	if(this->getPositionX() < stWalkRect.getMinX()){
		backX = stWalkRect.getMinX() + 10;
		bIsBack = true;
	}else if(this->getPositionX() > stWalkRect.getMaxX()){
		backX = stWalkRect.getMaxX() - 10;
		bIsBack = true;
	}

	if(this->getPositionY() < stWalkRect.getMinY()){
		backY = stWalkRect.getMinY() + 10;
		bIsBack = true;
	}else if(this->getPositionY() > stWalkRect.getMaxY()){
		backY = stWalkRect.getMaxY() - 10;
		bIsBack = true;
	}

	if(bIsBack){
		setPosition(ccp(backX, backY));
		ForceIdle();
	}
}

void CQStageEntity::JumpAtkWithAction(CCPoint stPos, const char* stAct){
	getArmature()->getAnimation()->setSpeedScale(1.0);
	if(getMyState() != kActionStateJumpAtk){
		setMyState(kActionStateJumpAtk);

		float fDashTime = 0.5;
		//float fInterval = CCDirector::sharedDirector()->getAnimationInterval();
		float fInterval = 1/60.0f;
		CCMovementData* stMoveData = mArmature->getAnimation()->getAnimationData()->getMovement(stAct);
		if(stMoveData){
			int startFrameIdx = CAnimationUtils::getFrameDataByEvent(mArmature, stAct, "skill_cast_eff")->frameID;  //跳起
			int endFrameIdx = CAnimationUtils::getFrameDataByEvent(mArmature, stAct, "jump_end_key")->frameID;		//落下
			fDashTime = (endFrameIdx - startFrameIdx)*fInterval/(stMoveData->scale);
		}else{
			CCLOG("[error::]CQStageEntity::JumpAtkWithAction::the QStageEntity has no %s action", stAct);
			return;
		}

		float fSymbolicX = 1.0f;
		if(getScaleX() > 0){
			fSymbolicX = 1.0f;
		}else{
			fSymbolicX = -1.0f;
		}

		float x = getPositionX();
		x += fSymbolicX * fabs(stPos.x);

		float y = getPositionY();
		y -= stPos.y;

		_velocity.x = (fSymbolicX * fabs(stPos.x)) / fDashTime;
		_velocity.y = (-stPos.y) / fDashTime;;

		m_stJumpAtkDstPos = ccp(x, y);
	}
}

bool CQStageEntity::isBackDashable() const{
	return (m_fNextDashableTime <= 0);
}

void CQStageEntity::Blind(){
	if (!IsBlindable()){
		return;
	}
	//目前没有其他要做的动作
	setMyState(kActionStateBlind);
	OtherBlind();
}

void CQStageEntity::OtherBlind(){

}


void CQStageEntity::Kick(){
	mArmature->getAnimation()->play("kick", -1, -1, 0);
	setMyState(kActionStateKick);
	OtherKick();
	//CCLog("kicks %p", this);
}


void CQStageEntity::OtherKick(){

}

void CQStageEntity::Immobilize(){
	if (!IsImmobilizible()){
		return;
	}
	Idle();
	//目前没有其他要做的动作
	setMyState(kActionStateImmobilize);
}

void CQStageEntity::OtherImmobilize(){


}


void CQStageEntity::MovementCallbackToRelease(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->stopAllActions();
		armature->removeFromParentAndCleanup(true);
	}
}

CCPoint CQStageEntity::GetSkillGunPoint(const string& stSkillAction){
	int iSkillActionIdx = 0;
	if (stSkillAction.find("1") != string::npos){
		iSkillActionIdx = 1;
	}else if (stSkillAction.find("2") != string::npos){
		iSkillActionIdx = 1;
	}
	char acBuf[32];

	if (iSkillActionIdx != 0){
		sprintf(&acBuf[0], "skill%d_gunpoint",iSkillActionIdx);		
	}

	if (mArmature->getBone(&acBuf[0])){
		return mArmature->getBone(&acBuf[0])->getPosition()*m_fBaseScale;
	}else{
		float x, y;
		string stCurAction = getArmature()->getAnimation()->getCurrentMovementID();

        if(stCurAction != ""){
            CCMovementBoneData* pstBone = getArmature()->getAnimation()->
                getAnimationData()->getMovement(stCurAction.c_str())->getMovementBoneData("skill_gunpoint");
            if (pstBone){
                x = pstBone->getFrameData(0)->x + mArmature->getBone("skill_gunpoint")->getBoneData()->x;
                y = pstBone->getFrameData(0)->y + mArmature->getBone("skill_gunpoint")->getBoneData()->y;
				x = x*mArmature->getScaleX();
				y = y*mArmature->getScaleY();
            }else{
                x = mArmature->getBone("skill_gunpoint")->getBoneData()->x * mArmature->getScaleX();
                y = mArmature->getBone("skill_gunpoint")->getBoneData()->y * mArmature->getScaleY();
            }
        }else{
            x = mArmature->getBone("skill_gunpoint")->getBoneData()->x * mArmature->getScaleX();
            y = mArmature->getBone("skill_gunpoint")->getBoneData()->y * mArmature->getScaleY();
        }
		return ccp(x, y)*m_fBaseScale;
	}
}

void CQStageEntity::setShadowVisiable(bool var){
	CCNode* pNode = getChildByTag(SHADOW_TAG);
	if(pNode != NULL){
		pNode->setVisible(var);
	}
}

void CQStageEntity::SetBulletDamageAdj(unsigned int iDamage){
	m_iBulletDamageAdj = iDamage;
}

void CQStageEntity::UnSetBulleteDamageAdj(){
	m_iBulletDamageAdj = 0;
}

void CQStageEntity::resetScale(){
	m_fTransformScale = 1.0f;
	if(getScaleX() != 1.0){
		setScaleX(1.0f);
	}

	if(getScaleY() != 1.0){
		setScaleY(1.0f);
	}
}

