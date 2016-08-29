#include "FightGuideControl.h"
#include "FightGuideDialog.h"
#include "FightGuideLayout.h"
#include "BaseScene.h"
#include "FightGuideScene.h"
#include "FightGuideHud.h"
#include "SceneManager.h"
#include "UserObj.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"
#include "evtlog/EvtLog.h"

#include "FightGuideCfgMaster.h"

CFightGuideControl::CFightGuideControl(){
	_stepID = -1;
	_direct = -1;
	m_bInitialized = false;
	m_bIsShowDialog = false;
	m_bIsInGuide = false;
	m_bIsShowForceGuide = false;
	m_pDialogLayout = NULL;
	m_pGuideLayout = NULL;
}

CFightGuideControl::~CFightGuideControl(){
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pDialogLayout);
		CC_SAFE_RELEASE_NULL(m_pGuideLayout);
		m_bInitialized = false;
	}
}

void CFightGuideControl::enterFightGuideScene(){
	P_FIGHT_OBJ->setFightType(GUIDE_FIGHT_TYPE);
	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
}

void CFightGuideControl::enterGuideNormalScene(){
	P_FIGHT_OBJ->setFightType(GUIDE_NORMAL_FIGHT_TYPE);
	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
}

void CFightGuideControl::startGuide(){
	_stepID = 0;
	m_bIsInGuide = true;
	init();

	nextStep();
}

void CFightGuideControl::init(){
	if(!m_bInitialized) {
		m_pDialogLayout = CFightGuideDialog::create();
		CC_SAFE_RETAIN(m_pDialogLayout);

		m_pGuideLayout = CFightGuideLayout::create();
		CC_SAFE_RETAIN(m_pGuideLayout);
		m_bInitialized = true;
	}
}

void CFightGuideControl::endGuide(){
	getGuideScene()->removeFireBtns();
	m_bIsInGuide = false;
}

void CFightGuideControl::showDialog() {
	if(m_bIsShowDialog){
		return;
	}

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCScene *pScene = pDirector->getRunningScene();

	if(pScene != NULL) {
		CCLayer *pLayer = dynamic_cast<BaseScene*>(pScene)->getLayer(LAYER_ID_TOP);
		UILayer *pTopLayer = dynamic_cast<UILayer*>(pLayer);

		if(m_pDialogLayout != NULL && m_pDialogLayout->getParent() == NULL) {
			pTopLayer->addWidget(m_pDialogLayout);
			m_pDialogLayout->showDialog();

			m_bIsShowDialog = true;
		}
	}
}

void CFightGuideControl::hideDialog() {
	if(m_pDialogLayout != NULL && m_pDialogLayout->getParent() != NULL) {
		m_pDialogLayout->removeFromParent(); 
		m_bIsShowDialog = false;
	}
}

CFightGuideLayout* CFightGuideControl::getGuideLayout(){
	return m_pGuideLayout;
}

CFightGuideScene* CFightGuideControl::getGuideScene(){
	CCDirector *pDirector = CCDirector::sharedDirector();
	CFightGuideScene *pScene = dynamic_cast<CFightGuideScene*>(pDirector->getRunningScene());

	return pScene;
}

void CFightGuideControl::addGuide(){
	if(m_bIsShowForceGuide){
		return;
	}

	if(m_pGuideLayout != NULL && m_pGuideLayout->getParent() == NULL) {
		CCDirector *pDirector = CCDirector::sharedDirector();
		BaseScene *pScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
		UILayer *pTopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_TOP));
		pTopLayer->addWidget(m_pGuideLayout);
		m_bIsShowForceGuide = true;
	}
}

void CFightGuideControl::removeGuide(){
	if(m_pGuideLayout != NULL && m_pGuideLayout->getParent() != NULL) {
		m_pGuideLayout->clearAllStencil();
		m_pGuideLayout->removeFromParent();
		m_bIsShowForceGuide = false;
	}
}

void CFightGuideControl::nextStep(){
	if(m_bIsInGuide == false){
		return;
	}

	_stepID++;

	CCDirector *pDirector = CCDirector::sharedDirector();
	CFightGuideScene *pScene = dynamic_cast<CFightGuideScene*>(pDirector->getRunningScene());

	pScene->setCanBrushMonster(false);
	pScene->removeFireBtns();
	hideDialog();
	removeGuide();

	const FightGuideCfg* stCfg = FIGHT_GUIDE_CFG->GetFightGuideCfgByID(_stepID);

	if(stCfg->type() == DIALOG_TYPE){  //对话
		showDialog();
	}else if(stCfg->type() == MONSTER_EMERGE_TYPE){  //怪物出现
		pScene->setCanBrushMonster(true);
	}else if(stCfg->type() == FIRE_CLEAN_TYPE){  //原地子弹消灭怪物
		pScene->openModuleEffect(pScene->getFireBtn());
		pScene->showFireGuide();

		ADD_EVT_LOG(EVT_LOG_DEMO_ATK); //敌人出现，出现对话，点击，提示点击攻击按钮

	}else if(stCfg->type() == UP_FIRE_CLEAN_TYPE){  //向上移动射击消灭
		_direct = UP_TYPE;
		pScene->showMoveFireGuide(UP_TYPE);

		ADD_EVT_LOG(EVT_LOG_DEMO_MOV_UP); //完成攻击，出现下一波怪物，提示向上移动攻击

	}else if(stCfg->type() == DOWN_FIRE_CLEAN_TYPE){  //向下移动射击消灭
		_direct = DOWN_TYPE;
		pScene->showMoveFireGuide(DOWN_TYPE);

		ADD_EVT_LOG(EVT_LOG_DEMO_MOV_DOWN); //完成攻击，出现下一波怪物，提示向下移动攻击

	}else if(stCfg->type() == MOVE_AHEAD_TYPE){  //引导向前
		pScene->showMoveToGuide();
	}else if(stCfg->type() == SKILL_CLEAN_TYPE){  //技能消灭
		pScene->openModuleEffect(pScene->getSkillBtn());
		pScene->showSkillGuide();

		ADD_EVT_LOG(EVT_LOG_DEMO_SKILL); //完成攻击，进入下一阶段，出现敌人，提示放技能

	}else if(stCfg->type() == END_GUIDE_TYPE){  //指引结束
		endGuide();
		pScene->moveToEnd();
	}
}

