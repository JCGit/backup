#include "CityControl.h"

#include "QEnvSet.h"

#include "ModuleID.pb.h"

#include "CfgMaster/TaskCfgMaster.h"
#include "CfgMaster/OSDCfgMaster.h"
#include "CfgMaster/ModuleOpenCfgMaster.h"

#include "basic/UIBoxLayer.h"

#include "game/Global.h"
#include "game/manual/ManualControl.h"
#include "game/Constant.h"
#include "game/task/TaskControl.h"
#include "game/UserObj.h"
#include "control/AudioManager.h"
#include "game/notification/NotificationControl.h"
#include "game/shop/ShopControl.h"
#include "game/signin/SignInControl.h"

#include "AutoPopControl.h"

CityControl::CityControl()
:m_pCityProxy(NULL)
,m_pCityScene(NULL)
,m_pCityScreenView(NULL)
,m_pSubEntryPanel(NULL)
,m_pLevelUpPanel(NULL)
,m_pGetAwardPanel(NULL)
,m_pAwardPromptPanel(NULL)
,m_uTaskNPCID(0)
,m_uMaxCommitedTaskID(0)
,m_uDelayedEntryID(ENTRY_UNKNOWN)
{
	P_MANUAL_CTRL; // 激活体力控制
}

CityControl::~CityControl()
{
	CC_SAFE_DELETE(m_pCityProxy);
	CC_SAFE_RELEASE(m_pCityScene);

	CC_SAFE_RELEASE_NULL(m_pCityScreenView);
	CC_SAFE_RELEASE_NULL(m_pSubEntryPanel);
	CC_SAFE_RELEASE_NULL(m_pLevelUpPanel);
	CC_SAFE_RELEASE_NULL(m_pGetAwardPanel);
	CC_SAFE_RELEASE_NULL(m_pAwardPromptPanel);
}

void CityControl::init() {
	/*UILayout *pCityLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui_v2/CityLayoutUI.ExportJson"));
	UILayout *pHeadLayout = dynamic_cast<UILayout*>(pCityLayout->getChildByName("head_layout"));
	pCityLayout->removeChild(pHeadLayout);*/
}

const unsigned int CityControl::getArriveNPCID() {
	return m_stCityObj.getArriveNPCID();
}

void CityControl::setArriveNPCID(const unsigned int uArriveNPCID) {
	m_stCityObj.setArriveNPCID(uArriveNPCID);
}

CityProxy* CityControl::getProxy() {
	if(m_pCityProxy == NULL) {
		m_pCityProxy = new CityProxy();
	}

	return m_pCityProxy;
}

CityScene* CityControl::getCityScene() {
	CityScene *pCityScene = dynamic_cast<CityScene*>(CCDirector::sharedDirector()->getRunningScene());

	return pCityScene;
}

CityScreenView* CityControl::getCityScreenView() {
	if(m_pCityScreenView == NULL) {
		P_NOTIFICATION_CTRL->init();
		m_pCityScreenView = CityScreenView::create();
		CC_SAFE_RETAIN(m_pCityScreenView);
	}

	return m_pCityScreenView;
}

SubEntryPanel* CityControl::getSubEntryPanel() {
	if(m_pSubEntryPanel == NULL) {
		m_pSubEntryPanel = SubEntryPanel::create();
		CC_SAFE_RETAIN(m_pSubEntryPanel);
	}

	return m_pSubEntryPanel;
}

LevelUpPanel* CityControl::getLevelUpPanel() {
	if(m_pLevelUpPanel == NULL) {
		m_pLevelUpPanel = LevelUpPanel::create();
		CC_SAFE_RETAIN(m_pLevelUpPanel);
	}

	return m_pLevelUpPanel;
}

GetAwardPanel* CityControl::getGetAwardPanel() {
	if(m_pGetAwardPanel == NULL) {
		m_pGetAwardPanel = GetAwardPanel::create();
		CC_SAFE_RETAIN(m_pGetAwardPanel);
	}

	return m_pGetAwardPanel;
}

AwardPromptPanel* CityControl::getAwardPromptPanel() {
	if(m_pAwardPromptPanel == NULL) {
		m_pAwardPromptPanel = AwardPromptPanel::create();
		CC_SAFE_RETAIN(m_pAwardPromptPanel);
	}

	return m_pAwardPromptPanel;
}

void CityControl::updateExp() {
	if(m_pCityScreenView != NULL) {
		m_pCityScreenView->updateExp();
	}
}

void CityControl::updateFightPower() {
	if(m_pCityScreenView != NULL) {
		m_pCityScreenView->updateFightPower();
	}
}

void CityControl::updateGold() {
    updateMoneyTopBar();

	if(m_pCityScreenView != NULL) {
		m_pCityScreenView->updateGold();
	}
}

