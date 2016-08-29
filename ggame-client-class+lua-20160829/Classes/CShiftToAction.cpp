//
//  CShiftMaskToXAction.cpp
//  
//
//  Created by Hui Zhen on 14-4-17.
//
//

#include "CShiftToAction.h"

CShiftMaskToXAction* CShiftMaskToXAction::create(float fDuration,  const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	CShiftMaskToXAction *pCutToAction = new CShiftMaskToXAction();
	pCutToAction->initWithDuration(fDuration,startPosition, destPosition,oriRec,fCutPos);
	pCutToAction->autorelease();

	return pCutToAction;
}

bool CShiftMaskToXAction::initWithDuration(float fDuration, const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	if (CCActionInterval::initWithDuration(fDuration))
	{
		m_destPosition = destPosition;
		m_startPosition = startPosition;
		m_positionDelta = ccpSub( m_destPosition, m_startPosition );
		m_recOrigin = oriRec;
		m_fStartCut = fCutPos;
		return true;
	}

	return false;
}

void CShiftMaskToXAction::update(float time){
	if(m_pTarget){
		CCSprite* child = dynamic_cast<CCSprite*>(m_pTarget);
		if (child )
		{
			CCPoint addPos = ccpMult(m_positionDelta, time);
			CCPoint newPos =  ccpAdd( m_startPosition, addPos );
			//移动mask需实时改变position,而object无须如此
			child->setPosition(newPos);

			float rectX = 0.0f;
			rectX = m_fStartCut +  addPos.x;
			CCRect newRect = m_recOrigin;
			newRect.size.width = m_recOrigin.size.width - rectX;

			newRect.origin.x = rectX;
#ifdef _DEBUG
			CCLOG("Time is %f, Cut to action:rect width is %f, position delta's (x,y) : (%f,%f) ,   addPos (%f,%f)",
				time,newRect.size.width,m_positionDelta.x,m_positionDelta.y,addPos.x,addPos.y);
#endif // _DEBUG

			child->setTextureRect(newRect);
		}

	}

}


CShiftObjectToXAction* CShiftObjectToXAction::create(float fDuration,  const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	CShiftObjectToXAction *pCutToAction = new CShiftObjectToXAction();
	pCutToAction->initWithDuration(fDuration,startPosition, destPosition,oriRec,fCutPos);
	pCutToAction->autorelease();

	return pCutToAction;
}

bool CShiftObjectToXAction::initWithDuration(float fDuration, const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	if (CCActionInterval::initWithDuration(fDuration))
	{
		m_destPosition = destPosition;
		m_startPosition = startPosition;
		m_positionDelta = ccpSub( m_destPosition, m_startPosition );
		m_recOrigin = oriRec;
		m_fStartCut = fCutPos;
		return true;
	}

	return false;
}

void CShiftObjectToXAction::update(float time){
	if(m_pTarget){
		CCSprite* child = dynamic_cast<CCSprite*>(m_pTarget);
		if (child )
		{
			CCPoint addPos = ccpMult(m_positionDelta, time);
			CCPoint newPos =  ccpAdd( m_startPosition, addPos );
			//child->setPosition(newPos);

			float rectX = 0.0f;
			rectX = m_fStartCut +  addPos.x;
			CCRect newRect = m_recOrigin;
			newRect.size.width = rectX;

			//newRect.origin.x = m_recOrigin.size.width -rectX;
#ifdef _DEBUG
			CCLOG("Time is %f, Cut to action:rect width is %f, position delta's (x,y) : (%f,%f) ,   addPos (%f,%f)",
				time,newRect.size.width,m_positionDelta.x,m_positionDelta.y,addPos.x,addPos.y);
#endif // _DEBUG
			child->setTextureRect(newRect);
		}

	}

}


CShiftMaskToYAction* CShiftMaskToYAction::create(float fDuration,  const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	CShiftMaskToYAction *pCutToAction = new CShiftMaskToYAction();
	pCutToAction->initWithDuration(fDuration,startPosition, destPosition,oriRec,fCutPos);
	pCutToAction->autorelease();

	return pCutToAction;
}

bool CShiftMaskToYAction::initWithDuration(float fDuration, const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	if (CCActionInterval::initWithDuration(fDuration))
	{
		m_destPosition = destPosition;
		m_startPosition = startPosition;
		m_positionDelta = ccpSub( m_destPosition, m_startPosition );
		m_recOrigin = oriRec;
		m_fStartCut = fCutPos;
		return true;
	}

	return false;
}

