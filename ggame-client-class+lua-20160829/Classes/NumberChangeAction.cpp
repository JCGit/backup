
#include "stdafx.h"


#include "NumberChangeAction.h"
#include "StringConverter.h"

USING_NS_CC;

void NumberChangeAction::startWithTarget( cocos2d::CCNode *pTarget )
{
	CCActionInterval::startWithTarget(pTarget);
	CCLabelBMFont *label = dynamic_cast<CCLabelBMFont*>(pTarget);
	mStartNumber = 0;
	if(label)
	{
		if(getDuration()<=0)
		{
			if(mDecimalCount == 0)
				label->setString(StringConverter::toString((int)mTargetNumber).c_str());
			else
			{
				char outputStr[32];
				if(mDecimalCount>7)mDecimalCount = 7;
				if(mTargetNumber>1e20)
					mTargetNumber = 1e20;
				std::string fmt = "%."+StringConverter::toString(mDecimalCount)+"f";
				sprintf(outputStr,fmt.c_str(),mTargetNumber);
				label->setString(outputStr);
			}
		}
		else
		{
			mStartNumber = StringConverter::parseReal(label->getString());
		}
	}
}

cocos2d::CCObject* NumberChangeAction::copyWithZone( cocos2d::CCZone* pZone )
{
	CCZone* pNewZone = NULL;
	NumberChangeAction* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (NumberChangeAction*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new NumberChangeAction();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration);
	pCopy->mTargetNumber = mTargetNumber;
	pCopy->mDecimalCount = mDecimalCount;
	pCopy->mStartNumber = mStartNumber;

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

NumberChangeAction* NumberChangeAction::create( float duration, float targetNumber, int decimalCount /*= 0*/ )
{
	NumberChangeAction *pRet = new NumberChangeAction();
	pRet->initWithDuration(duration);
	pRet->autorelease();
	pRet->mTargetNumber = targetNumber;
	pRet->mDecimalCount = decimalCount;

	return pRet;
}

CCActionInterval* NumberChangeAction::reverse( void )
{
	 return NumberChangeAction::create(m_fDuration, mTargetNumber,mDecimalCount);
}

void NumberChangeAction::update( float time )
{
	if(!m_pTarget)return;

	CCLabelBMFont *label = dynamic_cast<CCLabelBMFont*>(m_pTarget);
	if(label && getDuration()>0)
	{
		float rate = getElapsed()/getDuration();
		
		float curNum = mStartNumber + rate*(mTargetNumber - mStartNumber);
		if(getElapsed()>getDuration())
			curNum = mTargetNumber;
		
		if(mDecimalCount == 0)
			label->setString(StringConverter::toString((int)curNum).c_str());
		else
		{
			char outputStr[32];
			if(mDecimalCount>7)mDecimalCount = 7;
			if(curNum>1e20)
				curNum = 1e20;
			std::string fmt = "%."+StringConverter::toString(mDecimalCount)+"f";
			sprintf(outputStr,fmt.c_str(),curNum);
			label->setString(outputStr);
		}

	}
}
