#ifndef ArenaPanel_h__
#define ArenaPanel_h__

#include "BasePanel.h"

#include "game/guide/GuideControl.h"

class CArenaPanel : public BasePanel, public GuideContainer
{
public:
	CArenaPanel();
	virtual ~CArenaPanel();

	CREATE_FUNC(CArenaPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);
	
	void updateUI();

	void updateNum(unsigned int fightNum, unsigned int buyNum);
	void updateMyRank(unsigned int rank);
	void updateMatchCost();
	void updateAwardBtn();

	void updateCleanCD();

	void open();
	void close();
protected:
private:
	void initUI();
	void buyFun();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void cleanFun();

	void timeCountDown(float dt);
private:
	UIButton* m_pRuleBtn;
	UIButton* m_pRankBtn;
	UIButton* m_pBuyBtn;
	UIButton* m_pMatchBtn;
	UIButton* m_pRecordBtn;
	UIButton* m_pGetBtn;
    UIButton* m_pExchangeBtn;
	UIButton* m_pBackBtn;
	UIButton* m_pCleanBtn;

	UILabel* m_pNumTxt;
	UILabel* m_pScoreTxt;
	UILabel* m_pRecordTxt;
	UILabel* m_pPowerTxt;
	UILabel* m_pAwardCoinTxt;
	UILabel* m_pArenaCoinTxt;
	UILabel* m_pMatchCostTxt;
    UILabel* m_pGongxunTxt; //ÎÒµÄ¹¦Ñ«
	UILabel* m_pMyRankTxt;
	UILabel* m_pTimeTxt;

	ImageView* m_pRoleBgImg;
	ImageView* m_pGetImg;

	UIPanel* m_pTimeLayout;
	UIPanel* m_pInfoLayer;

	unsigned int m_uTimeCount;
};

#endif // ArenaPanel_h__
