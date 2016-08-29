#include "CityScreenView.h"

#include "defines.h"

#include "Macros.pb.h"

#include "CfgMaster/ExpCfgMaster.h"
#include "CfgMaster/TaskCfgMaster.h"
#include "CfgMaster/WarMapCfgMaster.h"
#include "CfgMaster/NPCCfgMaster.h"
#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/ModuleOpenCfgMaster.h"
#include "CfgMaster/EntryCfgMaster.h"

#include "basic/BasicTypes.h"
#include "basic/ImageNumber.h"
#include "basic/ToolUtils.h"
#include "basic/CCColorUtil.h"
#include "basic/BaseScene.h"
#include "basic/UIBoxLayer.h"

#include "game/Constant.h"
#include "game/UserObj.h"
#include "game/task/TaskControl.h"
#include "game/manual/ManualControl.h"
#include "game/login/UserGoldControl.h"
#include "game/vip/VIPControl.h"
#include "game/vip/BuyCoinPanel.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "tip/TipControl.h"
#include "game/notification/NotificationControl.h"
#include "game/city/CityControl.h"

#include "BottomEntryLayout.h"
#include "TopEntryLayout.h"
#include "HeadEntryLayout.h"
#include "EntryControl.h"
#include "signin/SignInControl.h"
#include "AutoPopControl.h"
#include "game/gen/GenStrengthenControl.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/equiplib/EquipLibControl.h"
#include "game/shop/ShopControl.h"
#include "game/shop/RmbShopPanel.h"
#include "game/fight/FightControl.h"
#include "game/refit/RefitControl.h"
#include "game/evolution/EvolutionControl.h"
#include "game/warmap/WarMapControl.h"

CityScreenView::CityScreenView()
:m_pLevelTxt(NULL)
,m_pVIPTxt(NULL)
,m_pFightPowerTxt(NULL)
,m_pTaskTraceLayout(NULL)
,m_pTaskNameTxt(NULL)
,m_pTaskDescTxt(NULL)
,m_pExpProgress(NULL)
,m_pExpProgressTxt(NULL)
,m_pAssetsLayout(NULL)
,m_pCoinValTxt(NULL)
,m_pCoinAddBtn(NULL)
,m_pGoldValTxt(NULL)
,m_pGoldAddBtn(NULL)
,m_pManualTimerTxt(NULL)
,m_pManualValTxt(NULL)
,m_pManualAddBtn(NULL)
,m_nRemainTime(0)
,m_pBottomEntryLayout(NULL)
,m_pTopEntryLayout(NULL)
,m_pHeadEntryLayout(NULL)
,m_pFuncBtn(NULL)
,m_pFuncWordsTxt(NULL)
,m_pBgMask(NULL)
,m_pBtnEffArmature(NULL)
,m_uNowModuleID(0)
,m_pTaskArmature(NULL)
{
}

CityScreenView::~CityScreenView()
{
	CToolUtils::removeArmatureFile("effect/ut00074/ut00074.ExportJson");
	CToolUtils::removeArmatureFile("effect/ut00085/ut00085.ExportJson");
}

