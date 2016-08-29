#ifndef PlayerListItem_h__
#define PlayerListItem_h__

#include "basic/BaseLayout.h"
#include "CSGuildMsg.pb.h"
#include "basic/BasicTypes.h"

class PlayerListItem: public BaseLayout
{
public:
	PlayerListItem();
	virtual ~PlayerListItem();
	
	static PlayerListItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSGuildReq &stGuildRqst);

	bool isSelected();

	void selected(bool bSelected);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIImageView *m_pBgSelectedImg;

	UILabel *m_pNameTxt;

	UILabel *m_pLevelTxt;

	UIButton *m_pLookupBtn;

	bool m_bSelected;

	UIN_t m_uin;

};

#endif // PlayerListItem_h__