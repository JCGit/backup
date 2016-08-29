#ifndef CoinGetWayPanel_h__
#define CoinGetWayPanel_h__

#include "basic/BasePanel.h"

class CoinGetWayPanel: public BasePanel
{
public:
	CoinGetWayPanel();
	virtual ~CoinGetWayPanel();

	static CoinGetWayPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UILayout *m_pEntryLayout;

};

#endif // CoinGetWayPanel_h__