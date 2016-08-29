#include "SceneTrap.h"
#include "Macros.pb.h"

const string CSceneTrap::SCENE_TRAP_DISAPPEAR = "scene_trap_disappear";

CSceneTrap::CSceneTrap()
:m_fStartTime(0)
{
	m_uType = CBaseSceneItem::TYPE_SCENE_TRAP;
	m_fBaseScle = 1.5f;
}

CSceneTrap::~CSceneTrap(){

}

void CSceneTrap::onEnter(){
	CCNode::onEnter();
	CCNode::setScale(m_fBaseScle);
}

void CSceneTrap::onExit(){
	unscheduleUpdate();
	removeAllChildrenWithCleanup(true);
	CCNode::onExit();
}

void CSceneTrap::update(float delta){
	m_fStartTime += delta;

	if(m_stCfg.type() != TRAP_TYPE_ONCE && m_fStartTime >= m_stCfg.cdtime()){
		Attack(false);
		m_fStartTime = 0;
	}
}

int CSceneTrap::LoadRes(unsigned int uiCfgID, bool isDirectAdd/* = false*/){
	const TrapCfg* pstCfg = TRAP_CFG_MASTER->GetTrapCfgByID(uiCfgID);

	if(pstCfg == NULL){
		CCLOG("unknown trap id %d\n", uiCfgID);
		return -1;
	}

	m_stCfg = *pstCfg;
	_trapID = uiCfgID;
	_targetType = pstCfg->targettype();
	_DamageCnt = pstCfg->damgecnt();

	LoadArmature(pstCfg->res().c_str(), isDirectAdd);

	return 0;
}

void CSceneTrap::resetDamageCnt(){
	_DamageCnt = m_stCfg.damgecnt();
}

void CSceneTrap::LoadArmature(const char* resPath, bool isDirectAdd/* = false*/){
	if (!_armature){
		_armature = CCArmature::create(resPath);	
		_armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CSceneTrap::handleFrameEvent));
		addChild(_armature);
	}
	setContentSize(_armature->getContentSize());

	if(!isDirectAdd){
		if(_armature->getAnimation()->getAnimationData()->getMovement("emerge")){
			_armature->getAnimation()->play("emerge", -1, -1, 0);
			_armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CSceneTrap::emergeMovementCallBack));
		}else{
			Idle();
		}
	}else{
		Idle();
	}

	if(m_stCfg.type() != TRAP_TYPE_ONCE){
		scheduleUpdate();
	}
	m_fStartTime = 0;
}

void CSceneTrap::Idle(){
	_state = TRAP_STATE_IDLE;
	if (_armature && _armature->getAnimation()->getAnimationData()->getMovement("fly")){
		_armature->getAnimation()->play("fly", -1, -1, 1);
	}
}

void CSceneTrap::Attack(bool isDie){
	if(isDie){
		_state = TRAP_STATE_DIE;
	}else{
		_state = TRAP_STATE_ATTACKING;
	}

	if (_armature && _armature->getAnimation()->getAnimationData()->getMovement("die")){
		_armature->getAnimation()->play("die", -1, -1, 0);
		_armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CSceneTrap::dieMovementCallBack));
	}else{
		if(isDie){
			DispatchEvent(SCENE_TRAP_DISAPPEAR, this);	
		}
	}
}

void CSceneTrap::handleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx){
	_currentFrameIdx = currentFrameIdx;
	DispatchEvent(stEvent, this);	

}

void CSceneTrap::emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		Idle();
	}
}

void CSceneTrap::dieMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		if(_state == TRAP_STATE_ATTACKING){
			Idle();
		}else{
			DispatchEvent(SCENE_TRAP_DISAPPEAR, this);
		}
	}
}


