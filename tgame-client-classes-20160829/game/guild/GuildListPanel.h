#ifndef GuildListPanel_h__
#define GuildListPanel_h__

#include "CSGuildMsg.pb.h"

#include "basic/BasePanel.h"
#include "basic/BasicTypes.h"

class GuildListPanel: public BasePanel
{
public:
	GuildListPanel();
	virtual ~GuildListPanel();

	static GuildListPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void refresh();

	// override end

	void updateGuildList();

	void updateFindGuild();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	bool isGuildApplied(UINT64_t uGuildID);

	void updateApplied(float dt);

private:
	UIButton *m_pBackBtn;

	UIListView *m_pGuildListView;

	UIButton *m_pFindBtn;

	UIButton *m_pGuildBtn;

	UILabel *m_pEmptyListTipTxt;

};

#endif // GuildListPanel_h__
