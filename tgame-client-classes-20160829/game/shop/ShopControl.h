#ifndef ShopControl_h__
#define ShopControl_h__

#include "vlib/base/CVSingleton.h"

#include "ShopProxy.h"
#include "BlackShopPanel.h"
#include "BlackShopItemInfoPanel.h"
#include "RestrictShopPanel.h"
#include "RestrictShopItemInfoPanel.h"
#include "BuyMonthCardPanel.h"
#include "FirstChargePanel.h"

class RmbShopPanel;

class ShopControl 
{
public:
	ShopControl();
	~ShopControl();
    
	void finalize();

    BlackShopPanel* getBlackShopPanel();
    BlackShopItemInfoPanel* getBlackShopItemInfoPanel();
    RmbShopPanel* getRmbShopPanel();

	CRestrictShopPanel* getRestrictShopPanel();
	CRestrictShopItemInfoPanel* getRestrictItemInfoPanel();

	BuyMonthCardPanel* getBuyMonthCardPanel();

	FirstChargePanel* getFirstChargePanel();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendStoreBuyRqst(unsigned int uType, unsigned int uCfgID, unsigned int uCnt); // ¹ºÂòµÀ¾ß

    ShopProxy* getProxy() {
        return &m_stShopProxy;
    }
    void setRMBBuyLoading(bool bIsShow);

	void sendRestrictShopQueryRqst();
	void sendRestrictShopBuyRqst(const int iItemIndex);
private:
	ShopProxy m_stShopProxy;
    
    BlackShopPanel* m_pBlackShopPanel;
    BlackShopItemInfoPanel* m_pBlackShopItemInfoPanel;
    RmbShopPanel* m_pRmbShopPanel;

	CRestrictShopPanel m_stRestrictShopPanel;
	CRestrictShopItemInfoPanel m_stRestrictItemInfoPanel;

	BuyMonthCardPanel *m_pBuyMonthCardPanel;

	FirstChargePanel *m_pFirstChargePanel;

};

#define P_SHOP_CTRL CVLib::CVSingleton<ShopControl>::Instance()

#endif // ShopControl_h__