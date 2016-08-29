#ifndef EllipseMove_h__
#define EllipseMove_h__

#include "cocos2d.h"

USING_NS_CC;

typedef struct ellipse_info
{
	CCPoint center; // 中心点
	float a; // 长半轴
	float b; // 短半轴
	float startRadian; // 开始弧度
	float deltaRadian; // 差量弧度
	bool clockwise; // 顺时针旋转
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