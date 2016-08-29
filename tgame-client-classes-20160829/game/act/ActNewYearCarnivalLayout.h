#ifndef ActNewYearCarnivalLayout_h__
#define ActNewYearCarnivalLayout_h__

#include "ActLayout.h"

class ActNewYearCarnivalLayout: public ActLayout
{
public:
	ActNewYearCarnivalLayout();
	virtual ~ActNewYearCarnivalLayout();

	static ActNewYearCarnivalLayout* create();

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

	UILayout *m_pRedEnvelopeLayout;

};

#endif // ActNewYearCarnivalLayout_h__