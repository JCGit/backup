#include "SkillEffLayer.h"
#include "Macros.pb.h"
#include "SkillCfgMaster.h"
#include "EffectSoundCfgMaster.h"
#include "game/control/AudioManager.h"

using namespace cocos2d;

CSkillEffLayer::CSkillEffLayer():
	_bShouldClear(false),
	m_bIsArrive(false),
	_flyState(SKILL_ANIMATION_STATE_NULL),
	_validDeadline(0),
	_velocity(CCPointZero),
	_acclereration(CCPointZero),
	_currentFrameIdx(0)
{
	_armature = NULL;
	m_stIDToDamgeCnt.clear();
    m_stCfg.set_triggertype(SKILL_TRIGGER_DIRECT_FOLLOW);
	m_fBasicScale = 1.5;
}

CSkillEffLayer::~CSkillEffLayer(){
	
}

const string CSkillEffLayer::HIT_KEY_FRAME_EVENT = "hit_key";
const string CSkillEffLayer::SPAWN_KEY_FRAME_EVENT = "spawn_key";
const string CSkillEffLayer::PLAY_SOUND_KEY_FRAME_EVENT = "play_sound_key";
const string CSkillEffLayer::WARNING_COMPLETE_EVENT = "warning_complete_event";
const string CSkillEffLayer::SHAKE_SCREEN_KEY_FRAME = "shake_screen";				//震屏帧事件

void CSkillEffLayer::onExit(){
	unscheduleUpdate();
	this->RemoveEventListener(this, PLAY_SOUND_KEY_FRAME_EVENT, eventListner_selector(CSkillEffLayer::HandlePlaySoundEvent));
	removeAllChildrenWithCleanup(true);
	CCNode::onExit();
}

void CSkillEffLayer::setScaleX(float fScaleX){
	if(_armature){
		_armature->setScaleX(fScaleX);
	}
}

float CSkillEffLayer::getScaleX(){
	if(_armature){
		return _armature->getScaleX();
	}else{
		return 1;
	}
}

void CSkillEffLayer::setScaleY(float fScaleY){
	if(_armature){
		_armature->setScaleY(fScaleY);
	}
}

float CSkillEffLayer::getScaleY(){
	if(_armature){
		return _armature->getScaleY();
	}else{
		return 1;
	}
}

void CSkillEffLayer::FlyToDir(CCPoint stVec){
	CCPoint stDir = ccpNormalize(stVec);
	_velocity = ccp(stDir.x * m_stCfg.flyspeed(), stDir.y * m_stCfg.flyspeed());
	Fly(true);
}

void CSkillEffLayer::FlyToPos(CCPoint stDst){
	CCPoint stVec = ccpSub(stDst, getPosition());
	return FlyToDir(stVec);
}

void CSkillEffLayer::FlyToPosWithDir(CCPoint stDst){
	CCPoint stRealPos;
	float xx;
	float yy;
	CCLOG("the scale is %f", getScaleX());
	if(getScaleX() < 0){  //怪物向左
		if(stDst.x >= getPositionX()){
			xx = getPositionX() - m_stCfg.effectx();
		}else if(getPositionX() - stDst.x <= m_stCfg.effectx()){
			xx = getPositionX() - m_stCfg.effectx();
		}else{
			xx = stDst.x;
		}
	}else{		//向右
		if(stDst.x <= getPositionX()){
			xx = getPositionX() + m_stCfg.effectx();
		}else if(stDst.x - getPositionX() <= m_stCfg.effectx()){
			xx = getPositionX() + m_stCfg.effectx();
		}else{
			xx = stDst.x;
		}
	}

	if(::abs(stDst.y - getPositionY()) >= m_stCfg.effecty()){
		yy = getPositionY() + m_stCfg.effecty()*((stDst.y - getPositionY())/::abs(stDst.y - getPositionY()));
	}else{
		yy = stDst.y;
	}
	stRealPos = ccp(xx, yy);
	_dstPos = stRealPos;

	FlyToPos(stRealPos);

	if(_armature){
		float len_y = stRealPos.y - getPositionY();
		float len_x = stRealPos.x - getPositionX();

		float tan_yx = ::abs(len_y)/::abs(len_x);
		float angle = 0;
		if(len_y > 0){
			angle = -atan(tan_yx)*180/M_PI;
		}else{
			angle = atan(tan_yx)*180/M_PI;
		}

		if(angle > 90){
			angle = 90;
		}else if(angle < -90){
			angle = -90;
		}

		_armature->setRotation(angle);
	}
}

