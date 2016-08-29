#include "PunctureTrap.h"
#include "Macros.pb.h"

CPunctureTrap::CPunctureTrap(){
	m_fStartTime = 0;
}

CPunctureTrap::~CPunctureTrap(){

}

void CPunctureTrap::onEnter(){
	CSceneTrap::onEnter();
}

void CPunctureTrap::onExit(){
	unscheduleUpdate();
	removeAllChildrenWithCleanup(true);
	CCNode::onExit();
}

void CPunctureTrap::update(float delta){
	m_fStartTime += delta;

	if(m_stCfg.type() != TRAP_TYPE_ONCE && m_fStartTime >= m_stCfg.cdtime()){
		Attack(false);
		m_fStartTime = 0;
	}
}

int CPunctureTrap::LoadRes(unsigned int uiCfgID, bool isDirectAdd/* = false*/){
	const TrapCfg* pstCfg = TRAP_CFG_MASTER->GetTrapCfgByID(uiCfgID);

	if(pstCfg == NULL){
		CCLOG("unknown trap id %d\n", uiCfgID);
		return -1;
	}

	m_stCfg = *pstCfg;
	_trapID = uiCfgID;
	_targetType = pstCfg->targettype();

	LoadArmature(pstCfg->res().c_str(), isDirectAdd);

	return 0;
}

void CPunctureTrap::LoadArmature(const char* resPath, bool isDirectAdd/* = false*/){
	if (!_armature){
		_armature = CCArmature::create(resPath);	
		_armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CPunctureTrap::handleFrameEvent));
		addChild(_armature);
	}
	setContentSize(_armature->getContentSize());

	scheduleUpdate();
	m_fStartTime = 0;
	_armature->setVisible(false);
	_state = TRAP_STATE_IDLE;
}

void CPunctureTrap::Attack(bool isDie){
	_state = TRAP_STATE_ATTACKING;

	Warning();
}

void CPunctureTrap::Warning(){
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize();
	CCScale9Sprite* pWarn = CCScale9Sprite::create("ui_v2/manteng_warning.png");
	pWarn->setAnchorPoint(ccp(1, 0.5));
	pWarn->setContentSize(CCSize(stSize.width/4, pWarn->getContentSize().height));

	float xx = _armature->getContentSize().width/2;
	pWarn->setPosition(xx, 0);
	addChild(pWarn, 0, 101);

	CCBlink* blink = CCBlink::create(3, 3);
	CCFiniteTimeAction* nextFun = CCCallFunc::create(this, callfunc_selector(CPunctureTrap::AttackAfterWarn));
	pWarn->runAction(CCSequence::create(blink, nextFun, NULL));
}

void CPunctureTrap::AttackAfterWarn(){
	removeChildByTag(101);

	_armature->setVisible(true);
	if (_armature && _armature->getAnimation()->getAnimationData()->getMovement("fly")){
		_armature->getAnimation()->play("fly", -1, -1, 0);
		_armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CPunctureTrap::atkMovementCallBack));
	}
}

void CPunctureTrap::handleFrameEvent(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx){
	_currentFrameIdx = currentFrameIdx;
	DispatchEvent(stEvent, this);	
}

void CPunctureTrap::atkMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		_armature->setVisible(false);
		_state = TRAP_STATE_IDLE;
	}
}

