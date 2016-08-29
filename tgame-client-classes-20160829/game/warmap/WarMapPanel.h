#ifndef WarMapPanel_h__
#define WarMapPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class IconFrameCell;

class WarMapPanel: public BasePanel, public GuideContainer
{
public:
	enum WarTab {
		WAR_TAB_NORMAL, // 普通副本
		WAR_TAB_ELITE, // 精英副本
	};
public:
	WarMapPanel();
	virtual ~WarMapPanel();

	static WarMapPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	virtual bool isMeetAutoOpen();

	// override end

	void switchTab(WarTab tab);
	void goToEliteByWarID(unsigned int warId);

	void updateUI();

	void matchTask(bool bMatchTask); // 匹配任务，指向当前任务对应的关卡

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void pageViewEventHandler(CCObject *pSender, PageViewEventType type);

	void updatePageStat(); // 更新页面状态

	void handlePageTurningLogic(); // 处理翻页逻辑

	void onEliteWarRefreshConfirm();

private:
	UIImageView *m_pBgImg;

	UIButton *m_pCloseBtn;

	UIImageView *m_pBgChapterTitleImg;

	UIImageView *m_pChapterTitleImg;

	UIListView *m_pTabListView;

	UIPageView *m_pWarPageView;

	UIPageView *m_pEliteWarPageView;

	UIButton *m_pPrevBtn;

	UIButton *m_pNextBtn;

	UILayout *m_pSupplyLayout;

	UIImageView *m_pBulletIconImg;

	IconFrameCell *m_pBulletIconFrameCell;

	UILabel *m_pBulletCntTxt;

	UIButton *m_pBulletSupplyBtn;

	UIImageView *m_pMedicineIconImg;

	IconFrameCell *m_pMedicineIconFrameCell;

	UILabel *m_pMedicineCntTxt;

	UIButton *m_pMedicineSupplyBtn;

	UILayout *m_pAwardLayout;

	UILabel *m_pStarCntTxt;

	UIButton *m_pStarAwardBtn;

	UILayout *m_pEliteActionLayout;

	UILabel *m_pRemainRefreshTimesValTxt;

	UIButton *m_pSweepBtn;

	UIButton *m_pRefreshBtn;

	UILayout* m_pCostLayout;

	UILabel* m_pCostTxt;

	unsigned int m_uWarTab;

	int m_nTabSelectedIdx;

	unsigned int m_uAwardStarCnt; // 奖励对应的星星数

	unsigned int m_uNextStarCnt; // 下一次需要的星星数

	bool m_bMatchTask;

	bool m_isBeFresh;//是否已经刷新

	int m_pElitePageShow;//需要显示的精英副本章节
};

#endif // WarMapPanel_h__