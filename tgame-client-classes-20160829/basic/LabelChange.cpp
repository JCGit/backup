#include "LabelChange.h"
#include "control/AudioManager.h"

CLabelChange::CLabelChange()
:m_nFromNum(0)
,m_nToNum(0)
,m_fInter(0)
{

}

CLabelChange::~CLabelChange(){

}

CLabelChange* CLabelChange::create(float duration, int fromNum, int toNum){
	CLabelChange *pRet = new CLabelChange();
	pRet->initWithDuration(duration, fromNum, toNum);
	pRet->autorelease();

	return pRet;
}

bool CLabelChange::initWithDuration(float duration, int fromNum, int toNum){
	if (CCActionInterval::initWithDuration(duration)){
		m_nFromNum = fromNum;
		m_nToNum = toNum;
		m_fInter = 0;
		return true;
	}

	return false;
}

CCObject* CLabelChange::copyWithZone(CCZone *pZone){
	CCZone* pNewZone = NULL;
	CLabelChange* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject){
		pCopy = (CLabelChange*)(pZone->m_pCopyObject);
	}
	else{
		pCopy = new CLabelChange();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_nFromNum, m_nToNum);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CLabelChange::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);

	CCLabelProtocol *pLabel = dynamic_cast<CCLabelProtocol*>(m_pTarget);
	if (pLabel)
	{
		CCString *numStr = CCString::createWithFormat("%i", m_nFromNum);
		pLabel->setString(numStr->getCString());
	}

}

CCActionInterval* CLabelChange::reverse(void){
	return CLabelChange::create(m_fDuration, m_nToNum, m_nFromNum);
}


void CLabelChange::update(float t){
	CCLabelProtocol *pLabel = dynamic_cast<CCLabelProtocol*>(m_pTarget);
	if (pLabel){
		int num = m_nFromNum + (m_nToNum - m_nFromNum) * t;
		CCString *numStr = CCString::createWithFormat("%i", num);
		pLabel->setString(numStr->getCString());

		m_fInter++;

		if(m_fInter > 3){
			m_fInter = 0;
			AudioManager::playEffect("roll_number.ogg");
		}
	}

}