void CSkillEffLayer::Fly(bool isLoop){
	if (!_armature){
		LoadArmature(m_stCfg.flyanimation().c_str());		
	}
	if (_armature){
		if (isLoop){
			_armature->getAnimation()->play("fly", -1, -1, 1);
		}else{
			_armature->getAnimation()->play("fly", -1, -1, 0);
		}
	}
}

void CSkillEffLayer::Cast(bool isLoop){
	if (!_armature){
		LoadArmature(m_stCfg.castanimation().c_str());		
	}
	if (_armature){
		if (isLoop){
			_armature->getAnimation()->play("fly", -1, -1, 1);
		}else{
			_armature->getAnimation()->play("fly", -1, -1, 0);
		}
	}
}


void CSkillEffLayer::CastToPos(CCPoint stDst){
	CCPoint stVec = ccpSub(stDst, getPosition());
	CCPoint stDir = ccpNormalize(stVec);
	_velocity = ccp(stDir.x * m_stCfg.flyspeed(), stDir.y * m_stCfg.flyspeed());
	Cast(true);
}

void CSkillEffLayer::update(float delta){
// 	if (fabs(_velocity.x) < 1 && fabs(_velocity.y) < 1){
// 		return;
// 	}
	this->setPosition(ccpAdd(this->getPosition(), ccpMult(_velocity, delta)));

	if ( IsFollowType() && _target && _flyState == SKILL_ANIMATION_STATE_CHASE){
		//现在已经在追踪目标了
		CCPoint vec = ccpSub(_target->GetCenterPos(), this->getPosition());
		vec = ccpNormalize(vec);
		vec.x = m_stCfg.flyspeed() * 20 * delta* vec.x;
		vec.y = m_stCfg.flyspeed() * 20 * delta* vec.y;
		CCPoint dir = ccpNormalize(ccpAdd(_velocity, vec));
		float angle =  ccpAngle(ccp(0, 1), dir);
		float origDegree = this->getRotation();
		float rotatedegree; 
// 		if (vec.x > 0){
// 			rotatedegree = (CC_RADIANS_TO_DEGREES(angle) - 90);
// 		}else{
// 			rotatedegree = 270 - CC_RADIANS_TO_DEGREES(angle) ;
// 		}
		rotatedegree = CC_RADIANS_TO_DEGREES(angle);
		 // - origDegree;
		//CCLOG("rotate %f degree\n", rotatedegree);
		this->setRotation(rotatedegree * -1.0);
		//CCPoint dir = ccpNormalize(ccpSub(_target->getPosition(), this->getPosition()));
		m_stCfg.set_flyspeed(m_stCfg.flyspeed() * (1 + delta)) ;
		if (m_stCfg.flyspeed() > 1200){
			m_stCfg.set_flyspeed(1200);
		}
		_velocity.x = m_stCfg.flyspeed() * dir.x;
		_velocity.y = m_stCfg.flyspeed() * dir.y;
		if (getZOrder() != _target->getZOrder()){
			this->getParent()->reorderChild(this, _target->getZOrder());
		}
		
	}else if (IsFollowType() && _target && _flyState == SKILL_ANIMATION_STATE_PREPARE){
		//计算与目标的距离，如果小于10，就开搞
		CCPoint stDistD = ccpSub(getDstPos(), getPosition());
		CCPoint stDistO = ccpSub(getOrigPos(), getPosition());
		if (stDistD.x * stDistO.x >=0 && stDistO.y * stDistD.y >= 0){			//应该要开始追踪目标了
			if (m_stCfg.triggertype() != SKILL_TRIGGER_SPAWN_FOLLOW){
				_flyState = SKILL_ANIMATION_STATE_CHASE;
			}else{
				_armature->getAnimation()->play("spawn", -1, -1, 0);			
				_flyState = SKILL_ANIMATION_STATE_NULL;

				//peng。。。。。一声，开始射出各种其他小特效,干，以后可别再问我要小特效的飞行音效啊。。。
				if (m_stCfg.atksound().length() && strcmp(m_stCfg.atksound().c_str(), "null")){
					AudioManager::playEffect(m_stCfg.atksound().c_str());
				}
			}			
		}

		if (getZOrder() != getOwnerEntity()->getZOrder() - 1){
			this->getParent()->reorderChild(this, getOwnerEntity()->getZOrder() - 1);
		}			

	}else if (m_stCfg.triggertype() == SKILL_TRIGGER_TRHOWN_TRAP){
		UpdateThrownTrapType(delta);
	}else if (m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FLY_TO){
		UpdateDirectFlyType(delta);
	}else if(m_stCfg.triggertype() == SKILL_TRIGGER_THROWN_COLIDE){
		UpdateThrownFlyType(delta);
	}
	
}

