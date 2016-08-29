#include "ArenaPanel.h"
#include "ArenaControl.h"
#include "NounHelper.h"
#include "TextHelper.h"
#include "Constant.h"
#include "defines.h"
#include "ArenaControl.h"
#include "UserObj.h"
#include "tip/TipControl.h"
#include "rank/RankControl.h"
#include "TimeEx.h"
#include "basic/ToolUtils.h"

#include "ArenaLvCfgMaster.h"
#include "ArenaDailyAwardCfgMaster.h"
#include "ArenaMatchCostCfgMaster.h"
#include "ArenaBuyNumCfgMaster.h"
#include "HeroCfgMaster.h"
#include "PopTipLayer.h"
#include "CommonDialog.h"
#include "GlobalCfgMaster.h"

using namespace std;

CArenaPanel::CArenaPanel(){
	m_uTimeCount = 0;
}

CArenaPanel::~CArenaPanel(){

}

bool CArenaPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/arena_panel_ui.ExportJson");
		addChild(m_pRootLayout);
		setOpenWithBgImg(true);
        setUseBackForClose(false);
		setShowMoneyTopBar(true);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CArenaPanel::initUI(){
	m_pInfoLayer = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("info_layer"));
	m_pTimeLayout = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("time_layout"));

	m_pRuleBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("rule_btn"));
	m_pRankBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("rank_btn"));
	m_pMatchBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("match_btn"));
	m_pRecordBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("record_btn"));
    m_pExchangeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("exchange_btn"));
	m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));

	m_pBuyBtn = dynamic_cast<UIButton*>(m_pInfoLayer->getChildByName("buy_btn"));
	m_pGetBtn = dynamic_cast<UIButton*>(m_pInfoLayer->getChildByName("get_btn"));
	m_pGongxunTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("gongxun_txt"));
	m_pNumTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("num_txt"));
	m_pScoreTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("score_txt"));
	m_pRecordTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("record_txt"));
	m_pPowerTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("power_txt"));
	m_pAwardCoinTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("award_coin_txt"));
	m_pArenaCoinTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("award_arena_coin_txt"));
	m_pMyRankTxt = dynamic_cast<UILabel*>(m_pInfoLayer->getChildByName("my_rank_txt"));
	m_pRoleBgImg = dynamic_cast<ImageView*>(m_pInfoLayer->getChildByName("role_bg_img"));
	m_pGetImg = dynamic_cast<ImageView*>(m_pInfoLayer->getChildByName("geted_img"));
	m_pGetImg->setVisible(false);

	m_pMatchCostTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("match_cost_txt"));
	m_pTimeTxt = dynamic_cast<UILabel*>(m_pTimeLayout->getChildByName("time_txt"));
	m_pCleanBtn = dynamic_cast<UIButton*>(m_pTimeLayout->getChildByName("clean_btn"));
	m_pTimeLayout->setVisible(false);

	m_pRuleBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
	m_pRankBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
	m_pBuyBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
	m_pMatchBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
	m_pRecordBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
    m_pExchangeBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
	m_pCleanBtn->addTouchEventListener(this, toucheventselector(CArenaPanel::touchEventHandler));
}

CCNode* CArenaPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "match_btn") {
		if(m_pMatchBtn != NULL && m_pMatchBtn->isVisible() && m_pMatchBtn->isTouchEnabled()) {
			pRet = m_pMatchBtn;
		}
	}

	return pRet;
}

void CArenaPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "rule_btn") == 0) {
			P_ARENA_CTRL->getRulePanel()->open();
		}else if(strcmp(pszWidgetName, "rank_btn") == 0) {
			P_ARENA_CTRL->getRankPanel()->open();
			P_ARENA_CTRL->getRankPanel()->updateUI();
		}else if(strcmp(pszWidgetName, "buy_btn") == 0) {
			unsigned int cost;
			unsigned int buyNum = P_ARENA_CTRL->getArenaObj()->getArenaInfo().todaybuynum() + 1;
			unsigned int maxNum = P_ARENA_CTRL->getArenaObj()->getArenaInfo().arenamaxbuycnt();
			if(buyNum - 1 >= maxNum){  //购买次数已用完
				COMMON_DIALOG->show(TextStr(ARENA_BUY_NUM_NOT_ENOUGH));
				return;
			}

			ARENA_BUY_NUM_CFG_MASTER->GetGold(buyNum, cost);
			P_TIP_CTRL->GoldCostTip(cost, this, dialogbtneventselector(CArenaPanel::buyFun), 
				TextStr(TEXT_BUY_ARENA_COST_TIP), maxNum - buyNum + 1);
		}else if(strcmp(pszWidgetName, "match_btn") == 0) {
			GUIDE_TOUCH(pWidget);
            //检查剩余次数
            const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
            unsigned int lastNum = MAX_ARENA_FREE_NUM + stArenaInfo.todaybuynum() - stArenaInfo.todayfightnum();
            if(lastNum <= 0){
                POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(ARENA_FIGHT_NUM_NOT_ENOUGH_TXT));
                return;
            }

			unsigned int matchCost;
			ARENA_MATCH_COST_CFG_MASTER->GetCostCoin(stArenaInfo.todaymatchnum() + 1, matchCost);
			if(!P_TIP_CTRL->CoinCostTip(matchCost, this)){
				return;
			}

			P_ARENA_CTRL->getMatchPanel()->open();
		}else if(strcmp(pszWidgetName, "record_btn") == 0) {
			float offsetX = m_pRecordBtn->getSize().width * 0.40f;
			float offsetY = m_pRecordBtn->getSize().height * 0.32f;
			CToolUtils::badgeWidget(m_pRecordBtn, 0, offsetX, offsetY);
			P_ARENA_CTRL->getRecordPanel()->open();		
		}else if(strcmp(pszWidgetName, "exchange_btn") == 0) {
			P_ARENA_CTRL->getExchangePanel()->open();			
		}else if(strcmp(pszWidgetName, "get_btn") == 0) {
			P_ARENA_CTRL->sendArenaGetAwardRqst();	
        } else if(strcmp(pszWidgetName, "exchange_btn") == 0) {
            P_ARENA_CTRL->getExchangePanel()->open();
		}else if(strcmp(pszWidgetName, "back_btn") == 0) {
			this->close();
		}else if(strcmp(pszWidgetName, "clean_btn") == 0) {
			unsigned int cleanCost = GLOBAL_CFG_MASTER->GetCfg().arenacleancdcost();
			P_TIP_CTRL->GoldCostTip(cleanCost, this, dialogbtneventselector(CArenaPanel::cleanFun), TextStr(TEXT_CLEAN_CD_TIP));
		}
		break;
	default:

		break;
	}
}

void CArenaPanel::cleanFun(){
	P_ARENA_CTRL->sendCleanCDRqst();
}

void CArenaPanel::buyFun(){
	P_ARENA_CTRL->sendArenaBuyFightNumRqst();	
}

void CArenaPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();

	this->unschedule(schedule_selector(CArenaPanel::timeCountDown));

	if(stArenaInfo.remainfightcd() > 0){
		m_uTimeCount = stArenaInfo.remainfightcd();
		m_pTimeLayout->setVisible(true);
		m_pCleanBtn->setTouchEnabled(true);
		m_pMatchBtn->setBright(false);
		m_pMatchBtn->setTouchEnabled(false);
		m_pTimeTxt->setText(TimeEx::getStringTimeForInt(stArenaInfo.remainfightcd(), "ms"));
		this->schedule(schedule_selector(CArenaPanel::timeCountDown), 1.0f);
	}else{
		m_pMatchBtn->setBright(true);
		m_pMatchBtn->setTouchEnabled(true);
		m_pTimeLayout->setVisible(false);
		m_pCleanBtn->setTouchEnabled(false);
	}

	m_pPowerTxt->setText(intToString(P_USER_OBJ->getUserInfo()->fightPower));
	m_pScoreTxt->setText(intToString(stArenaInfo.score()));

	updateNum(stArenaInfo.todayfightnum(), stArenaInfo.todaybuynum());

	string str = intToString(stArenaInfo.winnum()) + NounStr(WIN_STRING_TXT) + "/";
	str += intToString(stArenaInfo.losenum()) + NounStr(FAIL_STRING_TXT);
	m_pRecordTxt->setText(str);

    m_pGongxunTxt->setText( intToString(stArenaInfo.arenamoney()) );

    const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stArenaInfo.score());
    if(stCfg) {
        string iconPath = "ui_v2/arena_lv/arena_lv_" + intToString(stCfg->level()) + ".png";
        m_pRoleBgImg->loadTexture(iconPath.c_str());

        const ArenaDailyAwardCfg* stAwardCfg = ARENA_DAILY_AWARD_CFG_MASTER->GetOneCfg(stCfg->level());
        for(int i = 0; i < stAwardCfg->elmtlst_size(); i++){
            const ArenaAwardElement& stElmt = stAwardCfg->elmtlst(i);
            if(stElmt.type() == THING_TYPE_COIN){
                m_pAwardCoinTxt->setText("x" + intToString(stElmt.cnt()));
            }else if(stElmt.type() == THING_TYPE_ARENA_MONEY){
                m_pArenaCoinTxt->setText("x" + intToString(stElmt.cnt()));
            }
        }
    }
	
	const CSArenaQueryRecordRsp& stRecord = P_ARENA_CTRL->getArenaObj()->getRecordInfo();
	float offsetX = m_pRecordBtn->getSize().width * 0.40f;
	float offsetY = m_pRecordBtn->getSize().height * 0.32f;
	CToolUtils::badgeWidgetWithPoint(m_pRecordBtn, stRecord.recordlst_size(), offsetX, offsetY);

	CToolUtils::badgeWidgetWithPoint(m_pMatchBtn, P_ARENA_CTRL->hasFreeChallenge() ? 1 : 0, offsetX, offsetY);

	updateMatchCost();
	updateAwardBtn();

	GUIDE_SHOW(GUIDE_UI_ARENA);
}

void CArenaPanel::timeCountDown(float dt){
	if(m_uTimeCount <= 0){
		this->unschedule(schedule_selector(CArenaPanel::timeCountDown));
		P_ARENA_CTRL->sendArenaQueryRqst();
		return;
	}

	m_uTimeCount--;
	m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uTimeCount, "ms"));
}

void CArenaPanel::updateCleanCD(){
	this->unschedule(schedule_selector(CArenaPanel::timeCountDown));
	P_ARENA_CTRL->sendArenaQueryRqst();
}

void CArenaPanel::updateMatchCost(){
    
	const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();

	unsigned int matchCost;
	ARENA_MATCH_COST_CFG_MASTER->GetCostCoin(stArenaInfo.todaymatchnum() + 1, matchCost);

	if(matchCost == 0){
		m_pMatchCostTxt->setText(TextStr(ARENA_FREE_MATCH));
	}else{
		char words[100];
		snprintf(words, 100, TextStr(ARENA_MATCH_COST), matchCost);
		m_pMatchCostTxt->setText(words);
	}
}

void CArenaPanel::updateAwardBtn(){
	const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
	if(stArenaInfo.ishavetodayaward()){
		m_pGetBtn->setVisible(true);
        m_pGetBtn->setEnabled(true);
		m_pGetImg->setVisible(false);

	}else{
        m_pGetBtn->setVisible(false);
        m_pGetBtn->setEnabled(false);
		m_pGetImg->setVisible(true);
	}
}

void CArenaPanel::updateNum(unsigned int fightNum, unsigned int buyNum){
	unsigned int lastNum = MAX_ARENA_FREE_NUM + buyNum - fightNum;
	m_pNumTxt->setText(intToString(lastNum) + "/" + intToString(MAX_ARENA_FREE_NUM));
}

void CArenaPanel::updateMyRank(unsigned int rank){
	const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stArenaInfo.score());
	string str = stCfg->name() + "  ";
	if(rank < 0){
		str += NounStr(NO_RANK_TIP);
	}else{
		str += intToString(rank + 1);
	}

	m_pMyRankTxt->setText(str);
}


void CArenaPanel::open(){
	BasePanel::open();

	P_ARENA_CTRL->sendArenaQueryRqst();
	P_RANK_CTL->sendRankQueryMyRankRqst(RANK_TYPE_ARENA);

	this->showLoading();
}

void CArenaPanel::close(){
	BasePanel::close();
}