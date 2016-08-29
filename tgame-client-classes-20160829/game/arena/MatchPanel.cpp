#include "MatchPanel.h"
#include "ArenaControl.h"
#include "defines.h"
#include "TextHelper.h"
#include "CommonDialog.h"
#include "Constant.h"
#include "UserObj.h"
#include "NounHelper.h"
#include "Resources.h"
#include "tip/TipControl.h"

#include "HeroCfgMaster.h"
#include "ArenaLvCfgMaster.h"
#include "ArenaMatchCostCfgMaster.h"

CMatchPanel::CMatchPanel(){

}

CMatchPanel::~CMatchPanel(){

}

bool CMatchPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/match_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pHandBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("hand_challenge_btn"));
		m_pAutoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("auto_challenge_btn"));
		m_pRematchBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("rematch_btn"));
		m_pAutoBtn->setEnabled(false);
		
		m_pHeadImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("head_img"));

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
		m_pRankTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("rank_txt"));
		m_pPowerTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("power_txt"));
		m_pMatchCostTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("match_cost_txt"));
		m_pMatchLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CMatchPanel::touchEventHandler));
		m_pHandBtn->addTouchEventListener(this, toucheventselector(CMatchPanel::touchEventHandler));
		m_pAutoBtn->addTouchEventListener(this, toucheventselector(CMatchPanel::touchEventHandler));
		m_pRematchBtn->addTouchEventListener(this, toucheventselector(CMatchPanel::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

CCNode* CMatchPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "hand_btn") {
		if(m_pHandBtn != NULL && m_pHandBtn->isVisible() && m_pHandBtn->isTouchEnabled()) {
			pRet = m_pHandBtn;
		}
	}

	return pRet;
}

void CMatchPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}else if(strcmp(pszWidgetName, "hand_challenge_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
			unsigned int lastNum = MAX_ARENA_FREE_NUM + stArenaInfo.todaybuynum() - stArenaInfo.todayfightnum();
			if(lastNum <= 0){
				COMMON_DIALOG->show(TextStr(ARENA_FIGHT_NUM_NOT_ENOUGH_TXT), NULL, NounStr(OK));
				return;
			}

			P_ARENA_CTRL->sendArenaStartFightRqst(P_ARENA_CTRL->getArenaObj()->getOpponentUin());
			this->showLoading();
		}else if(strcmp(pszWidgetName, "auto_challenge_btn") == 0) {

		}else if(strcmp(pszWidgetName, "rematch_btn") == 0) {
			const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
			unsigned int matchCost;
			ARENA_MATCH_COST_CFG_MASTER->GetCostCoin(stArenaInfo.todaymatchnum() + 1, matchCost);
            if(!P_TIP_CTRL->CoinCostTip(matchCost, this)){
				return;
			}

			P_ARENA_CTRL->sendArenaFindOpponentRqst();	
		}
		break;
	default:

		break;
	}
}

void CMatchPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const CSArenaFindOpponentRsp& stFindInfo = P_ARENA_CTRL->getArenaObj()->getFindInfo();

	m_pNameTxt->setText(stFindInfo.name());
	m_pPowerTxt->setText(intToString(stFindInfo.fightpower()));
	m_pMatchLevelTxt->setText(intToString(stFindInfo.opponentlevel()));

	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stFindInfo.score());
	m_pRankTxt->setText(CCString::createWithFormat("%s  %d",stCfg->name().c_str(), stFindInfo.score())->getCString());

	string url = HEAD_ICON_PATH + HERO_CFG_MASTER->GetHeroCfgByID(stFindInfo.showid())->headicon() + PNG_TYPE_NAME;
	m_pHeadImg->loadTexture(url.c_str());

	updateMatchCost();

	GUIDE_SHOW(GUIDE_UI_ARENA_MATCH);
}

void CMatchPanel::updateMatchCost(){
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

void CMatchPanel::matchFailFun(){
	if(this->isOpen()){
		this->close();
	}

	COMMON_DIALOG->show(TextStr(ARENA_MATCH_FAIL_TXT), NULL, NounStr(OK));
}

void CMatchPanel::open(){
	BasePanel::open();

	P_ARENA_CTRL->sendArenaFindOpponentRqst();

	this->showLoading(this, callfunc_selector(CMatchPanel::matchFailFun));
}

void CMatchPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}