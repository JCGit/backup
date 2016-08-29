#ifndef BossWarQuitPanel_h__
#define BossWarQuitPanel_h__

#include "BasePanel.h"

class CBossWarQuitPanel : public BasePanel
{
public:
	CBossWarQuitPanel();
	virtual ~CBossWarQuitPanel();

	CREATE_FUNC(CBossWarQuitPanel);

	bool init();

	void updateUI();

	void open();
	void close();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	Button* m_pCloseBtn;
	Button* m_pSureBtn;
	Button* m_pCancelBtn;
};

#endif // BossWarQuitPanel_h__
