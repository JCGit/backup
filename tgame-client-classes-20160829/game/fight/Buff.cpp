#include "Buff.h"

CBuff* CBuff::create(unsigned int uID){
	CBuff *pRet = new CBuff();

	if(pRet != NULL && pRet->initWithID(uID)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CBuff::initWithID(unsigned int uID){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!CCNode::init());

		m_uOwnerID = uID;

		bRet = true;
	} while (0);

	return bRet;
}

void CBuff::Play(){
	if(m_pstArmature == NULL){
		return;
	}

	if(m_pstArmature->getAnimation()->getAnimationData()->getMovement("emerge")){
		m_pstArmature->getAnimation()->play("emerge", -1, -1, 0);
		m_pstArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CBuff::emergeMovementCallBack));
	}else{
		m_pstArmature->getAnimation()->play("fly", -1, -1, 1);
	}
}

void CBuff::emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		if(m_pstArmature == NULL){
			return;
		}

		m_pstArmature->getAnimation()->play("fly", -1, -1, 1);
	}
}