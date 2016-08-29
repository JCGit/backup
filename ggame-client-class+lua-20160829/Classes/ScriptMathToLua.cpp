
#include "stdafx.h"

#include "ScriptMathToLua.h"
#include "ServerDateManager.h"
#include "StringConverter.h"
#include "DataTableManager.h"
#include "BlackBoard.h"
#include "Language.h"
#include "GameMessages.h"
#include "PacketManager.h"
#include "HP.pb.h"
#include "libOS.h"
USING_NS_CC;
ScriptMathToLua::ScriptMathToLua(void)
{

}

ScriptMathToLua::~ScriptMathToLua(void)
{
}


void ScriptMathToLua::showWXChat()
{
	libOS::getInstance()->WeChatInit("wxd930ea5d5a258f4f");
	if(libOS::getInstance()->WeChatIsInstalled())
	{
		libOS::getInstance()->WeChatOpen();
	}
	else
	{
		libOS::getInstance()->WeChatInstall();
	}
}

void ScriptMathToLua::setSwallowsTouches(CCScrollView* _scroll)
{
	CCTouchHandler* pHandler = CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(_scroll);
	if (pHandler)
	{
		CCTargetedTouchHandler* pTh = dynamic_cast<CCTargetedTouchHandler*>(pHandler);
		if (pTh)
		{
			pTh->setSwallowsTouches(true);
		}
	}
}