#include "Bullet.h"
#include "Macros.pb.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "role/QStageEntity.h"

USING_NS_CC;
USING_NS_CC_EXT;

const string CBullet::BULLET_HIT_KEY = "hit_key";

void CBullet::LoadRes(){
	_bullet = CCArmature::create(_res.c_str());		
}

CBullet::CBullet():
	_bullet(NULL),
	_atk(0),
	_atkMagic(NULL_MAGIC),
	_atkRatio(0),
	_atkType(REMOTE_ORDINARY),
	_destination(CCPointZero),
	_depth(1),
	_light(1),
	_trail(TRA_ISO_HORI_LINE),
	_zalias(0),
	_bulletID(0),
	m_bShouldClear(false),
	_target(NULL),
	_party(0),
	_undealedTime(0.2f),
	_maxPenatrate(0),
	_attenuation(0),
	_cfg(NULL),
	_penatrate(0),
	_evtCnt(0)
{
	for (int i = 0; i < 5; i++){
		m_apHitTargets[i] = 0;
	}
}

CBullet::~CBullet(){
	//CC_SAFE_RELEASE_NULL(_bullet);
}

void CBullet::onEnter(){
	CCNode::onEnter();
	if (_bullet){
		addChild(_bullet);
	}
	scheduleUpdate();
}


void CBullet::onExit(){
	if (_bullet){
		removeChild(_bullet, true);
	}	
	CCNode::onExit();
	unscheduleUpdate();
}

void CBullet::update(float dt){
	updatePostion(dt);
}

void CBullet::updatePostion(float dt){
	if (!_bullet){
		return;
	}
	switch (_trail)
	{
	case TRA_ISO_HORI_LINE:
		straightLineFly(dt);
		break;
	case TRA_CON_HORI_LINE:
		//没有飞行轨迹，这个是直接瞬间作用瞬间移动的，不会存在任何尾焰的问题
		//跟着自己的onwer走
		straightContinous(dt, true);
		break;
	case TRA_ISO_FORWARD_TRACE:
		traceForwardTarget(dt);
		break;
	case TRA_ISO_FULL_TRACE:
		traceTarget(dt);
		break;
	case TRA_CON_SPREAD:
		spread(dt);
		break;
	case TRA_JTL_LINE:
		straightContinous(dt, false);
		break;
	default:
		break;
	}
}

void CBullet::straightLineFly(float dt){
	float fXDistToDeath = _destination.x - getPositionX();	
	if (fXDistToDeath * getScaleX() <= 0
		|| getPenatrate() == getMaxPenatrate()){
		m_bShouldClear = true;
		return;
	}

	setPositionX(getPositionX() + getScaleX() * dt * _speed * getVelocity().x);
	setPositionY(getPositionY() + getScaleY() * dt * _speed * getVelocity().y);
}

void CBullet::straightContinous(float dt, bool isSpread){
	if (_ownerEntity->IsDead()){
		m_bShouldClear = true;
		return;
	}

	if (_ownerEntity->GetBulletID() != _bulletID || !_ownerEntity->isShooting()){
		m_bShouldClear = true;
		return;
	}

	if (!_ownerEntity->GetFirePosition().equals(getPosition())){
		setPosition(_ownerEntity->GetFirePosition());
	}

	if (getZOrder() != _ownerEntity->getZOrder()){
		this->getParent()->reorderChild(this, _ownerEntity->getZOrder());
	}

	if(!isSpread){
		if (getScaleX() * _ownerEntity->getScaleX() < 0){
			setScaleX(-1*getScaleX());
		}
		return;
	}

	float fScaleX = _hitDist / getContentSize().width;
	if (getScaleX() < 0){
		fScaleX *= -1;
	}

	if (getScaleX() * _ownerEntity->getScaleX() < 0){
		fScaleX *= -1;
	}else{
		//fScaleX *= _ownerEntity->getScaleX();		
	}

	if (getScaleX() != fScaleX){
		setScaleX(fScaleX);
	}

	setScaleY(getDepth());
}


void CBullet::traceForwardTarget(float dt){

	CQStageEntity* pstTargetObj = dynamic_cast<CQStageEntity*>(_target);
	if (!_target || !pstTargetObj){
		m_bShouldClear = true;
		return;
	}

	if (pstTargetObj->IsDead()){
		m_bShouldClear = true;
		return;
	}

	float fYDist = pstTargetObj->getPositionY() - getPositionY();
	float fXDist = pstTargetObj->getPositionX() - getPositionX();
	float fXDistToDeath = _destination.x - getPositionX();

	if (fXDistToDeath * getScaleX() <= 0){
		m_bShouldClear = true;
		return;
	}

	float fHitTime = fabs(fXDist) / _speed;
	float fYSpeed = fYDist / fHitTime * 2;

	setPositionY(getPositionY() + fYSpeed * dt);
	setPositionX(getPositionX() + getScaleX() * dt * _speed);
	if (getZOrder() != pstTargetObj->getZOrder()){
		this->getParent()->reorderChild(this, pstTargetObj->getZOrder());
	}

	
}

