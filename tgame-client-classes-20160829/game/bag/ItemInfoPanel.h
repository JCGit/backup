#ifndef ItemInfoPanel_h__
#define ItemInfoPanel_h__

#include "basic/BasePanel.h"
#include "CSEquipItemMsg.pb.h"

class ItemInfoPanel: public BasePanel
{
public:
	ItemInfoPanel();
	virtual ~ItemInfoPanel();
	
	static ItemInfoPanel* create();

	// override start

	virtual bool init();

	// override end

	void updateInfo(const int iItemCfgID, const int iItemType, const bool bIsShowUseBtn = true, const unsigned int uItemCnt = 1, int nBagIdx = 1);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

    UIImageView* m_pBgItemImg;
	UIImageView *m_pItemImg;

	UILabel *m_pItemNameTxt;

	UILabel *m_pItemTypeNameTxt;

	UILabel *m_pSalePriceTxt;

	UILabel *m_pSalePriceValTxt;

	UILabel *m_pItemDescTxt;

	UIButton *m_pUseBtn;

	int m_nItemCfgID;

	int m_nBagIdx;

};

#endif // ItemInfoPanel_h__