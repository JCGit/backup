#ifndef ActNewYearGiftBagLayout_h__
#define ActNewYearGiftBagLayout_h__

#include "ActLayout.h"

class ActNewYearGiftBagLayout: public ActLayout
{
public:
	ActNewYearGiftBagLayout();
	virtual ~ActNewYearGiftBagLayout();

	static ActNewYearGiftBagLayout* create();

	// override start

	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	virtual void updateInfo(const vmsg::CSCouponActStatusItem& info);

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILabel *m_pActTimeValTxt;

	UIButton *m_pGreetingBtn;

	CCArmature *m_pGreetingEff;

};

#endif // ActNewYearGiftBagLayout_h__