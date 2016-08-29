#ifndef __UserGoldControl_H_
#define __UserGoldControl_H_

#include "vlib/base/CVSingleton.h"
#include "UserGoldProxy.h"



//RMB商城用到的物品信息
struct BuyProductInfo {
    std::string strProductID; //本次买的物品ID
    int iProductCount; //道具的数量
    int iRealPayMoney; //实际支付的金额，单位：分 
    int iSinglePrice; //道具的单价,单位:  分
    std::string strProductName; //本次买的物品名字


    BuyProductInfo() {
        strProductID.clear();
        iProductCount = 0;
        iRealPayMoney = 0;
        iSinglePrice = 0;
        strProductName.clear();
    }
};

//app store 购买时物品信息
struct AppAuthedBuyInfo {
    std::string strOrderID; //本次买的订单ID
    std::string strProductID; //本次买的物品ID
    int iProductCount; //道具的数量
    int iTotalMoneyAmount; //总价

    AppAuthedBuyInfo() {
        reset();
    }
    void reset() {
        strOrderID.clear();
        strProductID.clear();
        iProductCount = 0;
        iTotalMoneyAmount = 0;
    }
};

class UserGoldControl {
public:
    UserGoldProxy* GetProxy() {
        return &m_stProxy;
    }

public:
    //支付相关接口
    void startBuy(const BuyProductInfo& stProductInfo);
    void doReSendBuy(); //code=16时，特殊处理，需要重复发起购买请求

    void onBuyRMBStoreRsp(const std::string& strRspJson);
    void doSDKPay(); //调用sdk支付
    void doLoginSubmitPlayerinfo();

    //app store
    void onAppAuthedBuyRsp(const AppAuthedBuyInfo& stInfo); //发起购买，获得订单号，用于购买成功时提交给后台发货
    AppAuthedBuyInfo& getAppAuthedBuyInfo() {
        return m_stAppAuthedBuyInfo;
    }
    void onAppAuthedBuyDone(const bool bIsSuccess);

	void onBagInfoRsp(const vmsg::CSBagInfo& stRsp);

	void onQueryRMBStoreItemRsp(const vmsg::CSQueryRMBStoreItemRsp& stRsp);
private:
    UserGoldProxy m_stProxy;

private:
    std::string m_strPayToken;
    std::string m_strSdkUid; 
    std::string m_strJsonPayInfo;

    BuyProductInfo m_stNowBuyProductInfo; //本次购买的物品信息

    AppAuthedBuyInfo m_stAppAuthedBuyInfo; // app store
};

#define P_USER_GOLD_CTRL CVLib::CVSingleton<UserGoldControl>::Instance()

#endif //__UserGoldControl_H_

