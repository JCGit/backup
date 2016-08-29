#include "GuideControl.h"

#include "Macros.pb.h"

#include "CfgMaster/GuideCfgMaster.h"
#include "CfgMaster/GuideStepCfgMaster.h"
#include "CfgMaster/StoryCfgMaster.h"

#include "basic/BaseScene.h"
#include "basic/UIBoxLayer.h"
#include "basic/ToolUtils.h"
#include "basic/CommonDialog.h"

#include "game/city/CityControl.h"
#include "game/bag/BagControl.h"
#include "game/bag/RoleInfoPanel.h"
#include "game/bag/EquipInfoPanel.h"
#include "game/task/TaskControl.h"
#include "game/plot/PlotControl.h"
#include "game/fight/LevelControl.h"
#include "game/arena/ArenaControl.h"
#include "game/friend/FriendControl.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/nihility/NihilityControl.h"
#include "game/equiplib/EquipLibControl.h"
#include "game/warmap/WarMapControl.h"
#include "game/refit/RefitControl.h"
#include "game/lottery/LotteryControl.h"
#include "game/gen/GenStrengthenControl.h"
#include "game/signin/SignInControl.h"
#include "game/evolution/EvolutionControl.h"

#define NODE_EXIT_CHECK_INTERVAL 1.0f // seconds

GuideControl::GuideControl()
:m_bInitialized(false)
,m_pGuideView(NULL)
,m_pGuideContainer(NULL)
,m_pGuideCallFunc(NULL)
,m_pGuideNode(NULL)
,m_bIsGuideDuring(false)
,m_uGuideID(0)
,m_uStepID(0)
,m_nStepIdx(-1)
,m_uRemainTimes(0)
,m_stShowRect(CCRectZero)
,m_bIsNewcomer(false)
{
}

GuideControl::~GuideControl()
{
	finalize();
}

void GuideControl::init() {
	if(!m_bInitialized) {
		// 引导配置
		
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00037/ut00037.ExportJson");

		// 引导视图
		m_pGuideView = GuideView::create();
		CC_SAFE_RETAIN(m_pGuideView);

		m_pGuideCallFunc = GuideCallFunc::create();
		CC_SAFE_RETAIN(m_pGuideCallFunc);

		CCLOG("guide ================ init");
		m_bInitialized = true;
	}
}

void GuideControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pGuideView);
		
		CC_SAFE_RELEASE_NULL(m_pGuideCallFunc);

		CToolUtils::removeArmatureFile("effect/ut00037/ut00037.ExportJson");

		CCLOG("guide ================ finalize");
		m_bInitialized = false;
	}
}

bool GuideControl::isGuideDuring() {
	return m_bInitialized && m_bIsGuideDuring;
}

void GuideControl::trigger(unsigned int uTriggerType, unsigned int uLinkID) {
	if(m_bInitialized) {
		if (m_bIsGuideDuring)//在引导中
		{
			return;
		}
		const GuideCfgSet &stGuideCfgSet = GUIDE_CFG_MASTER->GetCfg();

		for(int i = 0; i < stGuideCfgSet.guidecfg_size(); i++) {
			const GuideCfg &stGuideCfg = stGuideCfgSet.guidecfg(i);

			if(stGuideCfg.triggertype() == uTriggerType && stGuideCfg.linkid() == uLinkID) { // 满足触发条件
				CCLOG("guide ================ trigger TriggerType=%u LinkID=%u", uTriggerType, uLinkID);
				startGuide(stGuideCfg.guideid()); // 开始引导
				break;
			}
		}
	}
}

void GuideControl::startGuide(unsigned int uGuideID) {
	if(m_bInitialized) {
		if(!GUIDE_CFG_MASTER->isValid(uGuideID)) {
			CCLOG("guide ================ error code=%d", ERROR_INVALID_GUIDE_ID);
			return;
		}

		const GuideCfg *pGuideCfg = GUIDE_CFG_MASTER->getGuideCfg(uGuideID);

		if(pGuideCfg != NULL && pGuideCfg->islocked() == 0) {
			m_uGuideID = uGuideID;

			// 重置一下
			m_pGuideContainer = NULL;
			m_pGuideNode = NULL;
			m_uStepID = 0;
			m_uRemainTimes = 0;
			m_stShowRect = CCRectZero;

			CCDirector *pDirector = CCDirector::sharedDirector();
			BaseScene *pScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
			pScene->cleanUI();

			CCLOG("guide ================ clean ui,start GuideID=%d", m_uGuideID);

			addGuide();
			m_bIsGuideDuring = true;
			m_nStepIdx = 0;
			pushStep();
		}
	}
}

