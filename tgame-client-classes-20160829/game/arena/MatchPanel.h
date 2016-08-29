#ifndef MatchPanel_h__
#define MatchPanel_h__

#include "BasePanel.h"

#include "game/guide/GuideControl.h"

class CMatchPanel : public BasePanel, public GuideContainer
{
public:
	CMatchPanel();
	virtual ~CMatchPanel();

	CREATE_FUNC(CMatchPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	void updateUI();
	void updateMatchCost();

	void open();
	void close();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void matchFailFun();			//∆•≈‰ ß∞‹
private:
	UIButton* m_pCloseBtn;
	UIButton* m_pHandBtn;
	UIButton* m_pAutoBtn;
	UIButton* m_pRematchBtn;

	ImageView* m_pHeadImg;

	UILabel* m_pNameTxt;
	UILabel* m_pRankTxt;
	UILabel* m_pPowerTxt;
	UILabel* m_pMatchCostTxt;
	UILabel* m_pMatchLevelTxt;
};

#endif // MatchPanel_h__
