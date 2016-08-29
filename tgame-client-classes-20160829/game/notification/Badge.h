#ifndef Badge_h__
#define Badge_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class Badge: public Widget
{
public:
	enum {
		BADGE_CEILING_NUM = 100,
		BADGE_TAG = 160,
	};

	enum BadgeStat {
		BADGE_STAT_NUM,
		BADGE_STAT_POINT,
	};

public:
	Badge();
	virtual ~Badge();

	static Badge* create();

	virtual bool init();

	void setStat(BadgeStat stat);

	void setNum(unsigned int num);

private:
	UIImageView *m_pBadgeImg;

	UILabel *m_pBadgeNumTxt;

	unsigned int m_uStat;

};

#endif // Badge_h__