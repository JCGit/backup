#pragma once
#include "cocos-ext.h"
#include "cocos2d.h"
USING_NS_CC;
USING_NS_CC_EXT;
class ScriptMathToLua 
{
public:
	ScriptMathToLua(void);
	~ScriptMathToLua(void);

	static void showWXChat();
	static void setSwallowsTouches(CCScrollView* _scroll);
};