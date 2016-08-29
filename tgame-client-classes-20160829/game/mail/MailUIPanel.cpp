#include "MailUIPanel.h"

#include "MailControl.h"
#include "MailProxy.h"
#include "MailListItem.h"
#include "defines.h"
#include "control/PanelSoundType.h"

using namespace vmsg;
USING_NS_CC;
USING_NS_CC_EXT;

MailUIPanel::MailUIPanel()
{
}

MailUIPanel::~MailUIPanel()
{
}

bool MailUIPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

        setClickOutToClose(true);
        m_uSoundType = MAIL_PANEL_TYPE;

		initLayoutFromJsonFile("ui_v2/mail_panel.ExportJson");
		addChild(m_pRootLayout);
		
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(MailUIPanel::touchEventHandler));

        m_pAutoDelBtn= dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("del_btn"));
        m_pAutoDelBtn->addTouchEventListener(this, toucheventselector(MailUIPanel::touchEventHandler));

        m_pAddAllBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("get_all_btn"));
        m_pAddAllBtn->addTouchEventListener(this, toucheventselector(MailUIPanel::touchEventHandler));

        m_pNumTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_tip_txt"));

        //list table view
        m_pTableView = CCTableView::create(this, CCSizeMake(732, 360));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);

        m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(18, 10));
        m_pTableView->setDelegate(this);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        UIImageView* pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg"));
        pBgImg->addNode(m_pTableView);

		bRet = true;
	} while(0);

	return bRet;
}

std::string MailUIPanel::getDescription() const {
	return "MailUIPanel";
}

void MailUIPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, "close_btn") == 0) {
				close();
            } else if(std::strcmp(pszWidgetName, "get_all_btn") == 0) {
                doGetAllAttach();
            } else if(std::strcmp(pszWidgetName, "del_btn") == 0) {
                if(m_stMailDataLst.size() <= 0) {
                    CCLOG("[warn]mail list is empty.");
                    break;
                }
                P_MAIL_CTRL->getProxy()->sendAutoDelRqst();
            } 
			break;
		default:
			break;
	}
}


void MailUIPanel::open() {
    BasePanel::open();
    P_MAIL_CTRL->getProxy()->sendQueryMail();
    this->showLoading();


}


 void MailUIPanel::close(){
     BasePanel::close();
     
     m_stMailDataLst.clear();

     //dump texture info
     CCLOG("MailUIPanel------------------------dump all texture cache begin------------");
     CCTextureCache::sharedTextureCache()->removeUnusedTextures();
     CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
     CCLOG("MailUIPanel------------------------dump all texture cache end--------------");
 }


void MailUIPanel::updateUI() {
    if (!isOpen()) {
        return;
    }
    this->closeLoading();

    m_stMailDataLst.clear();

    vector<UNIT64_t> stOpenMailLst; //已读的
    map<UNIT64_t, CSMail>& stMailData = P_MAIL_CTRL->getMailObj()->getRefMailData();
    //未读的显示在上面
    map<UNIT64_t, CSMail>::iterator iter = stMailData.begin();
    int iListIdx = 0;
    bool bIsSel = false;
    for(; iter != stMailData.end(); ++iter) {
        const CSMail& stMail = iter->second;
        if(stMail.hadread()) {
            stOpenMailLst.push_back(stMail.mailid());
            continue; //读过的方后面显示
        } else {
            m_stMailDataLst.push_back(stMail.mailid());
        }
        
    }

    //show readed mail
    for(unsigned int i = 0; i < stOpenMailLst.size(); ++i) {
        m_stMailDataLst.push_back(stOpenMailLst[i]);
    }

	int dataSize = m_stMailDataLst.size();
    m_pNumTipTxt->setText(intToString(dataSize) + "/100" );

    m_pTableView->reloadData();
}

void MailUIPanel::doGetAllAttach() {
    std::vector<UNIT64_t> stMailIDLst;
    for(unsigned int i = 0; i < m_stMailDataLst.size(); ++i) {
        const UNIT64_t iMailID = m_stMailDataLst[i];

        const CSMail* pstMail = P_MAIL_CTRL->getMailObj()->getMail(iMailID);
        
        if(pstMail && pstMail->hasattach() && pstMail->hadgetattach() == false) {
            stMailIDLst.push_back(pstMail->mailid());
        }
    }

    if(stMailIDLst.size() <= 0) {
        CCLOG("[warn]MailUIPanel::doGetAllAttach, no mail can get attach");
        return;
    }
    P_MAIL_CTRL->getProxy()->sendGetAttachAll(stMailIDLst);
    
}



CCSize MailUIPanel::cellSizeForTable(CCTableView *table){
    return CCSizeMake(table->getContentSize().width, 120);
}

unsigned int MailUIPanel::numberOfCellsInTableView(CCTableView *table){
    return m_stMailDataLst.size();
}

CCTableViewCell* MailUIPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();

    MailListItem *pItem = NULL;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        pItem = MailListItem::create();
        cell->addChild(pItem, 0, MAIL_LIST_ITEM_TAG);
    } else {
        pItem = dynamic_cast<MailListItem*>(cell->getChildByTag(MAIL_LIST_ITEM_TAG));
    }
    
    const CSMail* pMail =  P_MAIL_CTRL->getMailObj()->getMail(m_stMailDataLst[idx]);
    if(pMail) {
        pItem->updateMailInfo(*pMail);
    }
    
    
    return cell;
}

void MailUIPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {

    //事件优先级的原因，特殊处理一下
    if(P_MAIL_CTRL->getMailInfoPanel()->isOpen()) {
        return;
    }

    MailListItem* curItem = NULL;
    curItem = dynamic_cast<MailListItem*>(cell->getChildByTag(MAIL_LIST_ITEM_TAG));
    if(NULL == curItem) {
        return;
    }
    curItem->onTouched();
}


