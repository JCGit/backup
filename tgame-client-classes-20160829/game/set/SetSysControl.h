#ifndef SetSysControl_h__
#define SetSysControl_h__

#include "vlib/base/CVSingleton.h"
#include <string>
#include "SetPanel.h"

using namespace std;

class SetSysControl
{
public:
	static const string INIT_SET;						//初始设置值

	enum SET_SYS_TYPE{
		ARENA_FIRST_TIME_TYPE = 0,						//是否第一次进入竞技场
		BACKGROUND_SOUND_TYPE = 1,						//背景音乐开关
		SOUND_EFFECT_TYPE = 2,							//音效开关
		HUD_ROCKER_STILL_TYPE = 3,						//摇杆锁定开关
	};
public:
	SetSysControl();
	virtual ~SetSysControl();

	void initSet();

	//获得设置值
	bool isSet(unsigned int uType);						

	//设置op的状态是否选中,是否与后台通讯
	void setOpStat(unsigned int uType, bool bSel, bool isSendUpdate = true);		

	//改变str字符串charIdx处的字符为ch
	string setCharByIdx(string str, const char* ch, unsigned int charIdx);		

	//字符串oriStr的长度增加为newLen，增加部分用字符0
	string add0str(string str, unsigned int newLen);

	//设置回报
	void doSetRsp();	

	void updateLocalData();

	CSetPanel* getSetPanel();

private:
	string m_strSys;

	bool m_bIsInited;

	CSetPanel m_stSetPanel;
};

#define P_SET_CTRL CVLib::CVSingleton<SetSysControl>::Instance()

#endif // SetSysControl_h__