void CSkillEffLayer::UpdateThrownFlyType(float dt){
	if (_flyState == SKILL_ANIMATION_STATE_FLYING){
		CCPoint stDistD = ccpSub(getDstPos(), getPosition());
		CCPoint stDistO = ccpSub(getOrigPos(), getPosition());
		if (stDistD.x * stDistO.x >=0 && stDistO.y * stDistD.y >= 0){
			Boom();
			_flyState = SKILL_ANIMATION_STATE_DONE;
			_velocity = CCPointZero;
		}else{
			ThrownTrail(dt);
		}
	}
}

void CSkillEffLayer::UpdateDirectFlyType(float dt){
	if(_flyState == SKILL_ANIMATION_STATE_PREPARE){
		CCPoint stDistD = ccpSub(getDstPos(), getPosition());
		CCPoint stDistO = ccpSub(getOrigPos(), getPosition());
		if (stDistD.x * stDistO.x >=0 && stDistO.y * stDistD.y >= 0){
			_flyState = SKILL_ANIMATION_STATE_STANDBY;
			_armature->getAnimation()->play("spawn", -1, -1, 0);
			_velocity = CCPointZero;
		}
	}else if (_flyState == SKILL_ANIMATION_STATE_STANDBY){

	}
}

void CSkillEffLayer::UpdateThrownTrapType(float dt){
	if (_flyState == SKILL_ANIMATION_STATE_PREPARE){
		CCPoint stDistD = ccpSub(getDstPos(), getPosition());
		CCPoint stDistO = ccpSub(getOrigPos(), getPosition());
		if (stDistD.x * stDistO.x >=0 && stDistO.y * stDistD.y >= 0){
			_flyState = SKILL_ANIMATION_STATE_STANDBY;
			_armature->getAnimation()->play("wait", -1, -1, 1);
			_validDeadline = m_stCfg.skillvalidtime(); 
			_velocity = CCPointZero;
		}else{
			ThrownTrail(dt);
		}
	}else if (_flyState == SKILL_ANIMATION_STATE_STANDBY && m_stCfg.skillvalidtime() != 0){  //技能持续时间为0，表示一直存在
		_validDeadline -= dt;
		if (_validDeadline <= 0){
			_bShouldClear = true;
		}
	}
}

void CSkillEffLayer::Boom(){
	if (!_armature){
		LoadArmature(m_stCfg.castanimation().c_str());		
	}
	if (_armature){
		_armature->getAnimation()->play("boom", -1, -1, 0);
	}

	_velocity = CCPointZero;
}

void CSkillEffLayer::ThrownTrail(float dt){
	//计算类抛物线
	setPositionX(_velocity.x * dt + getPositionX());
	setPositionY(_velocity.y * dt + getPositionY());
	_velocity.y += _acclereration.y * dt;
	_velocity.x += _acclereration.x * dt;

	//CCLog("cur pos %f, %f, v %f, %f", getPositionX(), getPositionY(), _velocity.x, _velocity.y);
}