void GuideControl::pushStep() {
	CCAssert(m_bInitialized, "Not initialized");

	const std::vector<GuideStepCfg*> &steps = GUIDE_STEP_CFG_MASTER->getSteps(m_uGuideID);
	GuideStepCfg *pGuideStepCfg = steps[m_nStepIdx];

	if(pGuideStepCfg == NULL) {
		CCLOG("guide ================ error code=%d", ERROR_INVALID_STEP_ID);
		endGuide();
		return;
	}

	m_uStepID = pGuideStepCfg->stepid();
	m_uRemainTimes = pGuideStepCfg->times();

	CCLOG("guide ================ step push GuideID=%d StepID=%d", m_uGuideID, m_uStepID);

	m_pGuideView->forced(pGuideStepCfg->isforced() == 1);

	// 不需等待
	if(pGuideStepCfg->iswaiting() == 0) {
		GuideContainer *pGuideContainer = getGuideContainer(pGuideStepCfg->viewtype());

		if(pGuideContainer != NULL && pGuideContainer->getGuideNodeByName(pGuideStepCfg->viewname()) != NULL) {
			execStep();
		}
	}
	
}

void GuideControl::execStep() {
	CCAssert(m_bInitialized, "Not initialized");

	const std::vector<GuideStepCfg*> &steps = GUIDE_STEP_CFG_MASTER->getSteps(m_uGuideID);
	GuideStepCfg *pGuideStepCfg = steps[m_nStepIdx];
	
	CCLOG("guide ================ step exec GuideID=%d StepID=%d", m_uGuideID, m_uStepID);

	if(pGuideStepCfg->steptype() == GUIDE_STEP_STORY) {
		P_PLOT_CTRL->setStoryID(pGuideStepCfg->linkid());
		P_PLOT_CTRL->showDialog();
		m_pGuideView->hide();
	} else if(pGuideStepCfg->steptype() == GUIDE_STEP_VIEW_CLICK) {
		GuideContainer *pGuideContainer = getGuideContainer(pGuideStepCfg->viewtype());

		if(pGuideContainer != NULL) {
			m_pGuideNode = pGuideContainer->getGuideNodeByName(pGuideStepCfg->viewname());
		} else if(pGuideStepCfg->viewtype() == GUIDE_UI_UILAYER) {
			BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
			UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));
			m_pGuideNode = pUILayer->getBackBtn();
		}

		CCSize size = CCSizeZero;

		if(m_pGuideNode != NULL) {
			UIWidget *pWidget = dynamic_cast<UIWidget*>(m_pGuideNode);

			if(pWidget != NULL) {
				size = pWidget->getSize();
			} else {
				size = m_pGuideNode->getContentSize();
			}

			CCPoint pos = CCPointZero;

			if(pGuideStepCfg->isforced() == 1) { // 强制
				if(m_pGuideNode->getParent() != NULL) {
					pos = m_pGuideNode->getParent()->convertToWorldSpace(m_pGuideNode->getPosition());
					//pos = m_pGuideNode->convertToWorldSpace(CCPointZero); // 与上一句等效
				} else {
					//return;
				}
			} else {
				pos =  m_pGuideNode->getPosition();
			}
			
			CCDirector *pDirector = CCDirector::sharedDirector();
			CCPoint screenOrigin = pDirector->getVisibleOrigin();
			CCPoint anchor = m_pGuideNode->getAnchorPoint();
			float posX = pos.x - size.width * anchor.x;
			float posY = pos.y - size.height * anchor.y;
			pos = m_pGuideView->convertToNodeSpace(ccp(posX, posY));
			m_stShowRect = CCRectMake(pos.x, pos.y, size.width, size.height);

			showGuide();
		} else {
			CCLOG("guide ================ error code=%d", ERROR_NULL_TOUCH_NODE);
			endGuide();
		}

	}

}

