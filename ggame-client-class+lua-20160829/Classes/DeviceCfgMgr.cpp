
#include "DeviceCfgMgr.h"
#include "libOS.h"
#include "DataTableManager.h"

DeviceCfgMgr::DeviceCfgMgr()
{
	
}

DeviceCfgMgr::~DeviceCfgMgr()
{

}

DeviceCfgMgr* DeviceCfgMgr::getInstance()
{
	return DeviceCfgMgr::Get();
}

int DeviceCfgMgr::getDeviceBlurType()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
	std::string info = libOS::getInstance()->getDeviceInfo();
	std::string::size_type pos = info.find_first_of('#');
	if (pos == std::string::npos)
	{		
		return DeviceBlurType_Low;
	}

	std::string token = info.substr(0,pos);
	unsigned int cpus = StringConverter::parseUnsignedInt(token);

	info = info.substr(pos+1,info.length());
	pos = info.find_first_of("#");
	if (pos == std::string::npos)
	{
		return DeviceBlurType_Low;
	}

	token = info.substr(0,pos);
	float cpuHZ = StringConverter::parseReal(token);

	info = info.substr(pos+1,info.length());	
	token = info;
	float memory = StringConverter::parseReal(token);

	int type = AndroidDeviceTableManager::Get()->getAndroidCfgType(cpus,cpuHZ,memory);
	switch (type)
	{
	case DeviceCfg_Low:
		{
			return DeviceBlurType_Low;
		}
		break;
	case DeviceCfg_Middle:
		{
			return DeviceBlurType_Middle;
		}
		break;
	case DeviceCfg_High:
		{
			return DeviceBlurType_High;
		}
		break;
	default:
		break;
	}

	return DeviceBlurType_Low;
#elif  CC_TARGET_PLATFORM == CC_PLATFORM_IOS 
	std::string platformInfo = libOS::getInstance()->getPlatformInfo();
	int pos = platformInfo.find('#');
	std::string iosDeviceName = platformInfo.substr(0,pos);
	IOSDeviceItem* item = IOSDeviceTableManager::Get()->getIOSDeviceItemByName(iosDeviceName);
	if (item)
	{
		switch (item->cfgType)
		{
		case DeviceCfg_Low:
			{
				return DeviceBlurType_Low;
			}
			break;
		case DeviceCfg_Middle:
			{
				return DeviceBlurType_Middle;
			}
			break;
		case DeviceCfg_High:
			{
				return DeviceBlurType_High;
			}
			break;
		default:
			break;
		}
	}

	return DeviceBlurType_Low;
#else
	return DeviceBlurType_High;
#endif
	return DeviceBlurType_Low;
}
