#ifndef EntryControl_h__
#define EntryControl_h__

#include "vlib/base/CVSingleton.h"

#include "EntryID.h"
#include "EntryGroupID.h"

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

#define Entry_Time_Lab 2

class EntryControl
{
public:
	EntryControl();
	~EntryControl();

	bool isEntryOpen(unsigned int uEntryID); // 入口是否开放

	void enter(unsigned int uEntryID, int uSecondID = -1); // 进入

	unsigned int getEntryIDByModuleID(unsigned int uModuleID); // 获取ModuleID对应的EntryID，失败返回0

	Button* createEntryBtn(unsigned int uEntryID);

};

#define P_ENTRY_CTRL CVLib::CVSingleton<EntryControl>::Instance()

#define ENTRY_PREFIX_NAME "entry_"

#endif // EntryControl_h__
