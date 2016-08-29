#ifndef GuildAdminPanel_h__
#define GuildAdminPanel_h__

#include "basic/BasePanel.h"
#include "CSGuildMsg.pb.h"

class GuildAdminPanel: public BasePanel
{
public:
	enum ListType {
		LIST_MEMBER, 
		LIST_ONLINE,
		LIST_APPLY,
	};

public:
	GuildAdminPanel();
	virtual ~GuildAdminPanel();

	static GuildAdminPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void updateUI();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void onDismissGuildConfirm();

	static bool compareLastLogin(const vmsg::CSGuildMember &stMemberA, const vmsg::CSGuildMember &stMemberB);

private:
	UIButton *m_pBackbtn;

	UIListView *m_pMemberListView;

	UIListView *m_pOperationListView;

	UIButton *m_pReplyBtn;

	UIButton *m_pLeaveBtn;

	UIButton *m_pDismissBtn;

};

#endif // GuildAdminPanel_h__