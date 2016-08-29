#ifndef RestrictShopItemInfoPanel_h__
#define RestrictShopItemInfoPanel_h__

#include "BasePanel.h"
#include "CSStoreMsg.pb.h"

using namespace vmsg;

class IconCell;

class CRestrictShopItemInfoPanel : public BasePanel
{
public:
	CRestrictShopItemInfoPanel();
	virtual ~CRestrictShopItemInfoPanel();

	CREATE_FUNC(CRestrictShopItemInfoPanel);

	bool init();

	virtual void open();
	virtual void close();

	void setInfo(const CSPanicBuyingStoreItem& oneItem, const int iIdx);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void doBuy();
	void onBuy();

private:
	UILabel* m_pNameTxt;
	UILabel* m_pNumTxt;
	UILabel* m_pNumLabel;
	UILabel* m_pDescTxt;
	UIImageView* m_pGoldIcon;
	UILabel* m_pGoldTxt;

	UIImageView* m_pCellBg;
	IconCell* m_pIconCell;

	UIButton* m_pBuyBtn;
	UIButton* m_pCloseBtn;

	bool m_bHasBought;
	bool m_bIsSellOut;
	int m_iItemIdx;
	int m_iGoldType;
	int m_iGoldCnt;
	int m_iProductType;

};

#endif // RestrictShopItemInfoPanel_h__
