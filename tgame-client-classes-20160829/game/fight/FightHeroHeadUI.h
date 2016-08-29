#ifndef FightHeroHeadUI_h__
#define FightHeroHeadUI_h__

#include "BaseLayout.h"
#include "ImageNumber.h"

using namespace std;

class CFightHeroHeadUI : public BaseLayout
{
public:
	CFightHeroHeadUI();
	virtual ~CFightHeroHeadUI();

	bool init();

	CREATE_FUNC(CFightHeroHeadUI);

	void setHPPercent(int nowHP, int maxHP);
	void setEPPercent(int nowEP, int maxEP);

	const CCPoint& getHeadPos();

	void updateUI();
protected:
private:
	UILoadingBar* hpBar;
	UILoadingBar* epBar;
	UILabel* hpTxt;
	UILabel* epTxt;

	ImageView* m_pHeadImg;
	Label* m_pLvTxt;
	Layout* m_pNumLayout;
	Layout* m_pTimeLayou;
	ImageView* m_pNumImg;
	LabelAtlas* m_pMinTxt;
	LabelAtlas* m_pSceTxt;
	ImageNumber* m_pNumTxt;
};

#endif // FightHeroHeadUI_h__
