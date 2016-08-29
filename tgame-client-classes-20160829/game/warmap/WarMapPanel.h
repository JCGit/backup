#ifndef WarMapPanel_h__
#define WarMapPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class IconFrameCell;

class WarMapPanel: public BasePanel, public GuideContainer
{
public:
	enum WarTab {
		WAR_TAB_NORMAL, // ��ͨ����
		WAR_TAB_ELITE, // ��Ӣ����
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

	void matchTask(bool bMatchTask); // ƥ������ָ��ǰ�����Ӧ�Ĺؿ�

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void pageViewEventHandler(CCObject *pSender, PageViewEventType type);

	void updatePageStat(); // ����ҳ��״̬

	void handlePageTurningLogic(); // ����ҳ�߼�

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

	unsigned int m_uAwardStarCnt; // ������Ӧ��������

	unsigned int m_uNextStarCnt; // ��һ����Ҫ��������

	bool m_bMatchTask;

	bool m_isBeFresh;//�Ƿ��Ѿ�ˢ��

	int m_pElitePageShow;//��Ҫ��ʾ�ľ�Ӣ�����½�
};

#endif // WarMapPanel_h__