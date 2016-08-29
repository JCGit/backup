#ifndef GuildApplyPanel_h__
#define GuildApplyPanel_h__

#include "basic/BasePanel.h"

class GuildApplyPanel: public BasePanel
{
public:
	GuildApplyPanel();
	virtual ~GuildApplyPanel();

	static GuildApplyPanel* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UILabel *m_pTitleTxt;

	UILabel *m_pNameTxt;

	UILabel *m_pLevelTxt;

	UIListView *m_pApplyListView;

	UIButton *m_pOKBtn;

	UIButton *m_pCancelBtn;

	//int m_nSelectedIdx;

};

#endif // GuildApplyPanel_h__
