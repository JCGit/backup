#include "SceneEffect.h"

const string CSceneEffect::SHAKE_SCREEN_KEY_FRAME = "shake_screen";				//震屏帧事件
const string CSceneEffect::DUST_PLAY_KEY_FRAME = "dust_play";					//灰尘帧事件
const string CSceneEffect::SCENE_EFFECT_SOUND_FRAME = "sound_frame";			//场景特效音效帧事件

CSceneEffect::CSceneEffect()
:_armature(NULL)
{

}

CSceneEffect::~CSceneEffect(){

}

void CSceneEffect::onEnter(){
	CCNode::onEnter();
	scheduleUpdate();
}

void CSceneEffect::onExit(){
	unscheduleUpdate();
	removeAllChildrenWithCleanup(true);
	CCNode::onExit();
}

void CSceneEffect::update(float delta){

}

int CSceneEffect::LoadRes(unsigned int uiEffectID){
	const SceneEffCfg* pstCfg = SCENE_EFFECT_CFG->GetSceneEffCfgByID(uiEffectID);
	if (pstCfg){
		m_stCfg = *pstCfg;
	}else{
		CCLOG("unknown scene effect id %d\n", uiEffectID);
		return -1;
	}

	LoadArmature(pstCfg->effres().c_str());

	return 0;
}

unsigned int CSceneEffect::getEffType(){
	return m_stCfg.type();
}

unsigned int CSceneEffect::getNextEffID(){
	return m_stCfg.nexteffid();
}

void CSceneEffect::LoadArmature(const char* resPath){
	if (!_armature){
		_armature = CCArmature::create(resPath);	
        if(_armature) {
            this->addChild(_armature);
        }
		
	}
	if (_armature){
		_armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CSceneEffect::HandleFrameEvent));	
		_armature->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CSceneEffect::MovementCallback));
		setContentSize(_armature->getContentSize());
	}
}

void CSceneEffect::HandleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx){
	DispatchEvent(stEvent, this);	
}

void CSceneEffect::MovementCallback(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE && armature->m_uID == getArmature()->m_uID)  {  
		this->removeFromParentAndCleanup(true);
	}
}

void CSceneEffect::play(bool isLoop){
	if (!_armature){
		LoadArmature(m_stCfg.effres().c_str());		
	}

	if (_armature){
		if(_armature->getAnimation()->getAnimationData()->movementNames.size() == 0){
			return;
		}
		if (isLoop){
			_armature->getAnimation()->play(_armature->getAnimation()->getAnimationData()->movementNames[0].c_str(), -1, -1, 1);
		}else{
			_armature->getAnimation()->play(_armature->getAnimation()->getAnimationData()->movementNames[0].c_str(), -1, -1, 0);
		}
	}
}