CityScreenView* CityScreenView::create() {
	CityScreenView *pRet = new CityScreenView();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CityScreenView::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CityScreenView::onEnter() {
	BaseLayout::onEnter();

	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();
	m_nRemainTime = pUserInfo->updateRemainTime;
	P_MANUAL_CTRL->startSyncManual(); // 开始同步体力恢复
	
	if(P_GUIDE_CTRL->getGuideID() != GUIDE_ID_FIRST_TASK || P_GUIDE_CTRL->getStepID() != 5) {
		GUIDE_SHOW(GUIDE_UI_CITY_SCREEN);
	}
}

void CityScreenView::onExit() {
	BaseLayout::onExit();

	UserInfo* pUserInfo = P_USER_OBJ->getMutableUserInfo();
	pUserInfo->updateRemainTime = m_nRemainTime;
	P_MANUAL_CTRL->endSyncManual();
}

CCNode* CityScreenView::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;
	UIButton *pBtn = NULL;

	if(name == "role_entry") {
		pBtn = m_pBottomEntryLayout->getEntryBtn(ENTRY_ROLE);
		unfoldEntry();
	} else if(name == "equip_culture_entry") {
		pBtn = m_pBottomEntryLayout->getEntryBtn(ENTRY_EQUIP_CULTURE);
		unfoldEntry();
	} else if(name == "weapon_lib_entry") {
		pBtn = m_pBottomEntryLayout->getEntryBtn(ENTRY_WEAPON_LIB);
		unfoldEntry();
	} else if(name == "social_entry") {
		pBtn = m_pBottomEntryLayout->getEntryBtn(ENTRY_SOCIAL);
		unfoldEntry();
	} else if(name == "challenge_entry") {
		pBtn = m_pBottomEntryLayout->getEntryBtn(ENTRY_CHALLENGE);
		unfoldEntry();
	} else if(name == "lottery_entry") {
		pBtn = m_pTopEntryLayout->getEntryBtn(ENTRY_LOTTERY);
	} else if(name == "refit_entry") {
		pBtn = m_pBottomEntryLayout->getEntryBtn(ENTRY_REFIT);
		unfoldEntry();
	} else if(name == "growth_entry") {
		pBtn = m_pBottomEntryLayout->getEntryBtn(ENTRY_GROWTH);
		unfoldEntry();
	}else if(name == "daily_task_entry"){
		pBtn = m_pTopEntryLayout->getEntryBtn(ENTRY_DAILY_TASK);
	}else if (name == "weapon_book_entry")
	{
		pBtn = m_pTopEntryLayout->getEntryBtn(ENTRY_WEAPON_BOOK);
	}else if (name == "daily_war_entry")
	{
		pBtn = m_pTopEntryLayout->getEntryBtn(ENTRY_DAILY_WAR);
	}else if (name == "sign_in_entry")
	{
		pBtn = m_pTopEntryLayout->getEntryBtn(ENTRY_SIGN_IN);
	}

	if(pBtn != NULL && pBtn->isRunning() && pBtn->isVisible() && pBtn->isTouchEnabled()) {
		pRet = pBtn;
	}

	if(name == "task_trace") {
		if(m_pTaskTraceLayout != NULL && m_pTaskTraceLayout->isRunning() && m_pTaskTraceLayout->isTouchEnabled()) {
			pRet = m_pTaskTraceLayout;
			
			if(m_pTaskArmature != NULL) {
				m_pTaskArmature->getAnimation()->stop();
				m_pTaskArmature->setVisible(false);
			}
		}
	}

	return pRet;
}

void CityScreenView::updateUI() {
	updateExp();
	updateFightPower();
	updateGold();
	updateCoin();
	updateManual();
	updateTask();
	updateEntry();
}

void CityScreenView::updateExp() {
	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
	
	if(pUserInfo->uin != 0) {
		unsigned int uLevel = pUserInfo->level;
		m_pLevelTxt->setNum(intToString(uLevel), 0, "Lv");

		UINT64_t uExp = pUserInfo->exp;
		UINT64_t uMaxExp = EXP_CFG_MASTER->getExpCfgByID(uLevel)->exp();
		m_pExpProgress->setPercent(100 * uExp / uMaxExp);
		m_pExpProgressTxt->setText(intToString(uExp) + "/" + intToString(uMaxExp));

		if(m_pTaskTraceLayout != NULL && !m_pTaskTraceLayout->isTouchEnabled()) {
			updateTask();
		}
	}
}

void CityScreenView::updateFightPower() {
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();

	if(pUserInfo->uin != 0) {
		m_pFightPowerTxt->setNum(intToString(pUserInfo->fightPower));
	}
}

void CityScreenView::updateGold() {
	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();

	if(pUserInfo->uin != 0) {
		m_pGoldValTxt->setText(intToString(pUserInfo->gold));
		m_pVIPTxt->setNum(intToString(pUserInfo->vipLv), 0.0f, "vip");
	}
}

void CityScreenView::updateCoin() {
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();
	
	if(pUserInfo->uin != 0) {
		m_pCoinValTxt->setText(intToString(pUserInfo->coin));
	}
}

void CityScreenView::updateManual() {
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();

	if(pUserInfo->uin != 0) {
		const unsigned int uRemainManual = pUserInfo->remainManual;
		m_pManualValTxt->setText(intToString(uRemainManual) + "/" + intToString(MAX_MANUAL_VAL));
	
		m_nRemainTime = pUserInfo->updateRemainTime;

		bool isCanAddManual = false;
		const vmsg::CSBuyManualInfo &stBuyManualInfo = P_MANUAL_CTRL->getBuyManualInfo();

		if(stBuyManualInfo.has_maxcnt()) {
			unsigned int uBuyCnt = stBuyManualInfo.buycnt();
			unsigned int uMaxCnt = stBuyManualInfo.maxcnt();

			isCanAddManual = uBuyCnt < uMaxCnt;
		}

		if(isCanAddManual && uRemainManual < MAX_MANUAL_VAL) {
			m_pManualAddBtn->setBright(true);
			m_pManualAddBtn->setTouchEnabled(true);
		} else {
			m_pManualAddBtn->setBright(false);
			m_pManualAddBtn->setTouchEnabled(false);
		}

		if(uRemainManual >= MAX_MANUAL_VAL) {
			m_pManualTimerTxt->setText("MAX");
		} else {
			string timestr = intToString(100 + m_nRemainTime / 60).substr(1, 2) + ":" + intToString(100 + m_nRemainTime % 60).substr(1, 2);
			m_pManualTimerTxt->setText(timestr);
		}
	}
}

