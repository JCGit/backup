#ifndef ArenaRankPanel_h__
#define ArenaRankPanel_h__

#include "BasePanel.h"

class CTypeTabLayout;

class CArenaRankPanel : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	CArenaRankPanel();
	virtual ~CArenaRankPanel();

	CREATE_FUNC(CArenaRankPanel);

	bool init();

	void updateUI();

	void updateMyRank();

	void open();
	void close();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
protected:
private:
	void initTabs();

	void setTabSelect(CTypeTabLayout* pTab, bool bSel);
	CTypeTabLayout* getTabByType(unsigned int uType);

	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void listViewEventHandler(CCObject *pSender, ListViewEventType type);
private:
	UIButton* m_pCloseBtn;
	UILabel* m_pMyLvTxt;
	UILabel* m_pMyRankTxt;
	UILabel* m_pTipTxt;
	ImageView* m_pRankBg;

	ListView* m_pBtnList;
	CTypeTabLayout* m_pSelectBtn;
	UILayout* m_pTabDemo;

	CCTableView* m_pTableView;
	UILayout* m_pRankDemo;

	unsigned int m_uCellCount;
};

#endif // ArenaRankPanel_h__
