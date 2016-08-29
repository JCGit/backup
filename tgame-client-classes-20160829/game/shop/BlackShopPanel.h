#ifndef BlackShopPanel_h__
#define BlackShopPanel_h__

#include "basic/BasePanel.h"
#include "CSPlayerMsg.pb.h"

using namespace vmsg;

class BlackShopItem;

class BlackShopPanel: public BasePanel
{
public:
    enum 
    {
        MAX_ITEM_NUM = 8, //最多8个物品
    };
	BlackShopPanel();
	virtual ~BlackShopPanel();
	
	static BlackShopPanel* create();
    
    virtual void open();
    virtual void close();

	virtual bool init();

	void updateUI(const CSBlackMarketInfo& stInfo);

private:
    void timeCountDown(float dt);
    void touchEventHandler(CCObject *pSender, TouchEventType type);
    void refreshFun();
private:
    BlackShopItem* m_stItemLst[MAX_ITEM_NUM];

    UILabel* m_pTimeTxt;
    UIButton* m_pRefreshBtn;
    UIButton* m_pCloseBtn;

    unsigned int m_uTimeCount;
    unsigned int m_uResetCost;
    unsigned int m_uRemainResetCnt; //剩余重置次数
};

#endif // BlackShopPanel_h__