void CShiftMaskToYAction::update(float time){
	if(m_pTarget){
		CCSprite* child = dynamic_cast<CCSprite*>(m_pTarget);
		if (child )
		{
			CCPoint addPos = ccpMult(m_positionDelta, time);
			CCPoint newPos =  ccpAdd( m_startPosition, addPos );
			child->setPosition(newPos);

			float rectY = 0.0f;
			rectY = m_fStartCut +  addPos.y;
			CCRect newRect = m_recOrigin;
			newRect.size.height = m_recOrigin.size.height - rectY;

			//newRect.origin.y = rectY;
#ifdef _DEBUG
			CCLOG("Time is %f, Cut to action:rect width is %f, position delta's (x,y) : (%f,%f) ,   addPos (%f,%f)",
				time,newRect.size.width,m_positionDelta.x,m_positionDelta.y,addPos.x,addPos.y);
#endif // _DEBUG
			child->setTextureRect(newRect);
		}

	}

}



CShiftObjectToYAction* CShiftObjectToYAction::create(float fDuration,  const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	CShiftObjectToYAction *pCutToAction = new CShiftObjectToYAction();
	pCutToAction->initWithDuration(fDuration,startPosition, destPosition,oriRec,fCutPos);
	pCutToAction->autorelease();

	return pCutToAction;
}

bool CShiftObjectToYAction::initWithDuration(float fDuration, const CCPoint& startPosition,const CCPoint& destPosition,const CCRect oriRec, int fCutPos)
{
	if (CCActionInterval::initWithDuration(fDuration))
	{
		m_destPosition = destPosition;
		m_startPosition = startPosition;
		m_positionDelta = ccpSub( m_destPosition, m_startPosition );
		m_recOrigin = oriRec;
		m_fStartCut = fCutPos;
		return true;
	}

	return false;
}

void CShiftObjectToYAction::update(float time){
	if(m_pTarget){
		CCSprite* child = dynamic_cast<CCSprite*>(m_pTarget);
		if (child )
		{
			CCPoint addPos = ccpMult(m_positionDelta, time);
			CCPoint newPos =  ccpAdd( m_startPosition, addPos );
			//child->setPosition(newPos);

			float rectY = 0.0f;
			rectY = m_fStartCut +  addPos.y;
			CCRect newRect = m_recOrigin;
			newRect.size.height = rectY;
			newRect.origin.y = m_recOrigin.size.height -rectY;
			//newRect.origin.y = rectY;
#ifdef _DEBUG
			CCLOG("Time is %f, Cut to action:rect width is %f, position delta's (x,y) : (%f,%f) ,   addPos (%f,%f)",
				time,newRect.size.width,m_positionDelta.x,m_positionDelta.y,addPos.x,addPos.y);
#endif // _DEBUG
			child->setTextureRect(newRect);
		}

	}

}


bool ShiftToManager::ExpSetObjectXPos(CCSprite * object,CCRect oriRect,float fCurrentPercent){
	if (fCurrentPercent>1.0f || fCurrentPercent<0.0f)
	{
		CCLOG("format is not correct, should be in [0.0,1.0]");
		return false;
	}

	if (object == NULL)
	{
		CCLOG("exp object is null");
		return false;
	}


	CCPoint currentPos = object->getPosition();
	CCRect spriteRect = oriRect;

	float fAbsoluteXPos = 0.0f;
	fAbsoluteXPos = spriteRect.size.width * fCurrentPercent;


	CCRect newRect = spriteRect;
	newRect.size.width = fAbsoluteXPos;
	newRect.origin.x = spriteRect.size.width -fAbsoluteXPos;
	object->setTextureRect(newRect);
	return true;
}


CShiftObjectToXAction* ShiftToManager::ExpShiftObjectToXPos(float fDuration, CCRect oriRect, CCSprite * object,float fFromPercent,float fToPercent){
	if (fFromPercent>1.0f || fFromPercent<0.0f || fToPercent>1.0f || fToPercent<0.0f)
	{
		CCLOG("@ExpShiftToYPos: Percentage format is not correct, should be in [0.0,1.0]");
		return NULL;
	}


	if (object == NULL)
	{
		CCLOG("exp object is null");
		return NULL;
	}

	CCPoint currentPos = object->getPosition();
	CCRect spriteRect = oriRect;
	//spriteRect.size = mask->getContentSize();
	//calculate the from pos of the rect bottom based on the Percent
	float fAbsFromXPos = 0.0f;
	fAbsFromXPos = spriteRect.size.width * fFromPercent;
	float fFromXPos = 0.0f;
	fFromXPos = fAbsFromXPos + currentPos.x;
	CCPoint fromPos = currentPos;
	fromPos.x = fFromXPos;

	float fAbsToXPos = 0.0f;
	fAbsToXPos = spriteRect.size.width * fToPercent;
	float fToXPos = 0.0f;
	fToXPos = fAbsToXPos + currentPos.x;
	CCPoint toPos = currentPos;
	toPos.x = fToXPos;

	CShiftObjectToXAction * pAction = CShiftObjectToXAction::create(fDuration,fromPos,toPos,spriteRect,fAbsFromXPos);
	return pAction ;

}



