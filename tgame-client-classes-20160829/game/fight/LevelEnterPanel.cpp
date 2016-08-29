#include "LevelEnterPanel.h"
#include "LevelControl.h"
#include "basic/SceneManager.h"
#include "LevelObj.h"
#include "WarMapCfgMaster.h"
#include "TimeEx.h"
#include "FightControl.h"
#include "friend/FriendControl.h"
#include "defines.h"
#include "UserObj.h"
#include "NounHelper.h"
#include "CommonDialog.h"
#include "IconCell.h"
#include "bag/BagControl.h"
#include "bag/ItemInfoPanel.h"
#include "MonsterCfgMaster.h"
#include "GlobalCfgMaster.h"
#include "PackageCfgMaster.h"
#include "game/city/CityControl.h"
#include "ModuleID.pb.h"
#include "FightObj.h"
#include "TextHelper.h"
#include "ToolUtils.h"
#include "tip/TipControl.h"
#include "manual/ManualControl.h"
#include "evtlog/EvtLog.h"
#include "warmap/WarMapControl.h"
#include "BuyInstanceCfgMaster.h"
#include "game/IconFrameCell.h"
#include "basic/PopTipLayer.h"

using namespace std;

#define MAX_CELL_CNT 5
#define BATCH_SWEEP_CNT 5
#define ITEM_SWEEP_CARD_CFG_ID 1

CLevelEnterPanel::CLevelEnterPanel()
:m_pCloseBtn(NULL)
,m_pManualCostTxt(NULL)
,m_pStar1Img(NULL)
,m_pStar2Img(NULL)
,m_pStar3Img(NULL)
,m_pFightTogetherBtn(NULL)
,m_pWinCondTxt(NULL)
,m_pDropListView(NULL)
,m_pSpecDropListView(NULL)
,m_pSweepBtn(NULL)
,m_pBatchSweepBtn(NULL)
,m_pEnterBtn(NULL)
,m_pNameTxt(NULL)
,m_pSweepCardImg(NULL)
,m_pSweepCardCntTxt(NULL)
,m_uRemainTimes(0)
{
	
}

CLevelEnterPanel::~CLevelEnterPanel(){

}

bool CLevelEnterPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while (0);

	return bRet;
}

CCNode* CLevelEnterPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "enter_btn") {
		if(m_pEnterBtn != NULL && m_pEnterBtn->isRunning() && m_pEnterBtn->isVisible() && m_pEnterBtn->isTouchEnabled()) {
			pRet = m_pEnterBtn;
		}
	}

	return pRet;
}

