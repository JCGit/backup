#ifndef VipLimitPanel_h__
#define VipLimitPanel_h__

#include "BasePanel.h"

class CVipLimitPanel : public BasePanel
{
public:
	CVipLimitPanel();
	virtual ~CVipLimitPanel();

	CREATE_FUNC(CVipLimitPanel);

	bool init();

	void updateUI(string descStr);

	void open();
	void close();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	Button* m_pCloseBtn;
	Button* m_pChargeBtn;
	Button* m_pCancelBtn;

	Label* m_pDescTxt;
};

#endif // VipLimitPanel_h__
