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
        MAX_TAB_NUM = 3, //3��tab
        FRIEND_LIST_ITEM_TAG = 101, //tag
        REQ_NUM_TIP_TAG = 11, //��������������ʾ
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
    
    void updateReqNumTipTxt(); //��������������ʾ

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
    const FRIEND_DATA_LIST* getCurTabDataLst(); //��ȡ��ǰtabҳ��Ҫ��ʾ��friendlst
private:
    CCTableView* m_pTableView;

    UIButton* m_pTabBtnFriend;
    UIButton* m_pTabBtnAdmin;
    UIButton* m_pTabBtnPropose;
    UIButton* m_pAddFriendBtn; //��Ӻ��Ѱ�ť

    UIImageView* m_pCloseBtnImg;
    UILabel* m_pNumTipTxt;

    UIButton* m_pPreSelTabBtn;

    int m_iCurLstItemStat; //��ǰ�б����ʾ״̬
};

#endif // FriendPanleUI_h__
