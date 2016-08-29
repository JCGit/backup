#pragma once
#include "cocos2d.h"
#include "Singleton.h"

USING_NS_CC;

/*
*硬件配置管理器，用于给一些特效提供硬件参数
*@author liu longfei
*/

enum DeviceBlurType
{
	DeviceBlurType_Low = 1,				//低
	DeviceBlurType_Middle = 2,			//中
	DeviceBlurType_High = 3,			//高
};

class DeviceCfgMgr
	:public Singleton<DeviceCfgMgr>
{
public:
	DeviceCfgMgr();
	~DeviceCfgMgr();

	int getDeviceBlurType();

	//for lua
	static DeviceCfgMgr* getInstance();
};



