#include "cocos2d.h"
#include "TUITableView.h"
#include "support/CCPointExtension.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"

TUITableView* TUITableView::create(TUITableViewDataSource* dataSource, CCSize size)
{
	TUITableView *table = new TUITableView();
	table->initWithViewSize(size);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateCellPositions();
	table->_updateContentSize();

	return table;
}

bool TUITableView::initWithViewSize(CCSize size)
{
	if (ScrollView::init())
	{
		m_pCellsUsed      = new CCArrayForObjectSorting();
		m_pCellsFreed     = new CCArrayForObjectSorting();
		m_pIndices        = new std::set<unsigned int>();
		m_eVordering      = kTUITableViewFillBottomUp;
		this->setDirection(SCROLLVIEW_DIR_VERTICAL);

		setInnerContainerSize(size);

		//CCScrollView::setDelegate(this);
		return true;
	}
	return false;
}

TUITableView::TUITableView()
: m_pTouchedCell(NULL)
, m_pIndices(NULL)
, m_pCellsUsed(NULL)
, m_pCellsFreed(NULL)
, m_pDataSource(NULL)
, m_pTableViewDelegate(NULL)
, m_eOldDirection(SCROLLVIEW_DIR_NONE)
{

}

TUITableView::~TUITableView()
{
	CC_SAFE_DELETE(m_pIndices);
	CC_SAFE_RELEASE(m_pCellsUsed);
	CC_SAFE_RELEASE(m_pCellsFreed);
	//unregisterAllScriptHandler();
}

void TUITableView::setVerticalFillOrder(TUITableViewVerticalFillOrder fillOrder)
{
	if (m_eVordering != fillOrder) {
		m_eVordering = fillOrder;
		if (m_pCellsUsed->count() > 0) {
			this->reloadData();
		}
	}
}

TUITableViewVerticalFillOrder TUITableView::getVerticalFillOrder()
{
	return m_eVordering;
}

void TUITableView::reloadData()
{
	m_eOldDirection = SCROLLVIEW_DIR_NONE;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_pCellsUsed, pObj)
	{
		TUITableViewCell* cell = (TUITableViewCell*)pObj;

		if(m_pTableViewDelegate != NULL) {
			m_pTableViewDelegate->tableCellWillRecycle(this, cell);
		}

		m_pCellsFreed->addObject(cell);
		cell->reset();
		if (cell->getParent() == this->getInnerContainer())
		{
			this->getInnerContainer()->removeChild(cell, true);
		}
	}

	m_pIndices->clear();
	m_pCellsUsed->release();
	m_pCellsUsed = new CCArrayForObjectSorting();

	this->_updateCellPositions();
	this->_updateContentSize();
	if (m_pDataSource->numberOfCellsInTableView(this) > 0)
	{
		this->scrollViewDidScroll(this);
	}
}

TUITableViewCell *TUITableView::cellAtIndex(unsigned int idx)
{
	TUITableViewCell *found = NULL;

	if (m_pIndices->find(idx) != m_pIndices->end())
	{
		found = (TUITableViewCell *)m_pCellsUsed->objectWithObjectID(idx);
	}

	return found;
}

void TUITableView::updateCellAtIndex(unsigned int idx)
{
	if (idx == CC_INVALID_INDEX)
	{
		return;
	}
	unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
	if (0 == uCountOfItems || idx > uCountOfItems-1)
	{
		return;
	}

	TUITableViewCell* cell = this->cellAtIndex(idx);
	if (cell)
	{
		this->_moveCellOutOfSight(cell);
	}
	cell = m_pDataSource->tableCellAtIndex(this, idx);
	this->_setIndexForCell(idx, cell);
	this->_addCellIfNecessary(cell);
}

void TUITableView::insertCellAtIndex(unsigned  int idx)
{
	if (idx == CC_INVALID_INDEX)
	{
		return;
	}

	unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
	if (0 == uCountOfItems || idx > uCountOfItems-1)
	{
		return;
	}

	TUITableViewCell* cell = NULL;
	int newIdx = 0;

	cell = (TUITableViewCell*)m_pCellsUsed->objectWithObjectID(idx);
	if (cell)
	{
		newIdx = m_pCellsUsed->indexOfSortedObject(cell);
		for (unsigned int i=newIdx; i<m_pCellsUsed->count(); i++)
		{
			cell = (TUITableViewCell*)m_pCellsUsed->objectAtIndex(i);
			this->_setIndexForCell(cell->getIdx()+1, cell);
		}
	}

	//[m_pIndices shiftIndexesStartingAtIndex:idx by:1];
	//insert a new cell
	cell = m_pDataSource->tableCellAtIndex(this, idx);
	this->_setIndexForCell(idx, cell);
	this->_addCellIfNecessary(cell);

	this->_updateCellPositions();
	this->_updateContentSize();
}

