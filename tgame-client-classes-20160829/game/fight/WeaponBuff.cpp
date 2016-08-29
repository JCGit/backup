#include "WeaponBuff.h"

const string CWeaponBuff::WEAPON_BUFF_BOTTOM_RES = "sk00003";

CWeaponBuff::CWeaponBuff(){
	m_pWeaponArm = NULL;
}

CWeaponBuff::~CWeaponBuff(){

}

void CWeaponBuff::onEnter(){
	//CCNode::onEnter();
	CSceneBuff::onEnter();
}

void CWeaponBuff::onExit(){
	CCNode::onExit();
}

void CWeaponBuff::update(float delta){
	
}

int CWeaponBuff::LoadRes(unsigned int uiBuffID, bool isDirectAdd/* = false*/){
	const FloorBuffCfg* pstCfg = FLOOR_BUFF_CFG->GetFloorBuffCfgByID(uiBuffID);

	if(pstCfg == NULL){
		CCLOG("unknown floor buff id %d\n", uiBuffID);
		return -1;
	}

	m_stCfg = *pstCfg;
	_buffID = pstCfg->buffid();
	_targetType = pstCfg->targettype();

	LoadArmature(WEAPON_BUFF_BOTTOM_RES.c_str(), isDirectAdd);

	return 0;
}

void CWeaponBuff::LoadArmature(const char* resPath, bool isDirectAdd/* = false*/){
	if (!_armature){
		_armature = CCArmature::create(resPath);	
		addChild(_armature);
	}
	if (_armature){
		setContentSize(_armature->getContentSize());
	}

	if(!isDirectAdd){
		if(_armature->getAnimation()->getAnimationData()->getMovement("emerge")){
			_armature->getAnimation()->play("emerge", -1, -1, 0);
			_armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CWeaponBuff::emergeMovementCallBack));
		}else{
			Idle();
		}
	}else{
		Idle();
	}

	_isDie = false;
}

void CWeaponBuff::Idle(){
	if (_armature && _armature->getAnimation()->getAnimationData()->getMovement("fly")){
		_armature->getAnimation()->play("fly", -1, -1, 1);
	}

	if(!m_pWeaponArm){
		m_pWeaponArm = CCArmature::create(m_stCfg.res().c_str());
		addChild(m_pWeaponArm);
	}

	if (m_pWeaponArm){
		m_pWeaponArm->getAnimation()->play("fly", -1, -1, 1);
	}
}

void CWeaponBuff::Die(){
	_isDie = true; 
	if (m_pWeaponArm){
		m_pWeaponArm->removeFromParentAndCleanup(true);
		m_pWeaponArm = NULL;
	}

	if (_armature && _armature->getAnimation()->getAnimationData()->getMovement("die")){
		_armature->getAnimation()->play("die", -1, -1, 0);
		_armature->getAnimation()->setMovementEventCallFunc(_armature, movementEvent_selector(CWeaponBuff::dieMovementCallBack));
	}else{
		DispatchEvent(SCENE_BUFF_DISAPPEAR, this);	
	}
}

void CWeaponBuff::emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		Idle();
	}
}

void CWeaponBuff::dieMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		DispatchEvent(SCENE_BUFF_DISAPPEAR, this);	
	}
}

