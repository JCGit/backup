#include "FriendPanelUI.h"
#include "control/PanelSoundType.h"
#include "FriendListItemUI.h"
#include "FriendControl.h"
#include "NounHelper.h"
#include "defines.h"
#include "VIPCfgMaster.h"
#include "UserObj.h"
#include "CCColorUtil.h"
#include "ToolUtils.h"

FriendPanelUI::FriendPanelUI()
{
    m_pTableView = NULL;
    m_pPreSelTabBtn = NULL;
}

FriendPanelUI::~FriendPanelUI()
{
}

bool FriendPanelUI::init() 
{
	bool bRet = false;

	do 
    {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);

		initLayoutFromJsonFile("ui_v2/friend_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		m_uSoundType = COMMON_PANEL_TYPE;
		
        m_pCloseBtnImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtnImg->setTouchEnabled(true);
        m_pCloseBtnImg->addTouchEventListener(this, toucheventselector(FriendPanelUI::touchEventHandler));

        m_pNumTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_tip_txt"));  

        m_pAddFriendBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_add_new"));    
        m_pAddFriendBtn->addTouchEventListener(this, toucheventselector(FriendPanelUI::touchEventHandler));

        //type btn
        m_pTabBtnFriend = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_friend")); 
        m_pTabBtnFriend->addTouchEventListener(this, toucheventselector(FriendPanelUI::touchEventHandler)); 

        m_pTabBtnAdmin = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_admin"));  
        m_pTabBtnAdmin->addTouchEventListener(this, toucheventselector(FriendPanelUI::touchEventHandler));

        m_pTabBtnPropose = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_propose"));  
        m_pTabBtnPropose->addTouchEventListener(this, toucheventselector(FriendPanelUI::touchEventHandler));

 
        //list table view
        m_pTableView = CCTableView::create(this, CCSizeMake(700, 390));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);

        m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(20, 15));
        m_pTableView->setDelegate(this);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        UIImageView* pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_layout"));
        pBgImg->addNode(m_pTableView);

        m_pPreSelTabBtn = m_pTabBtnFriend; //default

        CCColorUtil::addStroke((CCLabelTTF*) m_pAddFriendBtn->getTextVirtualRenderer());

		bRet = true;
	} 
    while(0);

	return bRet;
}

void FriendPanelUI::open()
{
    BasePanel::open();
    P_FRIEND_CTRL->getProxy()->sendQueryFriendRqst();
    P_FRIEND_CTRL->getProxy()->sendGetProposeRqst(); //拉一下推荐列表吧
    this->showLoading();

}


void FriendPanelUI::close()
{
    BasePanel::close();
    if(m_pPreSelTabBtn) {
        m_pPreSelTabBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1) );
        m_pPreSelTabBtn->setTitleFontSize(24);
        m_pPreSelTabBtn->setBright(true);
        m_pPreSelTabBtn->setTouchEnabled(true);
    }
    m_pPreSelTabBtn = m_pTabBtnFriend; //default
}

CCNode* FriendPanelUI::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "tab_recommend_friend_btn") {
		if(m_pTabBtnPropose != NULL && m_pTabBtnPropose->isVisible() && m_pTabBtnPropose->isTouchEnabled()) {
			pRet = m_pTabBtnPropose;
		}
	} else if(name == "add_friend_btn") {
		if(m_iCurLstItemStat == FriendListItemUI::ESTAT_MORE) {
			CCPoint pos = m_pTableView->getParent()->convertToWorldSpace(m_pTableView->getPosition());

			for(unsigned int i = 0; i < numberOfCellsInTableView(m_pTableView); i++) {
				CCTableViewCell *pCell = m_pTableView->cellAtIndex(i);
				FriendListItemUI *pItem = dynamic_cast<FriendListItemUI*>(pCell->getChildByTag(FRIEND_LIST_ITEM_TAG));
				
				if(pItem->getAddFriendBtn()) {
					pRet = pItem->getAddFriendBtn();
					break;
				}
			}
		}
	}

	return pRet;
}

