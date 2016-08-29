#ifndef NPageView_h__
#define NPageView_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class NPageView: public ScrollView
{
public:
	NPageView();
	virtual ~NPageView();

	static NPageView* create();

	// override start

	virtual bool init();

	// override end

	void scrollToDestination(const CCPoint& dst, float time, bool attenuated);

private:

};

#endif // NPageView_h__