void CLevelEnterPanel::initUI(){
	initLayoutFromJsonFile("ui_v2/WarInfoPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CLevelEnterPanel::touchEventHandler));

	m_pManualCostTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("manual_cost_val_txt"));

	m_pFullStarConditionTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("perfect_cond_val_txt"));

	m_pStar1Img = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("score_star_0"));

	m_pStar2Img = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("score_star_1"));

	m_pStar3Img = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("score_star_2"));

	m_pFightTogetherBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("fight_together_btn"));
	m_pFightTogetherBtn->addTouchEventListener(this, toucheventselector(CLevelEnterPanel::touchEventHandler));

	m_pWinCondTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("win_cond_val_txt"));
	
	m_pDropListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("drop_list"));
	m_pDropListView->addEventListenerListView(this, listvieweventselector(CLevelEnterPanel::listViewEventHandler));

	m_pSpecDropListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("spec_drop_list"));
	m_pSpecDropListView->addEventListenerListView(this, listvieweventselector(CLevelEnterPanel::listViewEventHandler));

	m_pSweepBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sweep_btn"));
	m_pSweepBtn->addTouchEventListener(this, toucheventselector(CLevelEnterPanel::touchEventHandler));
	
    m_pBatchSweepBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("batch_sweep_btn"));
	m_pBatchSweepBtn->addTouchEventListener(this, toucheventselector(CLevelEnterPanel::touchEventHandler));
	
	m_pEnterBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("enter_btn"));
	m_pEnterBtn->addTouchEventListener(this, toucheventselector(CLevelEnterPanel::touchEventHandler));
	
	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("war_name_txt"));
	
	m_pSweepCardImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sweep_card_img"));

	m_pSweepCardCntTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("sweep_card_cnt_txt"));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void CLevelEnterPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				this->close();			
			} else if(pSender == m_pEnterBtn){
				GUIDE_TOUCH(pWidget);
				//体力不足
				if(P_USER_OBJ->checkManual(WAR_MAP_CFG->GetWarMapCfgByID(_levelID)->manual()) != 0){
					COMMON_DIALOG->show(TextStr(TEXT_MANUAL_BUY_TIP), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(CLevelEnterPanel::buyManualTip));
					//COMMON_DIALOG->show(NounStr(LOW_MANUAL_TXT), NULL, NounStr(OK));
					return;
				}

				//背包已满
				if(P_BAG_CTRL->isFull()){
					COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP), NULL, NounStr(OK));
					return;
				}

				//次数已用完
				if(m_uRemainTimes <= 0){
					P_TIP_CTRL->getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_BUY_INSTANCE);
					return;
				}

				P_LEVEL_OBJ->setEnterLevelID(_levelID);

				// 有好友助战按钮了
				/*if(P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_FRIEND)) {
					this->close();
					P_LEVEL_CTL->getFriendHelpPanel()->open();
					P_FRIEND_CTRL->getProxy()->sendQueryFriendRqst();
				} else {
					P_LEVEL_OBJ->setIsHasFriend(false);
					P_FIGHT_OBJ->setFightType(NORMAL_FIGHT_TYPE);
					P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
					showLoading();
                    
                    //evt log
                    if(_levelID == WAR_MAP_CFG->GetCfg().warmapcfg(0).id() ) {
                        //第一个任务指引过来的
                        ADD_EVT_LOG(EVT_LOG_FIRST_TASK_ENTER_WAR);
                    }
				}*/
			
				P_LEVEL_OBJ->setIsHasFriend(false);
				P_FIGHT_OBJ->setFightType(NORMAL_FIGHT_TYPE);
				P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
				showLoading();

				//evt log
				if(_levelID == WAR_MAP_CFG->GetCfg().warmapcfg(0).id() ) {
					//第一个任务指引过来的
					ADD_EVT_LOG(EVT_LOG_FIRST_TASK_ENTER_WAR);
				}
			}else if(pSender == m_pSweepBtn) {    // 扫荡
				doSweep(1);
			}else if(pSender == m_pBatchSweepBtn){   // 多次扫荡
				doSweep(BATCH_SWEEP_CNT);
			}else if(pSender == m_pFightTogetherBtn){   // 好友助战
				close();
				P_LEVEL_CTL->getFriendHelpPanel()->open();
				P_FRIEND_CTRL->getProxy()->sendQueryFriendRqst();
			}
			break;
		default:;
	}
}

void CLevelEnterPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(pSender == m_pDropListView) {
				int nSelectedIdx = m_pDropListView->getCurSelectedIndex();
				IconFrameCell *pCell = dynamic_cast<IconFrameCell*>(m_pDropListView->getItem(nSelectedIdx));

				if(pCell != NULL) {
					P_TIP_CTRL->showItemTipPanel(pCell->getType(), pCell->getCfgID(), pCell->getCnt(), false);
				}
			} else if(pSender == m_pSpecDropListView) {
				int nSelectedIdx = m_pSpecDropListView->getCurSelectedIndex();
				WarMapCfg* pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(_levelID);

				if(pWarMapCfg != NULL) {
					const HidenAward &stAward = pWarMapCfg->hidenawards(nSelectedIdx);
					P_WAR_MAP_CTRL->getAwardBoxTipPanel()->show(stAward.wintype(), stAward.wincondition(), stAward.winawardid());
				}
			}
			break;
		default:;
	}
}

void CLevelEnterPanel::doBuyCnt(){
	google::protobuf::RepeatedPtrField<vmsg::CSWarMapInfo> mapInfos = P_LEVEL_OBJ->getMapInfo().infos();
	int idx = _levelID - P_LEVEL_OBJ->getMapInfo().fromid();
	vmsg::CSWarMapInfo mapInfo;

	int iStarNum = 0;

	if(idx >= mapInfos.size() || idx < 0){
		return;
	}

	mapInfo = mapInfos.Get(idx);
	unsigned int uCost = BUY_INSTANCE_CFG_MASTER->GetBuyCostCfgByCnt(mapInfo.boughtcnt() + 1)->cost();
	P_TIP_CTRL->GoldCostTip(uCost, this, dialogbtneventselector(CLevelEnterPanel::buyCnt), TextStr(TEXT_BUY_WAR_FIGHT_CNT_TIP));
}

