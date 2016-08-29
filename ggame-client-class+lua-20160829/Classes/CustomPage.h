#ifndef _CUSTOMPAGE_H_
#define _CUSTOMPAGE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BasePage.h"

#define CACHE_MAX_WIDTH  256
#define CACHE_MAX_HEIGHT 256

class CustomPage : public BasePage
{
public:
	enum PAGE_TYPE
	{
		PT_COVER,
		PT_CONTENT,
		PT_CONTENT_MESSAGE,
		PT_LUA,
	};

	virtual PAGE_TYPE getPageType() = 0;
};

#endif