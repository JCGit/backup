#ifndef CDKPanel_h__
#define CDKPanel_h__

#include "BasePanel.h"

class CDKPanel : public BasePanel
{
public:
	CDKPanel();
	virtual ~CDKPanel();

	CREATE_FUNC(CDKPanel);

	bool init();

	void close();
private:
	void sureFun();
	void hideTip();
	string trimSpace(string& str);
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	Button* m_pCloseBtn;
	Button* m_pSureBtn;
	Button* m_pCancelBtn;

	Label* m_pTipTxt;
	TextField* m_pInputTxt;
};

#endif // CDKPanel_h__