void TUITableView::removeCellAtIndex(unsigned int idx)
{
	if (idx == CC_INVALID_INDEX)
	{
		return;
	}

	unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
	if (0 == uCountOfItems || idx > uCountOfItems-1)
	{
		return;
	}

	unsigned int newIdx = 0;

	TUITableViewCell* cell = this->cellAtIndex(idx);
	if (!cell)
	{
		return;
	}

	newIdx = m_pCellsUsed->indexOfSortedObject(cell);

	//remove first
	this->_moveCellOutOfSight(cell);

	m_pIndices->erase(idx);
	this->_updateCellPositions();
	//[m_pIndices shiftIndexesStartingAtIndex:idx+1 by:-1];
	for (unsigned int i=m_pCellsUsed->count()-1; i > newIdx; i--)
	{
		cell = (TUITableViewCell*)m_pCellsUsed->objectAtIndex(i);
		this->_setIndexForCell(cell->getIdx()-1, cell);
	}
}

TUITableViewCell *TUITableView::dequeueCell()
{
	TUITableViewCell *cell;

	if (m_pCellsFreed->count() == 0) {
		cell = NULL;
	} else {
		cell = (TUITableViewCell*)m_pCellsFreed->objectAtIndex(0);
		cell->retain();
		m_pCellsFreed->removeObjectAtIndex(0);
		cell->autorelease();
	}
	return cell;
}

void TUITableView::_addCellIfNecessary(TUITableViewCell * cell)
{
	if (cell->getParent() != this->getInnerContainer())
	{
		this->getInnerContainer()->addChild(cell);
	}
	m_pCellsUsed->insertSortedObject(cell);
	m_pIndices->insert(cell->getIdx());
	// [m_pIndices addIndex:cell.idx];
}

void TUITableView::_updateContentSize()
{
	CCSize size = CCSizeZero;
	unsigned int cellsCount = m_pDataSource->numberOfCellsInTableView(this);

	if (cellsCount > 0)
	{
		float maxPosition = m_vCellsPositions[cellsCount];

		switch (this->getDirection())
		{
		case SCROLLVIEW_DIR_VERTICAL:
			size = CCSizeMake(maxPosition, getInnerContainerSize().height);
			break;
		default:
			size = CCSizeMake(getInnerContainerSize().width, maxPosition);
			break;
		}
	}

	this->setContentSize(size);

	if (m_eOldDirection != _direction)
	{
		if (_direction == SCROLLVIEW_DIR_VERTICAL)
		{
			this->scrollChildren(0,0);		//todo
		}
		else
		{
			this->scrollChildren(0,0);		//todo
		}
		m_eOldDirection = _direction;
	}
}

CCPoint TUITableView::_offsetFromIndex(unsigned int index)
{
	CCPoint offset = this->__offsetFromIndex(index);

	const CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, index);
	if (m_eVordering == kTUITableViewFillTopDown)
	{
		offset.y = this->getInnerContainer()->getContentSize().height - offset.y - cellSize.height;
	}
	return offset;
}

CCPoint TUITableView::__offsetFromIndex(unsigned int index)
{
	CCPoint offset;
	CCSize  cellSize;

	switch (this->getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		offset = ccp(m_vCellsPositions[index], 0.0f);
		break;
	default:
		offset = ccp(0.0f, m_vCellsPositions[index]);
		break;
	}

	return offset;
}

unsigned int TUITableView::_indexFromOffset(CCPoint offset)
{
	int index = 0;
	const int maxIdx = m_pDataSource->numberOfCellsInTableView(this)-1;

	if (m_eVordering == kTUITableViewFillTopDown)
	{
		offset.y = this->getInnerContainer()->getContentSize().height - offset.y;
	}
	index = this->__indexFromOffset(offset);
	if (index != -1)
	{
		index = MAX(0, index);
		if (index > maxIdx)
		{
			index = CC_INVALID_INDEX;
		}
	}

	return index;
}

