#ifndef BaseWidget_h__
#define BaseWidget_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::gui;

class BaseWidget: public UIWidget
{
public:
	BaseWidget();
	virtual ~BaseWidget();

	// override start
	virtual bool init();

	virtual std::string getDescription() const;

	// override end

};

#endif // BaseWidget_h__
