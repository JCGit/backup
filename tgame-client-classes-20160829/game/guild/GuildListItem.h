#ifndef GuildListItem_h__
#define GuildListItem_h__

#include "basic/BaseLayout.h"
#include "CSGuildMsg.pb.h"

class GuildListItem: public BaseLayout
{
public:
	enum GuildStat {
		GUILD_NORM, // 普通
		GUILD_MINE, // 我的
		GUILD_FULL, // 已满
	};

public:
	GuildListItem();
	virtual ~GuildListItem();
	
	static GuildListItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSGuildBrief &stGuildInfo, bool bIsApply);

	const vmsg::CSGuildBrief& getGuildInfo();

	void setStat(GuildStat stat);

	unsigned int getStat();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIImageView *m_pGuildLevelImg;

	UILabel *m_pGuildNameTxt;

	UILabel *m_pGuildNoticeTxt;

	UILabel *m_pGuildSizeTxt;

	UIButton *m_pJoinBtn;

	UIImageView *m_pGuildStatImg;

	vmsg::CSGuildBrief m_stGuildInfo;

	unsigned int m_uStat;
};

#endif // GuildListItem_h__