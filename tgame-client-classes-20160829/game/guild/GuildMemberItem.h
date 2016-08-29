#ifndef GuildMemberItem_h__
#define GuildMemberItem_h__

#include "basic/BaseLayout.h"
#include "basic/BasicTypes.h"
#include "CSGuildMsg.pb.h"

class GuildMemberItem: public BaseLayout
{
public:
	GuildMemberItem();
	virtual ~GuildMemberItem();

	static GuildMemberItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSGuildMember &stMember);

	//void updateUI(const vmsg::CSGuildReq &stReq);

	//bool isSelected();

	//void selected(bool bSelected);

	UIN_t getMemberUIN();

	//void enableCheckBox(bool enable);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILabel *m_pPrivilegeTxt;

	UILabel *m_pPlayerNameTxt;

	UIImageView *m_pPlayerHeadImg;

	UILabelAtlas *m_pPlayerLevelTxt;

	UIButton *m_pImpeachBtn;

	//UIButton *m_pCheckBoxBtn;

	bool m_bSelected;

	UIN_t m_uin;

};

#endif // GuildMemberItem_h__