#include "GenStrengthenPanel.h"
#include "GenStrengthenControl.h"
#include "tip/TipControl.h"
#include "defines.h"
#include "UserObj.h"

#include "GenTypeCfgMaster.h"
#include "PlayerEnforceCfgMaster.h"

using namespace vmsg;

CGenStrengthenPanel::CGenStrengthenPanel(){
	m_uGenType = REGULAR_PLAYER_ENFORCE;
	m_uCostCnt = 0;
	m_pBtnLst = NULL;
}

CGenStrengthenPanel::~CGenStrengthenPanel(){
	if(m_pBtnLst) {
		m_pBtnLst->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_pBtnLst);
	}
}

bool CGenStrengthenPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/gen_panel_ui.ExportJson");
		addChild(m_pRootLayout);
		setOpenWithBgImg(true);
		setUseBackForClose(false);
		setShowMoneyTopBar(true);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

CCNode* CGenStrengthenPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "back_btn") {
		if(m_pCloseBtn != NULL && m_pCloseBtn->isRunning() && m_pCloseBtn->isTouchEnabled()) {
			pRet = m_pCloseBtn;
		}
	} else if(name == "gene_btn") {
		if(m_pGenBtn != NULL && m_pGenBtn->isRunning() && m_pGenBtn->isTouchEnabled()) {
			pRet = m_pGenBtn;
		}
	}

	return pRet;
}

void CGenStrengthenPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CGenStrengthenPanel::touchEventHandler));

	m_pGenBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("gen_btn"));
	m_pGenBtn->addTouchEventListener(this, toucheventselector(CGenStrengthenPanel::touchEventHandler));

	m_pTypeImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("type_tip_img"));
	m_pCostIcon = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("cost_icon"));
	m_pAtkBarBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("atk_bar_bg"));
	m_pHpBarBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("hp_bar_bg"));
	m_pDefBarBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("def_bar_bg"));
	m_pFullImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("full_img"));
	m_pFullImg->setVisible(false);

	m_pTipTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("tip_txt"));
	m_pCostTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("cost_txt"));
	m_pAtkUpTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("atk_up_txt"));
	m_pHpUpTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("hp_up_txt"));
	m_pDefUpTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("def_up_txt"));
	m_pAtkBarTxt = dynamic_cast<Label*>(m_pAtkBarBg->getChildByName("bar_txt"));
	m_pHpBarTxt = dynamic_cast<Label*>(m_pHpBarBg->getChildByName("bar_txt"));
	m_pDefBarTxt = dynamic_cast<Label*>(m_pDefBarBg->getChildByName("bar_txt"));

	m_pAtkProgress = dynamic_cast<LoadingBar*>(m_pAtkBarBg->getChildByName("bar"));
	m_pHpProgress = dynamic_cast<LoadingBar*>(m_pHpBarBg->getChildByName("bar"));
	m_pDefProgress = dynamic_cast<LoadingBar*>(m_pDefBarBg->getChildByName("bar"));

	m_pBtnLst = CCArray::create();
	m_pBtnLst->retain();

	for(unsigned int i = 0; i < 4; i++){
		string btnStr = "type_btn_" + intToString(i); 
		Button* pBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName(btnStr.c_str()));
		pBtn->addTouchEventListener(this, toucheventselector(CGenStrengthenPanel::touchEventHandler));

		m_pBtnLst->addObject(pBtn);
	}
}

void CGenStrengthenPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	unsigned int uLv = P_USER_OBJ->getUserInfo()->level;
	unsigned int uTotalAtkAdd = P_USER_OBJ->getUserInfo()->totalAtkAdd;
	unsigned int uTotalHpAdd = P_USER_OBJ->getUserInfo()->totalHpAdd;
	unsigned int uTotalDefAdd = P_USER_OBJ->getUserInfo()->totalDefAdd;
	const PlayerEnforceCfg* stCfg = PLAYER_ENFORCE_CFG_MASTER->GetCfgByLev(uLv);

	if(stCfg == NULL){
		return;
	}

	unsigned int uTotalMax = stCfg->enforcetotalmax();

	m_pAtkBarTxt->setText(intToString(uTotalAtkAdd) + "/" + intToString(uTotalMax));
	m_pHpBarTxt->setText(intToString(uTotalHpAdd) + "/" + intToString(uTotalMax));
	m_pDefBarTxt->setText(intToString(uTotalDefAdd) + "/" + intToString(uTotalMax));

	m_pAtkUpTxt->setText(intToString(uTotalAtkAdd*stCfg->atkadd()));
	m_pHpUpTxt->setText(intToString(uTotalHpAdd*stCfg->hpadd()));
	m_pDefUpTxt->setText(intToString(uTotalDefAdd*stCfg->defadd()));

	m_pAtkProgress->setPercent(100*uTotalAtkAdd/uTotalMax);
	m_pHpProgress->setPercent(100*uTotalHpAdd/uTotalMax);
	m_pDefProgress->setPercent(100*uTotalDefAdd/uTotalMax);

	if(uTotalAtkAdd == uTotalMax && uTotalHpAdd == uTotalMax && uTotalDefAdd == uTotalMax){  //3项全满
		m_pFullImg->setVisible(true);
		m_pGenBtn->setEnabled(false);
	}else{
		m_pFullImg->setVisible(false);
		m_pGenBtn->setEnabled(true);
	}

	GUIDE_SHOW(GUIDE_UI_GENE_STRENGTHEN);
}

void CGenStrengthenPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			this->close();
		}else if(strcmp(pszWidgetName, "gen_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			doGenCost();
		}else if(strWidgetName.find("type_btn_") != string::npos) {
			string sub = strWidgetName.substr(strWidgetName.find_first_not_of("type_btn_"));
			int iIdx = atoi(sub.c_str());

			if(m_uGenType == iIdx + 1){  
				Button* pBtn = dynamic_cast<Button*>(m_pBtnLst->objectAtIndex(iIdx));
				if(pBtn){
					pBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
				}
				return;
			}
			selectType(iIdx + 1);
		}
		break;
	default:

		break;
	}
}

void CGenStrengthenPanel::selectType(unsigned int uType){
	const GenTypeCfg* stCfg = GEN_TYPE_CFG_MASTER->GetCfgByType(uType);
	if(stCfg == NULL){
		return;
	}

	if(P_USER_OBJ->getUserInfo()->vipLv < stCfg->limitviplv()){  //vip等级不足
		P_TIP_CTRL->ShowVipLimitTip(stCfg->limitviptip());
		return;
	}

	unsigned int uPreType = m_uGenType;
	m_uGenType = uType;

	if(m_uGenType == REGULAR_PLAYER_ENFORCE){  //普通强化,消耗联邦币
		m_pCostIcon->loadTexture("share/icon_coin.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pCostIcon->loadTexture("share/icon_gold.png", UI_TEX_TYPE_PLIST);
	}

	m_uCostCnt = P_GEN_CTRL->getCostByType(m_uGenType);
	m_pCostTxt->setText("x" + intToString(m_uCostCnt));

	char str[200];
	snprintf(str, 200, stCfg->costtip().c_str(), m_uCostCnt);
	m_pTipTxt->setText(str);

	string typeStr = "gen_ui/gen_type_big_img_" + intToString(uType - 1) + ".png";
	m_pTypeImg->loadTexture(typeStr.c_str(), UI_TEX_TYPE_PLIST);

	if(m_uGenType - 1 < 0 || m_uGenType - 1 > m_pBtnLst->count()){
		return;
	}

	for(unsigned int i = 0; i < m_pBtnLst->count(); i++){
		Button* pBtn = dynamic_cast<Button*>(m_pBtnLst->objectAtIndex(i));
		if(i == m_uGenType - 1){
			pBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
		}else{
			pBtn->setBrightStyle(BRIGHT_NORMAL);
		}
	}
}

void CGenStrengthenPanel::doGenCost(){
	if(m_uGenType == REGULAR_PLAYER_ENFORCE){  //普通强化,消耗联邦币
		if(P_TIP_CTRL->CoinCostTip(m_uCostCnt, this)){
			doGen();
		}
	}else{
		P_TIP_CTRL->GoldCostTip(m_uCostCnt, this, dialogbtneventselector(CGenStrengthenPanel::doGen), TextStr(TEXT_GEN_COST_GOLD_TIP));
	}
}

void CGenStrengthenPanel::doGen(){
	P_GEN_CTRL->getGenEffPanel()->open();
	P_GEN_CTRL->setGenType(m_uGenType);
	P_GEN_CTRL->sendGenStrengthenRqst(m_uGenType);

	this->showLoading();
}

void CGenStrengthenPanel::open(){
	BasePanel::open();

	P_GEN_CTRL->sendQueryGenStrengthenRqst();

	//默认为普通强化
	selectType(REGULAR_PLAYER_ENFORCE);

	this->showLoading();
}

void CGenStrengthenPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}

