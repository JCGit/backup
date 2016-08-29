#ifndef __UserGoldControl_H_
#define __UserGoldControl_H_

#include "vlib/base/CVSingleton.h"
#include "UserGoldProxy.h"



//RMB�̳��õ�����Ʒ��Ϣ
struct BuyProductInfo {
    std::string strProductID; //���������ƷID
    int iProductCount; //���ߵ�����
    int iRealPayMoney; //ʵ��֧���Ľ���λ���� 
    int iSinglePrice; //���ߵĵ���,��λ:  ��
    std::string strProductName; //���������Ʒ����


    BuyProductInfo() {
        strProductID.clear();
        iProductCount = 0;
        iRealPayMoney = 0;
        iSinglePrice = 0;
        strProductName.clear();
    }
};

//app store ����ʱ��Ʒ��Ϣ
struct AppAuthedBuyInfo {
    std::string strOrderID; //������Ķ���ID
    std::string strProductID; //���������ƷID
    int iProductCount; //���ߵ�����
    int iTotalMoneyAmount; //�ܼ�

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
    //֧����ؽӿ�
    void startBuy(const BuyProductInfo& stProductInfo);
    void doReSendBuy(); //code=16ʱ�����⴦����Ҫ�ظ�����������

    void onBuyRMBStoreRsp(const std::string& strRspJson);
    void doSDKPay(); //����sdk֧��
    void doLoginSubmitPlayerinfo();

    //app store
    void onAppAuthedBuyRsp(const AppAuthedBuyInfo& stInfo); //�����򣬻�ö����ţ����ڹ���ɹ�ʱ�ύ����̨����
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

    BuyProductInfo m_stNowBuyProductInfo; //���ι������Ʒ��Ϣ

    AppAuthedBuyInfo m_stAppAuthedBuyInfo; // app store
};

#define P_USER_GOLD_CTRL CVLib::CVSingleton<UserGoldControl>::Instance()

#endif //__UserGoldControl_H_

