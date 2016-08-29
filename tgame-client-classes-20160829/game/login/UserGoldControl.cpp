#include <vector>
#include "UserGoldControl.h"
#include "ChannelInf.h"
#include "UserObj.h"
#include "defines.h"
#include "CocoStudio/Json/rapidjson/document.h"
#include "CocoStudio/Json/rapidjson/filestream.h"
#include "CocoStudio/Json/rapidjson/writer.h"
#include "CocoStudio/Json/rapidjson/stringbuffer.h"
#include "evtlog/EvtLog.h"
#include "shop/ShopControl.h"
#include "tip/TipControl.h"
#include "bag/BagControl.h"
#include "FaceBookSDK.h"
#include "ChannelInfoCallback.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
//ios
#include "../../../IOSiAPGameInf.h"
#endif

using namespace std;


void UserGoldControl::doReSendBuy() {
    CCLog("[trace]UserGoldControl::doReSendBuy" );
    startBuy(m_stNowBuyProductInfo);
}

void UserGoldControl::startBuy(const BuyProductInfo& stProductInfo) {
    m_stNowBuyProductInfo = stProductInfo;
    m_strPayToken = ChannelInf::GetPayToken();
    m_strSdkUid = ChannelInf::GetSDKUid();
 
 //   CCLog("[trace]UserGoldControl::startBuy,before PayToken [%s], sdk uid [%s]", m_strPayToken.c_str(), m_strSdkUid.c_str() );
 //   std::string uidInGame = intToString(UINT64_t(P_USER_OBJ->getUserInfo()->uin));
 //   const std::string serverId = intToString(P_USER_OBJ->getUserInfo()->zoneid );

 //   
 //   rapidjson::Document jsonDoc;
 //   jsonDoc.Parse<0>("{}");
 //   rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
 //   
 //   jsonDoc.AddMember("channel", ChannelInf::GetChannelName().c_str(), allocator);
 //   jsonDoc.AddMember("uid", m_strSdkUid.c_str(), allocator);
 //   jsonDoc.AddMember("token", m_strPayToken.c_str(), allocator);
 //   jsonDoc.AddMember("appUid", uidInGame.c_str(), allocator);
 //   jsonDoc.AddMember("serverId", serverId.c_str(), allocator);
 //   jsonDoc.AddMember("productName", m_stNowBuyProductInfo.strProductName.c_str(), allocator);
 //   jsonDoc.AddMember("realPayMoney", m_stNowBuyProductInfo.iRealPayMoney, allocator);
 //   jsonDoc.AddMember("singlePrice", m_stNowBuyProductInfo.iSinglePrice, allocator);
 //   jsonDoc.AddMember("ext", "ext", allocator);
 //   jsonDoc.AddMember("productCount",m_stNowBuyProductInfo.iProductCount , allocator);
 //   jsonDoc.AddMember("productId", m_stNowBuyProductInfo.strProductID.c_str(), allocator);

 //   rapidjson::StringBuffer  buffer;
 //   rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
 //   jsonDoc.Accept(writer);
 //   
 //   CCLog("[trace]UserGoldControl::startBuy,str=%s", buffer.GetString());
	//if (!ChannelInfoCallback::isAnySdk()){
	//	//变色龙按照老流程走
	//	P_USER_GOLD_CTRL->GetProxy()->sendBuyRMBStoreRqst(buffer.GetString());
	//}else{//anysdk需要直接发起
	//	 doSDKPay();
	//}
    //
   

	//海外SDK调用支付
	FaceBookSDK::onPay(m_stNowBuyProductInfo.iRealPayMoney);
}



void UserGoldControl::onBuyRMBStoreRsp(const std::string& strRspJson) {
    CCLog("[trace]UserGoldControl::onBuyRMBStoreRsp,pay info rsp=[%s], str len %d", strRspJson.c_str(), strRspJson.length() );
    m_strJsonPayInfo = strRspJson;

    doSDKPay();
}



