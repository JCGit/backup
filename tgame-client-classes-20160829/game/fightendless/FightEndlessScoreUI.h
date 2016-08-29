#ifndef FightEndlessScoreUI_h__
#define FightEndlessScoreUI_h__

#include "BaseLayout.h"

class CFightEndlessScoreUI : public BaseLayout
{
public:
	CFightEndlessScoreUI();
	virtual ~CFightEndlessScoreUI();

	CREATE_FUNC(CFightEndlessScoreUI);

	bool init();

	void setInfo(unsigned int stage);

	void setScore(unsigned int score);

	void setCoin(unsigned int coin);

	void setPackage(unsigned int package);
protected:
private:
	Label* m_pInfoTxt;
	Label* m_pScoreTxt;
	Label* m_pCoinTxt;
	Label* m_pPackageTxt;

	unsigned int m_uPreCoin;
};

#endif // FightEndlessScoreUI_h__