void CityScreenView::updateTask() {
	const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

	if(P_FIGHT_CTRL->isLastFightFailed()) {
		unsigned int uEntryID = P_CITY_CTRL->getDelayedEntryID();

		if(uEntryID == ENTRY_SHOP) {
			P_AUTO_POP_CTRL->addPopPanel(P_SHOP_CTRL->getRmbShopPanel());
		} else if(uEntryID == ENTRY_GENE_STRENGTHEN) {
			P_AUTO_POP_CTRL->addPopPanel(P_GEN_CTRL->getGenPanel());
		} else if(uEntryID == ENTRY_WEAPON_LIB) {
			P_AUTO_POP_CTRL->addPopPanel(P_WEAPON_CTRL->getWeaponLibPanel());
		} else if(uEntryID == ENTRY_EQUIP_CULTURE) {
			P_AUTO_POP_CTRL->addPopPanel(P_EQUIP_LIB_CTRL->getEquipLibListPanel());
		} else if(uEntryID == ENTRY_REFIT) {
			P_AUTO_POP_CTRL->addPopPanel(P_REFIT_CTRL->getRefitPanel());
		} else if(uEntryID == ENTRY_EVOLUTION) {
			P_AUTO_POP_CTRL->addPopPanel(P_EVOLUTION_CTRL->getEvolutionPanel());
		} else {
			if(P_FIGHT_CTRL->isWarMapFight()) {
				P_FIGHT_CTRL->setWarMapFight(false);
				P_AUTO_POP_CTRL->addPopPanel(P_WAR_MAP_CTRL->getWarMapPanel());
			}
		}

		P_FIGHT_CTRL->setLastFightFailed(false);
		P_CITY_CTRL->setDelayedEntryID(ENTRY_UNKNOWN);
	} else {
		if(P_FIGHT_CTRL->isWarMapFight()) {
			P_FIGHT_CTRL->setWarMapFight(false);
			const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

			if(stTask.ByteSize() > 0 && stTask.taskstat() != vmsg::TASK_STAT_FINISH) {
				P_AUTO_POP_CTRL->addPopPanel(P_WAR_MAP_CTRL->getWarMapPanel());
			}/* else {
				P_AUTO_POP_CTRL->addPopPanel(P_WAR_MAP_CTRL->getWarMapPanel());
			}*/
		}
	}

	// 在同屏场景中才弹出
	if(P_CITY_CTRL->isInCity()) {
		P_AUTO_POP_CTRL->popPanel();
	}

	if(m_bUIInited && stTask.has_taskid() && TASK_CFG_MASTER->isValid(stTask.taskid())) {

        //P_SIGN_IN_CTRL->checkNeedAutoOpen(); //是否需要自动打开签到

		const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(stTask.taskid());
		m_pTaskNameTxt->setText(pTaskCfg->taskname());
		string desc = "";
		bool isTaskAvailable = false;
		unsigned int uTaskTargetType = pTaskCfg->tasktarget().targettype();
		unsigned int uTaskStat = stTask.taskstat();

		if(uTaskTargetType == vmsg::TASK_TARGET_KILL_MONSTER) { // 杀怪

		} else if(uTaskTargetType == vmsg::TASK_TARGET_CLEAR_WAR) { // 通关副本
			const WarMapCfg *pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(pTaskCfg->tasktarget().r1());
			desc = pWarMapCfg->name();

			if(uTaskStat == vmsg::TASK_STAT_ORG) {
				if(P_USER_OBJ->getUserInfo()->level >= pTaskCfg->level()) {
					isTaskAvailable = true;
				} else {
					isTaskAvailable = false;
				}
			} else if(uTaskStat == vmsg::TASK_STAT_RECEIVED) {
				isTaskAvailable = true;
			} else if(uTaskStat == vmsg::TASK_STAT_FINISH) {
				isTaskAvailable = true;
			}
		} else if(uTaskTargetType == vmsg::TASK_TARGET_TALK_NPC) { // npc对话
			const NPCCfg *pNPCCfg = NPC_CFG_MASTER->getNPCCfgByID(pTaskCfg->tasktarget().r1());
			desc = pNPCCfg->npcname();
			isTaskAvailable = true;
		}

		m_pTaskDescTxt->setText(desc);
		m_pTaskTraceLayout->setTouchEnabled(true);

		if(isTaskAvailable) {
			m_pTaskArmature->getAnimation()->play("fly", -1, -1, 1);
			m_pTaskArmature->setVisible(true);
		} else {
			m_pTaskArmature->getAnimation()->stop();
			m_pTaskArmature->setVisible(false);
		}

	} else {
		m_pTaskNameTxt->setText(TextStr(TEXT_NONE_VALID_TASK));
		m_pTaskDescTxt->setText("");
	}
}

