#include "FriendProposePanel.h"

#include "FriendControl.h"
#include "FriendProxy.h"
#include "FriendProposeItem.h"


USING_NS_CC;
USING_NS_CC_EXT;

FriendProposePanel::FriendProposePanel()
{
}

FriendProposePanel::~FriendProposePanel()
{
}

bool FriendProposePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/friend_propose_panel_ui.ExportJson");
		addChild(m_pRootLayout);
		
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(FriendProposePanel::touchEventHandler));

        m_pAddAllBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("do_auto_add_btn"));
        m_pAddAllBtn->addTouchEventListener(this, toucheventselector(FriendProposePanel::touchEventHandler));


        //list table view
        m_pTableView = CCTableView::create(this, CCSizeMake(410, 390));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);

        m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(20, 9));
        m_pTableView->setDelegate(this);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

        UIImageView* pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_inner"));
        pBgImg->addNode(m_pTableView);

		bRet = true;
	} while(0);

	return bRet;
}

std::string FriendProposePanel::getDescription() const {
	return "FriendProposePanel";
}

void FriendProposePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, "close_btn") == 0) {
				close();
            } else if(std::strcmp(pszWidgetName, "do_auto_add_btn") == 0) {
                doAdd();
            } 
			break;
		default:
			break;
	}
}


void FriendProposePanel::open() {
    BasePanel::open();
    P_FRIEND_CTRL->getProxy()->sendGetProposeRqst();
    this->showLoading();
}


 void FriendProposePanel::close(){
     BasePanel::close();
     
 }


void FriendProposePanel::updateUI() {
    if (!isOpen()) {
        return;
    }
    this->closeLoading();

    m_pTableView->reloadData();
}

void FriendProposePanel::doAdd() {
    std::vector<UIN_t> stUinLst;

    for(int i = 0; i < P_FRIEND_CTRL->getProposeRsp().friendlst_size(); ++i ) {
        stUinLst.push_back( P_FRIEND_CTRL->getProposeRsp().friendlst(i).uin() );
    }

    P_FRIEND_CTRL->getProxy()->sendBatchAddRqst(stUinLst);

    close(); //¹Ø±ÕÃæ°å
    
}



CCSize FriendProposePanel::cellSizeForTable(CCTableView *table){
    return CCSizeMake(table->getContentSize().width, 130);
}

unsigned int FriendProposePanel::numberOfCellsInTableView(CCTableView *table){
    return P_FRIEND_CTRL->getProposeRsp().friendlst_size();
}

CCTableViewCell* FriendProposePanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();

    FriendProposeItem *pItem = NULL;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        pItem = FriendProposeItem::create();
        cell->addChild(pItem, 0, FRIEND_LIST_ITEM_TAG);
    }else{
        pItem = dynamic_cast<FriendProposeItem*>(cell->getChildByTag(FRIEND_LIST_ITEM_TAG));
    }
    if((int)idx < P_FRIEND_CTRL->getProposeRsp().friendlst_size()) {
        pItem->setInfo(P_FRIEND_CTRL->getProposeRsp().friendlst(idx));
    }

    return cell;
}

void FriendProposePanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {

}


