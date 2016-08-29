#pragma once
#include "cocos2d.h"
#include "Singleton.h"

USING_NS_CC;

/*
*Ӳ�����ù����������ڸ�һЩ��Ч�ṩӲ������
*@author liu longfei
*/

enum DeviceBlurType
{
	DeviceBlurType_Low = 1,				//��
	DeviceBlurType_Middle = 2,			//��
	DeviceBlurType_High = 3,			//��
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



