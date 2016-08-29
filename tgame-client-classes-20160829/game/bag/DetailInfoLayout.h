#ifndef DetailInfoLayout_h__
#define DetailInfoLayout_h__

#include "basic/BaseLayout.h"

#include "CSPlayerMsg.pb.h"

class ImageNumber;

class DetailInfoLayout: public BaseLayout
{
public:
	DetailInfoLayout();
	virtual ~DetailInfoLayout();

	static DetailInfoLayout* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	void updateUI();

	void updateUI(const vmsg::CSPlayerInfo &stPlayerInfo, const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UILabel *m_pRoleNameTxt;

	ImageNumber *m_pVIPTxt;

	UILabel *m_pLevelValTxt;

	UILabel *m_pHpValTxt;

	UILoadingBar *m_pExpProgress;

	UILabel *m_pExpProgressTxt;

	UILabel *m_pFightPowerValTxt;

	UILabel *m_pUINValTxt;

	UILabel *m_pDefValTxt;

	UILabel *m_pAtkValTxt;

	UILabel *m_pHitValTxt;

	UILabel *m_pAtkTypeValTxt;

	UILabel *m_pDefTypeValTxt;

	UILabel *m_pThunderAtkValTxt;

	UILabel *m_pPoisonAtkValTxt;

	UILabel *m_pFireAtkValTxt;

	UILabel *m_pIceAtkValTxt;

	UILabel *m_pBiochemistryAtkValTxt;

	UILabel *m_pThunderDefValTxt;

	UILabel *m_pPoisonDefValTxt;

	UILabel *m_pFireDefValTxt;

	UILabel *m_pIceDefValTxt;

	UILabel *m_pBiochemistryDefValTxt;

};

#endif // DetailInfoLayout_h__