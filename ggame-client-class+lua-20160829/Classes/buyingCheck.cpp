
#include "stdafx.h"

#include "buyingCheck.h"
#include "GamePackets.h"
#include "cocos2d.h"
#include "ServerDateManager.h"
#include "GamePrecedure.h"
#include "StringConverter.h"
#include "MessageBoxPage.h"
#include "GameMessages.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "ThreadSocket.h"
#endif

#include "json/json.h"
const std::string initJson = "{\"list\":[]}";
buyingCheck::buyingCheck(void)
{
	
}


buyingCheck::~buyingCheck(void)
{
}

void buyingCheck::init()
{
	libPlatformManager::getPlatform()->registerListener(this);
	//PacketManager::Get()->registerPacketHandler(OPCODE_GET_SHOP_PURCHASE_RETURN_S,this);
}

/**
{
	[
		{"s":"serial1"},
		{"s":"serial2"}
	]
}
*/

void buyingCheck::onBuyinfoSent( libPlatform*, bool success, const BUYINFO& info, const std::string& log )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	if (ThreadSocket::Get()->getState() == ThreadSocket::SS_UNINITIALIZED)
	{

		PacketManager::Get()->reconnect();
	}

#endif
	std::list<std::string> buyinglist;
	if(success)
	{
		Json::Value val;
		val["s"] = info.cooOrderSerial;

		std::string buyingFile = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey(
			("BuyingCheck_"+StringConverter::toString(GamePrecedure::Get()->getServerID())+"_"+GamePrecedure::Get()->getUin()).c_str(),
			initJson);
		Json::Reader reader;
		Json::Value root;
		bool ret = reader.parse(buyingFile, root);
		if(!ret || root["list"].empty() || !root["list"].isArray())
		{
			root.clear();
			std::string emp(initJson);
			reader.parse(emp,root);
		}
		root["list"].append(val);
		Json::FastWriter writer;
		std::string res = writer.write(root);
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("BuyingCheck",res);
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
		
	}
}

void buyingCheck::check()
{
	std::string buyingFile = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey(
		("BuyingCheck_"+StringConverter::toString(GamePrecedure::Get()->getServerID())+"_"+GamePrecedure::Get()->getUin()).c_str(),
		initJson);
	Json::Reader reader;
	Json::Value root;
	bool ret = reader.parse(buyingFile, root);
	if(ret && !root["list"].empty() && root["list"].isArray())
	{
		//暂时注释掉BuyCheck 功能，需要后期考虑重做，by zhenhui 2014/7/30
		/*OPShopPurchaseCheck che;
		che.set_version(1);
		for(int i=0;i<root["list"].size();++i)
		{
		Json::Value val = root["list"][i];
		if(!val["s"].empty())
		{
		std::string serial = (val["s"].asString());
		che.add_cooorderserial(serial.c_str());
		}
		}
		PacketManager::Get()->sendPakcet(OPCODE_GET_SHOP_PURCHASE_CHECK_C,&che);*/
	}
}

void buyingCheck::onReceivePacket( const int opcode, const ::google::protobuf::Message* packet )
{
	//if(opcode == OPCODE_GET_SHOP_PURCHASE_RETURN_S)
	//{
	//	const OPShopPurchaseCheckRet* msg = dynamic_cast<const OPShopPurchaseCheckRet*>(packet);
	//	if(msg)
	//	{
	//		//MSG_BOX("receive refresh goldcoins");
	//		std::set<std::string> serials;
	//		if(ServerDateManager::Get()->getUserBasicInfo().goldcoins<msg->totalmoney())
	//		{
	//			MsgRechargeSuccess msg;
	//			MessageManager::Get()->sendMessage(&msg);

	//			MSG_BOX(Language::Get()->getString("@PaySuccess"));
	//		}
 //           ServerDateManager::Get()->getUserBasicInfo().goldcoins = msg->totalmoney();
	//		ServerDateManager::Get()->getUserBasicInfo().viplevel = msg->viplevel();
	//		if(msg->has_rechargenum())
 //           {
	//			ServerDateManager::Get()->getUserBasicInfo().rechargenum = msg->rechargenum();
	//		}
 //           MsgTitleStatusChange _titleChangeStatus;
 //           MessageManager::Get()->sendMessage(&_titleChangeStatus);
	//		if(msg->has_tool())
	//		{
	//			CCLog("has_tool");
	//			ServerDateManager::Get()->_getMarketInfo().firstPay = false;
	//			GetPropManager::Get()->gotTool(msg->tool());
	//		}

	//		std::string buyingFile = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("BuyingCheck",initJson);
	//		Json::Reader reader;
	//		Json::Value root;
	//		bool ret = reader.parse(buyingFile, root);
	//		if(ret && !root["list"].empty() && root["list"].isArray())
	//		{
	//			for(int i=0;i<root["list"].size();++i)
	//			{
	//				Json::Value val = root["list"][i];
	//				if(!val["s"].empty())
	//				{
	//					std::string serial = (val["s"].asString());
	//					serials.insert(serial);
	//				}
	//			}

	//			for(int i = 0;i<msg->cooorderserial_size();++i)
	//			{
	//				std::string serial = msg->cooorderserial(i);
	//				std::set<std::string>::iterator it = serials.find(serial);
	//				if(it!=serials.end())
	//					serials.erase(it);
	//			}

	//			root.clear();
	//			std::string emp(initJson);
	//			reader.parse(emp,root);

	//			std::set<std::string>::iterator it = serials.begin();
	//			for(;it!=serials.end();++it)
	//			{
	//				Json::Value val;
	//				val["s"] = *it;
	//				root["list"].append(val);
	//			}
	//			Json::FastWriter writer;
	//			std::string res = writer.write(root);
	//			cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("BuyingCheck",res);
	//			cocos2d::CCUserDefault::sharedUserDefault()->flush();
	//		}

	//	}
	//}
}
