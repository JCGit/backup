#include "NihilityRankPanel.h"
#include "defines.h"
#include "rank/RankControl.h"
#include "fightendless/FightEndlessRankItem.h"
#include "NounHelper.h"
#include "UserObj.h"

#define ENDLESS_ANK_ITEM_TAG 101

CNihilityRankPanel::CNihilityRankPanel(){
	m_pTableView = NULL;
	m_pRankDemo = NULL;
	m_uCellCount = 0;
}

CNihilityRankPanel::~CNihilityRankPanel(){
	CC_SAFE_RELEASE_NULL(m_pRankDemo);
}

bool CNihilityRankPanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/endless_rank_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CNihilityRankPanel::initUI(){
	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pEnterBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("enter_btn"));
	m_pLayer = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("layout"));
	m_pTipTxt = dynamic_cast<UILabel*>(m_pLayer->getChildByName("tip_txt"));
	m_pMyRankTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("rank_txt"));
	m_pTipTxt->setVisible(false);

	CFightEndlessRankItem* pDemo = CFightEndlessRankItem::create();
	m_pRankDemo = pDemo->getRootLayout();
	m_pRankDemo->retain();

	m_pTableView = CCTableView::create(this, CCSizeMake(490, 350));
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(ccp(13, 0));
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pLayer->addNode(m_pTableView);

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CNihilityRankPanel::touchEventHandler));
	m_pEnterBtn->addTouchEventListener(this, toucheventselector(CNihilityRankPanel::touchEventHandler));
}

CCSize CNihilityRankPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 50);
}

unsigned int CNihilityRankPanel::numberOfCellsInTableView(CCTableView *table){
	return m_uCellCount;
}

CCTableViewCell* CNihilityRankPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	CFightEndlessRankItem* pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = CFightEndlessRankItem::create(m_pRankDemo);
		cell->addChild(pItem, 0, ENDLESS_ANK_ITEM_TAG);
	}else{
		pItem = dynamic_cast<CFightEndlessRankItem*>(cell->getChildByTag(ENDLESS_ANK_ITEM_TAG));
	}

	const CSRankQueryRsp& stRankInfo = P_RANK_CTL->getRankInfo();
	pItem->setRank(idx);
	if(idx < (unsigned int)stRankInfo.infos_size()){
		pItem->setObj(stRankInfo.infos(idx));
	}

	return cell;
}

void CNihilityRankPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	CFightEndlessRankItem* pItem = dynamic_cast<CFightEndlessRankItem*>(cell->getChildByTag(ENDLESS_ANK_ITEM_TAG));
	UIN_t uin = pItem->getObj().uin();
	if(uin != P_USER_OBJ->getUserInfo()->uin) {
		//P_GUILD_CTRL->getGuildMemberPanel()->setPanelType(GuildMemberPanel::TYPE_MEMBER_RANK);
		//P_GUILD_CTRL->getGuildMemberPanel()->open();
		//P_LOGIN_CTRL->getProxy()->sendQueryOtherPlayerInfoRqst(uin);
	}
}

void CNihilityRankPanel::updateUI(){
	const CSRankQueryRsp& stRankInfo = P_RANK_CTL->getRankInfo();

	if(stRankInfo.infos_size() <= 0){  //ÔÝÎÞÅÅÃû
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
}

void CNihilityRankPanel::updateMyRank(int rank){
	if(rank < 0){
		m_pMyRankTxt->setText(NounStr(NO_RANK_TIP));
		return;
	}

	m_pMyRankTxt->setText(to_string(rank + 1));
}

void CNihilityRankPanel::open(){
	BasePanel::open();

	P_RANK_CTL->setIsRankPanel(false);
	P_RANK_CTL->sendRankQueryRqst(RANK_TYPE_NOTHINGLANDSCORE, 0, MAX_LIST_NUM - 1);
	P_RANK_CTL->sendRankQueryMyRankRqst(RANK_TYPE_NOTHINGLANDSCORE);
}

void CNihilityRankPanel::close(){
	BasePanel::close();
}

void CNihilityRankPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();
	
	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			close();
		}else if(strcmp(pszWidgetName, "enter_btn") == 0) {
			close();

		}
		break;
	default:
		break;
	}
}