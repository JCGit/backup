#ifndef GuildJoinReplyPanel_h__
#define GuildJoinReplyPanel_h__

#include "basic/BasePanel.h"

class GuildJoinReplyPanel: public BasePanel
{
public:
	GuildJoinReplyPanel();
	virtual ~GuildJoinReplyPanel();

	static GuildJoinReplyPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pApplyListView;

	UIButton *m_pAgreeBtn;

	UIButton *m_pDenyBtn;

	UILayout *m_pSelectAllLayout;

	UICheckBox *m_pCheckboxBtn;

};

#endif // GuildJoinReplyPanel_h__