#ifndef RelivePanel_h__
#define RelivePanel_h__

#include "BasePanel.h"
#include "vlib/QEventDispatcher.h"

class CRelivePanel : public BasePanel, public CQEventDispatcher
{
public:
	static const string HERO_RELIVE_EVENT;				//复活成功事件
	static const string HERO_CANCEL_RELIVE_EVENT;		//取消复活事件
public:
	CRelivePanel();
	virtual ~CRelivePanel();

	CREATE_FUNC(CRelivePanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void openWithCallFun(CCObject* target, SEL_CallFunc callbackFun);

	void doReliveDone();			//复活成功
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void doRelive();
	void reliveFun();

	Button* m_pCloseBtn;
	Button* m_pReliveBtn;
	Button* m_pCancelBtn;

	ImageView* m_pBtnGoldImg;
	ImageView* m_pBtnLabelImg;
	ImageView* m_pBtnReliveIcon;

	Layout* m_pCostItemLayout;
	Layout* m_pCostGoldLayout;

	Label* m_pBtnCostTxt;
	Label* m_pCostItemRightTxt;
	Label* m_pCostGoldNumTxt;
	Label* m_pCostGoldRightTxt;

	CCObject*    m_pTarget;
	SEL_CallFunc m_pEventSelector;

	bool m_bIsCostGold;				//是否消耗星钻
	unsigned int m_uCostGold;		//消耗星钻数量
};

#endif // RelivePanel_h__
