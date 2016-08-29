#ifndef EvolutionPanel_h__
#define EvolutionPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class EvolutionPanel: public BasePanel, public GuideContainer
{
public:
	enum TabType {
		TAB_EVOLUTION, // 进化
		TAB_SET_SKILL, // 设置技能
		TAB_MIN = TAB_EVOLUTION,
		TAB_MAX = TAB_SET_SKILL,
	};
public:
	EvolutionPanel();
	virtual ~EvolutionPanel();
	
	static EvolutionPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void onExit();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	// override end

	void switchTab(unsigned int uTabType);

	void updateUI();

	void updateEvolution();

	void updateSkillList();

	void showSkill(unsigned int uSkillIdx);

	void updateStone();

	void evolve();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void pageViewEventHandler(CCObject *pSender, PageViewEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	std::string getAddText(unsigned int uAddVal);

	std::string getDodgeCDText(unsigned int uDodgeVal);

private:
	UIButton *m_pBackBtn;

	UIListView *m_pTabListView;

	UILayout *m_pEvolutionLayout; // 进化信息

	UIPageView *m_pStarPage;

	UILabel *m_pPageNumTxt;

	UILabel *m_pNeedLevelTxt;

	UILabel *m_pNeedCoinValTxt;

	UILabel *m_pNeedPowerStoneValTxt;

	UIButton *m_pEvolutionBtn;

	UILayout *m_pSetSkillLayout; // 设置进化技

	UIImageView *m_pSkillIconImg;

	UILabel *m_pSkillDescValTxt;

	UILoadingBar *m_pSkillExpProgress;

	UILabel *m_pSkillExpProgressTxt;

	UIButton *m_pSetSkillBtn;

	UILayout *m_pAttrAddLayout; // 属性加成

	UILabel *m_pHpValTxt;

	UILabel *m_pPowerValTxt;

	UILabel *m_pAttackValTxt;

	UILabel *m_pDefenseValTxt;

	UILabel *m_pDodgeDistanceValTxt;

	UILayout *m_pSkillsLayout; // 进化技

	UIListView *m_pSkillListView;

	UILayout *m_pAttrTipLayout; // 加成提示

	UILabel *m_pAttrTxt;

	UILabel *m_pAttrValTxt;

	unsigned int m_uNextEvolutionID;

	unsigned int m_uNeedLevel;

	unsigned int m_uNeedCoin;

	unsigned int m_uNeedStone;

	unsigned int m_uTabType;

	int m_nTabSelectedIdx;

	int m_nSkillSelectedIdx;

	unsigned int m_uStoneCnt;

};

#endif // EvolutionPanel_h__