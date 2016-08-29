#include "SceneBuff.h"
#include "SkillCfgMaster.h"

const string CSceneBuff::SCENE_BUFF_DISAPPEAR = "scene_buff_disappear";

CSceneBuff::CSceneBuff()
:m_fStartTime(0)
,_isDie(false)
{
	m_uType = CBaseSceneItem::TYPE_FLOOR_BUFF;
	m_fBaseScle = 1.5f;
}

CSceneBuff::~CSceneBuff(){

}

void CSceneBuff::onEnter(){
	CCNode::onEnter();
	CCNode::setScale(m_fBaseScle);
}

void CSceneBuff::onExit(){
	unscheduleUpdate();
	removeAllChildrenWithCleanup(true);
	CCNode::onExit();
}

void CSceneBuff::update(float delta){
	m_fStartTime += delta;
	//time为0表示一直存在
	if(m_stCfg.time() != 0 && m_fStartTime > (float)m_stCfg.time()){
		DispatchEvent(SCENE_BUFF_DISAPPEAR, this);	//时间到了，抛出事件
	}
}

int CSceneBuff::LoadRes(unsigned int uiBuffID, bool isDirectAdd/* = false*/){
	//this->setAnchorPoint(ccp(0.5, 0.5));	//描点在中心
	const FloorBuffCfg* pstCfg = FLOOR_BUFF_CFG->GetFloorBuffCfgByID(uiBuffID);
	
	if(pstCfg == NULL){
		CCLOG("unknown floor buff id %d\n", uiBuffID);
		return -1;
	}

	m_stCfg = *pstCfg;
	_buffID = pstCfg->buffid();
	_targetType = pstCfg->targettype();

	LoadArmature(pstCfg->res().c_str(), isDirectAdd);

	return 0;
}

void CSceneBuff::LoadArmature(const char* resPath, bool isDirectAdd/* = false*/){
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
			_armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CSceneBuff::emergeMovementCallBack));
		}else{
			Idle();
		}
	}else{
		Idle();
	}
	

	_isDie = false;

	//time为0表示一直存在
	if(m_stCfg.time() != 0){
		scheduleUpdate();
	} 
	m_fStartTime = 0;
}

void CSceneBuff::Idle(){
	if (_armature && _armature->getAnimation()->getAnimationData()->getMovement("fly")){
		_armature->getAnimation()->play("fly", -1, -1, 1);
	}
}

void CSceneBuff::Die(){
	_isDie = true;
	if (_armature && _armature->getAnimation()->getAnimationData()->getMovement("die")){
		_armature->getAnimation()->play("die", -1, -1, 0);
		_armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CSceneBuff::dieMovementCallBack));
	}else{
		DispatchEvent(SCENE_BUFF_DISAPPEAR, this);	
	}
}

void CSceneBuff::emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		Idle();
	}
}

void CSceneBuff::dieMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		DispatchEvent(SCENE_BUFF_DISAPPEAR, this);	
	}
}