void FriendPanelUI::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const string strWidgetName(pWidget->getName());
    
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strWidgetName == "btn_add_new") {
				P_FRIEND_CTRL->getFriendAddPanelUI()->open();
				GUIDE_TOUCH(pWidget);
			} else if(strWidgetName == "tab_btn_friend") {
                setSelTabBtn(m_pTabBtnFriend);
            } else if(strWidgetName == "tab_btn_admin") {
                setSelTabBtn(m_pTabBtnAdmin);
            } else if(strWidgetName == "tab_btn_propose") {
				GUIDE_TOUCH(pWidget);
                setSelTabBtn(m_pTabBtnPropose);

				scheduleOnce(schedule_selector(FriendPanelUI::onTableViewReload), 0.2f); // 坑爹！！！不能判断TabbleView什么时候更新完成
            } else if(strWidgetName == "close_btn") {
                close();
            }
            //CCLOG("---%s", strWidgetName.c_str());
		default:;
	}
}

void FriendPanelUI::setSelTabBtn(UIButton* pSelBtn) {
    if(NULL == pSelBtn) {
        return;
    }
    if(m_pPreSelTabBtn != NULL){
        m_pPreSelTabBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1) );
        m_pPreSelTabBtn->setTitleFontSize(22);
        m_pPreSelTabBtn->setBright(true);
        m_pPreSelTabBtn->setTouchEnabled(true);
    }
    m_pPreSelTabBtn = pSelBtn;
    m_pPreSelTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae) ); //按下
    m_pPreSelTabBtn->setTitleFontSize(24);
    m_pPreSelTabBtn->setBright(false);
    m_pPreSelTabBtn->setTouchEnabled(false);


    //get stat
    m_iCurLstItemStat;
    if(m_pPreSelTabBtn == m_pTabBtnFriend) {
        m_iCurLstItemStat = FriendListItemUI::ESTAT_FRIEND;
    } else if (m_pPreSelTabBtn == m_pTabBtnAdmin) {
        m_iCurLstItemStat = FriendListItemUI::ESTAT_ADMIN;
    } else if(m_pPreSelTabBtn == m_pTabBtnPropose) {
        m_iCurLstItemStat = FriendListItemUI::ESTAT_MORE;
    } else {
        m_iCurLstItemStat = FriendListItemUI::ESTAT_NULL;
    }

    m_pTableView->reloadData();
}

void FriendPanelUI::onTableViewReload(float dt) {
	GUIDE_SHOW(GUIDE_UI_FRIEND);
}

void FriendPanelUI::updateUI()
{
    if(!this->isOpen()) {
        return;
    }
    this->closeLoading();

	CCPoint offset = m_pTableView->getContentOffset();

    setSelTabBtn(m_pPreSelTabBtn);

	// 赠送体力不回到顶部
	if(m_pPreSelTabBtn == m_pTabBtnFriend && P_FRIEND_CTRL->isPresentedManual()) {
		P_FRIEND_CTRL->setPresentedManual(false);
		m_pTableView->setContentOffset(offset);
	}

    //VIP_MODULE_FRIEND
    const VIPCfg* pVipCfg = VIP_CFG_MASTER->getVIPCfg(VIP_MODULE_FRIEND);
    int iMaxNum = 60;
    if(pVipCfg) {
        const int iVipLv = P_USER_OBJ->getUserInfo()->vipLv;
        if(iVipLv < pVipCfg->vipparam_size()) {
            iMaxNum = pVipCfg->vipparam(iVipLv);
        }
    }

    m_pNumTipTxt->setText(intToString(P_FRIEND_CTRL->getFriendRsp().friendlst_size()) + "/" + intToString(iMaxNum) );

    //updateReqNumTipTxt();

	float offsetX = m_pTabBtnAdmin->getSize().width * 0.36f;
	float offsetY = m_pTabBtnAdmin->getSize().height * 0.18f;
	CToolUtils::badgeWidget(m_pTabBtnAdmin, P_FRIEND_CTRL->getFriendReqNum(), offsetX, offsetY);

	if(P_FRIEND_CTRL->isCanRecvManul()) {
		vmsg::CSFriendQueryRsp& stFriendRsp = P_FRIEND_CTRL->getFriendRsp();
		CToolUtils::badgeWidget(m_pTabBtnFriend, stFriendRsp.recvmanualuinlst_size(), offsetX, offsetY);
	} else {
		CToolUtils::badgeWidget(m_pTabBtnFriend, 0, offsetX, offsetY);
	}
	
	GUIDE_SHOW(GUIDE_UI_FRIEND);
}

