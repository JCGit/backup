#include "WarMapPanel.h"

#include "defines.h"

#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "basic/NounHelper.h"
#include "basic/ToolUtils.h"
#include "basic/ItemUtils.h"
#include "basic/CCColorUtil.h"
#include "ConstHelper.h"

#include "CfgMaster/WarMapCfgMaster.h"
#include "CfgMaster/EliteWarMapCfgMaster.h"

#include "game/UserObj.h"
#include "game/fight/LevelObj.h"
#include "game/fight/EliteWarPage.h"
#include "game/fight/FightControl.h"
#include "game/tip/TipControl.h"
#include "game/bag/BagControl.h"
#include "game/IconFrameCell.h"

#include "WarMapControl.h"
#include "WarPage.h"
#include "evtlog/EvtLog.h"

WarMapPanel::WarMapPanel()
:m_pBgImg(NULL)
,m_pCloseBtn(NULL)
,m_pBgChapterTitleImg(NULL)
,m_pChapterTitleImg(NULL)
,m_pTabListView(NULL)
,m_pWarPageView(NULL)
,m_pEliteWarPageView(NULL)
,m_pPrevBtn(NULL)
,m_pNextBtn(NULL)
,m_pSupplyLayout(NULL)
,m_pBulletIconImg(NULL)
,m_pBulletIconFrameCell(NULL)
,m_pBulletCntTxt(NULL)
,m_pBulletSupplyBtn(NULL)
,m_pMedicineIconImg(NULL)
,m_pMedicineIconFrameCell(NULL)
,m_pMedicineCntTxt(NULL)
,m_pMedicineSupplyBtn(NULL)
,m_pAwardLayout(NULL)
,m_pStarCntTxt(NULL)
,m_pStarAwardBtn(NULL)
,m_pEliteActionLayout(NULL)
,m_pRemainRefreshTimesValTxt(NULL)
,m_pSweepBtn(NULL)
,m_pRefreshBtn(NULL)
,m_pCostLayout(NULL)
,m_pCostTxt(NULL)
,m_uWarTab(WAR_TAB_NORMAL)
,m_uAwardStarCnt(0)
,m_uNextStarCnt(0)
,m_nTabSelectedIdx(-1)
,m_bMatchTask(false)
,m_isBeFresh(false)
,m_pElitePageShow(-1)
{
}

WarMapPanel::~WarMapPanel()
{
}

WarMapPanel* WarMapPanel::create() {
	WarMapPanel *pRet = new WarMapPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WarMapPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void WarMapPanel::open() {
	BasePanel::open();

	showLoading();

	CToolUtils::loadArmatureRGBA4444("effect/ut00004/ut00004.ExportJson");
	CToolUtils::loadArmatureRGBA4444("effect/ut00075/ut00075.ExportJson");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/elite_stages.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/key_stages.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/war_map.plist");

	if (m_isBeFresh)
	{
		m_isBeFresh = false;
	}

	if(!m_bUIInited) {
		initUI();
	}

	// open时会看是否从任务追踪过来，查询放在switchTab
	switchTab(m_bMatchTask || m_uWarTab == WAR_TAB_NORMAL ? WAR_TAB_NORMAL : WAR_TAB_ELITE);
}

void WarMapPanel::close() {
	BasePanel::close();
	m_pElitePageShow = -1;
	m_bMatchTask = false; // 重置
	CToolUtils::removeArmatureFile("effect/ut00004/ut00004.ExportJson");
	CToolUtils::removeArmatureFile("effect/ut00075/ut00075.ExportJson");
}

CCNode* WarMapPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(!m_bUIInited) {
		return NULL;
	}

	if(name == "war_map_cell") {
		WarPage *pWarPage = dynamic_cast<WarPage*>(m_pWarPageView->getPage(m_pWarPageView->getCurPageIndex()));

		if(pWarPage != NULL) {
			Widget *pWidget = pWarPage->getCurLevelCell();

			if(pWidget != NULL) {
				pRet = pWidget;
			}
		}
	} else if(name == "elite_war_map_cell") {
		const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();
		int nCurPageIdx = stEliteWarInfo.elitewarid() - 1;
		EliteWarPage *pEliteWarPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(nCurPageIdx));

		if(pEliteWarPage != NULL) {
			Widget *pWidget = pEliteWarPage->getCurWarMapCell();

			if(pWidget != NULL && pWidget->isRunning()) {
				pRet = pWidget;
			}
		}
	} else if(name == "elite_tab_btn") {
		if(!isOpen()) {
			open();
		}

		UIButton *pTabBtn = m_pTabListView->isEnabled() ? dynamic_cast<UIButton*>(m_pTabListView->getItem(WAR_TAB_ELITE)) : NULL;

		if(pTabBtn != NULL && pTabBtn->isRunning() && pTabBtn->isTouchEnabled()) {
			pRet = pTabBtn;
		}
	}

	return pRet;
}

