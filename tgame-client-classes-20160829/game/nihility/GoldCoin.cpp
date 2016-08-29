#include "GoldCoin.h"

const string CGoldCoin::GLOD_END_EVENT = "glod_end_event";  //·ÉÐÐ½áÊø

CGoldCoin::CGoldCoin()
:m_pArm(NULL)
,m_pTarget(NULL)
,m_bIsFlying(false)
{

}

CGoldCoin::~CGoldCoin(){

}

void CGoldCoin::onEnter(){
	CCNode::onEnter();
}

void CGoldCoin::onExit(){
	CCNode::onExit();
	this->unscheduleUpdate();
	m_pTarget = NULL;
}

bool CGoldCoin::init(){
	bool bRet = false;
	do {
		CC_BREAK_IF(!CCNode::init());
		bRet = true;
	} while(0);
	return bRet;
}

void CGoldCoin::loadRes(string resStr){
	if(m_pArm && m_pArm->getParent()){
		m_pArm->removeFromParentAndCleanup(true);
		m_pArm = NULL;
	}

	m_pArm = CCArmature::create(resStr.c_str());
	m_pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CGoldCoin::handleGlodActComplete));
	addChild(m_pArm);
}

void CGoldCoin::setTarge(CCNode* pNode){
	m_pTarget = pNode;
}

void CGoldCoin::startAct(){
	if(!m_pArm){
		return;
	}
	m_pArm->getAnimation()->play("emerge_0", -1, -1, 0);
}

void CGoldCoin::handleGlodActComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		string str = name;
		if(str == "emerge_0"){
			m_pArm->getAnimation()->play("fly_0", -1, -1, -1);
			m_bIsFlying = true;
			this->scheduleUpdate();
		}else if(str == "die_0"){
			this->DispatchEvent(GLOD_END_EVENT, this);
		}
	}
}

void CGoldCoin::update(float delta){
	if(!m_pTarget){
		m_bIsFlying = false;
		m_pArm->getAnimation()->play("die_0", -1, -1, 0);
		this->unscheduleUpdate();
		return;
	}

	CCPoint stPos = this->getPosition();
	CCPoint stTargetPos = m_pTarget->getPosition() + ccp(50, 90);
	CCPoint stWorldPos = m_pTarget->getParent()->convertToWorldSpace(stTargetPos);
	CCPoint stDesPos = this->getParent()->convertToNodeSpace(stWorldPos);


	if(stPos.getDistance(stDesPos) < 20) {
		m_bIsFlying = false;
		m_pArm->getAnimation()->play("die_0", -1, -1, 0);
		this->unscheduleUpdate();
	}else{
		CCPoint dv = ccpNormalize(stDesPos - stPos);
		CCPoint _velocity = ccp(dv.x * 1000, dv.y * 1000);
		this->setPosition(ccpAdd(this->getPosition(), ccpMult(_velocity, delta)));
	}
}

