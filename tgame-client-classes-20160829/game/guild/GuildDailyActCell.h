#ifndef GuildDailyActCell_h__
#define GuildDailyActCell_h__

#include "basic/BaseLayout.h"

class GuildDailyActCell: public BaseLayout
{
public:
	GuildDailyActCell();
	virtual ~GuildDailyActCell();
	
	static GuildDailyActCell* create();

	// override start

	virtual bool init();

	// override end

	void updateUI();

	bool isSelected();

	void selected(bool bSelected);

private:
	UILabel *m_pActTitleTxt;

	UILabel *m_pRemainTimesTxt;

	UILabel *m_pRemainTimesValTxt;

	UILabel *m_pAwardTxt;

	UILabel *m_pAwardValTxt;

	UIImageView *m_pBgSelectedImg;

	bool m_bSelected;

};

#endif // GuildDailyActCell_h__