int CSkillEffLayer::LoadRes(unsigned int uiSkilliD){
	const SkillCfg* pstCfg = SKILL_CFG_MASTER->GetSkillCfgByID(uiSkilliD);
	if (pstCfg){
		m_stCfg = *pstCfg;
	}else{
		CCLOG("unknown skill id %d\n", uiSkilliD);
		return -1;
	}

	switch (m_stCfg.triggertype() )
	{
	case SKILL_TRIGGER_DIRECT_FOLLOW:
	case SKILL_TRIGGER_SPAWN_FOLLOW:
	case SKILL_TRIGGER_DIRECT_FLY_TO:
		_flyState = SKILL_ANIMATION_STATE_PREPARE;
		break;
	case SKILL_TRIGGER_FLY_COLIDE:
	case SKILL_TRIGGER_FLY_DIRECT_COLIDE:
		_flyState = SKILL_ANIMATION_STATE_FLYING;
		break;
	default:
		break;
	}	

	return 0;
}

void CSkillEffLayer::LoadArmRes(){
	if (!_armature){
		LoadArmature(m_stCfg.flyanimation().c_str());		
	}
}

void CSkillEffLayer::LoadArmature(const char* resPath)
{
    if(0 == strcmp(resPath, "null")) {
        CCAssert(0, "[error]CSkillEffLayer::LoadArmature, resPath is null! ");
    }
    /*
	if (_armature &&  strcmp(_armature->getName().c_str(), resPath) != 0){		
		
	}*/

	if (!_armature){
		_armature = CCArmature::create(resPath);
        if(_armature) {
            this->addChild(_armature);
        }
		
	}
	if (_armature){
		_armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CSkillEffLayer::HandleFrameEvent));	
		_armature->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CSkillEffLayer::MovementCallback));
		this->AddEventListener(this, PLAY_SOUND_KEY_FRAME_EVENT, eventListner_selector(CSkillEffLayer::HandlePlaySoundEvent));
		setContentSize(_armature->getContentSize());
	}

}

void CSkillEffLayer::onEnter(){
	CCNode::onEnter();
	scheduleUpdate();
	CCNode::setScale(m_fBasicScale);
}


void CSkillEffLayer::HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx)
{
	_currentFrameIdx = currentFrameIdx;
	DispatchEvent(stEvent, this);	
	
}

void CSkillEffLayer::MovementCallback(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE && armature->m_uID == getArmature()->m_uID)  
	{  
		if (strcmp(name, "spawn") == 0){
			_flyState = SKILL_ANIMATION_STATE_DONE;	
		}else if(armature->getName() == "warning" && strcmp(name, "fly") == 0){
			DispatchEvent(WARNING_COMPLETE_EVENT, this);
		}
	}
}

bool CSkillEffLayer::IsFollowType() const{

	return (m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FOLLOW 
		|| m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_FOLLOW
		|| m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_FOLLOW_ON_GROUND);

}

bool CSkillEffLayer::IsCollideType() const{
	return ((m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FOLLOW && getFlyState() == SKILL_ANIMATION_STATE_CHASE)
		|| (m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_FOLLOW && getFlyState() == SKILL_ANIMATION_STATE_CHASE)
		|| m_stCfg.triggertype() == SKILL_TRIGGER_FLY_COLIDE
		|| m_stCfg.triggertype() == SKILL_TRIGGER_FLY_DIRECT_COLIDE
		|| (m_stCfg.triggertype() == SKILL_TRIGGER_TRHOWN_TRAP && getFlyState() == SKILL_ANIMATION_STATE_STANDBY)
		|| (m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FLY_TO && getFlyState() == SKILL_ANIMATION_STATE_STANDBY)
		|| (m_stCfg.triggertype() == SKILL_TRIGGER_THROWN_COLIDE && getFlyState() == SKILL_ANIMATION_STATE_FLYING));
}

bool CSkillEffLayer::IsAreaType(){

	return (m_stCfg.triggertype() == SKILL_TRIGGER_AREA);

}

bool CSkillEffLayer::HasCastAnimation(){
	return (m_stCfg.castanimation().length() && strcmp(m_stCfg.castanimation().c_str(), "null") != 0);
}

