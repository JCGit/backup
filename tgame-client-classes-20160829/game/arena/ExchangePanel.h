#ifndef ExchangePanel_h__
#define ExchangePanel_h__

#include "BasePanel.h"

class CExchangePanel : public BasePanel
{
public:
	CExchangePanel();
	virtual ~CExchangePanel();

	CREATE_FUNC(CExchangePanel);

	bool init();

	void updateUI();

	void open();
	void close();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void timeCountDown(float dt);
	void refreshFun();
private:
	UIButton* m_pRefreshBtn;
	UIButton* m_pCloseBtn;

	UILabel* m_pTimeTxt;
	UILabel* m_pCoinTxt;

	UIPanel* m_pItemLayout;

	CCArray* m_stItemLst;

	unsigned int m_uTimeCount;
};

#endif // ExchangePanel_h__
