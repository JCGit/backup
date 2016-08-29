#ifndef BuyItemPanel_h__
#define BuyItemPanel_h__

#include "BasePanel.h"
#include "ClientSvrMsg.pb.h"

class IconFrameCell;

class CBuyItemPanel : public BasePanel
{
public:
	CBuyItemPanel();
	virtual ~CBuyItemPanel();

	CREATE_FUNC(CBuyItemPanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void setCfgID(unsigned int uCfgID, unsigned int uType = vmsg::THING_TYPE_ITEM);

	unsigned int getCfgID(){
		return m_uCfgID;
	}

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void doBuy();
	void buyItemFun();

	ImageView* m_pIconImg;
	ImageView* m_pOneCoinImg;
	ImageView* m_pTenCoinImg;

	Label* m_pDescTxt;
	Label* m_pBuyOneTxt;
	Label* m_pBuyTenTxt;

	Button* m_pCloseBtn;
	Button* m_pBuyOneBtn;
	Button* m_pBuyTenBtn;

	IconFrameCell *m_pIconFrameCell;

	unsigned int m_uCfgID;
	unsigned int m_uItemType;

	bool m_bIsCostGold;
	bool m_bIsBuyOne;
	unsigned int m_uOneCost;
	unsigned int m_uTenCost;
};

#endif // BuyItemPanel_h__
