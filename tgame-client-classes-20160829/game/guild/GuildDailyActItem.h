#ifndef GuildDailyActItem_h__
#define GuildDailyActItem_h__

#include "basic/BaseLayout.h"

class GuildDailyActItem: public BaseLayout
{
public:
	GuildDailyActItem();
	virtual ~GuildDailyActItem();

	static GuildDailyActItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

	bool isSelected();

	void selected(bool bSelected);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILabel *m_pActNameTxt;

	UILabel *m_pRemainTimesValTxt;

	UIButton *m_pGotoBtn;

	bool m_bSelected;

};

#endif // GuildDailyActItem_h__