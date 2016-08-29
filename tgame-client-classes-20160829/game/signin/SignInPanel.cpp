#include "SignInPanel.h"
#include "SignInListItem.h"
#include "UserObj.h"
#include "defines.h"
#include "login/LoginControl.h"
#include "DailyRegAwardCfgMaster.h"
#include "PopTipLayer.h"
#include "game/city/CityControl.h"
#include "ModuleID.pb.h"
#include "game/city/AutoPopControl.h"

SignInPanel::SignInPanel()
{
    m_pTableView = NULL;
    m_iCanReg = 0;
    m_iRegCnt = 0;

}

SignInPanel::~SignInPanel()
{
}

SignInPanel* SignInPanel::create() {
	SignInPanel *pRet = new SignInPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool SignInPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/sign_in_award_ui.ExportJson");
		addChild(m_pRootLayout);

        setClickOutToClose(true);

        UIButton* pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        pCloseBtn->addTouchEventListener(this, toucheventselector(SignInPanel::touchEventHandler));

        m_pTableView = CCTableView::create(this, CCSizeMake(550, 400));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);
        m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(35, 20));
        m_pTableView->setDelegate(this);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

        UIImageView* pBgInner1 = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_inner_1"));
        pBgInner1->addNode(m_pTableView);

        m_pTimeTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_tip_txt"));

        m_pBgLeft = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_left"));
        char str[100] = {0};
        for(int i = 0; i < LIST_MAX_NUM; ++i) {
            snprintf(str, 100, "day_txt_%d", i);
            m_stDayTxtLst[i] = dynamic_cast<UILabel*>(m_pBgLeft->getChildByName( str ) );
        }

        m_pDaySigninNode = CCSpriteBatchNode::create("ui_v2/other.png");
        m_pDaySigninNode->setAnchorPoint(ccp(0, 0));
        m_pDaySigninNode->setPosition(0, 0);
        m_pBgLeft->addNode(m_pDaySigninNode);
        

		bRet = true;
	} while(0);

	return bRet;
}

void SignInPanel::open() {
	BasePanel::open();
    
    updateUI();
}

void SignInPanel::close() {
    BasePanel::close();
    m_pDaySigninNode->removeAllChildrenWithCleanup(true);
    m_pBgLeft->removeChildByTag(TAG_TODAY_ICON, true);

	P_AUTO_POP_CTRL->onClosePanel(this);
}

bool SignInPanel::isMeetAutoOpen() {
	bool bRet = false;

	do {
		// is module open
		CC_BREAK_IF(!P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_SIGNIN));

		// can sign in or not
		CC_BREAK_IF(P_USER_OBJ->getUserInfo()->stDailyRegInfo.canreg() <= 0);

		bRet = true;
	} while(0);

	return bRet;
}

void SignInPanel::updateUI() {

    m_iRegCnt = P_USER_OBJ->getUserInfo()->stDailyRegInfo.regcnt();
    m_iCanReg = P_USER_OBJ->getUserInfo()->stDailyRegInfo.canreg();

    updateDayInfo();

    m_pTableView->reloadData();

    m_pTableView->scrollToCellIndex(m_iRegCnt);
	GUIDE_SHOW(GUIDE_UI_SIGNIN);
}

void SignInPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
            if(std::strcmp(pszWidgetName, "close_btn") == 0) {
				close();
            } else if(std::strcmp(pszWidgetName, "today_sign_in") == 0) {
                //bg_left
                P_LOGIN_CTRL->getProxy()->sendDailySignInRqst();
            }
			break;
		default:;
	}
}



CCSize SignInPanel::cellSizeForTable(CCTableView *table){
    return CCSizeMake(table->getContentSize().width, 100);
}

unsigned int SignInPanel::numberOfCellsInTableView(CCTableView *table){
    return LIST_MAX_NUM;
}

CCTableViewCell* SignInPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();
    SignInListItem *pItem = NULL;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        pItem = SignInListItem::create();
        cell->addChild(pItem, 0, LIST_ITEM_TAG);
    }else{
        pItem = dynamic_cast<SignInListItem*>(cell->getChildByTag(LIST_ITEM_TAG));
    }

    int iStat = 0;
    if((int)idx < (m_iRegCnt) ) {
        iStat = SignInListItem::SIGN_ITEM_STAT_DONE;
    } else {
        iStat = SignInListItem::SIGN_ITEM_STAT_IDLE;
    }

    pItem->setInfo(idx, iStat);

    if(m_iCanReg) {
        if(idx == m_iRegCnt) {
            pItem->setTodayCanSign(true);
        }
    } else {
        if(idx == (m_iRegCnt - 1)) {
            pItem->setTodayCanSign(false);
        }
    }
    
    return cell;
}

void SignInPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    if(!isPanelOnTop()) {
        return;
    }
    SignInListItem *pItem = dynamic_cast<SignInListItem*>(cell->getChildByTag(LIST_ITEM_TAG));
    if(NULL == pItem) {
        return;
    }
    pItem->onTouch(m_stGlobalTouchPt);
}

void SignInPanel::updateDayInfo() {
    m_pDaySigninNode->removeAllChildrenWithCleanup(true);
    m_pBgLeft->removeChildByTag(TAG_TODAY_ICON, true);

    UILabel* pTxt;
    int i = 0;
    for(i = 0; i < (m_iRegCnt); ++i) {
        pTxt = m_stDayTxtLst[i];
        pTxt->setVisible(false);

        CCSprite* pimg = CCSprite::createWithSpriteFrameName("other/signin_done_icon.png");
        pimg->setPosition(pTxt->getPosition());

        m_pDaySigninNode->addChild(pimg);
    }

    if(i == m_iRegCnt && m_iCanReg) {
        //今天的签到情况
        pTxt = m_stDayTxtLst[i];
        pTxt->setVisible(false);
        UIImageView* pImg = UIImageView::create();
        pImg->loadTexture("other/signin_icon.png", UI_TEX_TYPE_PLIST);
        pImg->setPosition(pTxt->getPosition());
        pImg->addTouchEventListener(this,  toucheventselector(SignInPanel::touchEventHandler) );
        pImg->setName("today_sign_in");
        pImg->setTouchEnabled(true);
        m_pBgLeft->addChild(pImg, 1, TAG_TODAY_ICON);
    }
    i++;
    for(; i < LIST_MAX_NUM; ++i) {
        pTxt = m_stDayTxtLst[i];
        pTxt->setVisible(true);
    }
}


void SignInPanel::popAwardTip() {
    const int iRegDay = P_USER_OBJ->getUserInfo()->stDailyRegInfo.regcnt();
    const DailyRegAwardCfg* pCfg = DAILYREGAWARD_CFG_MASTER->GetCfg(iRegDay);
    if(NULL == pCfg) {
        return;
    }

    for(int i = 0; i < pCfg->awardlst_size(); ++i) {
        const DailyRegAwardElement& stElmt = pCfg->awardlst(i);
        POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.elementtype(), stElmt.elementid(), stElmt.elementcnt() );
    }
}

CCNode* SignInPanel::getGuideNodeByName(const std::string &name)
{
	CCNode* pRet = NULL;
	if (name == "sign_btn")
	{
		//pRet = dynamic_cast<SignInListItem*>( m_pTableView->cellAtIndex(0))->getSignBtn();
		pRet = m_pBgLeft->getChildByName("today_sign_in");
	}

	return pRet;
}