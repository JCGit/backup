#ifndef DailyActEntryPanel_h__
#define DailyActEntryPanel_h__

#include "basic/BasePanel.h"

class DailyActEntryPanel: public BasePanel
{
public:
	DailyActEntryPanel();
	virtual ~DailyActEntryPanel();
	
	static DailyActEntryPanel* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILabel *m_pActNameTxt;

	UILabel *m_pRemainTimesValTxt;

	UILabel *m_pCoinValTxt;

	UILabel *m_pExpValTxt;

	UILabel *m_pPropValTxt;

	UIButton *m_pExitBtn;

	UIButton *m_pGotoBtn;

};

#endif // DailyActEntryPanel_h__