#ifndef GuildJoinReplyItem_h__
#define GuildJoinReplyItem_h__

#include "basic/BaseLayout.h"
#include "CSGuildMsg.pb.h"

class GuildJoinReplyItem: public BaseLayout
{
public:
	GuildJoinReplyItem();
	virtual ~GuildJoinReplyItem();

	static GuildJoinReplyItem* create();

	// override start

	virtual bool init();

	// override end

	bool isSelected();

	void selected(bool bSelected);

	void setInfo(const vmsg::CSGuildReq &stGuildRqst);

private:
	void initUI();

private:
	bool m_bSelected;

	UILabel *m_pNameTxt;

	UILabel *m_pLevelTxt;

	UICheckBox *m_pCheckboxBtn;

};

#endif // GuildJoinReplyItem_h__