void CityControl::updateCoin() {
	if(m_pCityScreenView != NULL) {
		m_pCityScreenView->updateCoin();
	}
}

void CityControl::updateManual() {
    updateMoneyTopBar();

	if(m_pCityScreenView != NULL) {
		m_pCityScreenView->updateManual();
	}
}

void CityControl::updateTask() {
	if(m_pCityScreenView != NULL) {
		m_pCityScreenView->updateTask();
	}
}

void CityControl::updateMail(){
	if(m_pCityScreenView != NULL) {
		m_pCityScreenView->mailNotify();
	}
}

void CityControl::updateOnlineAndMorrow()
{
	m_pCityScreenView->updateOnlineAndMorrow();
}

unsigned int CityControl::getTaskNPCID() {
	return m_uTaskNPCID;
}

void CityControl::setTaskNPCID(unsigned int uTaskNPCID) {
	m_uTaskNPCID = uTaskNPCID;
}

bool CityControl::isHeroMoving() {
	bool bRet = false;
	
	CityScene *pCityScene = getCityScene();

	if(pCityScene != NULL) {
		CMainHero *pHero = pCityScene->getRole(P_USER_OBJ->getUserInfo()->uin);
		bRet = pHero->IsWalking();
	}

	return bRet;
}

bool CityControl::isNPCInScene(unsigned int uSceneID, unsigned int uNPCID) {
	bool bRet = false;
	OSDCfg *pOSDCfg = OSD_CFG_MASTER->getOSDCfgByID(uSceneID);

	for(int i = 0; i < pOSDCfg->npclst_size(); i++) {
		if(uNPCID == pOSDCfg->npclst(i)) {
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool CityControl::isNPCInCurScene(unsigned int uNPCID) {
	return isNPCInScene(getCityScene()->getSceneID(), uNPCID);
}

void CityControl::moveToNPC(unsigned int uNPCID) {
	if(isHeroMoving()) { // 正在移动中
		return;
	}

	unsigned int uCurSceneID = getCityScene()->getSceneID();

	if(isNPCInScene(uCurSceneID, uNPCID)) { // npc在当前场景中
		getCityScene()->moveToNPC(uNPCID);
	} else {
		const OSDCfgSet &cfgSet = OSD_CFG_MASTER->GetCfg();

		for(int i = 0; i < cfgSet.osdcfg_size(); i++) {
			const OSDCfg &stOSDCfg = cfgSet.osdcfg(i);
			unsigned int uSceneID = stOSDCfg.sceneid();

			if(isNPCInScene(uSceneID, uNPCID)) {
				setTaskNPCID(uNPCID);
				getCityScene()->moveToScene(uSceneID);
				break;
			}

		}
	}

}

void CityControl::updateNPCTaskState(unsigned int uTaskID){
	const TaskCfg* pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(uTaskID);
	if(getCityScene() == NULL){
		CCLOG("now the scene is null");
		return;
	}

	unsigned int uCurSceneID = getCityScene()->getSceneID();

	if(uTaskID > 1){  //检查上一个任务的npc状态
		const TaskCfg* pLastTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(uTaskID - 1);
		if (!pLastTaskCfg)	return;

		if(isNPCInScene(uCurSceneID, pLastTaskCfg->getnpcid())) { // npc在当前场景中
			getCityScene()->updateNPCTaskState(pLastTaskCfg->getnpcid());
		}

		if(isNPCInScene(uCurSceneID, pLastTaskCfg->commitnpcid())) { // npc在当前场景中
			getCityScene()->updateNPCTaskState(pLastTaskCfg->commitnpcid());
		}
	}

	if(pTaskCfg != NULL) {
		if(isNPCInScene(uCurSceneID, pTaskCfg->getnpcid())) { // npc在当前场景中
			getCityScene()->updateNPCTaskState(pTaskCfg->getnpcid());
		}

		if(isNPCInScene(uCurSceneID, pTaskCfg->commitnpcid())) { // npc在当前场景中
			getCityScene()->updateNPCTaskState(pTaskCfg->commitnpcid());
		}
	}
}

void CityControl::moveToBattlePortal() {
	if(isHeroMoving()) { // 正在移动中
		return;
	}

	getCityScene()->moveToBattlePortal();
}

void CityControl::showPlotDialog() {
	getCityScene()->showPlotDialog();
}

bool CityControl::checkModuleOpen(unsigned int uTaskID) {
	bool bRet = false;
	const ModuleOpenCfgSet &stCfgSet = MODULE_OPEN_CFG_MASTER->GetCfg();
	m_stOpenModules.clear();

	for(int i = 0; i < stCfgSet.moduleopencfg_size(); i++) {
		const ModuleOpenCfg &stModuleOpenCfg = stCfgSet.moduleopencfg(i);
		unsigned int uModuleTaskID = stModuleOpenCfg.taskid();
		unsigned int uModuleID = stModuleOpenCfg.moduleid();

		if(uModuleTaskID == uTaskID) {
			if(uModuleID == vmsg::MODULE_ID_MONTHCARD) {
				P_AUTO_POP_CTRL->addPopPanel(P_SHOP_CTRL->getBuyMonthCardPanel());
			} else if(uModuleID == vmsg::MODULE_ID_FIRST_CHARGE_PKG) {
				P_AUTO_POP_CTRL->addPopPanel(P_SHOP_CTRL->getFirstChargePanel());
			} 
			/*else if(uModuleID == vmsg::MODULE_ID_SIGNIN) {
			P_AUTO_POP_CTRL->addPopPanel(P_SIGN_IN_CTRL->getSignInPanel());
			} */
			else {
				m_stOpenModules.push_back(uModuleID);
			}

			bRet = true;
		}
	}

	P_AUTO_POP_CTRL->popPanel();

	return bRet;
}

unsigned int CityControl::getModuleIDByTaskID(unsigned int uTaskID)
{
	unsigned int bRet = 0;
	const ModuleOpenCfgSet &stCfgSet = MODULE_OPEN_CFG_MASTER->GetCfg();

	for(int i = 0; i < stCfgSet.moduleopencfg_size(); i++) {
		const ModuleOpenCfg &stModuleOpenCfg = stCfgSet.moduleopencfg(i);
		unsigned int uModuleTaskID = stModuleOpenCfg.taskid();

		if(uModuleTaskID == uTaskID) {
			bRet = stModuleOpenCfg.moduleid();
			break;
		}
	}
	return bRet;
}

bool CityControl::isModuleOpen(unsigned int uModuleID) {
	bool bRet = false;
    if(QEnvSet::neil()) {
        return true;
    }
	ModuleOpenCfg *pModuleOpenCfg = MODULE_OPEN_CFG_MASTER->getModuleOpenCfg(uModuleID);

	if(pModuleOpenCfg != NULL) {
		unsigned int uModuleTaskID = pModuleOpenCfg->taskid();
		const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

		if(stTask.taskstat() < vmsg::TASK_STAT_OVER) {
			bRet = uModuleTaskID < stTask.taskid();
		} else {
			bRet = uModuleTaskID <= stTask.taskid();
		}
	} else { // 找不到的ModuleID默认为直接开放
		bRet = true;
	}

	return bRet;
}

bool CityControl::isSceneOpen(unsigned int uSceneID) {
	bool bRet = false;

	OSDCfg *pOSDCfg = OSD_CFG_MASTER->getOSDCfgByID(uSceneID);

	if(pOSDCfg != NULL) {
		const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

		if(stTask.taskid() > pOSDCfg->taskid()
		|| (stTask.taskid() == pOSDCfg->taskid() && stTask.taskstat() >= vmsg::TASK_STAT_RECEIVED)
		|| pOSDCfg->taskid() == 1) {
			bRet = true;
		}
	}

	return bRet;
}

bool CityControl::isInCity() {
	bool bRet = false;

	BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());

	bRet = pScene != NULL && pScene->getSceneType() == SCENE_OSD;

	return bRet;
}

void CityControl::showLvUpEffect() {
    CityScene* pScene = getCityScene();
    if(NULL == pScene) {
        return;
    }

    CMainHero* pMainHero = pScene->getMainHero();
    if(NULL == pMainHero) {
        return;
    }
    int ypos = pMainHero->getCenterToBottom();
    P_GLOBAL->showEffOnNode(pMainHero, "ut00031", "effect/ut00031/ut00031.ExportJson", 
        ccp(0, 0), ccp(255, 255));

    AudioManager::playEffect( "ut00031.ogg" );
}


void CityControl::updateWeaponAvatar() {
    CityScene* pCity = getCityScene();
    if(!pCity) {
        return;
    }
    CMainHero* pHero = pCity->getMainHero();
    if(pHero) {
		pHero->setWeaponCfgID(P_USER_OBJ->getUserInfo()->weapon.cfgid());
        pHero->ReloadWeaponRes();
    }
}


void CityControl::updateMoneyTopBar() {
    CityScene* pScene = getCityScene();
    if(NULL == pScene) {
        return;
    }
    UIBoxLayer* pBoxLayer = (UIBoxLayer*) pScene->getLayer(LAYER_ID_UI);
    if(NULL == pBoxLayer) {
        return;
    }
    pBoxLayer->updateMoneyBar();
}

vector<unsigned int>& CityControl::getOpenModules() {
	return m_stOpenModules;
}

unsigned int CityControl::getDelayedEntryID() {
	return m_uDelayedEntryID;
}

void CityControl::setDelayedEntryID(unsigned int uEntryID) {
	m_uDelayedEntryID = uEntryID;
}