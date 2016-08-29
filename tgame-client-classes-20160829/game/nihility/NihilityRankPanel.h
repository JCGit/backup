#ifndef NihilityRankPanel_h__
#define NihilityRankPanel_h__

#include "BasePanel.h"

const int MAX_LIST_NUM = 50;

class CNihilityRankPanel : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	CNihilityRankPanel();
	virtual ~CNihilityRankPanel();

	CREATE_FUNC(CNihilityRankPanel);

	bool init();

	void updateUI();
	void updateMyRank(int rank);

	virtual void open();
	virtual void close();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton* m_pCloseBtn;
	UIButton* m_pEnterBtn;
	UIPanel* m_pLayer;
	UILabel* m_pTipTxt;
	UILabel* m_pMyRankTxt;

	CCTableView* m_pTableView;
	UILayout* m_pRankDemo;
	unsigned int m_uCellCount;
};

#endif // NihilityRankPanel_h__
