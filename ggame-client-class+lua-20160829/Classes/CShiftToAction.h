#pragma once
#include "cocos2d.h"
#include "../actions/CCActionInterval.h"

/*
Author: Hui Zhen
Date: 2014/4/17

Function: Create the Action used for the exp bar(or other object need to shift) shifting. Support both X and Y direction.

Prepare: two sprite, one is the exp or exp related sprite,  another is the exp mask sprite.

Detail: 
1. one method is move the "mask" to the specific position or rate, here, the mask is on top of the object sprite.
2. another more general method is move the exp object or sprite to the specific position or percentage. Notice, here the object's on top of the mask, take care of the Z order. Just try several times.


*/


USING_NS_CC;


class CShiftMaskToXAction : public CCActionInterval
{
public:
	/** creates the action */
	static CShiftMaskToXAction* create(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);
	/** initializes the action */
	bool initWithDuration(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);

	virtual void update(float time);


private:
	CCPoint m_destPosition;
	CCPoint m_startPosition;
	CCPoint m_positionDelta;
	CCRect m_recOrigin;
	int m_fStartCut;
};



class CShiftObjectToXAction : public CCActionInterval
{
public:
	/** creates the action */
	static CShiftObjectToXAction* create(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);
	/** initializes the action */
	bool initWithDuration(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);

	virtual void update(float time);


private:
	CCPoint m_destPosition;
	CCPoint m_startPosition;
	CCPoint m_positionDelta;
	CCRect m_recOrigin;
	int m_fStartCut;
};


class CShiftMaskToYAction : public CCActionInterval
{
public:
	/** creates the action */
	static CShiftMaskToYAction* create(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);
	/** initializes the action */
	bool initWithDuration(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);

	virtual void update(float time);


private:
	CCPoint m_destPosition;
	CCPoint m_startPosition;
	CCPoint m_positionDelta;
	CCRect m_recOrigin;
	int m_fStartCut;
};


class CShiftObjectToYAction : public CCActionInterval
{
public:
	/** creates the action */
	static CShiftObjectToYAction* create(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);
	/** initializes the action */
	bool initWithDuration(float fDuration, const CCPoint& startPosition, const CCPoint& destPosition,const CCRect oriRec, int fCutPos);

	virtual void update(float time);


private:
	CCPoint m_destPosition;
	CCPoint m_startPosition;
	CCPoint m_positionDelta;
	CCRect m_recOrigin;
	int m_fStartCut;
};



class ShiftToManager
{
public:

	//������X������ƶ�Action
	//���þ�������sprite���ض��İٷֱȣ��ٷֱ�����[0,1],ԭ������������� texture rect.
	static bool ExpSetObjectXPos(CCSprite * object,CCRect oriRect,float fCurrentPercent);

	//�ƶ�object��ĳһpercent ����һ��percent, ����,oriRect��Ҫ����Ϊ����ԭʼ��С��
	static CShiftObjectToXAction* ExpShiftObjectToXPos(float fDuration, CCRect oriRect, CCSprite * mask,float fFromPercent,float fToPercent);


	//������Y������ƶ�Action
	//���þ�������mask���ض��İٷֱȣ��ٷֱ�����[0,1]
	static bool ExpSetMaskYPos(CCSprite * object,CCRect oriRect,const CCPoint& oriPosition,float fCurrentPercent);

	//�ƶ�mask ��ĳһpercent ����һ��percent����0.0->0.5,������0.8->0.5
	static CShiftMaskToYAction* ExpShiftMaskToYPos(float fDuration, CCRect oriRect, const CCPoint& oriPosition,float fFromPercent,float fToPercent);

	//���þ�������sprite���ض��İٷֱȣ��ٷֱ�����[0,1],ԭ������������� texture rect.
	static bool ExpSetObjectYPos(CCSprite * object,CCRect oriRect,float fCurrentPercent);

	//�ƶ�object ��ĳһpercent �� ��һ��percent, ����,oriRect��Ҫ����Ϊ����ԭʼ��С��
	static CShiftObjectToYAction* ExpShiftObjectToYPos(float fDuration, CCRect oriRect, CCSprite * mask,float fFromPercent,float fToPercent);
};

