#ifndef RestrictShopItem_h__
#define RestrictShopItem_h__

#include "BaseLayout.h"
#include "CSStoreMsg.pb.h"

using namespace vmsg;

class IconCell;

class CRestrictShopItem : public BaseLayout
{
public:
	CRestrictShopItem();
	virtual ~CRestrictShopItem();

	CREATE_FUNC(CRestrictShopItem);

	virtual bool init() {return true;};
	void initLayout(UILayout* pRefLayout);

	void setInfo(const CSPanicBuyingStoreItem& oneItem, const int iIdx);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	UILayout* m_pRefLayout;

	UILabel* m_pNameTxt;
	UIImageView* m_pCellBg;

	UIImageView* m_pGoldIcon;
	UIImageView* m_pStateImg;
	UILabel* m_pGoldTxt;
	UILabel* m_pNumTxt;

	IconCell* m_pIconCell;

	CSPanicBuyingStoreItem m_stItemInfo;
	int m_iItemIndex; 
};


#endif // RestrictShopItem_h__
