#include "ArenaRankPanel.h"
#include "defines.h"
#include "NounHelper.h"
#include "ArenaControl.h"
#include "ArenaRankItem.h"
#include "TypeTabLayout.h"
#include "UserObj.h"

#include "ArenaLvCfgMaster.h"

#define ARENA_RANK_ITEM_TAG 101

CArenaRankPanel::CArenaRankPanel(){
	m_pTableView = NULL;
	m_pRankDemo = NULL;
	m_pTabDemo = NULL;
	m_pSelectBtn = NULL;
	m_uCellCount = 0;
}

CArenaRankPanel::~CArenaRankPanel(){
	CC_SAFE_RELEASE_NULL(m_pRankDemo);
	CC_SAFE_RELEASE_NULL(m_pTabDemo);
}

bool CArenaRankPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/arena_rank_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pRankBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("rank_list_bg"));

		m_pMyLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("my_lv_txt"));
		m_pMyRankTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("my_rank_txt"));
		m_pTipTxt = dynamic_cast<UILabel*>(m_pRankBg->getChildByName("tip_txt"));
		m_pTipTxt->setVisible(false);

		CArenaRankItem* pDemo = CArenaRankItem::create();
		m_pRankDemo = pDemo->getRootLayout();
		m_pRankDemo->retain();

		CTypeTabLayout* pTabDemo = CTypeTabLayout::create();
		m_pTabDemo = pTabDemo->getRootLayout();
		m_pTabDemo->retain();

		m_pTableView = CCTableView::create(this, CCSizeMake(581, 368));
		m_pTableView->setDirection(kCCScrollViewDirectionVertical);
		m_pTableView->setAnchorPoint(CCPointZero);
		m_pTableView->setPosition(ccp(14, 7));
		m_pTableView->setDelegate(this);
		m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		m_pRankBg->addNode(m_pTableView);

		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CArenaRankPanel::touchEventHandler));

		initTabs();

		bRet = true;
	} while (0);

	return bRet;
}

void CArenaRankPanel::initTabs(){
	m_pBtnList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("tab_list"));
	m_pBtnList->addEventListenerListView(this, listvieweventselector(CArenaRankPanel::listViewEventHandler));
	m_pBtnList->setItemsMargin(0);

	unsigned int totalSize = ARENA_LV_CFG_MASTER->GetCfg().arenalvcfg_size(); 
	CTypeTabLayout* pTab = NULL;
	for(unsigned int i = 0; i < totalSize; i++){
		const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetOneCfg(totalSize - i);
		pTab = CTypeTabLayout::create(m_pTabDemo);
		pTab->setType(stCfg->level());
		m_pBtnList->pushBackCustomItem(pTab);
	}
}

CCSize CArenaRankPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 50);
}

unsigned int CArenaRankPanel::numberOfCellsInTableView(CCTableView *table){
	return m_uCellCount;
}

CCTableViewCell* CArenaRankPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	CArenaRankItem* pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = CArenaRankItem::create(m_pRankDemo);
		cell->addChild(pItem, 0, ARENA_RANK_ITEM_TAG);
	}else{
		pItem = dynamic_cast<CArenaRankItem*>(cell->getChildByTag(ARENA_RANK_ITEM_TAG));
	}

	const CSRankQueryArenaArmyRankRsp& stRankInfo = P_ARENA_CTRL->getArenaObj()->getRankInfo();
	if(idx < (unsigned int)stRankInfo.infos_size()){
		pItem->setInfo(stRankInfo.infos(idx), idx);
	}

	return cell;
}

void CArenaRankPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){

}

void CArenaRankPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}
		break;
	default:

		break;
	}
}

void CArenaRankPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case LISTVIEW_ONSELECTEDITEM_END:
		if(std::strcmp(pszWidgetName, m_pBtnList->getName()) == 0) {
			int nSelectedIdx = m_pBtnList->getCurSelectedIndex();
			CTypeTabLayout* pTab = dynamic_cast<CTypeTabLayout*>(m_pBtnList->getItem(nSelectedIdx));
			if(pTab){
				setTabSelect(pTab, true);
				if(m_pSelectBtn){
					setTabSelect(m_pSelectBtn, false);
					m_pSelectBtn = NULL;
				}
				m_pSelectBtn = pTab;

				const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
				const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stArenaInfo.score());
				UIN_t uin = 0;
				if(pTab->getType() == stCfg->level()){
					uin = P_USER_OBJ->getUserInfo()->uin;
				}
				P_ARENA_CTRL->sendQueryArenaArmyRankRqst(pTab->getType(), uin);
				this->showLoading();
			}
		}
		break;
	default:
		break;
	}
}

void CArenaRankPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();

	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stArenaInfo.score());
	m_pMyLvTxt->setText(stCfg->name());

	const CSRankQueryArenaArmyRankRsp& stRankInfo = P_ARENA_CTRL->getArenaObj()->getRankInfo();
    
	if(stRankInfo.infos_size() <= 0){  //暂无排名
		m_pTableView->setVisible(false);
		m_pTableView->setTouchEnabled(false);
		m_pTipTxt->setVisible(true);
		return;
	}

	m_pTableView->setVisible(true);
	m_pTableView->setTouchEnabled(true);
	m_pTipTxt->setVisible(false);

	m_uCellCount = stRankInfo.infos_size();
	m_pTableView->reloadData();

	updateMyRank();
}

void CArenaRankPanel::updateMyRank(){
	int rank = P_ARENA_CTRL->getArenaObj()->getMyRank() + 1;
	const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stArenaInfo.score());
	string str = stCfg->name() + "  ";
	if(rank < 0){
		str += NounStr(NO_RANK_TIP);
	}else{
		str += intToString(rank);
	}

	m_pMyRankTxt->setText(str);
}

void CArenaRankPanel::open(){
	BasePanel::open();

	//手动调用
	m_pBtnList->sortAllChildren();

	//默认选中当前军衔
	const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stArenaInfo.score());
	unsigned int totalSize = ARENA_LV_CFG_MASTER->GetCfg().arenalvcfg_size();
	CTypeTabLayout* pTab = dynamic_cast<CTypeTabLayout*>(m_pBtnList->getItem(totalSize - stCfg->level()));
	if(pTab){
		setTabSelect(pTab, true);
		if(m_pSelectBtn){
			setTabSelect(m_pSelectBtn, false);
			m_pSelectBtn = NULL;
		}
		m_pSelectBtn = pTab;

		UIN_t uin = 0;
		if(pTab->getType() == stCfg->level()){
			uin = P_USER_OBJ->getUserInfo()->uin;
		}
		P_ARENA_CTRL->sendQueryArenaArmyRankRqst(pTab->getType(), uin);
		this->showLoading();

		float fPer = 100.0f*(m_pBtnList->getIndex(pTab) + 1)/totalSize;
		m_pBtnList->jumpToPercentVertical(fPer);
	}
}

void CArenaRankPanel::close(){
	BasePanel::close();
}

CTypeTabLayout* CArenaRankPanel::getTabByType(unsigned int uType){
	CCObject* pstObj;
	CTypeTabLayout* pTab = NULL;
	CCARRAY_FOREACH(m_pBtnList->getItems(), pstObj){
		pTab = dynamic_cast<CTypeTabLayout*>(pstObj);
		if(pTab->getType() == uType){
			return pTab;
		}
	}

	return NULL;
}

void CArenaRankPanel::setTabSelect(CTypeTabLayout* pTab, bool bSel){
	if(bSel){
		pTab->getTabBtn()->setBrightStyle(BRIGHT_HIGHLIGHT);
		pTab->getTabBtn()->setTouchEnabled(false);
		pTab->getTabBtn()->setTitleColor(ccc3(0xff, 0xfa, 0xae));
		pTab->getTabBtn()->setTitleFontSize(24);
	}else{
		pTab->getTabBtn()->setBrightStyle(BRIGHT_NORMAL);
		pTab->getTabBtn()->setTouchEnabled(true);
		pTab->getTabBtn()->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
		pTab->getTabBtn()->setTitleFontSize(22);
	}
}


