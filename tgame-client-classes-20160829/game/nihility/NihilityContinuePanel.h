#ifndef NihilityContinuePanel_h__
#define NihilityContinuePanel_h__

#include "BasePanel.h"

class CNihilityContinuePanel : public BasePanel
{
public:
	CNihilityContinuePanel();
	virtual ~CNihilityContinuePanel();

	CREATE_FUNC(CNihilityContinuePanel);

	bool init();

	void updateUI();

	void continueFight();

	void buyManualTip();

	void buyManual();

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

#endif // NihilityContinuePanel_h__