void CLevelEnterPanel::buyCnt(){
	P_FIGHT_CTRL->getFightProxy()->sendBuyInstanceRqst(_levelID);
}

void CLevelEnterPanel::buyManualTip(){
	P_WAR_MAP_CTRL->setEnterWarManualNotEnough(true);
	P_TIP_CTRL->LowManualBuyTip(this, dialogbtneventselector(CLevelEnterPanel::buyManual));
}

void CLevelEnterPanel::buyManual() {
	P_MANUAL_CTRL->sendBuyManualRqst();
}

void CLevelEnterPanel::doSweep(unsigned int uNum){
	//次数不足
	if(m_uRemainTimes == 0 || m_uRemainTimes < uNum){
		P_TIP_CTRL->getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_BUY_INSTANCE);
		return;
	}

	if(P_BAG_CTRL->getItemCnt(vmsg::ITEM_SWEEP_ID) < uNum){   //扫荡卡不足，弹出通用扫荡卡不足面板
		//P_TIP_CTRL->getSweepCardShotPanel()->open();
		POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_SWEEPCARD_LOW_TIP));
		return;
	}

	unsigned int needManaul = WAR_MAP_CFG->GetWarMapCfgByID(_levelID)->manual()*uNum;
	if(P_USER_OBJ->checkManual(needManaul) != 0){
		COMMON_DIALOG->show(TextStr(TEXT_MANUAL_BUY_TIP), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(CLevelEnterPanel::buyManualTip));
		return;
	}

	if(P_BAG_CTRL->isFull()){
		COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP));
		return;
	}

	P_FIGHT_CTRL->getFightProxy()->sendSweepWarRqst(_levelID, uNum);
	P_LEVEL_CTL->getSweepInfoPanel()->open();

	P_LEVEL_OBJ->setSweepLevelID(_levelID);
	P_LEVEL_OBJ->setSweepNum(uNum);
	this->close();
}

void CLevelEnterPanel::setLevelID(unsigned int var){
	_levelID = var;
	updateUI();

    //evt log
    if(_levelID == WAR_MAP_CFG->GetCfg().warmapcfg(0).id() ) {
        //第一个任务指引过来的
        ADD_EVT_LOG(EVT_LOG_FIRST_TASK_SEL_WAR);
    }
}

unsigned int CLevelEnterPanel::getLevelID(){
	return _levelID;
}

void CLevelEnterPanel::updateInfoCallBack(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}
}

