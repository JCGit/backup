#ifndef GuildOptionsPanel_h__
#define GuildOptionsPanel_h__

#include "basic/BasePanel.h"

class GuildOptionsPanel: public BasePanel
{
public:
	GuildOptionsPanel();
	virtual ~GuildOptionsPanel();
	
	static GuildOptionsPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void updateUI();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void onImpeachDialog();

	void onLeaveDialog();

	void onDismissDialog();

private:
	UIButton *m_pCloseBtn;

	UIListView *m_pOptionsListView;

};

#endif // GuildOptionsPanel_h__