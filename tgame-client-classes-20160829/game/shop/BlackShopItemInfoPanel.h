#ifndef BlackShopItemInfoPanel_h__
#define BlackShopItemInfoPanel_h__

#include "basic/BasePanel.h"
#include "CSPlayerMsg.pb.h"
using namespace vmsg;

class IconCell;
class BlackShopItemInfoPanel: public BasePanel
{
public:
    enum 
    {
        TAG_IMG_FRAG    = 43, //ÀÈ∆¨Õº±Í
    };
	BlackShopItemInfoPanel();
	virtual ~BlackShopItemInfoPanel();
	
	static BlackShopItemInfoPanel* create();
    
    virtual void open();
    virtual void close();

	virtual bool init();

    void setInfo(const CSBlackProductInfo& oneItem, const int iIdx);

private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);
    void doBuy();
    void onBuy();
private:

    UILabel* m_pNameTxt;
    UILabel* m_pNumTxt;
    UILabel* m_pDescTxt;
    UIImageView* m_pGoldIcon;
    UILabel* m_pGoldTxt;

    UIImageView* m_pCellBg;
    IconCell* m_pIconCell;

    UIButton* m_pBuyBtn;
    UIButton* m_pCloseBtn;

    bool m_bHasBought;
    int m_iItemIdx;
    int m_iGoldType;
    int m_iGoldCnt;
    int m_iProductType;
};

#endif // BlackShopItemInfoPanel_h__