CShiftMaskToYAction* ShiftToManager::ExpShiftMaskToYPos(float fDuration, CCRect oriRect, const CCPoint& oriPosition,float fFromPercent,float fToPercent){
	if (fFromPercent>1.0f || fFromPercent<0.0f || fToPercent>1.0f || fToPercent<0.0f)
	{
		CCLOG("@ExpShiftToYPos: Percentage format is not correct, should be in [0.0,1.0]");
		return NULL;
	}

	CCPoint currentPos = oriPosition;
	CCRect spriteRect = oriRect;
	//spriteRect.size = mask->getContentSize();
	//calculate the from pos of the rect bottom based on the Percent
	float fAbsFromYPos = 0.0f;
	fAbsFromYPos = spriteRect.size.height * fFromPercent;
	float fFromYPos = 0.0f;
	fFromYPos = fAbsFromYPos + currentPos.y;
	CCPoint fromPos = currentPos;
	fromPos.y = fFromYPos;

	float fAbsToYPos = 0.0f;
	fAbsToYPos = spriteRect.size.height * fToPercent;
	float fToYPos = 0.0f;
	fToYPos = fAbsToYPos + currentPos.y;
	CCPoint toPos = currentPos;
	toPos.y = fToYPos;

	CShiftMaskToYAction * pAction = CShiftMaskToYAction::create(fDuration,fromPos,toPos,spriteRect,fAbsFromYPos);
	return pAction ;

}

bool ShiftToManager::ExpSetMaskYPos(CCSprite * object,CCRect oriRect,const CCPoint& oriPosition,float fCurrentPercent){
	if (fCurrentPercent>1.0f || fCurrentPercent<0.0f)
	{
		CCLOG("format is not correct, should be in [0.0,1.0]");
		return false;
	}
	if (object == NULL)
	{
		CCLOG("exp object is null");
		return false;
	}

	CCPoint currentPos = oriPosition;
	CCRect spriteRect = oriRect;

	float fAbsoluteYPos = 0.0f;
	fAbsoluteYPos = spriteRect.size.height * fCurrentPercent;

	float fYPos = 0.0f;
	fYPos = fAbsoluteYPos + currentPos.y;

	object->setPositionY(fYPos);

	CCRect newRect = spriteRect;
	newRect.size.height = spriteRect.size.height - fAbsoluteYPos;

	object->setTextureRect(newRect);
	return true;
}

bool ShiftToManager::ExpSetObjectYPos(CCSprite * object, CCRect oriRect,float fCurrentPercent){
	if (fCurrentPercent>1.0f || fCurrentPercent<0.0f)
	{
		CCLOG("format is not correct, should be in [0.0,1.0]");
		return false;
	}

	if (object == NULL)
	{
		CCLOG("exp object is null");
		return false;
	}

	CCPoint currentPos = object->getPosition();
	CCRect spriteRect = oriRect;

	float fAbsoluteYPos = 0.0f;
	fAbsoluteYPos = spriteRect.size.height * fCurrentPercent;


	CCRect newRect = spriteRect;
	newRect.size.height = fAbsoluteYPos;
	newRect.origin.y = spriteRect.size.height -fAbsoluteYPos;
	object->setTextureRect(newRect);
	return true;
}


CShiftObjectToYAction* ShiftToManager::ExpShiftObjectToYPos(float fDuration, CCRect oriRect, CCSprite * object,float fFromPercent,float fToPercent){
	if (fFromPercent>1.0f || fFromPercent<0.0f || fToPercent>1.0f || fToPercent<0.0f)
	{
		CCLOG("@ExpShiftToYPos: Percentage format is not correct, should be in [0.0,1.0]");
		return NULL;
	}

	if (object == NULL)
	{
		CCLOG("exp object is null");
		return NULL;
	}

	CCPoint currentPos = object->getPosition();
	CCRect spriteRect = oriRect;
	//spriteRect.size = mask->getContentSize();
	//calculate the from pos of the rect bottom based on the Percent
	float fAbsFromYPos = 0.0f;
	fAbsFromYPos = spriteRect.size.height * fFromPercent;
	float fFromYPos = 0.0f;
	fFromYPos = fAbsFromYPos + currentPos.y;
	CCPoint fromPos = currentPos;
	fromPos.y = fFromYPos;

	float fAbsToYPos = 0.0f;
	fAbsToYPos = spriteRect.size.height * fToPercent;
	float fToYPos = 0.0f;
	fToYPos = fAbsToYPos + currentPos.y;
	CCPoint toPos = currentPos;
	toPos.y = fToYPos;

	CShiftObjectToYAction * pAction = CShiftObjectToYAction::create(fDuration,fromPos,toPos,spriteRect,fAbsFromYPos);
	return pAction ;

}

