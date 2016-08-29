#ifndef NormalListTab_h__
#define NormalListTab_h__

#include "BaseLayout.h"

class CNormalListTab : public BaseLayout
{
public:
	CNormalListTab();
	virtual ~CNormalListTab();

	static CNormalListTab* create(UILayout *pRefLayout);

	virtual bool initWithLayout(UILayout *pRefLayout);

	void updateUI();
	void updateMyRank();
	void updateGift();

	void startCountDown();
	void clearCountDown();
protected:
private:
	void touchEventHandler(CCObject* pSender, TouchEventType type);
	void downTime(float dt);
private:
	UIImageView* m_pTitleBg;
	UILabel* m_pTitleNameTxt;
	UILabel* m_pTitlePowerTxt;

	UIPanel* m_pAwardLayer;
	UILabel* m_pCoinTxt;
	UILabel* m_pArenaCoinTxt;
	UILabel* m_pTimeTxt;
	UILabel* m_pTipTxt;

	UIImageView* m_pMyRankBg;
	UIImageView* m_pMyPowerBg;
	UILabel* m_pMyRankTxt;
	UILabel* m_pMyPowerLabel;
	UILabel* m_pMyPowerTxt;

	int _totalTime;

	CC_PROPERTY(unsigned int, _rankType, RankType);
};

#endif // NormalListTab_h__