bool WarMapPanel::isMeetAutoOpen() {
	return true;
}

void WarMapPanel::switchTab(WarTab tab) {
	m_uWarTab = tab;

	if(!m_bUIInited) {
		return;
	}

	showLoading();

	UIButton *pTabBtn = NULL;

	if(m_nTabSelectedIdx >= 0) {
		pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_nTabSelectedIdx));
		pTabBtn->setBrightStyle(BRIGHT_NORMAL);
		pTabBtn->setTouchEnabled(true);
	}

	pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(tab));
	pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
	pTabBtn->setTouchEnabled(false);
	m_nTabSelectedIdx = tab;

	if(m_uWarTab == WAR_TAB_ELITE) {
		GUIDE_TOUCH(pTabBtn);
	}

	if(m_uWarTab == WAR_TAB_NORMAL) {
		P_WAR_MAP_CTRL->sendQueryWarMapInfoRqst();
	} else if(m_uWarTab == WAR_TAB_ELITE) {
		const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();
		const EliteWarMapCfg *pLastEliteWarMapCfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(P_WAR_MAP_CTRL->getEliteWarMapID());
		// 精英副本结束回来
		if(pLastEliteWarMapCfg != NULL && pLastEliteWarMapCfg->warid() <= stEliteWarInfo.elitewarid()) {
			int nCurPageIdx = m_pEliteWarPageView->getCurPageIndex();
			EliteWarPage *pCurPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(nCurPageIdx));
			P_FIGHT_CTRL->getFightProxy()->sendQueryEliteWarMapRqst(pCurPage->getEliteWarChapterID());
		} else {
			P_FIGHT_CTRL->getFightProxy()->sendQueryEliteWarMapRqst(0); // 回最大章节信息
		}
	}

	m_pWarPageView->setEnabled(m_uWarTab == WAR_TAB_NORMAL);
	m_pEliteWarPageView->setEnabled(m_uWarTab == WAR_TAB_ELITE);

	m_pSupplyLayout->setEnabled(m_uWarTab == WAR_TAB_NORMAL);
	m_pAwardLayout->setEnabled(m_uWarTab == WAR_TAB_NORMAL);
	m_pEliteActionLayout->setEnabled(m_uWarTab == WAR_TAB_ELITE);
	m_pEliteActionLayout->setVisible(m_uWarTab == WAR_TAB_ELITE);
}

void WarMapPanel::goToEliteByWarID(unsigned int warId)
{
	int pIndex = 0;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_pEliteWarPageView->getPages(), pObj)
	{
		EliteWarPage* pElitePage = dynamic_cast<EliteWarPage*>(pObj);
		if (pElitePage)
		{
			if (pElitePage->getEliteWarChapterID() == warId)
			{
				break;
			}
			pIndex++;
		}
	}
	m_pElitePageShow = pIndex;
}

