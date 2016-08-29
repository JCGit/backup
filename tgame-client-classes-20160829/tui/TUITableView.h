#ifndef TUITableView_h__
#define TUITableView_h__

#include "CocoStudio/GUI/UIWidgets/ScrollWidget/UIScrollView.h"
#include "TUITableViewCell.h"

#include <set>
#include <vector>

class TUITableView;

typedef enum {
	kTUITableViewFillTopDown,
	kTUITableViewFillBottomUp
} TUITableViewVerticalFillOrder;

class TUITableViewDelegate 
{
public:
	virtual void tableCellTouched(TUITableView* table, TUITableViewCell* cell) = 0;

	virtual void tableCellHighlight(TUITableView* table, TUITableViewCell* cell){};

	virtual void tableCellUnhighlight(TUITableView* table, TUITableViewCell* cell){};

	virtual void tableCellWillRecycle(TUITableView* table, TUITableViewCell* cell){};

	//add by neilwu
	CCPoint m_stGlobalTouchPt; //touche事件end时，触发的点坐标(全局坐标)
};

class TUITableViewDataSource
{
public:
	virtual ~TUITableViewDataSource() {}

	virtual CCSize tableCellSizeForIndex(TUITableView *table, unsigned int idx) {
		return cellSizeForTable(table);
	};

	virtual CCSize cellSizeForTable(TUITableView *table) {
		return CCSizeZero;
	};

	virtual TUITableViewCell* tableCellAtIndex(TUITableView *table, unsigned int idx) = 0;

	virtual unsigned int numberOfCellsInTableView(TUITableView *table) = 0;
};

class TUITableView : public ScrollView
{
public:
	TUITableView();
	virtual ~TUITableView();

	static TUITableView* create(TUITableViewDataSource* dataSource, CCSize size);

	TUITableViewDataSource* getDataSource() { return m_pDataSource; }
	void setDataSource(TUITableViewDataSource* source) { m_pDataSource = source; }

	TUITableViewDelegate* getDelegate() { return m_pTableViewDelegate; }
	void setDelegate(TUITableViewDelegate* pDelegate) { m_pTableViewDelegate = pDelegate; }

	void setVerticalFillOrder(TUITableViewVerticalFillOrder order);
	TUITableViewVerticalFillOrder getVerticalFillOrder();

	bool initWithViewSize(CCSize size);

	void updateCellAtIndex(unsigned int idx);

	void insertCellAtIndex(unsigned int idx);

	void removeCellAtIndex(unsigned int idx);

	void reloadData();

	TUITableViewCell *dequeueCell();

	TUITableViewCell *cellAtIndex(unsigned int idx);

	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view) {}

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);


	//void scrollToCellIndex(int index); //滚动到指定的index,neilwu add

protected:

	TUITableViewCell *m_pTouchedCell;
    
    TUITableViewVerticalFillOrder m_eVordering;

	std::set<unsigned int>* m_pIndices;

	std::vector<float> m_vCellsPositions;

	CCArrayForObjectSorting* m_pCellsUsed;

	CCArrayForObjectSorting* m_pCellsFreed;

	TUITableViewDataSource* m_pDataSource;

	TUITableViewDelegate* m_pTableViewDelegate;

	SCROLLVIEW_DIR m_eOldDirection;

	int __indexFromOffset(CCPoint offset);
	unsigned int _indexFromOffset(CCPoint offset);
	CCPoint __offsetFromIndex(unsigned int index);
	CCPoint _offsetFromIndex(unsigned int index);

	void _moveCellOutOfSight(TUITableViewCell *cell);
	void _setIndexForCell(unsigned int index, TUITableViewCell *cell);
	void _addCellIfNecessary(TUITableViewCell * cell);

	void _updateCellPositions();

public:
	void _updateContentSize();

	//enum TUITableViewScriptEventType
	//{
	//	kTUITableViewScroll   = 0,
	//	kTUITableViewZoom,
	//	kTUITableCellTouched,
	//	kTUITableCellHighLight,
	//	kTUITableCellUnhighLight,
	//	kTUITableCellWillRecycle,
	//	kTUITableCellSizeForIndex,
	//	kTUITableCellSizeAtIndex,
	//	kNumberOfCellsInTUITableView,
	//};
	//void unregisterAllScriptHandler();

};


#endif // TUITableView_h__
