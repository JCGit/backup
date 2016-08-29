#ifndef FindPartnerPanel_h__
#define FindPartnerPanel_h__

#include "BasePanel.h"

class FindPartnerPanel: public BasePanel
{
public:
	FindPartnerPanel();
	virtual ~FindPartnerPanel();
	
	static FindPartnerPanel* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pPartnerListView;

	UIButton *m_pInviteBtn;

	UIButton *m_pCancelBtn;

	int m_nSelectedIdx;
};

#endif // FindPartnerPanel_h__