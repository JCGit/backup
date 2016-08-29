#include "FriendHelpPanel.h"
#include "FriendHelpItem.h"
#include "LevelObj.h"
#include "FightControl.h"
#include "FightObj.h"
#include "UserObj.h"
#include "LevelControl.h"
#include "CommonDialog.h"
#include "friend/FriendControl.h"
#include "PopTipLayer.h"
#include "TextHelper.h"
#include "CCColorUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

FriendHelpPanel::FriendHelpPanel()
{
    m_pSelectedItem = NULL;
}

FriendHelpPanel::~FriendHelpPanel()
{
}

bool FriendHelpPanel::init() {
    bool bRet = false;

    do {
        CC_BREAK_IF(!BasePanel::init());

        initLayoutFromJsonFile("ui_v2/friend_help_panel_ui.ExportJson");
        addChild(m_pRootLayout);

        m_pNoFriendTip = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("no_friend_tip"));
        m_pNoFriendTip->setVisible(false);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(FriendHelpPanel::touchEventHandler));

        m_pGoFightBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("do_auto_add_btn"));
        m_pGoFightBtn->addTouchEventListener(this, toucheventselector(FriendHelpPanel::touchEventHandler));

        m_pGetFriendBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("get_friend_btn"));
        m_pGetFriendBtn->addTouchEventListener(this, toucheventselector(FriendHelpPanel::touchEventHandler));

        //list table view
        m_pTableView = CCTableView::create(this, CCSizeMake(640, 390));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);

        m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(18, 6));
        m_pTableView->setDelegate(this);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        UIImageView* pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_inner"));
        pBgImg->addNode(m_pTableView);

        CCColorUtil::addStroke((CCLabelTTF*)m_pGoFightBtn->getTextVirtualRenderer());
        CCColorUtil::addStroke((CCLabelTTF*)m_pGetFriendBtn->getTextVirtualRenderer());

        bRet = true;
    } while(0);

    return bRet;
}

std::string FriendHelpPanel::getDescription() const {
    return "FriendHelpPanel";
}

void FriendHelpPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch(type) {
    case TOUCH_EVENT_ENDED:
        if(std::strcmp(pszWidgetName, "close_btn") == 0) {
            close();
        } else if(std::strcmp(pszWidgetName, "do_auto_add_btn") == 0) {
            doStartFight();
        } else if( std::strcmp(pszWidgetName, "get_friend_btn") == 0 ) {
            P_FRIEND_CTRL->getProposePanel()->open();
        }
        break;
    default:
        break;
    }
}


void FriendHelpPanel::open() {
    BasePanel::open();
    //P_FRIEND_CTRL->getProxy()->sendGetProposeRqst();
    m_stHelpFriendLst.clear();
    m_pSelectedItem = NULL;
}


void FriendHelpPanel::close(){
    BasePanel::close();
    if(m_pSelectedItem) {
        m_pSelectedItem->setSelected(false);
    }
    m_pSelectedItem = NULL;
    m_stHelpFriendLst.clear();

    this->closeLoading();
}


void FriendHelpPanel::updateUI() {
    if (!isOpen()) {
        return;
    }

    vmsg::CSFriendQueryRsp stMsg = P_FRIEND_CTRL->getFriendRsp();
    if(stMsg.friendlst_size() == 0){
        m_pNoFriendTip->setVisible(true);
		m_pTableView->reloadData();
        return;
    }

    m_pNoFriendTip->setVisible(false);
    m_stHelpFriendLst.clear();
    for(int i = 0; i < stMsg.friendlst_size(); i++) {
        if(stMsg.friendlst(i).uin() == P_USER_OBJ->getUserInfo()->uin){
            CCLOG("the fight friend is have myself , it's error");
            continue;
        }

        m_stHelpFriendLst.push_back(stMsg.friendlst(i));
    }

    m_pNoFriendTip->setVisible(m_stHelpFriendLst.size() <= 0);

    if(m_stHelpFriendLst.size() > 1) {
        sort(m_stHelpFriendLst.begin(), m_stHelpFriendLst.end(), compareFriend);
    }

    m_pTableView->reloadData();
}


bool FriendHelpPanel::compareFriend(const vmsg::CSOneFriend& aFriend, const vmsg::CSOneFriend& bFriend) {
    return aFriend.fightcnt() < bFriend.fightcnt();
}

void FriendHelpPanel::doStartFight() {
    //开始战斗
    if(NULL == m_pSelectedItem){
        P_LEVEL_OBJ->setIsHasFriend(false);
        P_FIGHT_OBJ->setFightType(NORMAL_FIGHT_TYPE);
        P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
    }else{
        P_LEVEL_OBJ->setIsHasFriend(true);
        P_FIGHT_OBJ->setFightType(NORMAL_FIGHT_TYPE);
        P_FIGHT_CTRL->getFightProxy()->sendInviteFriendFightRqst(m_pSelectedItem->getInfo().uin());
    }


    this->showLoading();
    
}



CCSize FriendHelpPanel::cellSizeForTable(CCTableView *table){
    return CCSizeMake(table->getContentSize().width, 127);
}

unsigned int FriendHelpPanel::numberOfCellsInTableView(CCTableView *table){
    return m_stHelpFriendLst.size();
}

CCTableViewCell* FriendHelpPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();

    FriendHelpItem *pItem = NULL;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        pItem = FriendHelpItem::create();
        cell->addChild(pItem, 0, FRIEND_LIST_ITEM_TAG);
    }else{
        pItem = dynamic_cast<FriendHelpItem*>(cell->getChildByTag(FRIEND_LIST_ITEM_TAG));
    }
    if(idx < m_stHelpFriendLst.size()) {
        pItem->setInfo(m_stHelpFriendLst[idx]);
    }

    return cell;
}

#include "tip/TipControl.h"

void FriendHelpPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    if(!this->isPanelOnTop()) {

        return;
    }
    FriendHelpItem* curItem = NULL;
    curItem = dynamic_cast<FriendHelpItem*>(cell->getChildByTag(FRIEND_LIST_ITEM_TAG));
    if(NULL == curItem) {
        return;
    }
    //检查消耗
    const vmsg::CSOneFriend& stInfo = curItem->getInfo();
    if(stInfo.has_invitecostcoin()) {
        if(!P_TIP_CTRL->CoinCostTip(stInfo.invitecostcoin(), this) ) {
            return;
        }
    } 

    if(m_pSelectedItem != curItem) {
        if(m_pSelectedItem) {
            m_pSelectedItem->setSelected(false);
        }
        m_pSelectedItem = curItem;
        m_pSelectedItem->setSelected(true);
    } else {
        curItem->setSelected(false);
        m_pSelectedItem = NULL;
    }
    
}


