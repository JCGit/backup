#ifndef RankPanel_h__
#define RankPanel_h__

#include "BasePanel.h"

const int RANK_MAX_NUM = 100;

class CRankPanel : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	enum TabType {
		TAB_FIGHT_POWER, // 战力
		TAB_INFINITE, // 无尽模式
		TAB_LEVEL, // 关卡
		TAB_GOLD, // 充值
	};

public:
	CRankPanel();
	virtual ~CRankPanel();

	CREATE_FUNC(CRankPanel);

	virtual bool init();

	virtual void open();

	virtual void close();

	void updateUI();
	void updateMyRank();
	void updateGift();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

	void startCountDown();
	void clearCountDown();

private:
	void initUI();

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void touchEventHandler(CCObject* pSender, TouchEventType type);

	unsigned int getCurRankType();

	void tabSelected(int nSelectedIdx);

	void updateTime(float dt);

private:
	UIListView *m_pTabListView;

	UIImageView* m_pIconImg;

	UIButton* m_pCloseBtn;

	UILabel *m_pMyRankValTxt;

	UILabel *m_pCoinValTxt;

	UILabel *m_pTimerTxt;

	int m_nTabSelectedIdx;

	CCTableView* m_pTableView;

	UILabel* m_pTipTxt;

	int _totalTime;

	unsigned int m_uCellCount;
};

#endif // RankPanel_h__
