#include "SceneItem.h"
#include "BaseMapScene.h"
#include "instance/InstanceEventUtils.h"
#include "control/AudioManager.h"
#include "instance/AnimationUtils.h"

const string CSceneItem::SCENE_ITEM_DISAPPEAR = "scene_item_disappear";
const string CSceneItem::SCENE_ITEM_FALLING_EVENT = "scene_item_falling_event";

CSceneItem::CSceneItem()
:_mArmature(NULL)
,m_fStartTime(0)
{
	_blockSize = CCSizeZero;
	m_fBaseScle = 1.5f;
}

CSceneItem::~CSceneItem(){

}

void CSceneItem::onEnter(){
	CCNode::onEnter();
	CCNode::setScale(m_fBaseScle);
}

void CSceneItem::onExit(){
	unscheduleUpdate();
	CCNode::onExit();
	this->removeAllChildrenWithCleanup(true);
}

CCArmature* CSceneItem::getArmature(){
	return _mArmature;
}

void CSceneItem::setArmature(CCArmature* var){
	_mArmature = var;
}

int CSceneItem::getCfgID(){
	return _cfgId;
}

void CSceneItem::setCfgID(int var){
	_cfgId = var;
	initDataFromCfg();
}

void CSceneItem::initDataFromCfg(){
	pstCfg = SCENE_ITEM_CFG->GetSceneItemCfgByID(_cfgId);

	_hp = pstCfg->hp();
	_maxHp = pstCfg->hp();
	_itemType = pstCfg->itemtype();
	_itemState = IDLE_STATE;
	_stHurtVoice = pstCfg->hurtvoice();;
	_stDieVoice = pstCfg->dievoice();

	_resPath = pstCfg->itemres();
}

void CSceneItem::loadRes(){
	this->setAnchorPoint(ccp(0.5f, 0.5f));	//描点在中心

	if (_mArmature == NULL){
		_mArmature = CCArmature::create(_resPath.c_str());
		_mArmature->setAnchorPoint(ccp(0, 0));
		_mArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CSceneItem::handleItemMoveCallEvent));

		setContentSize(_mArmature->getContentSize());
		addChild(_mArmature);

		//播放空闲的动画
		if (_mArmature->getAnimation()->getAnimationData()->getMovement("emerge")){
			_mArmature->getAnimation()->play("emerge", -1, -1, 0);
		}else{
			idle();
		}

		_blockSize = CAnimationUtils::getBlockRect(_mArmature).size;
	}

	if(pstCfg->time() != 0){  //time为0表示一直存在
		scheduleUpdate();
	}
	m_fStartTime = 0;
}

void CSceneItem::update(float dt){
	m_fStartTime += dt;
	//time为0表示一直存在
	if(pstCfg->time() != 0 && m_fStartTime > (float)pstCfg->time()){
		DispatchEvent(SCENE_ITEM_DISAPPEAR, this);	//时间到了，抛出事件
	}
}

void CSceneItem::idle(){
	_mArmature->stopAllActions();
	//播放idle动画
	_mArmature->getAnimation()->play("healthy_stand", -1, -1, 1);

	_itemState = IDLE_STATE;
}

void CSceneItem::hurtWithDemage(float fDamage){
	if(_itemType == ITEM_TYPE_BLOCK_DESTROY){
		if(_itemState == DIE_STATE || checkIsDie()){
			return;
		}

		_hp -= (int)fDamage;

		showHurt();

		if(_hp < _maxHp/2 && _itemState != INJURED_STATE){  //血量为1/2时，为重伤状态
			injured();
		}

		checkIsDie();
	}
}

void CSceneItem::showHurt(){
	if(_itemState == DIE_STATE || _itemState == HURT_STATE){
		return;
	}

	if(_itemState == IDLE_STATE){
		if(_mArmature->getAnimation()->getAnimationData()->getMovement("healthy_hurt")){
			_mArmature->getAnimation()->play("healthy_hurt", -1, -1, 0);
		}
	}else if(_itemState == INJURED_STATE){
		if(_mArmature->getAnimation()->getAnimationData()->getMovement("injured_hurt")){
			_mArmature->getAnimation()->play("injured_hurt", -1, -1, 0);
		}else if(_mArmature->getAnimation()->getAnimationData()->getMovement("healthy_hurt")){
			_mArmature->getAnimation()->play("healthy_hurt", -1, -1, 0);
		}
	}


	if (getDieVoice().length() && strcmp(getHurtVoice().c_str(), "null")){
		AudioManager::playEffect(getHurtVoice().c_str());
	}

	_itemState = HURT_STATE;
}

void CSceneItem::injured(){
	_itemState = INJURED_STATE;

	//播放重伤动画
	if(_mArmature->getAnimation()->getAnimationData()->getMovement("injured_stand")){
		_mArmature->getAnimation()->play("injured_stand", -1, -1, 1);
	}else{
		_mArmature->getAnimation()->play("healthy_stand", -1, -1, 1);
	}
}

bool CSceneItem::checkIsDie(){
	if(_itemType == ITEM_TYPE_BLOCK_DESTROY){
		if(_hp <= 0){
			die();
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

void CSceneItem::die(){
	if(_itemState == DIE_STATE){
		return;
	}

	_itemState = DIE_STATE;

	//播放死亡动画
	if(_mArmature && _mArmature->getAnimation()->getAnimationData()->getMovement("die")){
		_mArmature->getAnimation()->play("die", -1, -1, 0);
	}

	//如果有死亡音效，也在这里播放
	if (getDieVoice().length() && strcmp(getDieVoice().c_str(), "null")){
		AudioManager::playEffect(getDieVoice().c_str());
	}
}

void CSceneItem::handleItemMoveCallEvent(CCArmature * armature, MovementEventType type, const char * name){
	string nameStr = name;
	if (type == COMPLETE){
		if(nameStr == "die"){
			DispatchEvent(SCENE_ITEM_FALLING_EVENT, this);
		}else if(nameStr == "healthy_hurt" || nameStr == "injured_hurt"){
			if(!checkIsDie()){
				if(_hp < _maxHp/2){  //血量为1/2时，为重伤状态
					injured();
				}else{
					idle();
				}
			}
		}else if(nameStr == "emerge"){
			idle();
		}
	}	
}
