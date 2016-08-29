#ifndef ActNewYearEnergyLayout_h__
#define ActNewYearEnergyLayout_h__

#include "ActLayout.h"

class ActNewYearEnergyLayout: public ActLayout
{
public:
	ActNewYearEnergyLayout();
	virtual ~ActNewYearEnergyLayout();

	static ActNewYearEnergyLayout* create();

	// override start

	virtual bool init();

	virtual void updateInfo(const vmsg::CSCouponActStatusItem& info);

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILabel *m_pActTimeValTxt;

	UILabel *m_pActIntroValTxt;

	UILoadingBar *m_pProgressBar;

	UIImageView *m_pProgressSliderImg;

	UILabel *m_pManualRecvValTxt;

	UIButton *m_pGetManualBtn;

};

#endif // ActNewYearEnergyLayout_h__