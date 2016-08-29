#ifndef BuyMedicalPanel_h__
#define BuyMedicalPanel_h__

#include "BasePanel.h"
#include "vlib/QEventDispatcher.h"

class IconFrameCell;

class CBuyMedicalPanel : public BasePanel, public CQEventDispatcher
{
public:
	static const string BUY_MEDICAL_SUCCESS_EVENT;				//购买医药箱成功事件
	static const string CANCEL_BUY_MEDICAL_EVENT;				//取消补充弹药事件
public:
	CBuyMedicalPanel();
	virtual ~CBuyMedicalPanel();

	CREATE_FUNC(CBuyMedicalPanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void openWithCallFun(CCObject* target, SEL_CallFunc callbackFun);

	unsigned int getCfgID(){
		return m_uCfgID;
	}

	void doBuyMedicalDone();

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

	bool m_bIsCostGold;
	bool m_bIsBuyOne;
	unsigned int m_uOneCost;
	unsigned int m_uTenCost;

	CCObject*    m_pTarget;
	SEL_CallFunc m_pEventSelector;
};

#endif // BuyMedicalPanel_h__