void WarMapPanel::updateUI() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	bool isEliteWarOpen = P_WAR_MAP_CTRL->isEliteWarOpen();
	m_pTabListView->setEnabled(isEliteWarOpen);

	if(m_uWarTab == WAR_TAB_NORMAL) {
		unsigned int uMaxWarMapID = P_LEVEL_OBJ->getMapInfo().newestwarmapid();
		vmsg::CSWarMapQueryRsp stMapInfo = P_LEVEL_OBJ->getMapInfo();
		unsigned int uMaxLevelNum = stMapInfo.toid() - stMapInfo.fromid() + 1;
		unsigned int uTotalPages = 0;
		const WarMapCfg *pCurWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(uMaxWarMapID + 1);

		if(pCurWarMapCfg != NULL) {
			uTotalPages = pCurWarMapCfg->warid();
		} else {
			const WarMapCfg *pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(uMaxWarMapID);
			uTotalPages = pWarMapCfg->warid();
		}

		WarPage* pWarPage = NULL;
		unsigned int i = 0;

		while(i < uTotalPages){
			pWarPage = dynamic_cast<WarPage*>(m_pWarPageView->getPage(i));

			if(pWarPage == NULL){
				pWarPage = WarPage::create(i+1);
				m_pWarPageView->addPage(pWarPage);
			}

			pWarPage->setTaskTraceStat(m_bMatchTask);
			pWarPage->updateUI();

			i++;
		}

		// 将多余的页删除掉
		while(i < m_pWarPageView->getPages()->count()) {
			m_pWarPageView->removePageAtIndex(i);
		}

		if(uTotalPages > 0) {
			int nScrollIdx = uTotalPages - 1;
			const WarMapCfg *pLastWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(P_WAR_MAP_CTRL->getWarMapID());

			// 副本结束回来
			if(pLastWarMapCfg != NULL && pLastWarMapCfg->warid() <= uTotalPages) {
				nScrollIdx = pLastWarMapCfg->warid() - 1;
			}

			m_pWarPageView->scrollToPage(nScrollIdx);
			GUIDE_SHOW(GUIDE_UI_WAR_MAP);
		}

		m_pBulletIconFrameCell->setInfo(vmsg::THING_TYPE_ITEM, vmsg::ITEM_ARMO_ID);
		m_pMedicineIconFrameCell->setInfo(vmsg::THING_TYPE_ITEM, ConstHelper(FIGHT_MEDICAL_CFG_DI));
		m_pBulletCntTxt->setText(string("x") + intToString(P_BAG_CTRL->getItemCnt(vmsg::ITEM_ARMO_ID)));
		m_pMedicineCntTxt->setText(string("x") + intToString(P_BAG_CTRL->getItemCnt(ConstHelper(FIGHT_MEDICAL_CFG_DI))));

		//evt log
		if(uMaxWarMapID == WAR_MAP_CFG->GetCfg().warmapcfg(0).id() ) {
			// 第一个任务指引过来的
			ADD_EVT_LOG(EVT_LOG_FIRST_TASK_OPEN_WAR_PANEL);
		}
	} else if(m_uWarTab == WAR_TAB_ELITE) {
		unsigned int nTotalPages = P_WAR_MAP_CTRL->getMaxEliteWarChapterID();
		EliteWarPage *pEliteWarPage = NULL;

		unsigned int i = 0;

		while(i < nTotalPages) {
			pEliteWarPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(i));

			if(pEliteWarPage == NULL) {
				pEliteWarPage = EliteWarPage::create();
				pEliteWarPage->setEliteWarChapterID(i+1);
				m_pEliteWarPageView->addPage(pEliteWarPage);
			}

			i++;
		}

		// 将多余的页删除掉
		while(i < m_pEliteWarPageView->getPages()->count()) {
			m_pEliteWarPageView->removePageAtIndex(i);
		}
		
		if (m_pElitePageShow>=0&&m_pEliteWarPageView->getCurPageIndex()!=m_pElitePageShow)
		{
			m_pEliteWarPageView->scrollToPage(m_pElitePageShow);
			EliteWarPage *pCurPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(m_pElitePageShow));
			if (pCurPage)
			{
				P_FIGHT_CTRL->getFightProxy()->sendQueryEliteWarMapRqst(pCurPage->getEliteWarChapterID());
			}
		}else
		{
			const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();
			// 每次翻页后都会查询，故只更新当前章节
			int nCurPageIdx = m_pEliteWarPageView->getCurPageIndex();
			EliteWarPage *pCurPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(nCurPageIdx));


			if (pCurPage && pCurPage->getEliteWarChapterID() == stEliteWarInfo.elitewarid()) { // 当前章节为查询章节
				pCurPage->updateUI();
				GUIDE_SHOW(GUIDE_UI_WAR_MAP);
			} else { // 滚动到查询章节
				m_pEliteWarPageView->scrollToPage(stEliteWarInfo.elitewarid() - 1);
			}
		}
		
	}
	if (!m_isBeFresh)
	{
		m_isBeFresh = true;
		handlePageTurningLogic();
	}
	
	updatePageStat();

	closeLoading();
}

void WarMapPanel::matchTask(bool bMatchTask) {
	m_bMatchTask = bMatchTask;
}

void WarMapPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/WarMapPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));
	m_pBgImg->loadTexture("chapter_map/war_page_bg.png");

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();
	float scaleX = screenSize.width / m_pBgImg->getSize().width;
	float scaleY = screenSize.height / m_pBgImg->getSize().height;
	float scale = scaleX > scaleY ? scaleX : scaleY;
	m_pBgImg->setScale(scale);

	float posX = 0.0f;
	float posY = 0.0f;

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));
	posX = screenOrigin.x + m_pCloseBtn->getSize().width / 2 + 18;
	posY = screenOrigin.y + screenSize.height - m_pCloseBtn->getSize().height / 2 - 16;
	m_pCloseBtn->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pBgChapterTitleImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_chapter_title_img"));
	posX = screenOrigin.x + screenSize.width / 2 - m_pBgChapterTitleImg->getSize().width / 2 - 20;
	posY = screenOrigin.y + screenSize.height - m_pBgChapterTitleImg->getSize().height / 2 - 3;
	m_pBgChapterTitleImg->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pChapterTitleImg = dynamic_cast<UIImageView*>(m_pBgChapterTitleImg->getChildByName("chapter_title_img"));

	m_pTabListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("tab_list"));
	m_pTabListView->addEventListenerListView(this, listvieweventselector(WarMapPanel::listViewEventHandler));
	posX = screenOrigin.x + screenSize.width / 2 + 40;
	posY = screenOrigin.y + screenSize.height - m_pTabListView->getSize().height - 2;
	m_pTabListView->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pWarPageView = dynamic_cast<UIPageView*>(m_pRootLayout->getChildByName("war_page"));
	m_pWarPageView->addEventListenerPageView(this, pagevieweventselector(WarMapPanel::pageViewEventHandler));
	posX = screenOrigin.x + screenSize.width / 2 - m_pWarPageView->getSize().width / 2;
	posY = screenOrigin.y + screenSize.height / 2 - m_pWarPageView->getSize().height / 2;
	m_pWarPageView->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pEliteWarPageView = dynamic_cast<UIPageView*>(m_pRootLayout->getChildByName("elite_war_page"));
	m_pEliteWarPageView->addEventListenerPageView(this, pagevieweventselector(WarMapPanel::pageViewEventHandler));
	posX = screenOrigin.x + screenSize.width / 2 - m_pEliteWarPageView->getSize().width / 2;
	posY = screenOrigin.y + screenSize.height / 2 - m_pEliteWarPageView->getSize().height / 2;
	m_pEliteWarPageView->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pPrevBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("prev_btn"));
	m_pPrevBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));
	posX = screenOrigin.x + m_pPrevBtn->getSize().width / 2 + 32;
	posY = screenOrigin.y + screenSize.height / 2;
	m_pPrevBtn->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pNextBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("next_btn"));
	m_pNextBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));
	posX = screenOrigin.x + screenSize.width - m_pNextBtn->getSize().width / 2 - 32;
	posY = screenOrigin.y + screenSize.height / 2;
	m_pNextBtn->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));
	
	m_pSupplyLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("supply_layout"));
	posX = screenOrigin.x + 32;
	posY = screenOrigin.y + 10;
	m_pSupplyLayout->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pBulletIconImg = dynamic_cast<UIImageView*>(m_pSupplyLayout->getChildByName("bullet_icon_img"));
	m_pBulletIconFrameCell = IconFrameCell::create();
	m_pBulletIconImg->addChild(m_pBulletIconFrameCell);
	m_pBulletIconImg->setEnabled(false);

	m_pBulletCntTxt = dynamic_cast<UILabel*>(m_pSupplyLayout->getChildByName("bullet_cnt_txt"));

	m_pBulletSupplyBtn = dynamic_cast<UIButton*>(m_pSupplyLayout->getChildByName("bullet_supply_btn"));
	m_pBulletSupplyBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));

	m_pMedicineIconImg = dynamic_cast<UIImageView*>(m_pSupplyLayout->getChildByName("medicine_icon_img"));
	m_pMedicineIconFrameCell = IconFrameCell::create();
	m_pMedicineIconImg->addChild(m_pMedicineIconFrameCell);
	m_pMedicineIconImg->setEnabled(false);

	m_pMedicineCntTxt = dynamic_cast<UILabel*>(m_pSupplyLayout->getChildByName("medicine_cnt_txt"));

	m_pMedicineSupplyBtn = dynamic_cast<UIButton*>(m_pSupplyLayout->getChildByName("medicine_supply_btn"));
	m_pMedicineSupplyBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));

	m_pAwardLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("award_layout"));
	posX = screenOrigin.x + screenSize.width - m_pAwardLayout->getSize().width - 32;
	posY = screenOrigin.y + 10;
	m_pAwardLayout->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pStarCntTxt = dynamic_cast<UILabel*>(m_pAwardLayout->getChildByName("star_cnt_txt"));

	m_pStarAwardBtn = dynamic_cast<UIButton*>(m_pAwardLayout->getChildByName("star_award_btn"));
	m_pStarAwardBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));

	m_pEliteActionLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("elite_action_layout"));
	posX = screenOrigin.x + screenSize.width - m_pEliteActionLayout->getSize().width - 32;
	posY = screenOrigin.y + 10;
	m_pEliteActionLayout->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pRemainRefreshTimesValTxt = dynamic_cast<UILabel*>(m_pEliteActionLayout->getChildByName("remain_refresh_times_val_txt"));

	m_pSweepBtn = dynamic_cast<UIButton*>(m_pEliteActionLayout->getChildByName("sweep_btn"));
	m_pSweepBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));

	m_pRefreshBtn = dynamic_cast<UIButton*>(m_pEliteActionLayout->getChildByName("refresh_btn"));
	m_pRefreshBtn->addTouchEventListener(this, toucheventselector(WarMapPanel::touchEventHandler));

	m_pCostLayout = dynamic_cast<UILayout*>(m_pEliteActionLayout->getChildByName("cost_layout"));

	m_pCostTxt = dynamic_cast<UILabel*>(m_pCostLayout->getChildByName("cost_txt"));

	m_bUIInited = true;
	
	//switchTab(WAR_TAB_NORMAL);
	
	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void WarMapPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
				P_WAR_MAP_CTRL->setWarMapID(0);
				P_WAR_MAP_CTRL->setEliteWarMapID(0);
			} else if(pSender == m_pPrevBtn) { // 上一页
				if(m_uWarTab == WAR_TAB_NORMAL) {
					m_pWarPageView->scrollToPage(m_pWarPageView->getCurPageIndex() - 1);
				} else if(m_uWarTab == WAR_TAB_ELITE) {
					m_pElitePageShow = -1;
					m_pEliteWarPageView->scrollToPage(m_pEliteWarPageView->getCurPageIndex() - 1);
				}
			} else if(pSender == m_pNextBtn){ // 下一页
				if(m_uWarTab == WAR_TAB_NORMAL) {
					m_pWarPageView->scrollToPage(m_pWarPageView->getCurPageIndex() + 1);
				} else if(m_uWarTab == WAR_TAB_ELITE) {
					m_pElitePageShow = -1;
					m_pEliteWarPageView->scrollToPage(m_pEliteWarPageView->getCurPageIndex() + 1);
				}
			} else if(pSender == m_pSweepBtn) { // 扫荡
				int nCurPageIdx = m_pEliteWarPageView->getCurPageIndex();
				EliteWarPage *pCurPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(nCurPageIdx));
				
				if(pCurPage != NULL) {
					unsigned int uChapterID = pCurPage->getEliteWarChapterID();
					P_FIGHT_CTRL->getFightProxy()->sendSweepEliteWarMapRqst(uChapterID);
				}
			} else if(pSender == m_pRefreshBtn) { // 刷新
				const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();

				if(stEliteWarInfo.remainresetcnt() > 0) {
					if(stEliteWarInfo.resetcost() > 0) {
						P_TIP_CTRL->GoldCostTip(stEliteWarInfo.resetcost(), this, dialogbtneventselector(WarMapPanel::onEliteWarRefreshConfirm));
					} else {
						COMMON_DIALOG->show(TextStr(TEXT_SURE_TO_REFRESH), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(WarMapPanel::onEliteWarRefreshConfirm));
					}
				} else {
					P_TIP_CTRL->getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_ELITEGOLDRESET);
				}

			} else if(pSender == m_pBulletSupplyBtn) { // 补充子弹
				P_BAG_CTRL->getBuyItemPanel()->open();
				P_BAG_CTRL->getBuyItemPanel()->setCfgID(vmsg::ITEM_ARMO_ID);
			} else if(pSender == m_pMedicineSupplyBtn) { // 补充药品
				P_BAG_CTRL->getBuyItemPanel()->open();
				P_BAG_CTRL->getBuyItemPanel()->setCfgID(ConstHelper(FIGHT_MEDICAL_CFG_DI));
			} else if(pSender == m_pStarAwardBtn) { // 领取奖励
				int nCurPageIdx = m_pWarPageView->getCurPageIndex();
				unsigned int uWarID = nCurPageIdx + 1;
				unsigned int uTotalStarCnt = 0;

				if(P_LEVEL_OBJ->getMapInfo().warstarawards_size() > nCurPageIdx) {
					const vmsg::CSWarStarAward &stStarAward = P_LEVEL_OBJ->getMapInfo().warstarawards(nCurPageIdx);
					uTotalStarCnt = stStarAward.totalstarcnt();
				}

				if(m_uAwardStarCnt != 0 && uTotalStarCnt >= m_uAwardStarCnt) { // 有可领取的
					showLoading();
					P_WAR_MAP_CTRL->sendGetStarAwardRqst(uWarID, m_uAwardStarCnt);
				} else if(m_uNextStarCnt > 0) {
					char text[100];
					snprintf(text, 100, TextStr(TEXT_STAR_CNT_UNREACHED), m_uNextStarCnt);
					if(P_LEVEL_OBJ->getMapInfo().warstarawards_size() > nCurPageIdx) {
						const vmsg::CSWarStarAward stStarAward = P_LEVEL_OBJ->getMapInfo().warstarawards(nCurPageIdx);
						//COMMON_DIALOG->show(text, this, stStarAward.starawards(0).staraward());
						P_WAR_MAP_CTRL->getStarAwardPanel()->show(text, stStarAward.starawards(0).staraward());
					}
					
				}
			}
			break;
		default:;
	}
}

void WarMapPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(pSender == m_pTabListView) {
				int nSelectedIdx = m_pTabListView->getCurSelectedIndex();
				// tab和idx是对应的
				switch(nSelectedIdx) {
					case WAR_TAB_NORMAL:
						P_WAR_MAP_CTRL->setWarMapID(0);
						P_WAR_MAP_CTRL->setEliteWarMapID(0);
						switchTab(WAR_TAB_NORMAL);
						break;
					case WAR_TAB_ELITE:
						P_WAR_MAP_CTRL->setWarMapID(0);
						P_WAR_MAP_CTRL->setEliteWarMapID(0);
						switchTab(WAR_TAB_ELITE);
						break;
					default:;
				}
			}
			break;
		default:;
	}
}

void WarMapPanel::pageViewEventHandler(CCObject *pSender, PageViewEventType type) {
	switch(type) {
		case PAGEVIEW_EVENT_TURNING:
			updatePageStat();
			handlePageTurningLogic();
			break;
		default:;
	}
}

void WarMapPanel::updatePageStat() {
	if(m_uWarTab == WAR_TAB_NORMAL) {
		int nTotalPages = m_pWarPageView->getPages()->count();
		int nCurPageIdx = m_pWarPageView->getCurPageIndex();

		m_pPrevBtn->setVisible(nCurPageIdx > 0);
		m_pPrevBtn->setTouchEnabled(nCurPageIdx > 0);

		m_pNextBtn->setVisible(nCurPageIdx < nTotalPages - 1);
		m_pNextBtn->setTouchEnabled(nCurPageIdx < nTotalPages - 1);

		for(int i = 0; i < nTotalPages; i++) {
			WarPage *pPage = dynamic_cast<WarPage*>(m_pWarPageView->getPage(i));
			pPage->setEnabled(abs(i - nCurPageIdx) <= 1);
		}

		WarPage *pCurPage = dynamic_cast<WarPage*>(m_pWarPageView->getPage(nCurPageIdx));

		if(pCurPage != NULL) {
			unsigned int uChapterID = pCurPage->getChapterID();
			string url = "war_map/text_chapter_" + intToString(uChapterID) + ".png";
			m_pChapterTitleImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
		}
	} else if(m_uWarTab == WAR_TAB_ELITE) {
		int nTotalPages = m_pEliteWarPageView->getPages()->count();
		int nCurPageIdx = m_pEliteWarPageView->getCurPageIndex();

		m_pPrevBtn->setVisible(nCurPageIdx > 0);
		m_pPrevBtn->setTouchEnabled(nCurPageIdx > 0);

		m_pNextBtn->setVisible(nCurPageIdx < nTotalPages - 1);
		m_pNextBtn->setTouchEnabled(nCurPageIdx < nTotalPages - 1);

		for(int i = 0; i < nTotalPages; i++) {
			EliteWarPage *pPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(i));
			pPage->setEnabled(abs(i - nCurPageIdx) <= 1);
		}

		EliteWarPage *pCurPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(nCurPageIdx));

		if(pCurPage != NULL) {
			unsigned int uChapterID = pCurPage->getEliteWarChapterID();
			string url = "war_map/text_chapter_" + intToString(uChapterID) + ".png";
			m_pChapterTitleImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);

			const vmsg::CSEliteWarInfo &stEliteWarInfo = P_LEVEL_OBJ->getEliteWarInfo();
			unsigned int uFightMapSize = stEliteWarInfo.fightmapids_size();
			const vector<EliteWarMapCfg*> &stChapter = ELITE_WAR_MAP_CFG_MASTER->getEliteWarChapter(uChapterID);

			m_pRemainRefreshTimesValTxt->setText(intToString(stEliteWarInfo.remainresetcnt()));

			if(stEliteWarInfo.remainresetcnt() > 0){
				if(stEliteWarInfo.resetcost() > 0){
					m_pCostLayout->setVisible(true);
					m_pCostTxt->setText(intToString(stEliteWarInfo.resetcost()));
				}else{
					m_pCostLayout->setVisible(false);
				}
			}else{
				m_pCostLayout->setVisible(false);
			}

			unsigned int uMaxEliteWarMapID = P_LEVEL_OBJ->getMaxEliteWarMapID();

			m_pSweepBtn->setVisible(uFightMapSize < stChapter.size() && uMaxEliteWarMapID >= stChapter.size() * uChapterID); // 本章还没打完则不能扫荡
			m_pSweepBtn->setTouchEnabled(m_pSweepBtn->isVisible());
		}
	}
}

