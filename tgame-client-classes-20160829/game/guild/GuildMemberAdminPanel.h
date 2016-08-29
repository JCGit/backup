#ifndef GuildMemberAdminPanel_h__
#define GuildMemberAdminPanel_h__

#include "basic/BasePanel.h"
#include "CSGuildMsg.pb.h"

class GuildMemberAdminPanel: public BasePanel
{
public:
	GuildMemberAdminPanel();
	virtual ~GuildMemberAdminPanel();

	static GuildMemberAdminPanel* create();

	// override start

	virtual bool init();

	// override end

	void openWithMemberInfo(const vmsg::CSGuildMember &stMemberInfo);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UILabel *m_pNameTxt;

	UILabel *m_pPrivilegeValTxt;

	UILabel *m_pLevelValTxt;

	UILabel *m_pLastLoginValTxt;

	UIButton *m_pChangePrivilegeBtn;

	UIButton *m_pKickOutBtn;

	vmsg::CSGuildMember m_stMemberInfo;

};

#endif // GuildMemberAdminPanel_h__