int TUITableView::__indexFromOffset(CCPoint offset)
{
	int low = 0;
	int high = m_pDataSource->numberOfCellsInTableView(this) - 1;
	float search;
	switch (this->getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		search = offset.x;
		break;
	default:
		search = offset.y;
		break;
	}

	while (high >= low)
	{
		int index = low + (high - low) / 2;
		float cellStart = m_vCellsPositions[index];
		float cellEnd = m_vCellsPositions[index + 1];

		if (search >= cellStart && search <= cellEnd)
		{
			return index;
		}
		else if (search < cellStart)
		{
			high = index - 1;
		}
		else
		{
			low = index + 1;
		}
	}

	if (low <= 0) {
		return 0;
	}

	return -1;
}

void TUITableView::_moveCellOutOfSight(TUITableViewCell *cell)
{
	if(m_pTableViewDelegate != NULL) {
		m_pTableViewDelegate->tableCellWillRecycle(this, cell);
	}

	m_pCellsFreed->addObject(cell);
	m_pCellsUsed->removeSortedObject(cell);
	m_pIndices->erase(cell->getIdx());
	// [m_pIndices removeIndex:cell.idx];
	cell->reset();
	if (cell->getParent() == this->getInnerContainer()) {
		this->getInnerContainer()->removeChild(cell, true);;
	}
}

void TUITableView::_setIndexForCell(unsigned int index, TUITableViewCell *cell)
{
	cell->setAnchorPoint(ccp(0.0f, 0.0f));
	cell->setPosition(this->_offsetFromIndex(index));
	cell->setIdx(index);
}

void TUITableView::_updateCellPositions() {
	int cellsCount = m_pDataSource->numberOfCellsInTableView(this);
	m_vCellsPositions.resize(cellsCount + 1, 0.0);

	if (cellsCount > 0)
	{
		float currentPos = 0;
		CCSize cellSize;
		for (int i=0; i < cellsCount; i++)
		{
			m_vCellsPositions[i] = currentPos;
			cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
			switch (this->getDirection())
			{
			case SCROLLVIEW_DIR_HORIZONTAL:
				currentPos += cellSize.width;
				break;
			default:
				currentPos += cellSize.height;
				break;
			}
		}
		m_vCellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
	}

}

void TUITableView::scrollViewDidScroll(ScrollView* view)
{
	unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
	if (0 == uCountOfItems)
	{
		return;
	}

	if(m_pTableViewDelegate != NULL) {
		//m_pTableViewDelegate->scrollViewDidScroll(this);
	}

	unsigned int startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
	CCPoint offset = ccpMult(this->getInnerContainer()->getPosition(), -1);
	maxIdx = MAX(uCountOfItems-1, 0);

	if (m_eVordering == kTUITableViewFillTopDown)
	{
		offset.y = offset.y + this->getInnerContainer()->getContentSize().height/this->getInnerContainer()->getScaleY();
	}
	startIdx = this->_indexFromOffset(offset);
	if (startIdx == CC_INVALID_INDEX)
	{
		startIdx = uCountOfItems - 1;
	}

	if (m_eVordering == kTUITableViewFillTopDown)
	{
		offset.y -= this->getInnerContainer()->getContentSize().height/this->getInnerContainer()->getScaleY();
	}
	else
	{
		offset.y += this->getInnerContainer()->getContentSize().height/this->getInnerContainer()->getScaleY();
	}
	offset.x += this->getInnerContainer()->getContentSize().width/this->getInnerContainer()->getScaleX();

	endIdx   = this->_indexFromOffset(offset);
	if (endIdx == CC_INVALID_INDEX)
	{
		endIdx = uCountOfItems - 1;
	}

#if 0 // For Testing.
	CCObject* pObj;
	int i = 0;
	CCARRAY_FOREACH(m_pCellsUsed, pObj)
	{
		CCTableViewCell* pCell = (CCTableViewCell*)pObj;
		CCLog("cells Used index %d, value = %d", i, pCell->getIdx());
		i++;
	}
	CCLog("---------------------------------------");
	i = 0;
	CCARRAY_FOREACH(m_pCellsFreed, pObj)
	{
		CCTableViewCell* pCell = (CCTableViewCell*)pObj;
		CCLog("cells freed index %d, value = %d", i, pCell->getIdx());
		i++;
	}
	CCLog("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

	if (m_pCellsUsed->count() > 0)
	{
		TUITableViewCell* cell = (TUITableViewCell*)m_pCellsUsed->objectAtIndex(0);

		idx = cell->getIdx();
		while(idx <startIdx)
		{
			this->_moveCellOutOfSight(cell);
			if (m_pCellsUsed->count() > 0)
			{
				cell = (TUITableViewCell*)m_pCellsUsed->objectAtIndex(0);
				idx = cell->getIdx();
			}
			else
			{
				break;
			}
		}
	}
	if (m_pCellsUsed->count() > 0)
	{
		TUITableViewCell *cell = (TUITableViewCell*)m_pCellsUsed->lastObject();
		idx = cell->getIdx();

		while(idx <= maxIdx && idx > endIdx)
		{
			this->_moveCellOutOfSight(cell);
			if (m_pCellsUsed->count() > 0)
			{
				cell = (TUITableViewCell*)m_pCellsUsed->lastObject();
				idx = cell->getIdx();

			}
			else
			{
				break;
			}
		}
	}

	for (unsigned int i=startIdx; i <= endIdx; i++)
	{
		//if ([m_pIndices containsIndex:i])
		if (m_pIndices->find(i) != m_pIndices->end())
		{
			continue;
		}
		this->updateCellAtIndex(i);
	}
}

void TUITableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!this->isVisible()) {
		return;
	}

	if (m_pTouchedCell){
		CCRect bb = this->boundingBox();
		bb.origin = m_pParent->convertToWorldSpace(bb.origin);

		if (bb.containsPoint(pTouch->getLocation()) && m_pTableViewDelegate != NULL)
		{
			//neilwu comment,touch点是相对于屏幕左下角原点的全局坐标
			//在m_pTableViewDelegate或m_pTouchedCell里根据需要，直接调用hitTest方法来判断是否点击
			m_pTableViewDelegate->m_stGlobalTouchPt = pTouch->getLocation(); 

			m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
			m_pTableViewDelegate->tableCellTouched(this, m_pTouchedCell);
		}

		m_pTouchedCell = NULL;
	}

	ScrollView::onTouchEnded(pTouch, pEvent);
}