void GuideControl::nextStep() {
	CCAssert(m_bInitialized, "Not initialized");

	/*CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->unscheduleSelector(schedule_selector(GuideCallFunc::checkGuideNodeExit), m_pGuideCallFunc);*/

	const std::vector<GuideStepCfg*> &steps = GUIDE_STEP_CFG_MASTER->getSteps(m_uGuideID);

	m_uRemainTimes--;

	unsigned int times = steps[m_nStepIdx]->times() - m_uRemainTimes;
	CCLOG("guide ================ step finish GuideID=%d StepID=%d times=%d", m_uGuideID, m_uStepID, times);

	if(m_nStepIdx == steps.size() - 1 && m_uRemainTimes == 0) { // 最后一步，完成此次引导
		endGuide();
	} else {
		if(steps[m_nStepIdx]->iscanover()) { // 至此可以算作完成了此次引导

		}

		if(m_uRemainTimes == 0) {
			hideGuide();
			m_nStepIdx++;
			pushStep();
		} else {
			execStep();
		}

	}
}

void GuideControl::endGuide() {
	CCAssert(m_bInitialized, "Not initialized");

	CCLOG("guide ================ end GuideID=%d StepID=%d", m_uGuideID, m_uStepID);

	removeGuide();
	// 重置初始值
	m_pGuideContainer = NULL;
	m_pGuideNode = NULL;
	m_bIsGuideDuring = false;
	m_uGuideID = 0;
	m_uStepID = 0;
	m_nStepIdx = -1;

	hideGuide();
}

void GuideControl::addGuide() {
	CCAssert(m_bInitialized, "Not initialized");
	//return;
	if(m_pGuideView != NULL && m_pGuideView->getParent() == NULL) {
		CCDirector *pDirector = CCDirector::sharedDirector();
		BaseScene *pScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
		CCAssert(pScene != NULL, "fuck...");
		UILayer *pTopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_TOP));
		pTopLayer->addWidget(m_pGuideView);
	}
}

void GuideControl::removeGuide() {
	CCAssert(m_bInitialized, "Not initialized");

	if(m_pGuideView != NULL && m_pGuideView->getParent() != NULL) {
		m_pGuideView->removeFromParent();
	}
}

void GuideControl::showGuide() {
	CCAssert(m_bInitialized, "Not initialized");

	if(m_pGuideView != NULL) {
		CCDirector *pDirector = CCDirector::sharedDirector();
		const std::vector<GuideStepCfg*> &steps = GUIDE_STEP_CFG_MASTER->getSteps(m_uGuideID);
		GuideStepCfg *pGuideStepCfg = steps[m_nStepIdx];
		//m_pGuideView->forced(pGuideCfg->isforced() == 1);
		
		CCPoint screenOrigin = pDirector->getVisibleOrigin();
		CCSize screenSize = pDirector->getVisibleSize();
		CCRect screenRect = CCRectMake(screenOrigin.x, screenOrigin.y, screenSize.width, screenSize.height);

		const CCRect &rect = m_stShowRect;

		bool hasError = false;

		if(rect.size.equals(CCSizeZero)) {
			CCLOG("guide ================ error code=%d", ERROR_TOUCH_ZERO);
			hasError = true;
		}

		if(rect.size.width < GuideView::MIN_WIDTH || rect.size.height < GuideView::MIN_HEIGHT) {
			CCLOG("guide ================ error code=%d", ERROR_TOUCH_TOO_SMALL);
			hasError = true;
		}

		CCRect viewRect = CCRectMake(0, 0, screenSize.width, screenSize.height); // rect不是全局的

		if(rect.getMinX() >= viewRect.getMaxX()
		|| rect.getMaxX() <= viewRect.getMinX()
		|| rect.getMinY() >= viewRect.getMaxY()
		|| rect.getMaxY() <= viewRect.getMinY()) { // 在屏幕之外
			CCLOG("guide ================ error code=%d", ERROR_TOUCH_OUTSIDE_SCREEN);
			hasError = true;
		}

		CCAssert(hasError == false, "guide error");

		if(!hasError) {
			if(!m_pGuideView->isRunning()) {
				addGuide();
			}

			m_pGuideView->show(rect, pGuideStepCfg->viewshape());
			//pDirector->getScheduler()->scheduleSelector(schedule_selector(GuideCallFunc::checkGuideNodeExit), m_pGuideCallFunc, NODE_EXIT_CHECK_INTERVAL, false);
		} else {
			endGuide();
		}

	}
}

