#ifndef RecordPanel_h__
#define RecordPanel_h__

#include "BasePanel.h"

class CRecordPanel : public BasePanel
{
public:
	CRecordPanel();
	virtual ~CRecordPanel();

	CREATE_FUNC(CRecordPanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void loading();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
private:
	UIButton* m_pCloseBtn;
	ListView* m_pList;
	UILabel* m_pTipTxt;
};

#endif // RecordPanel_h__
