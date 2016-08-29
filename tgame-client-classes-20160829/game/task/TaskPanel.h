#ifndef TaskPanel_h__
#define TaskPanel_h__

#include "basic/BasePanel.h"
#include "CSTaskMsg.pb.h"
#include "../guide/GuideContainer.h"

class TaskPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:
	enum TabType {
		TAB_DAILYTASK, // 日常任务
		TAB_MILESTONE, // 成就
		TAB_MAINTASK, // 主线任务
	};

public:
	TaskPanel();
	virtual ~TaskPanel();

	static TaskPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

	void switchTab(unsigned int uTabType);

	void badgeTabBtn(unsigned int uTabType, unsigned int uBadgeNum);

	void updateTask();

	void showTask(unsigned int uTaskID);

	void updateDailyTask();

	void updateMilestone();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

	CCNode* getGuideNodeByName(const std::string &name);
private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void scheduleGuideShow(float dt);
private:
	UIButton *m_pBackBtn;

	UIListView *m_pTabListView;

	UILayout *m_pTaskLayout;

	UILabel *m_pTaskNameTxt;

	UILabel *m_pTaskDescTxt;

	UILabel *m_pRecvNPCNameValTxt;

	UILabel *m_pCommitNPCNameValTxt;

	UILabel *m_pCoinValTxt;

	UILabel *m_pExpValTxt;

	UIImageView *m_pBgAwardImg;

	UIButton *m_pTraceBtn;

	UIListView *m_pDailyTaskListView;

	UIListView *m_pMilestoneListView;

	unsigned int m_uTabType;

	int m_nTabSelectedIdx;

	CCTableView* m_pTableView;

	vector<vmsg::CSTask> m_stTaskLst;

};

#endif // TaskPanel_h__