void GuideControl::hideGuide() {
	CCAssert(m_bInitialized, "Not initialized");

	if(m_pGuideView != NULL) {
		m_pGuideView->hide();
	}
}

bool GuideControl::isGuideContainer(GUIDE_UI_TYPE type) {
	CCAssert(m_bInitialized, "Not initialized");

	const GuideStepCfg *pGuideStepCfg = GUIDE_STEP_CFG_MASTER->getGuideStepCfg(m_uGuideID, m_uStepID);

	return pGuideStepCfg != NULL && type == pGuideStepCfg->viewtype();
}

bool GuideControl::isGuideNode(CCNode *pNode) {
	CCAssert(m_bInitialized, "Not initialized");

	return pNode != NULL && m_pGuideNode == pNode && pNode->isRunning() && pNode->isVisible();
}

bool GuideControl::isNewcomer() {
	return m_bIsNewcomer;
}

void GuideControl::setNewcomer(bool isNew) {
	m_bIsNewcomer = isNew;
}

GuideContainer* GuideControl::getGuideContainer(unsigned int uViewType) {
	GuideContainer *pRet = NULL;
	
	switch(uViewType) {
		case GUIDE_UI_NONE:				// 不需操作显示对象
			pRet = NULL;
			break;
		case GUIDE_UI_CITY_SCREEN:		// 同屏主界面
			pRet = P_CITY_CTRL->getCityScreenView();
			break;
		case GUIDE_UI_SUB_ENTRY:		// 二级入口
			pRet = P_CITY_CTRL->getSubEntryPanel();
			break;
		case GUIDE_UI_ROLE:				// 角色
			pRet = P_BAG_CTRL->getRoleInfoPanel();
			break;
		case GUIDE_UI_EQUIP_INFO:		// 装备信息
			pRet = P_BAG_CTRL->getEquipInfoPanel();
			break;
		case GUIDE_UI_EQUIP_LIB:		// 装备养成
			pRet = P_EQUIP_LIB_CTRL->getEquipLibPanel();
			break;
		case GUIDE_UI_EQUIP_SEL:		// 装备选择
			pRet = P_EQUIP_LIB_CTRL->getEquipSelPanel();
			break;
		case GUIDE_UI_WEAPON_LIB:		// 枪械库
			pRet = P_WEAPON_CTRL->getWeaponLibPanel();
			break;
		case GUIDE_UI_WEAPON_SELECT:	// 枪械选择

			break;
		case GUIDE_UI_WEAPON_INFO:		// 枪械信息
			pRet = P_WEAPON_CTRL->getWeaponInfoPanel();
			break;
		case GUIDE_UI_WEAPON_MATERIAL:	// 枪械材料
			pRet = P_WEAPON_CTRL->getSelMatPanel();
			break;
		case GUIDE_UI_WEAPON_STEP_DONE:	// 枪械进阶成功
			pRet = P_WEAPON_CTRL->getWeaponAdvDonePanel();
			break;
		case GUIDE_UI_LOTTERY:			// 夺宝奇兵
			pRet = P_LOTTERY_CTRL->getLotteryPanel();
			break;
		case GUIDE_UI_WAR_MAP:			// 关卡选择
			pRet = P_WAR_MAP_CTRL->getWarMapPanel();
			break;
		case GUIDE_UI_WAR_ENTER:		// 进入战斗
			pRet = P_LEVEL_CTL->getEnterPanel();
			break;
		case GUIDE_UI_FRIEND:			// 好友
			pRet = P_FRIEND_CTRL->getFriendPanelUI();
			break;
		case GUIDE_UI_ARENA:			// 竞技场
			pRet = P_ARENA_CTRL->getArenaPanel();
			break;
		case GUIDE_UI_ARENA_MATCH:		// 竞技场匹配
			pRet = P_ARENA_CTRL->getMatchPanel();
			break;
		case GUIDE_UI_NOTHING_LAND:		// 虚无之地
			pRet = P_NIHILITY_CTRL->getNihilityPanel();
			break;
		case GUIDE_UI_COMMON_DIALOG:	// 弹出对话框
			pRet = COMMON_DIALOG;
			break;
		case GUIDE_UI_EQUIP_LIST:	// 枪械列表
			pRet = P_EQUIP_LIB_CTRL->getEquipLibListPanel();
			break;
		case GUIDE_UI_NOTHING_LAND_ENTER: // 虚无之地入口
			pRet = P_NIHILITY_CTRL->getGenPanel();
			break;
		case GUIDE_UI_ELITE_WAR_ENTER: // 进入精英副本
			pRet = P_LEVEL_CTL->getEliteWarEntryPanel();
			break;
		case GUIDE_UI_REFIT: // 改装
			pRet = P_REFIT_CTRL->getRefitPanel();
			break;
		case GUIDE_UI_REFIT_MATERIAL: // 改装材料
			pRet = P_REFIT_CTRL->getRefitMaterialPanel();
			break;
		case GUIDE_UI_WEAPON_ARRAY: // 战备
			pRet = P_WEAPON_CTRL->getArrayPanel();
			break;
		case GUIDE_UI_WEAPON_SEL: // 枪械选择
			pRet = P_WEAPON_CTRL->getWeaponSelPanel();
			break;
		case GUIDE_UI_GENE_STRENGTHEN: // 基因强化
			pRet = P_GEN_CTRL->getGenPanel();
			break;
		case GUIDE_UI_STRENGTHEN_EFFECT: // 强化效果
			pRet = P_GEN_CTRL->getGenEffPanel();
			break;
		case GUIDE_UI_CHANGEWEAPON_INFO:// 阵位上枪械信息
			pRet = P_WEAPON_CTRL->getArrInfoPanel();
			break;
		case GUIDE_UI_DAILYTASK:
			pRet = P_TASK_CTRL->getTaskPanel();
			break;
		case GUIDE_UI_SIGNIN:
			pRet = P_SIGN_IN_CTRL->getSignInPanel();
			break;
		case GUIDE_UI_EVOLUTION:
			pRet = P_EVOLUTION_CTRL->getEvolutionPanel();
			break;
		default:;
	}

	return pRet;
}

