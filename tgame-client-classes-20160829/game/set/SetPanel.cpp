#include "SetPanel.h"
#include "BaseScene.h"
#include "HudLayer.h"
#include "SetSysControl.h"
#include "control/AudioManager.h"
#include "GameInf.h"
#include "FaceBookSDK.h"

CSetPanel::CSetPanel(){

}

CSetPanel::~CSetPanel(){

}

bool CSetPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/set_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(true);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CSetPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pExitBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("exit_btn"));
	m_pBindBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("bind_btn"));
	m_pFeedbackBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("feedback_btn"));


	m_stInitPos = m_pFeedbackBtn->getPosition();

	m_pSoundBtn = dynamic_cast<CheckBox*>(m_pRootLayout->getChildByName("sound_btn"));
	m_pSoundEffBtn = dynamic_cast<CheckBox*>(m_pRootLayout->getChildByName("sound_eff_btn"));
	m_pHudBtn = dynamic_cast<CheckBox*>(m_pRootLayout->getChildByName("hud_btn"));

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CSetPanel::touchEventHandler));
	m_pExitBtn->addTouchEventListener(this, toucheventselector(CSetPanel::touchEventHandler));
	m_pBindBtn->addTouchEventListener(this, toucheventselector(CSetPanel::touchEventHandler));
	m_pFeedbackBtn->addTouchEventListener(this, toucheventselector(CSetPanel::touchEventHandler));

	m_pSoundBtn->addEventListenerCheckBox(this, checkboxselectedeventselector(CSetPanel::checkBoxCallBackFun));
	m_pSoundEffBtn->addEventListenerCheckBox(this, checkboxselectedeventselector(CSetPanel::checkBoxCallBackFun));
	m_pHudBtn->addEventListenerCheckBox(this, checkboxselectedeventselector(CSetPanel::checkBoxCallBackFun));
}

void CSetPanel::updateUI(){
	m_pSoundBtn->setSelectedState(AudioManager::isBackgroundOn());
	m_pSoundEffBtn->setSelectedState(AudioManager::isEffectOn());
	m_pHudBtn->setSelectedState(P_SET_CTRL->isSet(SetSysControl::HUD_ROCKER_STILL_TYPE));

	if(FaceBookSDK::getIsGuest()){  //游客
		m_pBindBtn->setEnabled(true);
		m_pFeedbackBtn->setPosition(m_stInitPos);
	}else{
		m_pBindBtn->setEnabled(false);
		m_pFeedbackBtn->setPositionX(m_pExitBtn->getPositionX());
	}
}

void CSetPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			close();
		}else if(strcmp(pszWidgetName, "exit_btn") == 0) {
			close();
            P_GAME_INF->doSDKLogout();
		}else if(strcmp(pszWidgetName, "bind_btn") == 0) {		//绑定账号
			FaceBookSDK::onBind();
			this->close();
		}
		else if (strcmp(pszWidgetName, "feedback_btn") == 0) {	//客服反馈
			FaceBookSDK::onFeedBack("");
		}
		break;
	default:;
	}
}

void CSetPanel::checkBoxCallBackFun(CCObject* pSender,CheckBoxEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strName = pszWidgetName;
	bool isSel = false;
	
	switch(type) {
	case CHECKBOX_STATE_EVENT_SELECTED:
		isSel = true;
		break;
	case CHECKBOX_STATE_EVENT_UNSELECTED:
		isSel = false;
		break;
	default:
		isSel = true;
		break;
	}

	setOpStat(strName, isSel);
}

void CSetPanel::setOpStat(string stName, bool isOn){
	unsigned int uType = SetSysControl::BACKGROUND_SOUND_TYPE;
	if(stName == "sound_btn"){  //背景音乐
		uType = SetSysControl::BACKGROUND_SOUND_TYPE;
	}else if(stName == "sound_eff_btn"){ //音效
		uType = SetSysControl::SOUND_EFFECT_TYPE;
	}else if(stName == "hud_btn"){ //摇杆
		uType = SetSysControl::HUD_ROCKER_STILL_TYPE;
	}

	bool bPreStat = P_SET_CTRL->isSet(uType);
	if(bPreStat != isOn){   //状态改变
		P_SET_CTRL->setOpStat(uType, isOn, true);

		setOpFun(uType);
	}
}

void CSetPanel::setOpFun(unsigned int uType){
	switch (uType)
	{
	case SetSysControl::BACKGROUND_SOUND_TYPE:
		if(AudioManager::isBackgroundOn()){
			AudioManager::playBackgroundMusic(AudioManager::getNowBgSound().c_str());
		}else{
			AudioManager::stopBackgroundMusic();
		}
		break;
	case SetSysControl::SOUND_EFFECT_TYPE:
		if(!AudioManager::isEffectOn()){
			AudioManager::stopAllEffects();
		}
		break;
	case SetSysControl::HUD_ROCKER_STILL_TYPE:
		setHudMove();
		break;
	default:
		break;
	}
}

void CSetPanel::setHudMove(){
	CCDirector *pDirector = CCDirector::sharedDirector();
	BaseScene *pRunningScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
	HudLayer* pHudLayer = dynamic_cast<HudLayer*>(pRunningScene->getLayer(LAYER_ID_HUD));
	bool bIsHudMove = !P_SET_CTRL->isSet(SetSysControl::HUD_ROCKER_STILL_TYPE);
	if(pHudLayer && bIsHudMove != pHudLayer->getIsFollowRole()){
		pHudLayer->setIsFollowRole(bIsHudMove);
	}
}

void CSetPanel::open(){
	BasePanel::open();

	updateUI();
}

void CSetPanel::close(){
	BasePanel::close();
}