void CityScreenView::updateEntry() {
	m_pHeadEntryLayout->updateEntry();
	m_pTopEntryLayout->updateEntry();
	m_pBottomEntryLayout->updateEntry();
}

void CityScreenView::mailNotify() {
	if(m_pHeadEntryLayout != NULL) {
		m_pHeadEntryLayout->mailNotify();
	}
}

void CityScreenView::manualTimer(float dt) {
	if(m_nRemainTime > 0) {
		m_nRemainTime--;

		if(isRunning()) {
			const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();

			if(pUserInfo->uin != 0) {
				const unsigned int uRemainManual = pUserInfo->remainManual;

				if(uRemainManual >= MAX_MANUAL_VAL) {
					m_pManualTimerTxt->setText("MAX");
				} else {
					string timestr = intToString(100 + m_nRemainTime / 60).substr(1, 2) + ":" + intToString(100 + m_nRemainTime % 60).substr(1, 2);
					m_pManualTimerTxt->setText(timestr);
				}
			}
		}

		if(m_nRemainTime == 0) {
			P_MANUAL_CTRL->sendQueryManualRqst();
		}
	}
}

void CityScreenView::moduleOpen(unsigned int uModuleID) {
	unsigned int uEntryID = P_ENTRY_CTRL->getEntryIDByModuleID(uModuleID);

	if(uEntryID != 0) {
		m_uNowModuleID = uModuleID;
		CCDirector *pDirector = CCDirector::sharedDirector();
		BaseScene *pScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
		UILayer *pTopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_TOP));
		CCSize visibleSize = pDirector->getVisibleSize();
		CCPoint visibleOrigin = pDirector->getVisibleOrigin();
		CCPoint centerPos = this->convertToNodeSpace(ccp(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2));

		if(m_pBgMask == NULL) {
			m_pBgMask = UILayout::create();
			m_pBgMask->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
			m_pBgMask->setBackGroundColorType(LAYOUT_COLOR_SOLID);
			m_pBgMask->setBackGroundColorOpacity(0x40);
			m_pBgMask->setTouchEnabled(true);
			m_pBgMask->setName("bg_mask");
			m_pBgMask->setSize(visibleSize);
			m_pBgMask->setPosition(this->convertToNodeSpace(visibleOrigin));
			pTopLayer->addWidget(m_pBgMask);
		}

		if(m_pFuncBtn == NULL) {
			m_pFuncBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
			
			std::string btnName = std::string("func") + intToString(uEntryID);
			m_pFuncBtn->setName(btnName.c_str());
			m_pFuncBtn->setPosition(centerPos);
			pTopLayer->addWidget(m_pFuncBtn);
			m_pFuncBtn->setVisible(false);
		}

		if(m_pFuncWordsTxt == NULL) {
			m_pFuncWordsTxt = UILabel::create();
			m_pFuncWordsTxt->setFontName("fonts/tgamefont.ttf");
			m_pFuncWordsTxt->setFontSize(28);
			m_pFuncWordsTxt->setColor(ccc3(0x00, 0xff, 0x00));
			CCColorUtil::addStroke(m_pFuncWordsTxt, 0.8f, ccc3(0xFF, 0xff, 0x00));
			CCColorUtil::addShadow(m_pFuncWordsTxt);
			m_pFuncWordsTxt->setPosition(ccp(centerPos.x, centerPos.y - m_pFuncBtn->getSize().height));
			pTopLayer->addWidget(m_pFuncWordsTxt);
			m_pFuncWordsTxt->setVisible(false);
			m_pFuncWordsTxt->setText(getModuleText(uModuleID));
		}

		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00011/ut00011.ExportJson");
		m_pBtnEffArmature = CCArmature::create("ut00011");
		pTopLayer->addChild(m_pBtnEffArmature, 0, 101);
		m_pBtnEffArmature->setPosition(centerPos);
		m_pBtnEffArmature->getAnimation()->play("fly01", -1, -1, 0);
		m_pBtnEffArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CityScreenView::onBtnEffPlayEvent));
		m_pBtnEffArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(CityScreenView::onBtnEffFrameEvent));

	}
}

