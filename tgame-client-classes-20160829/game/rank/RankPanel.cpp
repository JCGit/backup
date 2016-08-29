#include "RankPanel.h"

#include "defines.h"

#include "CfgMaster/WarMapCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/TimeEx.h"

#include "game/UserObj.h"
#include "game/guild/GuildControl.h"
#include "game/login/LoginControl.h"
#include "game/bag/BagControl.h"

#include "RankControl.h"
#include "RankItem.h"

#define RANK_ITEM_TAG 101

CRankPanel::CRankPanel()
:m_pTabListView(NULL)
,m_pMyRankValTxt(NULL)
,m_pCoinValTxt(NULL)
,m_pIconImg(NULL)
,m_pTimerTxt(NULL)
,m_nTabSelectedIdx(-1)
,m_pTableView(NULL)
,m_pTipTxt(NULL)
,m_uCellCount(0)
{
	
}

CRankPanel::~CRankPanel(){
	
}

bool CRankPanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/RankPanelUI.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CRankPanel::initUI(){
	m_pTabListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("tab_list"));
	m_pTabListView->addEventListenerListView(this, listvieweventselector(CRankPanel::listViewEventHandler));

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	 m_pCloseBtn->addTouchEventListener(this, toucheventselector(CRankPanel::touchEventHandler));

	m_pIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_img"));

	m_pMyRankValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("my_rank_val_txt"));

	m_pCoinValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_val_txt"));

	m_pTimerTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("timer_txt"));

	m_nTabSelectedIdx = 0; // 初始选择
	UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_nTabSelectedIdx));
	pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
	pTabBtn->setTouchEnabled(false);
	pTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));  //按下
	pTabBtn->setTitleFontSize(24);

	m_pTableView = CCTableView::create(this, CCSizeMake(542, 322));
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(ccp(262, 212));
	m_pTableView->setDelegate(this);
	m_pTableView->setTouchPriority(96);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pRootLayout->addNode(m_pTableView);

	m_pTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("tip_txt"));
	m_pTipTxt->setVisible(false);

	//屏蔽充值排行
	UIButton *pChargTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(3));
	pChargTabBtn->setEnabled(false);

}

CCSize CRankPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 46);
}

unsigned int CRankPanel::numberOfCellsInTableView(CCTableView *table){
	return m_uCellCount;
}

CCTableViewCell* CRankPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	CRankItem* pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = CRankItem::create();
		cell->addChild(pItem, 0, RANK_ITEM_TAG);
	}else{
		pItem = dynamic_cast<CRankItem*>(cell->getChildByTag(RANK_ITEM_TAG));
	}

	const CSRankQueryRsp& stRankInfo = P_RANK_CTL->getRankInfo();
	pItem->setRank(idx);
	if(idx < (unsigned int)stRankInfo.infos_size()){
		pItem->setObj(stRankInfo.infos(idx), getCurRankType());
	}

	return cell;
}

void CRankPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	CRankItem* pItem = dynamic_cast<CRankItem*>(cell->getChildByTag(RANK_ITEM_TAG));
	UIN_t uin = pItem->getObj().uin();
	if(uin != P_USER_OBJ->getUserInfo()->uin) {
		/*P_GUILD_CTRL->getGuildMemberPanel()->setPanelType(GuildMemberPanel::TYPE_MEMBER_RANK);
		P_GUILD_CTRL->getGuildMemberPanel()->open();
		P_LOGIN_CTRL->getProxy()->sendQueryOtherPlayerInfoRqst(uin);*/
		//close();
		
		P_BAG_CTRL->requestOtherRoleInfo(uin);
	}
}

void CRankPanel::updateUI(){
	const CSRankQueryRsp& stRankInfo = P_RANK_CTL->getRankInfo();
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
}

void CRankPanel::updateMyRank(){
	const CSRankQueryMyRankRsp& stMyRank = P_RANK_CTL->getMyRankInfo();

	if(stMyRank.rank() < 0){
		m_pMyRankValTxt->setText(NounStr(NO_RANK_TIP));
		return;
	}

	m_pMyRankValTxt->setText(to_string(stMyRank.rank() + 1));

	if(getCurRankType() == RANK_TYPE_WARMAP){
		WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(stMyRank.score());
		if(stCfg != NULL){
			CCLOG("CNormalListTab::there is no war map cfg, cfg id == %d", stMyRank.score());
			//m_pMyPowerTxt->setText(stCfg->name());
		}
	}else{
		//m_pMyPowerTxt->setText(to_string(stMyRank.score()));
	}

}