void GuideControl::checkGuideNodeExit() {
	if(m_pGuideNode != NULL) {
		if(!m_pGuideNode->isRunning()) {
			CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
			pScheduler->unscheduleSelector(schedule_selector(GuideCallFunc::checkGuideNodeExit), m_pGuideCallFunc);
			CCLOG("guide ================ error code=%d", ERROR_TOUCH_ZERO);
			endGuide();
		}

	}
}

bool GuideControl::isStoryStep() {
	bool bRet = false;

	if(m_bInitialized) {
		const std::vector<GuideStepCfg*> &steps = GUIDE_STEP_CFG_MASTER->getSteps(m_uGuideID);
		GuideStepCfg *pGuideStepCfg = steps[m_nStepIdx];

		bRet = pGuideStepCfg != NULL && pGuideStepCfg->steptype() == GUIDE_STEP_STORY;
	}

	return bRet;
}

bool GuideControl::isTaskTriggerGuide(unsigned int uTaskID) {
	bool bRet = false;

	if(m_bInitialized) {
		const GuideCfgSet &stGuideCfgSet = GUIDE_CFG_MASTER->GetCfg();

		for(int i = 0; i < stGuideCfgSet.guidecfg_size(); i++) {
			const GuideCfg &stGuideCfg = stGuideCfgSet.guidecfg(i);

			if(stGuideCfg.triggertype() == GUIDE_TRIGGER_TASK && stGuideCfg.linkid() == uTaskID) { 
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

unsigned int GuideControl::getGuideID() {
	return m_uGuideID;
}

unsigned int GuideControl::getStepID() {
	return m_uStepID;
}