void CityScreenView::foldEntry() {
	m_pBottomEntryLayout->foldEntry();
}

void CityScreenView::unfoldEntry() {
	m_pBottomEntryLayout->unfoldEntry();
}

void CityScreenView::badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum) {
	if(m_pBottomEntryLayout != NULL) {
		m_pBottomEntryLayout->badgeEntryBtn(uEntryID, uBadgeNum);
	}

	if(m_pTopEntryLayout != NULL) {
		m_pTopEntryLayout->badgeEntryBtn(uEntryID, uBadgeNum);
	}

	if(m_pHeadEntryLayout != NULL) {
		m_pHeadEntryLayout->badgeEntryBtn(uEntryID, uBadgeNum);
	}

	SubEntryPanel *pSubEntryPanel = P_CITY_CTRL->getSubEntryPanel();

	if(pSubEntryPanel != NULL) {
		pSubEntryPanel->badgeEntryBtn(uEntryID, uBadgeNum);
	}
}

void CityScreenView::bringAssetsToTop() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();
	BaseScene *pScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());

	if(pScene != NULL) {
		UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));

		if(pUILayer != NULL) {
			if(m_pAssetsLayout != NULL && m_pAssetsLayout->getParent() != pUILayer) {
				CC_SAFE_RETAIN(m_pAssetsLayout);
				m_pAssetsLayout->removeFromParent();
				float posX = screenOrigin.x + screenSize.width - m_pAssetsLayout->getSize().width - 2;
				float posY = screenOrigin.y + screenSize.height - m_pAssetsLayout->getSize().height;
				m_pAssetsLayout->setPosition(ccp(posX, posY));
				pUILayer->addWidget(m_pAssetsLayout);
			}
		}
	}
}

void CityScreenView::bringAssetsToBottom() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();

	if(m_pAssetsLayout != NULL && m_pAssetsLayout->getParent() != m_pRootLayout) {
		m_pAssetsLayout->removeFromParent();
		float posX = screenOrigin.x + screenSize.width - m_pAssetsLayout->getSize().width - 2;
		float posY = screenOrigin.y + screenSize.height - m_pAssetsLayout->getSize().height;
		m_pAssetsLayout->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));
		m_pRootLayout->addChild(m_pAssetsLayout);
		CC_SAFE_RELEASE(m_pAssetsLayout);
	}
}

