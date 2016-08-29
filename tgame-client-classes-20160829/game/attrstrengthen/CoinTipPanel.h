#ifndef CoinTipPanel_h__
#define CoinTipPanel_h__

#include "basic/BasePanel.h"

class CoinTipPanel: public BasePanel
{
public:
	CoinTipPanel();
	virtual ~CoinTipPanel();
	
	static CoinTipPanel* create();

	// override start

	virtual bool init();

	// override end

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIButton *m_pGotoBtn;

};

#endif // CoinTipPanel_h__