bool TUITableView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!this->isVisible()) {
		return false;
	}

	bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);

	unsigned int        index;
	CCPoint           point;

	point = this->getInnerContainer()->convertTouchToNodeSpace(pTouch);

	index = this->_indexFromOffset(point);
	if (index == CC_INVALID_INDEX)
	{
		m_pTouchedCell = NULL;
	}
	else
	{
		m_pTouchedCell  = this->cellAtIndex(index);
	}

	if (m_pTouchedCell && m_pTableViewDelegate != NULL) {
		m_pTableViewDelegate->tableCellHighlight(this, m_pTouchedCell);
	}

	return touchResult;
}

void TUITableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	ScrollView::onTouchMoved(pTouch, pEvent);

	if (m_pTouchedCell) {
		if(m_pTableViewDelegate != NULL) {
			m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
		}

		m_pTouchedCell = NULL;
	}
}

void TUITableView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	ScrollView::onTouchCancelled(pTouch, pEvent);

	if (m_pTouchedCell) {
		if(m_pTableViewDelegate != NULL) {
			m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
		}

		m_pTouchedCell = NULL;
	}
}

//void TUITableView::scrollToCellIndex(int index) {
//	CCPoint offset = ccpSub(CCPointZero, _offsetFromIndex(index));
//	float newX, newY;
//	CCPoint maxInset, minInset;
//
//	maxInset = this->maxContainerOffset();
//	minInset = this->minContainerOffset();
//
//	//check to see if offset lies within the inset bounds
//	newX     = MIN(offset.x, maxInset.x);
//	newX     = MAX(newX, minInset.x);
//	newY     = MIN(offset.y, maxInset.y);
//	newY     = MAX(newY, minInset.y);
//
//	setContentOffset(ccp(newX, newY), true);
//}


//void TUITableView::unregisterAllScriptHandler()
//{
//	unregisterScriptHandler(kTUITableViewScroll);
//	unregisterScriptHandler(kTUITableViewZoom);
//	unregisterScriptHandler(kTUITableCellTouched);
//	unregisterScriptHandler(kTUITableCellHighLight);
//	unregisterScriptHandler(kTUITableCellUnhighLight);
//	unregisterScriptHandler(kTUITableCellWillRecycle);
//	unregisterScriptHandler(kTUITableCellSizeForIndex);
//	unregisterScriptHandler(kTUITableCellSizeAtIndex);
//	unregisterScriptHandler(kNumberOfCellsInTUITableView);
//}



