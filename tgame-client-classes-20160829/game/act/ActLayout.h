#ifndef ActLayout_h__
#define ActLayout_h__

#include "basic/BaseLayout.h"
#include "CSCouponMsg.pb.h"

class ActLayout: public BaseLayout
{
public:
	ActLayout() {
		mOffsetPoint = CCPoint(0,1);
	};
	virtual ~ActLayout() {};

	virtual void updateInfo(const vmsg::CSCouponActStatusItem& info) = 0;

protected:
	cocos2d::CCPoint	mOffsetPoint;//tableview刷新前的起始位置
};

#endif // ActLayout_h__