void CRankPanel::updateGift(){
	const CSAwardRankRsp& stAwardInfo = P_RANK_CTL->getRankAwardInfo();
	const google::protobuf::RepeatedPtrField<vmsg::CSAwardRankInfo>& stInfos = stAwardInfo.infos();

	for(int i = 0; i < stInfos.size(); i++){
		const vmsg::CSAwardRankInfo& tempInfo = stInfos.Get(i);

		if(getCurRankType() == tempInfo.ranktype()){
			if(tempInfo.haslastaward()){
				m_pCoinValTxt->setText(intToString(tempInfo.coin()));
			}else{
				m_pCoinValTxt->setText("0");
			}
			break;
		}
	}

	//得到倒计时，先加1
	_totalTime = stAwardInfo.remaintime();
	_totalTime++;
	m_pTimerTxt->setText(TimeEx::getStringTimeForInt(_totalTime));

	startCountDown();
}

void CRankPanel::startCountDown() {
	schedule(schedule_selector(CRankPanel::updateTime), 1);
}

void CRankPanel::clearCountDown() {
	unschedule(schedule_selector(CRankPanel::updateTime));
}

void CRankPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();
	UIButton *pTabBtn = NULL;

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(strcmp(pszListViewName, m_pTabListView->getName()) == 0) {
				int nTabSelectedIdx = m_pTabListView->getCurSelectedIndex();

				if(m_nTabSelectedIdx >= 0) {
					pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_nTabSelectedIdx));
					pTabBtn->setBrightStyle(BRIGHT_NORMAL);
					pTabBtn->setTouchEnabled(true);
					pTabBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
					pTabBtn->setTitleFontSize(22);
				}

				pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(nTabSelectedIdx));
				pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
				pTabBtn->setTouchEnabled(false);
				pTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));  //按下
				pTabBtn->setTitleFontSize(24);
				m_nTabSelectedIdx = nTabSelectedIdx;

				tabSelected(m_nTabSelectedIdx);
			}
			break;
		default:;
	}

}

void CRankPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();
		} 
		break;
	default:
		break;
	}
}

unsigned int CRankPanel::getCurRankType() {
	unsigned int uRankType = 0;
	
	switch(m_nTabSelectedIdx) {
		case TAB_FIGHT_POWER:
			uRankType = RANK_TYPE_FIGHTPOWER;
			break;
		case TAB_LEVEL:
			uRankType = RANK_TYPE_WARMAP;
			break;
		case TAB_INFINITE:
			uRankType = RANK_TYPE_NOTHINGLANDSCORE;
			break;
		case TAB_GOLD:
			uRankType = RANK_TYPE_PAY;
			break;
		default:;
	}

	return uRankType;
}

void CRankPanel::tabSelected(int nSelectedIdx) {
	unsigned int uRankType = 0;
	
	switch(nSelectedIdx) {
		case TAB_FIGHT_POWER:
			uRankType = RANK_TYPE_FIGHTPOWER;
			break;
		case TAB_LEVEL:
			uRankType = RANK_TYPE_WARMAP;
			break;
		case TAB_INFINITE:
			uRankType = RANK_TYPE_NOTHINGLANDSCORE;
			break;
		case TAB_GOLD:
			uRankType = RANK_TYPE_PAY;
			break;
		default:;
	}

	P_RANK_CTL->sendRankQueryRqst(uRankType, 0, RANK_MAX_NUM - 1);
	P_RANK_CTL->sendRankQueryMyRankRqst(uRankType);

	//查询奖励
	P_RANK_CTL->sendQueryAwardRankRqst();
}

void CRankPanel::updateTime(float dt) {
	_totalTime--;

	if(_totalTime <= 0){
		m_pTimerTxt->setText(TimeEx::getStringTimeForInt(0));
		unschedule(schedule_selector(CRankPanel::updateTime));
		P_RANK_CTL->sendQueryAwardRankRqst();	//时间到了再次查询
		return;
	}

	m_pTimerTxt->setText(TimeEx::getStringTimeForInt(_totalTime));
}

void CRankPanel::open(){
	BasePanel::open();

	P_RANK_CTL->setIsRankPanel(true);
	tabSelected(m_nTabSelectedIdx);
}

void CRankPanel::close(){
	clearCountDown();
	
	BasePanel::close();
}