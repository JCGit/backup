#include "GenStrengthenEffectPanel.h"
#include "GenStrengthenControl.h"
#include "TextHelper.h"
#include "defines.h"
#include "UserObj.h"

#include "PlayerEnforceCfgMaster.h"

CGenStrengthenEffectPanel::CGenStrengthenEffectPanel(){

}

CGenStrengthenEffectPanel::~CGenStrengthenEffectPanel(){

}

bool CGenStrengthenEffectPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/gen_eff_panel_ui.ExportJson");
		addChild(m_pRootLayout);
		
		setClickOutToClose(false);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

CCNode* CGenStrengthenEffectPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "retain_btn") {
		if(m_pSaveBtn != NULL && m_pSaveBtn->isRunning() && m_pSaveBtn->isEnabled() && m_pSaveBtn->isTouchEnabled()) {
			pRet = m_pSaveBtn;
		} else if(m_pSureBtn != NULL && m_pSureBtn->isRunning() && m_pSureBtn->isEnabled() && m_pSureBtn->isTouchEnabled()) {
			pRet = m_pSureBtn;
		}
	}

	return pRet;
}

void CGenStrengthenEffectPanel::initUI(){
	m_pSaveBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("save_btn"));
	m_pSaveBtn->addTouchEventListener(this, toucheventselector(CGenStrengthenEffectPanel::touchEventHandler));

	m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));
	m_pCancelBtn->addTouchEventListener(this, toucheventselector(CGenStrengthenEffectPanel::touchEventHandler));

	m_pSureBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("sure_btn"));
	m_pSureBtn->addTouchEventListener(this, toucheventselector(CGenStrengthenEffectPanel::touchEventHandler));
	m_pSureBtn->setEnabled(false);

	m_pAtkTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("atk_txt"));
	m_pHpTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("hp_txt"));
	m_pDefTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("def_txt"));

	m_pAtkTip = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("atk_tip"));
	m_pHpTip = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("hp_tip"));
	m_pDefTip = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("def_tip"));
}

void CGenStrengthenEffectPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	int uAtkAdd = P_GEN_CTRL->getAtkAdd();
	int uDefAdd = P_GEN_CTRL->getDefAdd();
	int uHpAdd = P_GEN_CTRL->getHpAdd();

	setAddStr(uAtkAdd, P_USER_OBJ->getUserInfo()->totalAtkAdd, m_pAtkTxt, m_pAtkTip);
	setAddStr(uHpAdd, P_USER_OBJ->getUserInfo()->totalHpAdd, m_pHpTxt, m_pHpTip);
	setAddStr(uDefAdd, P_USER_OBJ->getUserInfo()->totalDefAdd, m_pDefTxt, m_pDefTip);

	bool bIsSureOnly = false; 
	if(P_GEN_CTRL->getGenType() != vmsg::SUPER_PLAYER_ENFOCE){	//非至尊强化
		if(uAtkAdd >= 0 && uDefAdd >= 0 && uHpAdd >= 0){
			bIsSureOnly = true;
		}else{
			bIsSureOnly = false;
		}
	}else{
		bIsSureOnly = true;
	}

	if(bIsSureOnly){
		m_pSaveBtn->setEnabled(false);
		m_pCancelBtn->setEnabled(false);
		m_pSureBtn->setEnabled(true);
	}else{
		m_pSaveBtn->setEnabled(true);
		m_pCancelBtn->setEnabled(true);
		m_pSureBtn->setEnabled(false);
	}

	GUIDE_SHOW(GUIDE_UI_STRENGTHEN_EFFECT);
}

void CGenStrengthenEffectPanel::setAddStr(int addVal, unsigned int totalAdd, Label* pLabel, ImageView* pTip){
	string addStr = "";
	pTip->setVisible(false);

	if(addVal > 0){
		addStr = "+" + intToString(addVal);
		pLabel->setColor(ccc3(0x01, 0xFF, 0x0D));
	}else if(addVal == 0){
		addStr = "0";
		if(totalAdd == 0){   //当前已经为下限值
			pTip->setVisible(true);
			pTip->loadTexture("gen_ui/down_limit_tip.png", UI_TEX_TYPE_PLIST);
			pLabel->setColor(ccc3(0xFF, 0x24, 0x00));
		}else{
			const PlayerEnforceCfg* stCfg = PLAYER_ENFORCE_CFG_MASTER->GetCfgByLev(P_USER_OBJ->getUserInfo()->level);
			if(stCfg && totalAdd == stCfg->enforcetotalmax()){   //当前已满
				pTip->setVisible(true);
				pTip->loadTexture("gen_ui/up_limit_tip.png", UI_TEX_TYPE_PLIST);
			}
			pLabel->setColor(ccc3(0x01, 0xFF, 0x0D));
		}
	}else{
		addStr = intToString(addVal);
		pLabel->setColor(ccc3(0xFF, 0x24, 0x00));
	}

	pLabel->setText(addStr);
}

void CGenStrengthenEffectPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "save_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			doConfirm(1);
		}else if(strcmp(pszWidgetName, "cancel_btn") == 0) {
			doConfirm(0);
		}else if(strcmp(pszWidgetName, "sure_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			doConfirm(1);
		}
		break;
	default:

		break;
	}
}

void CGenStrengthenEffectPanel::doConfirm(unsigned int isAccept){
	P_GEN_CTRL->setIsAccept(isAccept);
	
	P_GEN_CTRL->sendGenStrengthenConfirmRqst(isAccept);

	this->close();
	this->showLoading();
}

void CGenStrengthenEffectPanel::open(){
	BasePanel::open();
}

void CGenStrengthenEffectPanel::close(){
	BasePanel::close();
}