bool CSkillEffLayer::HasFlyAnimation(){
	return (m_stCfg.flyanimation().length() && strcmp(m_stCfg.flyanimation().c_str(), "null") != 0);
}

bool CSkillEffLayer::HasHurtAnimation(){
	return (m_stCfg.hurtanimation().length() && strcmp(m_stCfg.hurtanimation().c_str(), "null") != 0);
}

void CSkillEffLayer::HandlePlaySoundEvent(const string& stEvent, CCObject* pstEventObj){
	if (m_stCfg.atksound().length() && strcmp(m_stCfg.atksound().c_str(), "null")){
		AudioManager::playEffect(m_stCfg.atksound().c_str());
	}
}


void CSkillEffLayer::PlayHitAnimation(){
	if (getArmature() && getArmature()->getAnimation()->getAnimationData()->getMovement("hit")){
		getArmature()->getAnimation()->play("hit");
		if (m_stCfg.atksound().length() && strcmp(m_stCfg.atksound().c_str(), "null")){
			AudioManager::playEffect(m_stCfg.atksound().c_str());
		}
	}
}

bool CSkillEffLayer::IsFlying(){
	return _flyState == SKILL_ANIMATION_STATE_FLYING;
}

bool CSkillEffLayer::IsHiting(){
	return _flyState == SKILL_ANIMATION_STATE_HITING;
}

bool CSkillEffLayer::IsChasing() const{
	return _flyState == SKILL_ANIMATION_STATE_CHASE;
}

void CSkillEffLayer::CalcAcceleration(){
	if (m_stCfg.triggertype() == SKILL_TRIGGER_TRHOWN_TRAP || m_stCfg.triggertype() == SKILL_TRIGGER_THROWN_COLIDE){
		float t1 = 1.20 * (_dstPos.x - getPositionX()) / m_stCfg.flyspeed();
		float t = fabs(t1);
		_acclereration.x = -2.0 * m_stCfg.flyspeed() / (5 * t1);
		_acclereration.y = 2.0 * (_dstPos.y - getPositionY()) / (t * t);
		if (getScaleX() > 0){
			_velocity.x = m_stCfg.flyspeed();
		}else{
			_velocity.x = 0.0 - m_stCfg.flyspeed();
		}
		
		_velocity.y = 0;
	}		
}

void CSkillEffLayer::Warning(unsigned int skillID){
	const SkillCfg* pstCfg = SKILL_CFG_MASTER->GetSkillCfgByID(skillID);
	if (pstCfg){
		m_stCfg = *pstCfg;
	}else{
		CCLOG("unknown skill id %d\n", skillID);
		return;
	}

	LoadArmature("warning");

	if (_armature){
		_armature->getAnimation()->play("fly", -1, -1, 0);
	}
}

bool CSkillEffLayer::IsCanDamge(unsigned int uID){
	map<unsigned int, int>::iterator stIter = m_stIDToDamgeCnt.find(uID);
	if(stIter == m_stIDToDamgeCnt.end()){
		m_stIDToDamgeCnt[uID] = m_stCfg.damagecnt();
		return true;
	}else{
		if(stIter->second > 0){
			return true;
		}else{
			return false;
		}
	}
}

void CSkillEffLayer::DecDamgeCnt(unsigned int uID){
	map<unsigned int, int>::iterator stIter = m_stIDToDamgeCnt.find(uID);
	if(stIter == m_stIDToDamgeCnt.end()){
		m_stIDToDamgeCnt[uID] = m_stCfg.damagecnt() - 1;
	}else{
		unsigned int cnt = stIter->second;
		m_stIDToDamgeCnt[uID] = cnt - 1;
	}
}

void CSkillEffLayer::pauseSkill(){
	this->pauseSchedulerAndActions();
	if(_armature){
		_armature->pauseSchedulerAndActions();
	}
}

void CSkillEffLayer::resumeSkill(){
	this->resumeSchedulerAndActions();
	if(_armature){
		_armature->resumeSchedulerAndActions();
	}
}

