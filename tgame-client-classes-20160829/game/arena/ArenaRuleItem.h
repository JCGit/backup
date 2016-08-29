#ifndef ArenaRuleItem_h__
#define ArenaRuleItem_h__

#include "BaseLayout.h"

class CArenaRuleItem : public BaseLayout
{
public:
	CArenaRuleItem();
	virtual ~CArenaRuleItem();

	CREATE_FUNC(CArenaRuleItem);

	bool init();

	void setLv(unsigned int lv);
	unsigned int getLv(){
		return m_uLevel;
	}
protected:
private:
	UILabel* m_pLvTxt;
	UILabel* m_pCoinTxt;
	UILabel* m_pArenaCoinTxt;

	unsigned int m_uLevel;
};

#endif // ArenaRuleItem_h__
