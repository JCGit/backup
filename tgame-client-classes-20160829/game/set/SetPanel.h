#ifndef SetPanel_h__
#define SetPanel_h__

#include "BasePanel.h"

class CSetPanel : public BasePanel
{
public:
	CSetPanel();
	virtual ~CSetPanel();

	CREATE_FUNC(CSetPanel);

	bool init();

	void initUI();

	void updateUI();

	void open();

	void close();

	void setOpStat(string stName, bool isOn);

	void setOpFun(unsigned int uType);

	void setHudMove();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void checkBoxCallBackFun(CCObject* pSender,CheckBoxEventType type);

	CheckBox* m_pSoundBtn;
	CheckBox* m_pSoundEffBtn;
	CheckBox* m_pHudBtn;

	Button* m_pCloseBtn;
	Button* m_pExitBtn;
	Button* m_pBindBtn;
	Button* m_pFeedbackBtn;


	CCPoint m_stInitPos;
private:
};

#endif // SetPanel_h__
