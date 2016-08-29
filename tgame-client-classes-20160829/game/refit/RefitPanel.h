#ifndef RefitPanel_h__
#define RefitPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class RefitPanel: public BasePanel, public GuideContainer
{
public:
	enum TabType {
		TAB_WEAPON, // 武器
		TAB_ARMOR, // 甲具
		TAB_SHOES, // 鞋子
		TAB_CLOAK, // 披风
		TAB_AMULET, // 护符
	};

public:
	RefitPanel();
	virtual ~RefitPanel();

	static RefitPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void refresh();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	// override end

	void switchTab(TabType type);

	void updateUI();

	void refitDone(); //  改装完成

	void badgeTabBtn(unsigned int uPart, unsigned int uBadgeNum);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void onEffDone(CCArmature *pArmature, MovementEventType type, const char *pszActionName);

	TabType idx2TabType(int idx);

	unsigned int idx2Part(int idx);

	int part2Idx(unsigned int uPart);

private:
	UIButton *m_pBackBtn;

	UIListView *m_pTabListView;

	UIImageView *m_pBigIconImg;

	UILayout *m_pCellsLayout;

	UILabel *m_pRefitLevelTxt;

	UILabel *m_pAtkValTxt;

	UILabel *m_pDefValTxt;

	UILabel *m_pHpValTxt;

	UILabel *m_pHitValTxt;

	UILabel *m_pCostValTxt;

	UIButton *m_pRefitBtn;

	int m_nTabSelectedIdx;

	int m_nRefitEffCnt;

};

#endif // RefitPanel_h__