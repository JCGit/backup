#ifndef ItemLackPanel_h__
#define ItemLackPanel_h__

#include "basic/BasePanel.h"

#include "CSGoldMsg.pb.h"

class ItemLackPanel: public BasePanel
{
public:
	ItemLackPanel();
	virtual ~ItemLackPanel();

	static ItemLackPanel* create();

	// override start

	virtual bool init();

	// override end

	bool isPaying();

	void promptBuyItem(unsigned int uProductID);

	void showProductInfo(const vmsg::CSRMBStoreProductInfo& stProductInfo);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void buyItem();

private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pItemImg;

	UILabel *m_pCntTxt;

	UILabel *m_pDescTxt;

	UIButton *m_pMoreBtn;

	UIButton *m_pBuyBtn;

	vmsg::CSRMBStoreProductInfo m_stProductInfo;

	bool m_bIsPaying;

};

#endif // ItemLackPanel_h__