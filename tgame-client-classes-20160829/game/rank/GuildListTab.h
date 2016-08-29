#ifndef GuildListTab_h__
#define GuildListTab_h__

#include "BaseLayout.h"

class CGuildListTab : public BaseLayout
{
public:
	CGuildListTab();
	virtual ~CGuildListTab();

	static CGuildListTab* create(UILayout *pRefLayout);

	virtual bool initWithLayout(UILayout *pRefLayout);

	void updateUI();
	void updateMyRank();
protected:
private:
	UIImageView* m_pMyRankBg;
	UIImageView* m_pMyPowerBg;
	UILabel* m_pMyRankTxt;
	UILabel* m_pMyPowerTxt;
};

#endif // GuildListTab_h__
