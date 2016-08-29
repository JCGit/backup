#ifndef __FRIEND_HELP_PANEL_H__
#define __FRIEND_HELP_PANEL_H__

#include "basic/BasePanel.h"
#include "CSFriendMsg.pb.h"
#include <vector>
using namespace std;
class FriendHelpItem;

class FriendHelpPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    enum 
    {
        FRIEND_LIST_ITEM_TAG = 101, //tag
    };

    FriendHelpPanel();
    virtual ~FriendHelpPanel();

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
    void doStartFight();
    bool static compareFriend(const vmsg::CSOneFriend& aFriend, const vmsg::CSOneFriend& bFriend);
private:
    std::vector<vmsg::CSOneFriend> m_stHelpFriendLst;

    CCTableView* m_pTableView;

    UIButton *m_pCloseBtn;
    UIButton *m_pGoFightBtn;
    UIButton *m_pGetFriendBtn; //»ñÈ¡ÍÆ¼ö

    UILabel* m_pNoFriendTip;

    FriendHelpItem* m_pSelectedItem;
};

#endif // __FRIEND_HELP_PANEL_H__
