#ifndef BlackShopItem_h__
#define BlackShopItem_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CSPlayerMsg.pb.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
using namespace vmsg;

class IconCell;

class BlackShopItem: public CCObject
{
public:
    enum 
    {
        TAG_IMG_BOUGHT  = 33, 
        TAG_IMG_FRAG    = 43, //ÀÈ∆¨Õº±Í
    };
	BlackShopItem();
	virtual ~BlackShopItem();
    CREATE_FUNC(BlackShopItem);
	
    virtual bool init() {return true;};
	void initLayout(UILayout* pRefLayout);

	void setInfo(const CSBlackProductInfo& oneItem, const int iIdx);

private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
    UILayout* m_pRefLayout;

    UILabel* m_pNameTxt;
    UIImageView* m_pCellBg;

    UIImageView* m_pGoldIcon;
    UILabel* m_pGoldTxt;

    IconCell* m_pIconCell;

    CSBlackProductInfo m_stItemInfo;
    int m_iItemIndex; 
};

#endif // BlackShopItem_h__