void CityScreenView::initUI() {
	initLayoutFromJsonFile("ui_v2/CityScreenViewUI.ExportJson");
	addChild(m_pRootLayout);
	setTouchEnabled(false);

	CToolUtils::loadArmatureRGBA4444("effect/ut00074/ut00074.ExportJson");
	CToolUtils::loadArmatureRGBA4444("effect/ut00085/ut00085.ExportJson");

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();
	float posX = 0.0f;
	float posY = 0.0f;

	UILayout *pHeadLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("head_layout"));
	posX = screenOrigin.x;
	posY = screenOrigin.y + screenSize.height - pHeadLayout->getSize().height;
	pHeadLayout->setPosition(convertToNodeSpace(ccp(posX, posY)));

	m_pLevelTxt = ImageNumber::create(24);
	UIImageView* pBgLevelImg = dynamic_cast<UIImageView*>(pHeadLayout->getChildByName("bg_level_img"));
	m_pLevelTxt->setAnchorPoint(ccp(0.5f, 0.0f));
	m_pLevelTxt->setPosition(pBgLevelImg->getPosition() + ccp(12, 6));
	pHeadLayout->addNode(m_pLevelTxt);
	m_pLevelTxt->setNum("1", 0.0f, "Lv");

	m_pVIPTxt = ImageNumber::create(25);
	m_pVIPTxt->setAnchorPoint(ccp(0.5f, 0.0f));
	m_pVIPTxt->setPosition(pBgLevelImg->getPosition() + ccp(160, 6));
	pHeadLayout->addNode(m_pVIPTxt);
	m_pVIPTxt->setNum("0", 0.0f, "vip");

	m_pFightPowerTxt = ImageNumber::create(20);
	UIImageView* pTextFightPowerImg = dynamic_cast<UIImageView*>(pHeadLayout->getChildByName("text_fight_power_img"));
	CCPoint pos = pTextFightPowerImg->getPosition();
	pos.x += pTextFightPowerImg->getSize().width / 2 + 10;
	m_pFightPowerTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pFightPowerTxt->setPosition(pos);
	pHeadLayout->addNode(m_pFightPowerTxt);

	UILayout *pResourceLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("resource_layout"));
	posX = screenOrigin.x + screenSize.width - pResourceLayout->getSize().width - 2;
	posY = screenOrigin.y + screenSize.height - pResourceLayout->getSize().height;
	pResourceLayout->setPosition(convertToNodeSpace(ccp(posX, posY)));
	m_pAssetsLayout = pResourceLayout;

	m_pCoinValTxt = dynamic_cast<UILabel*>(pResourceLayout->getChildByName("coin_val_txt"));

	m_pCoinAddBtn = dynamic_cast<UIButton*>(pResourceLayout->getChildByName("coin_add_btn"));
	m_pCoinAddBtn->addTouchEventListener(this, toucheventselector(CityScreenView::touchEventHandler));

	m_pGoldValTxt = dynamic_cast<UILabel*>(pResourceLayout->getChildByName("gold_val_txt"));

	m_pGoldAddBtn = dynamic_cast<UIButton*>(pResourceLayout->getChildByName("gold_add_btn"));
	m_pGoldAddBtn->addTouchEventListener(this, toucheventselector(CityScreenView::touchEventHandler));

	m_pManualTimerTxt = dynamic_cast<UILabel*>(pResourceLayout->getChildByName("manual_timer_txt"));

	m_pManualValTxt = dynamic_cast<UILabel*>(pResourceLayout->getChildByName("manual_val_txt"));

	m_pManualAddBtn = dynamic_cast<UIButton*>(pResourceLayout->getChildByName("manual_add_btn"));
	m_pManualAddBtn->addTouchEventListener(this, toucheventselector(CityScreenView::touchEventHandler));

	UILayout *pHeadEntryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("head_entry_layout"));
	posX = screenOrigin.x + 16;
	posY = pHeadLayout->getPositionY() - pHeadEntryLayout->getSize().height - 10;
	pHeadEntryLayout->setPosition(convertToNodeSpace(ccp(posX, posY)));
	m_pHeadEntryLayout = HeadEntryLayout::create(pHeadEntryLayout);

	UILayout *pTaskTraceLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("task_trace_layout"));
	posX = screenOrigin.x + 10;
	posY = screenOrigin.y + screenSize.height / 2;
	pTaskTraceLayout->setPosition(convertToNodeSpace(ccp(posX, posY)));
	pTaskTraceLayout->setTouchEnabled(true);
	pTaskTraceLayout->addTouchEventListener(this, toucheventselector(CityScreenView::touchEventHandler));
	m_pTaskTraceLayout = pTaskTraceLayout;

	m_pTaskArmature = CCArmature::create("ut00074");
	m_pTaskArmature->setPosition(CCPoint(pTaskTraceLayout->getSize()) / 2);
	pTaskTraceLayout->addNode(m_pTaskArmature);
	m_pTaskArmature->setVisible(false);

	m_pTaskNameTxt = dynamic_cast<UILabel*>(pTaskTraceLayout->getChildByName("task_name_txt"));
	
	m_pTaskDescTxt = dynamic_cast<UILabel*>(pTaskTraceLayout->getChildByName("task_desc_txt"));

	UILayout *pTopEntryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("top_entry_layout"));
	posX = screenOrigin.x + screenSize.width - pTopEntryLayout->getSize().width - 10;
	posY = pResourceLayout->getPositionY() - pTopEntryLayout->getSize().height - 10;
	pTopEntryLayout->setPosition(convertToNodeSpace(ccp(posX, posY)));
	m_pTopEntryLayout = TopEntryLayout::create(pTopEntryLayout);

	UILayout *pExpLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("exp_layout"));
	posX = screenOrigin.x + screenSize.width - pExpLayout->getSize().width - 2;
	posY = screenOrigin.y + 2;
	pExpLayout->setPosition(convertToNodeSpace(ccp(posX, posY)));

	m_pExpProgress = dynamic_cast<UILoadingBar*>(pExpLayout->getChildByName("exp_progress"));

	m_pExpProgressTxt = dynamic_cast<UILabel*>(pExpLayout->getChildByName("exp_progress_txt"));

	UILayout *pBottomEntryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("bottom_entry_layout"));
	posX = screenOrigin.x + screenSize.width - pBottomEntryLayout->getSize().width - 8;
	posY = pExpLayout->getPositionY() + pExpLayout->getSize().height + 2;
	pBottomEntryLayout->setPosition(convertToNodeSpace(ccp(posX, posY)));
	m_pBottomEntryLayout = BottomEntryLayout::create(pBottomEntryLayout);

	NotificationLayout *pLayout = P_NOTIFICATION_CTRL->getNotificationLayout();

	if(pLayout != NULL && pLayout->getParent() == NULL) {
		posX = m_pTaskTraceLayout->getPositionX() + m_pTaskTraceLayout->getSize().width + 16;
		posY = screenOrigin.y + screenSize.height / 2 + (m_pTaskTraceLayout->getSize().height - pLayout->getSize().height) / 2;
		pLayout->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));
		m_pRootLayout->addChild(pLayout);
		pLayout->setEnabled(false);
	}

	m_bUIInited = true;
}