void CBullet::spread(float dt){
	_speed = getCfg()->flyspeed() - getOwner()->getVelocity().y;
	setPositionX(getPositionX() + getScaleX() * dt * _speed);
	float fXDistToDeath = _destination.x - getPositionX();	
	if (fXDistToDeath * getScaleX() <= 0){
		m_bShouldClear = true;
		return;
	}
}

void CBullet::traceTarget(float dt){
	if (!_target){
		m_bShouldClear = true;
		return;
	}
	CQStageEntity* pstTargetObj = dynamic_cast<CQStageEntity*>(_target);
	if (!pstTargetObj){
		m_bShouldClear = true;
		return;
	}

	if (pstTargetObj->IsDead()){
		m_bShouldClear = true;
	}
	CCPoint vec = ccpSub(pstTargetObj->getPosition(), this->getPosition());
	vec = ccpNormalize(vec);
	vec.x = _speed * 10 * dt * vec.x;
	vec.y = _speed * 10 * dt * vec.y;
	CCPoint dir = ccpNormalize(ccpAdd(_velocity, vec));
	float angle =  ccpAngle(ccp(0, 1), vec);
	float origDegree = this->getRotation();
	float rotatedegree; 
	if (vec.x > 0){
		rotatedegree = (CC_RADIANS_TO_DEGREES(angle) - 90);
	}else{
		rotatedegree = 270 - CC_RADIANS_TO_DEGREES(angle) ;
	}
	// - origDegree;
	//CCLOG("rotate %f degree\n", rotatedegree);
	this->setRotation(rotatedegree);
	//CCPoint dir = ccpNormalize(ccpSub(_target->getPosition(), this->getPosition()));
	_speed = int(_speed * (1 + dt));	
	if (_speed > 1000){
		_speed =1000;
	}
	_velocity.x = _speed * dir.x;
	_velocity.y = _speed * dir.y;

	CCPoint stDesirePos = ccp(getPositionX() + _velocity.x * dt, getPositionY() + _velocity.y * dt);
	setPosition(stDesirePos);
	if (getZOrder() != pstTargetObj->getZOrder()){
		this->getParent()->reorderChild(this, pstTargetObj->getZOrder());
	}
}

void CBullet::Fly(){
	//根据亮度算出来应该播放哪个动画
	if (!_bullet){		
		if (strcmp("null", _res.c_str()) != 0){
			_bullet = CCArmature::create(_res.c_str());
			addChild(_bullet);
			setContentSize(_bullet->getContentSize());
			_bullet->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CBullet::HandleMovementEvent));
			_bullet->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CBullet::HandleFrameEvent));
		}

		if (!_bullet){
			return;
		}
		if (_trail == TRA_CON_HORI_LINE){
			_bullet->setAnchorPoint(ccp(0, 0.5));
		}
	}

	//CCLog("bullet fly %p", _bullet);
	if (getTrail() == TRA_CON_SPREAD){
		_bullet->getAnimation()->play("fly", -1, -1, 0);
	}else{
		_bullet->getAnimation()->play("fly", -1, -1, 1);
	}
	
	
}


void CBullet::HandleMovementEvent(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
		setShouldClear(true);
	}	
}

void CBullet::HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx){
	string strEventStr;
	strEventStr.assign(stEvent);
	size_t offset = 0;
	size_t pos = 0;
	while((pos = strEventStr.find(";", offset)) != string::npos){
		string singleEvent = strEventStr.substr(offset, pos - offset);
		DispatchEvent(singleEvent, this);
		offset = pos + 1;
	}

	string singleEvent = strEventStr.substr(offset, strEventStr.length() - offset);
	if (singleEvent.length()){
		DispatchEvent(singleEvent, this);
	}
}

bool CBullet::canTargetBeAdded(unsigned int uID){
	if (uID == 0){
		return false;
	}

	for (int i = 0; i < 5; i++){
		if (m_apHitTargets[i] == 0){
			return true;
		}else if (m_apHitTargets[i] == uID){
			return false;
		}
	}

	return false;
}


void CBullet::AddTargetToPenatrate(unsigned int uID){
	if (uID == 0){
		return;
	}

	for (int i = 0; i < 5; i++){
		if (m_apHitTargets[i] == 0){
			m_apHitTargets[i] = uID;
			return;
		}else if (m_apHitTargets[i] == uID){
			return;
		}
	}
}