void UserGoldControl::doSDKPay() {

    std::string uidInGame(intToString(UINT64_t(P_USER_OBJ->getUserInfo()->uin)) );
    std::string userNameInGame( P_USER_OBJ->getUserInfo()->nick );
    std::string serverId( intToString(P_USER_OBJ->getUserInfo()->zoneid ) );
	std::string strOrderID;
	std::string strPayInfo;
	if (!ChannelInfoCallback::isAnySdk()){
		//不是anysdk的，走老流程
		rapidjson::Document jsonDoc;
		jsonDoc.Parse<0>(m_strJsonPayInfo.c_str());
		if(jsonDoc.HasParseError()) {
			CCLog("[error]UserGoldControl::doSDKPay,fail to parse rsp json,error=[%s]", jsonDoc.GetParseError());
			return;
		} 
		
		if(jsonDoc.HasMember("orderId") && jsonDoc["orderId"].IsString()) {
			strOrderID = jsonDoc["orderId"].GetString();
		} else {
			CCLog("[error]UserGoldControl::doSDKPay,fail to get orderId");
			return;
		}
		
		if(jsonDoc.HasMember("payInfo") && jsonDoc["payInfo"].IsString()) {
			strPayInfo = jsonDoc["payInfo"].GetString();
		} else {
			CCLog("[error]UserGoldControl::doSDKPay,fail to get payInfo");
			return;
		}
	}else{
		//anysdk不需要这些信息
		strOrderID = "";
		strPayInfo = "";
	}   


    

    CCLog("[trace]UserGoldControl::doSDKPay");
    ChannelInf::Buy(strOrderID, 
		uidInGame, 
		userNameInGame,
		serverId, 
        m_stNowBuyProductInfo.strProductName, 
        m_stNowBuyProductInfo.strProductID, 
        strPayInfo,  
        m_stNowBuyProductInfo.iProductCount, 
        m_stNowBuyProductInfo.iRealPayMoney );

	CCLog("[trace]UserGoldControl::doSDKPay,uidInGame=[%s],userNameInGame=[%s],serverId=[%s],\
		  productName=[%s],payInfo=[%s],productId=[%s],productCount=[%d],realPayMoney=[%d]", 
		  uidInGame.c_str(), userNameInGame.c_str(), serverId.c_str(), 
		  m_stNowBuyProductInfo.strProductName.c_str(), 
		  strPayInfo.c_str(), 
		  m_stNowBuyProductInfo.strProductID.c_str(),  
		  m_stNowBuyProductInfo.iProductCount, 
		  m_stNowBuyProductInfo.iRealPayMoney );
    //evt log
    P_EVT_LOG->logTDGamePay(uidInGame, "", m_stNowBuyProductInfo.iRealPayMoney, "CNY", "sdk");
}



void UserGoldControl::doLoginSubmitPlayerinfo() {

    std::string uidInGame(intToString(UINT64_t(P_USER_OBJ->getUserInfo()->uin)) );
    std::string userNameInGame( P_USER_OBJ->getUserInfo()->nick );
    int zoneId = P_USER_OBJ->getUserInfo()->zoneid;
    std::string zoneName( P_USER_OBJ->getUserInfo()->zoneName );
    std::string roleLv( intToString(P_USER_OBJ->getUserInfo()->level) );

    ChannelInf::submitPlayerInfo(uidInGame, userNameInGame, roleLv, zoneId, zoneName);
    CCLog("[trace]UserGoldControl::doLoginSubmitPlayerinfo,uidInGame=[%s],userNameInGame=[%s],roleLv=[%s],zoneID=[%d],zoneName=[%s]",
        uidInGame.c_str(), userNameInGame.c_str(), roleLv.c_str(), zoneId, zoneName.c_str() );
}



void UserGoldControl::onAppAuthedBuyRsp(const AppAuthedBuyInfo& stInfo) {
    m_stAppAuthedBuyInfo = stInfo;
    CCLog("[trace]UserGoldControl::onAppAuthedBuyRsp,orderId=[%s],productId=[%s],num=[%d]", 
        stInfo.strOrderID.c_str(), stInfo.strProductID.c_str(), stInfo.iProductCount );
    
    //调用iOS接口，发起app store 支付
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
    //ios
    std::vector<std::string> stIDList;
    stIDList.push_back(stInfo.strProductID);
    IOS_IAP_GAME_INF->iap->requestProducts(stIDList); //回包里调用paymentWithProduct
    CCLog("[trace]UserGoldControl::onAppAuthedBuyRsp,call ios iAP rqst done.");
#endif
}

void UserGoldControl::onAppAuthedBuyDone(const bool bIsSuccess) {
    CCLog("[trace]UserGoldControl::onAppAuthedBuyDone,bIsSuccess=[%d]", bIsSuccess );
    P_SHOP_CTRL->setRMBBuyLoading(false); //stop loading
    if(!bIsSuccess) {
        return;
    }
    //send notify to server
    //TODO: totalMoney暂时写为1吧
    m_stProxy.sendAppAuthedBuySucceedRqst(m_stAppAuthedBuyInfo.strProductID, m_stAppAuthedBuyInfo.iTotalMoneyAmount, 
        m_stAppAuthedBuyInfo.strOrderID, m_stAppAuthedBuyInfo.iProductCount);
    m_stAppAuthedBuyInfo.reset();
}

void UserGoldControl::onBagInfoRsp(const vmsg::CSBagInfo& stRsp){
	P_BAG_CTRL->setBagInfo(stRsp);
	
}

void UserGoldControl::onQueryRMBStoreItemRsp(const vmsg::CSQueryRMBStoreItemRsp& stRsp){
	if(P_TIP_CTRL->getSweepCardShotPanel()->isOpen()){
		P_TIP_CTRL->getSweepCardShotPanel()->updateUI(stRsp);
	}

	if(P_SHOP_CTRL->getBuyMonthCardPanel()->isOpen()) {
		P_SHOP_CTRL->getBuyMonthCardPanel()->updateProductInfo(stRsp.productinfo());
	}

	if(P_TIP_CTRL->getItemLackPanel()->isPaying()) {
		P_TIP_CTRL->getItemLackPanel()->showProductInfo(stRsp.productinfo());
	}
}