void CityScreenView::touchEventHandler(CCObject *pSender, TouchEventType type) {
	Widget *pWidget = dynamic_cast<Widget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pTaskTraceLayout->getName()) == 0) {
				const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

				if(stTask.has_taskid()
				&& TASK_CFG_MASTER->isValid(stTask.taskid())) {
					const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(stTask.taskid());

					if(stTask.taskstat() == vmsg::TASK_STAT_ORG
					&& P_USER_OBJ->getUserInfo()->level < pTaskCfg->level()) { // 等级不够
						/*char text[100];
						snprintf(text, 100, TextStr(TEXT_TASK_LEVEL_UNREACHED), pTaskCfg->level());
						COMMON_DIALOG->show(text);*/
						P_TIP_CTRL->getLevelNotEnoughPanel()->openWithLimitLevel(pTaskCfg->level());
					} else {
						GUIDE_TOUCH(m_pTaskTraceLayout);
						m_pTaskArmature->getAnimation()->stop();
						m_pTaskArmature->setVisible(false);
						P_TASK_CTRL->autoTask();
					}
				}
			} else if(strcmp(pszWidgetName, m_pCoinAddBtn->getName()) == 0) {
				P_VIP_CTRL->getBuyCoinPanel()->open();
			} else if(strcmp(pszWidgetName, m_pGoldAddBtn->getName()) == 0) {
				P_SHOP_CTRL->getRmbShopPanel()->open();
			} else if(strcmp(pszWidgetName, m_pManualAddBtn->getName()) == 0) {
				P_TIP_CTRL->LowManualBuyTip(this, dialogbtneventselector(CityScreenView::buyManual));
			} else if(m_pFuncBtn != NULL && strcmp(pszWidgetName, m_pFuncBtn->getName()) == 0) {
				m_pFuncBtn->setTouchEnabled(false);
				m_pBtnEffArmature->getAnimation()->play("fly03", -1, -1, 0);
			} else if(m_pBgMask != NULL && strcmp(pszWidgetName, m_pBgMask->getName()) == 0) {
				m_pBgMask->setTouchEnabled(false);
				m_pBtnEffArmature->getAnimation()->play("fly03", -1, -1, 0);
			}
			break;
		default:;
	}
}

void CityScreenView::buyManual() {
	P_MANUAL_CTRL->sendBuyManualRqst();
}