void FriendPanelUI::updateProposeRsp() {
    if(!this->isOpen()) {
        return;
    }
    this->closeLoading();
    if(m_pPreSelTabBtn == m_pTabBtnPropose) {
        setSelTabBtn(m_pTabBtnPropose);
    }
    
}

CCSize FriendPanelUI::cellSizeForTable(CCTableView *table){
    return CCSizeMake(table->getContentSize().width, 130);
}

unsigned int FriendPanelUI::numberOfCellsInTableView(CCTableView *table){
    const FRIEND_DATA_LIST* pLst = getCurTabDataLst();
    if(pLst) {
        return pLst->size();
    } else {
        return 0;
    }
}

CCTableViewCell* FriendPanelUI::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();
    
    FriendListItemUI *pItem = NULL;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        pItem = FriendListItemUI::create();
        cell->addChild(pItem, 0, FRIEND_LIST_ITEM_TAG);
    }else{
        pItem = dynamic_cast<FriendListItemUI*>(cell->getChildByTag(FRIEND_LIST_ITEM_TAG));
    }
    const FRIEND_DATA_LIST* pLst = getCurTabDataLst();
    if(pLst && (int)idx < pLst->size()) {
        pItem->setStat(m_iCurLstItemStat);
        pItem->setInfo(pLst->Get(idx));

    }

    return cell;
}

void FriendPanelUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    if(!this->isPanelOnTop()) {
        return;
    }
    FriendListItemUI* curItem = NULL;
    curItem = dynamic_cast<FriendListItemUI*>(cell->getChildByTag(FRIEND_LIST_ITEM_TAG));
    if(NULL == curItem) {
        return;
    }
    curItem->onItemTouched(m_stGlobalTouchPt);
    
}


const FRIEND_DATA_LIST* FriendPanelUI::getCurTabDataLst() {
    if(NULL == m_pPreSelTabBtn){
        return NULL;
    }
    vmsg::CSFriendQueryRsp& stRsp = P_FRIEND_CTRL->getFriendRsp();

    if(m_pPreSelTabBtn == m_pTabBtnFriend) {
        return &(stRsp.friendlst() );
    } else if (m_pPreSelTabBtn == m_pTabBtnAdmin) {
        return &(stRsp.adminreqlst() );
    } else if(m_pPreSelTabBtn == m_pTabBtnPropose) {
        return &(P_FRIEND_CTRL->getProposeRsp().friendlst() );
    } else {
        return NULL;
    }

}

void FriendPanelUI::updateReqNumTipTxt() {
    //set add req num
    UILabelAtlas* pLabel = (UILabelAtlas* )(m_pTabBtnAdmin->getChildByTag(REQ_NUM_TIP_TAG));
    const int iTipNum = P_FRIEND_CTRL->getFriendReqNum();
    if(iTipNum <= 0) {
        if(pLabel) {
            pLabel->removeFromParent();
        } 
        return;
    }

    if(NULL == pLabel) { 
        pLabel = UILabelAtlas::create();
        pLabel->setPosition(ccp(m_pTabBtnAdmin->getSize().width/2 * 0.8, m_pTabBtnAdmin->getSize().height/2 * 0.8));
        pLabel->setProperty("0123456789", "ui_v2/numbers/number_6.png", 24, 25, "0");
        m_pTabBtnAdmin->addChild(pLabel, 3, REQ_NUM_TIP_TAG);
    }

    pLabel->setStringValue(intToString(iTipNum));
}