void WarMapPanel::handlePageTurningLogic() {
	if(m_uWarTab == WAR_TAB_NORMAL) {
		int nCurPageIdx = m_pWarPageView->getCurPageIndex();
		unsigned int uWarID = nCurPageIdx + 1;
		
		// reset
		m_uAwardStarCnt = 0;
		m_uNextStarCnt = 0;
		m_pStarAwardBtn->setVisible(false);
		m_pStarAwardBtn->setTouchEnabled(false);

		if(P_LEVEL_OBJ->getMapInfo().warstarawards_size() > nCurPageIdx) {
			const vmsg::CSWarStarAward &stStarAward = P_LEVEL_OBJ->getMapInfo().warstarawards(nCurPageIdx);

			for(int i = 0; i < stStarAward.starawards_size(); i++) {
				const vmsg::CSStarAwardElmt &stStarAwardElmt = stStarAward.starawards(i);
				
				// 未达成条件
				if(stStarAward.totalstarcnt() < stStarAwardElmt.starcnt()) {
					m_uNextStarCnt = stStarAwardElmt.starcnt();
					break;
				} else { // 已达成
					if(stStarAwardElmt.claimstatus() == 0) { // 没有领奖
						m_uAwardStarCnt = stStarAwardElmt.starcnt();
					}
				}
			}

			m_pStarCntTxt->setText(string("x") + intToString(stStarAward.totalstarcnt()));
			m_pStarAwardBtn->setVisible(m_uAwardStarCnt != 0 || m_uNextStarCnt != 0);
			m_pStarAwardBtn->setTouchEnabled(m_uAwardStarCnt != 0 || m_uNextStarCnt != 0);
		} else {
			m_pStarCntTxt->setText(string("x") + intToString(0));
		}

	} else if(m_uWarTab == WAR_TAB_ELITE) {
		int nCurPageIdx = m_pEliteWarPageView->getCurPageIndex();
		EliteWarPage *pCurPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(nCurPageIdx));
		
		if(pCurPage != NULL) {
			P_FIGHT_CTRL->getFightProxy()->sendQueryEliteWarMapRqst(pCurPage->getEliteWarChapterID());
		}
	}
}

void WarMapPanel::onEliteWarRefreshConfirm() {
	int nCurPageIdx = m_pEliteWarPageView->getCurPageIndex();
	EliteWarPage *pCurPage = dynamic_cast<EliteWarPage*>(m_pEliteWarPageView->getPage(nCurPageIdx));

	if(pCurPage != NULL) {
		unsigned int uChapterID = pCurPage->getEliteWarChapterID();
		P_FIGHT_CTRL->getFightProxy()->sendResetEliteWarMapRqst(uChapterID);
	}
}
