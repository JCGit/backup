#ifndef __FRIEND_PROPOSE_PANEL_H__
#define __FRIEND_PROPOSE_PANEL_H__

#include "basic/BasePanel.h"

class FriendProposePanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    enum 
    {
        FRIEND_LIST_ITEM_TAG = 101, //tag
    };

	FriendProposePanel();
	virtual ~FriendProposePanel();

	// override
	virtual bool init();

	virtual std::string getDescription() const;

    virtual void open();
    virtual void close();
    void updateUI();

    //overwrite
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);
    void doAdd();
private:

    CCTableView* m_pTableView;

    UIButton *m_pCloseBtn;
    UIButton *m_pAddAllBtn;

};

#endif // __FRIEND_PROPOSE_PANEL_H__
