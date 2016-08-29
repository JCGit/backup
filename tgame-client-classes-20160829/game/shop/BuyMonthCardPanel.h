#ifndef BuyMonthCardPanel_h__
#define BuyMonthCardPanel_h__

#include "basic/BasePanel.h"

#include "CSGoldMsg.pb.h"

class BuyMonthCardPanel: public BasePanel
{
public:
	BuyMonthCardPanel();
	virtual ~BuyMonthCardPanel();

	static BuyMonthCardPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual bool isMeetAutoOpen();

	// override end

	void updateProductInfo(const vmsg::CSRMBStoreProductInfo& stProductInfo);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void buyMonthCard();

private:
	UIButton *m_pCloseBtn;

	UIButton *m_pBuyBtn;

	vmsg::CSRMBStoreProductInfo m_stProductInfo;

};

#endif // BuyMonthCardPanel_h__