#ifndef EllipseMove_h__
#define EllipseMove_h__

#include "cocos2d.h"

USING_NS_CC;

typedef struct ellipse_info
{
	CCPoint center; // ���ĵ�
	float a; // ������
	float b; // �̰���
	float startRadian; // ��ʼ����
	float deltaRadian; // ��������
	bool clockwise; // ˳ʱ����ת
} EllipseInfo;

class EllipseMove: public CCActionInterval
{
public:
	EllipseMove();
	virtual ~EllipseMove();

	static EllipseMove* create(float duration, const EllipseInfo &info);

	bool initWithDuration(float duration, const EllipseInfo &info);

	virtual void startWithTarget(CCNode *pTarget);

	virtual void stop();

	virtual void update(float time);

	virtual CCActionInterval* reverse();

private:
	EllipseInfo m_stEllipseInfo;

};

#endif // EllipseMove_h__