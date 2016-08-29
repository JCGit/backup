#ifndef GuildAdminMemberItem_h__
#define GuildAdminMemberItem_h__

#include "basic/BaseLayout.h"
#include "basic/BasicTypes.h"
#include "CSGuildMsg.pb.h"

class GuildAdminMemberItem: public BaseLayout
{
public:
	GuildAdminMemberItem();
	virtual ~GuildAdminMemberItem();

	static GuildAdminMemberItem* create();

	// override start

	virtual bool init();

	// override end

	const vmsg::CSGuildMember& getInfo();

	void setInfo(const vmsg::CSGuildMember &stMember);

private:
	void initUI();

private:
	UILabel *m_pNameTxt;

	UILabel *m_pLevelTxt;

	UILabel *m_pPrivilegeTxt;

	UILabel *m_pLastLoginTxt;

	vmsg::CSGuildMember m_stMemberInfo;

};

#endif // GuildAdminMemberItem_h__