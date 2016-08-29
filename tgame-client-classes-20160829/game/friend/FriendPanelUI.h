#ifndef FriendPanleUI_h__
#define FriendPanleUI_h__

#include "basic/BasePanel.h"
#include "CSFriendMsg.pb.h"

#include "game/guide/GuideControl.h"

typedef ::google::protobuf::RepeatedPtrField< ::vmsg::CSOneFriend > FRIEND_DATA_LIST;

class FriendPanelUI: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:
    enum 
    {
        MAX_TAB_NUM = 3, //3个tab
        FRIEND_LIST_ITEM_TAG = 101, //tag
        REQ_NUM_TIP_TAG = 11, //好友申请数量提示
    };
    FriendPanelUI();
    virtual ~FriendPanelUI();

    // override
    virtual bool init();
    virtual void open();
    virtual void close();

	virtual CCNode* getGuideNodeByName(const std::string &name);

    void updateUI(); 
    void updateProposeRsp();
    
    void updateReqNumTipTxt(); //好友申请数量提示

    //overwrite
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);
    void setSelTabBtn(UIButton* pSelBtn);
	void onTableViewReload(float dt);
    const FRIEND_DATA_LIST* getCurTabDataLst(); //获取当前tab页需要显示的friendlst
private:
    CCTableView* m_pTableView;

    UIButton* m_pTabBtnFriend;
    UIButton* m_pTabBtnAdmin;
    UIButton* m_pTabBtnPropose;
    UIButton* m_pAddFriendBtn; //添加好友按钮

    UIImageView* m_pCloseBtnImg;
    UILabel* m_pNumTipTxt;

    UIButton* m_pPreSelTabBtn;

    int m_iCurLstItemStat; //当前列表的显示状态
};

#endif // FriendPanleUI_h__
