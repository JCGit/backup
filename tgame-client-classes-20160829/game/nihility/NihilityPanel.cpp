#include "NihilityPanel.h"
#include "UserObj.h"
#include "defines.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "NihilityControl.h"
#include "CommonDialog.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"
#include "rank/RankControl.h"
#include "bag/BagControl.h"

#include "GlobalCfgMaster.h"

CNihilityPanel::CNihilityPanel(){

}

CNihilityPanel::~CNihilityPanel(){

}

bool CNihilityPanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/nihility_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CNihilityPanel::initUI(){
	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pNormalBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("normal_btn"));

	Label* pScoreTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("max_score"));
	Label* pRankTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("max_rank"));
	Label* pFloorTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("max_floor"));
	Label* pManualTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("manual_txt"));
	pScoreTxt->setVisible(false);
	pRankTxt->setVisible(false);
	pFloorTxt->setVisible(false);
	pManualTxt->setVisible(false);

	m_pRankTipImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("rank_tip_img"));
	m_pRankTipImg->setVisible(false);

	m_pScoreTxt = ImageNumber::create(21);
	m_pScoreTxt->setScale(0.8f);
	m_pScoreTxt->setNum("0");
	m_pScoreTxt->setAnchorPoint(ccp(0, 0.5));
	m_pRootLayout->addNode(m_pScoreTxt, 10);
	m_pScoreTxt->setPosition(pScoreTxt->getPosition() + ccp(0, m_pScoreTxt->getContentSize().height/2));

	m_pRankTxt = ImageNumber::create(21);
	m_pRankTxt->setAnchorPoint(ccp(0, 0.5));
	m_pRankTxt->setScale(0.8f);
	m_pRankTxt->setNum("0");
	m_pRootLayout->addNode(m_pRankTxt, 10);
	m_pRankTxt->setPosition(pRankTxt->getPosition() + ccp(0, m_pRankTxt->getContentSize().height*m_pRankTxt->getScale()/2));

	m_pFloorTxt = ImageNumber::create(21);
	m_pFloorTxt->setNum("0");
	m_pFloorTxt->setAnchorPoint(ccp(0, 0.5));
	m_pRootLayout->addNode(m_pFloorTxt, 10);
	m_pFloorTxt->setPosition(pFloorTxt->getPosition() + ccp(0, m_pFloorTxt->getContentSize().height/2));

	m_pManualTxt = ImageNumber::create(21);
	m_pManualTxt->setNum("0");
	m_pManualTxt->setScale(0.8f);
	m_pManualTxt->setAnchorPoint(ccp(0, 0.5));
	m_pRootLayout->addNode(m_pManualTxt, 10);
	m_pManualTxt->setPosition(pManualTxt->getPosition() + ccp(0, m_pManualTxt->getContentSize().height/2));

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CNihilityPanel::touchEventHandler));
	m_pNormalBtn->addTouchEventListener(this, toucheventselector(CNihilityPanel::touchEventHandler));
}

CCNode* CNihilityPanel::getGuideNodeByName(const string &name) {
	UIButton *pBtn = NULL;

	if(name == "normal_challenge_btn") {
		if(m_pNormalBtn != NULL && m_pNormalBtn->isRunning() && m_pNormalBtn->isVisible() && m_pNormalBtn->isTouchEnabled()) {
			pBtn = m_pNormalBtn;
		}
	}

	return pBtn;
}

void CNihilityPanel::updateUI(){
	unsigned int uFloor = P_USER_OBJ->getUserInfo()->stNihilityInfo.floor();
	unsigned int uScore = P_USER_OBJ->getUserInfo()->stNihilityInfo.score();

	m_pScoreTxt->setNum(intToString(uScore), -1);
	m_pFloorTxt->setNum(intToString(uFloor));
	m_pManualTxt->setNum(intToString(GLOBAL_CFG_MASTER->GetCfg().infinitemanual()));

	GUIDE_SHOW(GUIDE_UI_NOTHING_LAND);
}

void CNihilityPanel::updateRank(int rank){
	if(rank < 0 || rank > 1000){
		m_pRankTipImg->setVisible(true);
		m_pRankTxt->setVisible(false);
	}else{
		m_pRankTipImg->setVisible(false);
		m_pRankTxt->setVisible(true);
		m_pRankTxt->setNum(intToString(rank));
	}
}

void CNihilityPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			close();
		}else if(strcmp(pszWidgetName, "normal_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			sendStart();
		}
		break;
	default:

		break;
	}
}

void CNihilityPanel::sendStart(){
	if(P_NIHILITY_CTRL->getbIsContinue()){
		P_NIHILITY_CTRL->getContinuePanel()->open();
	}else{
		P_NIHILITY_CTRL->getGenPanel()->open();
	}
}

void CNihilityPanel::open(){
	BasePanel::open();

	updateUI();

	P_RANK_CTL->setIsRankPanel(false);
	P_RANK_CTL->sendRankQueryMyRankRqst(RANK_TYPE_NOTHINGLANDSCORE);
	P_NIHILITY_CTRL->sendNihilityUnfinishedQueryRqst();

	this->showLoading();
}

void CNihilityPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}

void CNihilityPanel::closeLoad(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}
}
