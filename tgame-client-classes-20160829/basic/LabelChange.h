#ifndef LabelChange_h__
#define LabelChange_h__

#include "cocos2d.h"

USING_NS_CC;

class CLabelChange : public CCActionInterval
{
public:
	CLabelChange();
	~CLabelChange();

	/** creates the action */
	static CLabelChange* create(float duration, int fromNum, int toNum);

	/** initializes the action */
	bool initWithDuration(float duration, int fromNum, int toNum);

	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void startWithTarget(CCNode *pTarget);
	virtual CCActionInterval* reverse(void);
	virtual void update(float time);
protected:
private:
	int m_nFromNum;
	int m_nToNum;
	int m_fInter;		//≤•∑≈“Ù–ßº‰∏Ù
};

#endif // LabelChange_h__