void CityScreenView::onBtnEffPlayEvent(CCArmature* pArmature, MovementEventType type, const char *pszActionName) {
	switch(type) {
		case COMPLETE:
			if(std::strcmp(pszActionName, "fly01") == 0) {
				pArmature->getAnimation()->play("fly02", -1, -1, 1);
				m_pBgMask->addTouchEventListener(this, toucheventselector(CityScreenView::touchEventHandler));
				m_pFuncBtn->setVisible(true);
				m_pFuncBtn->addTouchEventListener(this, toucheventselector(CityScreenView::touchEventHandler));
				m_pFuncWordsTxt->setVisible(true);
				/*std::string url = SOUND_RES_PATH + "module_open.ogg";
				AudioManager::playEffect(url.c_str());*/
			}else if(std::strcmp(pszActionName, "fly03") == 0) {
				std::string strWidgetName = m_pFuncBtn->getName();
				unsigned int uEntryID = std::atol(strWidgetName.substr(std::string("func").size(), strWidgetName.size()).c_str());
				m_pBgMask->removeFromParent();
				m_pBgMask = NULL;
				m_pFuncBtn->setEnabled(false); // remove after fly arrived
				m_pFuncWordsTxt->removeFromParent();
				m_pFuncWordsTxt = NULL;
				updateEntry(); // 更新入口

				CCPoint startPos = m_pBtnEffArmature->getPosition();
				CCPoint endPos = CCPointZero;

				if(uEntryID == ENTRY_DAILY_TASK
				|| uEntryID == ENTRY_SIGN_IN
				|| uEntryID == ENTRY_BUY_COIN
				|| uEntryID == ENTRY_SHOP
				|| uEntryID == ENTRY_LOTTERY
				|| uEntryID == ENTRY_WEAPON_BOOK
				|| uEntryID == ENTRY_DAILY_ACT
				|| uEntryID == ENTRY_LIMITED_SHOP
				|| uEntryID == ENTRY_ACTIVITIES
				|| uEntryID == ENTRY_DAILY_WAR
				|| uEntryID == ENTRY_VIP) {
					endPos = convertToNodeSpace(m_pTopEntryLayout->convertToWorldSpace(m_pTopEntryLayout->calcEntryPos(uEntryID)));
				} else {
					if(m_pBottomEntryLayout->isEntryUnfold()) {
						unsigned int uTopLevelEntryID = uEntryID;
						if (ENTRY_CFG_MASTER->getChildEntryIDSize(uEntryID) == 0)
						{
							uTopLevelEntryID = ENTRY_CFG_MASTER->getParentEntryID(uEntryID);
							if (uTopLevelEntryID==0)
							{
								uTopLevelEntryID = uEntryID;
							}
						}

						//unsigned int uTopLevelEntryID = uEntryID >> 8 > 0 ? uEntryID >> 8 : uEntryID;
						endPos = convertToNodeSpace(m_pBottomEntryLayout->convertToWorldSpace(m_pBottomEntryLayout->calcEntryPos(uTopLevelEntryID)));
					} else {
						endPos = convertToNodeSpace(m_pBottomEntryLayout->convertToWorldSpace(m_pBottomEntryLayout->getEntryOriginPos()));
					}
				}

				float angle = (endPos - startPos).getAngle();
				angle = atan2f(endPos.y - startPos.y, endPos.x - startPos.x);
				m_pBtnEffArmature->setRotation(-angle * 180 / M_PI - 90);

				CCActionInterval *pEaseMove = CCEaseIn::create(CCMoveTo::create(0.2f, endPos), 1.6f);
				CCActionInstant *pFunc = CCCallFunc::create(this, callfunc_selector(CityScreenView::flyArrived));

				m_pBtnEffArmature->runAction(CCSequence::create(pEaseMove, pFunc, NULL));

			} else if(std::strcmp(pszActionName, "fly04") == 0) {
				if(pArmature->getParent() != NULL) {
					pArmature->removeFromParent();
                }
                CToolUtils::removeArmatureFile("effect/ut00011/ut00011.ExportJson");
			}
			break;
		default:;
	}
}

void CityScreenView::onBtnEffFrameEvent(CCBone *pBone, const char *pszFrameName, int a, int b) {
	if(std::strcmp(pszFrameName, "icon_appear") == 0) {

	} else if(std::strcmp(pszFrameName, "icon_disappear") == 0) {

	}
}

void CityScreenView::flyArrived() {
	m_pBtnEffArmature->getAnimation()->play("fly04", -1, -1, 0);
	string btnName = m_pFuncBtn->getName();
	unsigned int uEntryID = atol(btnName.substr(string("func").size(), btnName.size()).c_str());
	m_pFuncBtn->removeFromParent();
	m_pFuncBtn = NULL;

	updateEntry();

	GUIDE_TRIGGER(GUIDE_TRIGGER_MODULE, m_uNowModuleID);

	if(uEntryID == ENTRY_DAILY_TASK
	|| uEntryID == ENTRY_SIGN_IN
	|| uEntryID == ENTRY_BUY_COIN
	|| uEntryID == ENTRY_SHOP
	|| uEntryID == ENTRY_LOTTERY
	|| uEntryID == ENTRY_WEAPON_BOOK
	|| uEntryID == ENTRY_DAILY_ACT) {
		GUIDE_SHOW(GUIDE_UI_CITY_SCREEN);
	} else {
		m_pBottomEntryLayout->unfoldEntry();
	}
}

string CityScreenView::getModuleText(unsigned int uModuleID) {
	std::string text = "";

	const ModuleOpenCfg *pModuleOpenCfg = MODULE_OPEN_CFG_MASTER->getModuleOpenCfg(uModuleID);
	text = pModuleOpenCfg != NULL ? pModuleOpenCfg->openwords() : "";

	return text;
}

void CityScreenView::updateOnlineAndMorrow()
{
	if (m_bUIInited&&m_pTopEntryLayout)
	{
		m_pTopEntryLayout->updateOnlineAndMorrow();
	}
}