void CLevelEnterPanel::updateUI(){
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	google::protobuf::RepeatedPtrField<vmsg::CSWarMapInfo> mapInfos = P_LEVEL_OBJ->getMapInfo().infos();
	unsigned int maxLevel = P_LEVEL_OBJ->getMapInfo().newestwarmapid();  //最新通关的ID
	WarMapCfg* pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(_levelID);
	int idx = _levelID - P_LEVEL_OBJ->getMapInfo().fromid();
	int remainCnt = 0;			//剩余次数
	vmsg::CSWarMapInfo mapInfo;
	
    int iStarNum = 0;
	if(idx < mapInfos.size() && idx >= 0){  //已经通关
		mapInfo = mapInfos.Get(idx);
		remainCnt = mapInfo.totalcnt() - mapInfo.fightcnt();
        iStarNum = mapInfo.score();
	}else{
		remainCnt = pWarMapCfg->repeatnum();
	}
	m_uRemainTimes = remainCnt;
    setScoreStar(iStarNum);
    m_pNameTxt->setText(pWarMapCfg->name());

	m_pManualCostTxt->setText(intToString(pWarMapCfg->manual()));

	unsigned int mfullstartime = pWarMapCfg->starcondition(1);
	char str[50] = {0};
	snprintf(str, 50, TextStr(TEXT_FULL_STAR_CONDITION), mfullstartime );
	m_pFullStarConditionTxt->setText(str);

	m_pFightTogetherBtn->setEnabled(P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_FRIEND));

    //通关提示文本
    m_pWinCondTxt->setText( P_LEVEL_CTL->getWinTypeStr(_levelID));

	bool bIsVis = P_CITY_CTRL->isModuleOpen(MODULE_ID_SWEEP);
	if(GLOBAL_CFG_MASTER->GetCfg().sweeponoff() == 1 && _levelID <= maxLevel){  //只有通关的关卡才能扫荡
		bIsVis = true;
	}else{
		bIsVis = false;
	}
	m_pSweepBtn->setVisible(bIsVis);
	m_pSweepBtn->setTouchEnabled(bIsVis);
	m_pBatchSweepBtn->setVisible(bIsVis);
	m_pBatchSweepBtn->setTouchEnabled(bIsVis);
	m_pSweepCardImg->setVisible(bIsVis);
	m_pSweepCardCntTxt->setVisible(bIsVis);

	m_pSweepCardCntTxt->setText(string("x") + intToString(P_BAG_CTRL->getItemCnt(ITEM_SWEEP_CARD_CFG_ID)));

	reset();

	m_pDropListView->removeAllItems();

	unsigned int uPkgID = pWarMapCfg->packageids(0);
	const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPkgID);

	if(pPackageCfg != NULL) {
		int nShowCellCnt = pPackageCfg->elmtlst_size() > MAX_CELL_CNT ? MAX_CELL_CNT : pPackageCfg->elmtlst_size();

		for(int i = 0; i < nShowCellCnt; i++) {
			IconFrameCell *pCell = IconFrameCell::create();
			pCell->setTouchEnabled(true);
			const PackageElmt &stElmt = pPackageCfg->elmtlst(i);
			pCell->setInfo(stElmt.elementtype(), stElmt.elementid(), stElmt.elementcntmin());
			m_pDropListView->pushBackCustomItem(pCell);
		}
	}

	m_pSpecDropListView->removeAllItems();
	
	for(int i = 0; i < pWarMapCfg->hidenawards_size(); i++) {
		const HidenAward &stAward = pWarMapCfg->hidenawards(i);
		UIImageView *pImg = UIImageView::create();

		if(i < mapInfo.hidenawards_size()) {
			const vmsg::CSWarMapHidenAwardElmt &stElmt = mapInfo.hidenawards(i);
			// 领取状态宝箱显示为打开
			pImg->loadTexture(stElmt.pacakcestatus() == 2 ? "war_map/award_box_opened.png" : "war_map/award_box_closed.png", UI_TEX_TYPE_PLIST);
		} else {
			pImg->loadTexture("war_map/award_box_closed.png", UI_TEX_TYPE_PLIST);
		}
		
		pImg->setTouchEnabled(true);
		m_pSpecDropListView->pushBackCustomItem(pImg);
	}

	GUIDE_SHOW(GUIDE_UI_WAR_ENTER);
}

void CLevelEnterPanel::reset(){
	m_pDropListView->removeAllItems();
}

void CLevelEnterPanel::setScoreStar(unsigned int num) {
	m_pStar1Img->setEnabled(true);
	m_pStar2Img->setEnabled(true);
	m_pStar3Img->setEnabled(true);

	if(num == 3){
		m_pStar1Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
	}else if(num == 2){
		m_pStar1Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/star_dark.png", UI_TEX_TYPE_PLIST);
	}else if(num == 1){
		m_pStar1Img->loadTexture("war_map/star_light.png", UI_TEX_TYPE_PLIST);
		m_pStar2Img->loadTexture("war_map/star_dark.png", UI_TEX_TYPE_PLIST);
		m_pStar3Img->loadTexture("war_map/star_dark.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pStar1Img->setEnabled(false);
		m_pStar2Img->setEnabled(false);
		m_pStar3Img->setEnabled(false);
	}
}

void CLevelEnterPanel::open(){
	BasePanel::open();

	showLoading();

	P_BAG_CTRL->sendQueryBagInfoRqst();

	if(!m_bUIInited) {
		initUI();
	}
}

void CLevelEnterPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}

