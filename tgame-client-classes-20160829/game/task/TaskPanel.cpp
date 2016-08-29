#include "TaskPanel.h"

#include "defines.h"

#include "vlib/base/CVTime.h"

#include "ModuleID.pb.h"

#include "CfgMaster/TaskCfgMaster.h"
#include "CfgMaster/NPCCfgMaster.h"
#include "CfgMaster/DailyTaskCfgMaster.h"
#include "CfgMaster/MilestoneCfgMaster.h"

#include "basic/TimeEx.h"
#include "basic/ToolUtils.h"
#include "basic/TextHelper.h"

#include "game/UserObj.h"
#include "game/city/CityControl.h"
#include "game/IconCell.h"
#include "game/control/AudioManager.h"
#include "game/control/PanelSoundType.h"
#include "game/Global.h"
#include "game/warmap/WarMapControl.h"

#include "TaskControl.h"
#include "DailyTaskItem.h"
#include "MilestoneItem.h"

#define TASK_ITEM_TAG 87
#define BADGE_TAG 101

TaskPanel::TaskPanel()
:m_pBackBtn(NULL)
,m_pTabListView(NULL)
,m_pTaskLayout(NULL)
,m_pTaskNameTxt(NULL)
,m_pTaskDescTxt(NULL)
,m_pRecvNPCNameValTxt(NULL)
,m_pCommitNPCNameValTxt(NULL)
,m_pCoinValTxt(NULL)
,m_pExpValTxt(NULL)
,m_pBgAwardImg(NULL)
,m_pTraceBtn(NULL)
,m_pDailyTaskListView(NULL)
,m_pMilestoneListView(NULL)
,m_uTabType(TAB_DAILYTASK)
,m_nTabSelectedIdx(0)
,m_pTableView(NULL)
{
}

TaskPanel::~TaskPanel()
{
}

TaskPanel* TaskPanel::create() {
	TaskPanel *pRet = new TaskPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool TaskPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void TaskPanel::open() {
    //neil add for debug
    /*CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCDataReaderHelper::sharedDataReaderHelper()->dumpAllCfgFile();
    AudioManager::dumpAllAudio();*/

	BasePanel::open();

	P_TASK_CTRL->sendQueryDailyTaskRqst();
	P_TASK_CTRL->sendQueryMilestoneRqst();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}
}

void TaskPanel::switchTab(unsigned int uTabType) {
	if(uTabType == TAB_DAILYTASK
	|| uTabType == TAB_MILESTONE
	|| uTabType == TAB_MAINTASK) {
		m_uTabType = uTabType;

		UIButton *pTabBtn = NULL;

		if(m_nTabSelectedIdx >= 0) {
			pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_nTabSelectedIdx));
			pTabBtn->setBrightStyle(BRIGHT_NORMAL);
			pTabBtn->setTouchEnabled(true);
			pTabBtn->getTextVirtualRenderer()->setScale(1.0f);
			pTabBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
		}

		pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_uTabType));
		pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
		pTabBtn->setTouchEnabled(false);
		pTabBtn->getTextVirtualRenderer()->setScale(1.2f);
		pTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));
		m_nTabSelectedIdx = m_uTabType;

		m_pTaskLayout->setEnabled(m_uTabType == TAB_MAINTASK);
		m_pDailyTaskListView->setEnabled(m_uTabType == TAB_DAILYTASK);
		m_pTableView->setVisible(m_uTabType == TAB_MILESTONE);
		
		
	}
}

void TaskPanel::badgeTabBtn(unsigned int uTabType, unsigned int uBadgeNum) {
	UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(uTabType));

	if(pTabBtn != NULL) {
		UIImageView *pBadgeImg = dynamic_cast<UIImageView*>(pTabBtn->getChildByTag(BADGE_TAG));

		if(uBadgeNum > 0) {
			if(pBadgeImg == NULL) {
				pBadgeImg = UIImageView::create();
				pBadgeImg->loadTexture("share/badge_x.png", UI_TEX_TYPE_PLIST);
				CCSize btnSize = pTabBtn->getSize();
				pBadgeImg->setPosition(ccp(btnSize.width / 3, btnSize.height / 3));
				pTabBtn->addChild(pBadgeImg, 3, BADGE_TAG);
			}
		} else {
			if(pBadgeImg != NULL) {
				pBadgeImg->removeFromParent();
			}
		}
	}
}

void TaskPanel::updateTask() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	// load task
	unsigned int level = P_USER_OBJ->getUserInfo()->level;
	unsigned int uTaskID = P_TASK_CTRL->getCurTask().taskid();
	showTask(uTaskID);

	closeLoading();
}

void TaskPanel::showTask(unsigned int uTaskID) {
	const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(uTaskID);

	if(pTaskCfg != NULL) {
		m_pTaskNameTxt->setText(pTaskCfg->taskname());
		m_pTaskDescTxt->setText(pTaskCfg->taskdesc());
		m_pRecvNPCNameValTxt->setText(NPC_CFG_MASTER->getNPCCfgByID(pTaskCfg->getnpcid())->npcname());
		m_pCommitNPCNameValTxt->setText(NPC_CFG_MASTER->getNPCCfgByID(pTaskCfg->commitnpcid())->npcname());
		m_pCoinValTxt->setText(intToString(pTaskCfg->award().coin()));
		m_pExpValTxt->setText(intToString(pTaskCfg->award().exp()));

		IconCell *pAwardCell = dynamic_cast<IconCell*>(m_pBgAwardImg->getChildByName("award_cell"));
		pAwardCell->showNumTxt(pTaskCfg->award().awardcnt() > 1);
		pAwardCell->setInfo(pTaskCfg->award().awardtype(), pTaskCfg->award().awardcnt(), pTaskCfg->award().awardcfgid(), ccc3(0x00, 0xff, 0x00));
		CToolUtils::adjustIconBgSize(pTaskCfg->award().awardtype(), pTaskCfg->award().awardcfgid(), m_pBgAwardImg);
		CToolUtils::adjustFragIcon(pTaskCfg->award().awardtype(),  m_pBgAwardImg);
		pAwardCell->setPositionX(m_pBgAwardImg->getSize().width / 2);
	} else {
		m_pTaskNameTxt->setText(TextStr(TEXT_NONE_VALID_TASK));
		m_pTaskDescTxt->setText("");
		m_pRecvNPCNameValTxt->setText("");
		m_pCommitNPCNameValTxt->setText("");
	}
}

void TaskPanel::updateDailyTask() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	m_pDailyTaskListView->removeAllItems();
	m_pDailyTaskListView->setVisible(true);

	unsigned int uTotalAwardCnt = 0;

	// month card
	/*const vmsg::CSMonthCardInfo &stMonthCardInfo = P_USER_OBJ->getUserInfo()->stMonthCardInfo;
	unsigned int uSvrTime = P_GLOBAL->getSvrTime();
	unsigned int uEndTime = stMonthCardInfo.endtime();
	unsigned int uAwardTime = stMonthCardInfo.lastawardtime();

	if(uEndTime > uSvrTime && !CVTimeUtils::IsSameDay(uAwardTime, uSvrTime)) { // 月卡没过期且今天还没领奖
		DailyTaskItem *pTaskItem = DailyTaskItem::create();
		pTaskItem->updateUI();
		m_pDailyTaskListView->pushBackCustomItem(pTaskItem);
		uTotalAwardCnt++;
	}*/

	const vmsg::CSDailyTaskInfo &stDailyTaskInfo = P_TASK_CTRL->getDailyTaskInfo();
	vector<vmsg::CSTask> stDailyTasks;

	for(int i = 0; i < stDailyTaskInfo.dailytasks_size(); i++) {
		const vmsg::CSTask &stTask = stDailyTaskInfo.dailytasks(i);

		if(stTask.taskstat() != vmsg::TASK_STAT_OVER) { // 已结束的不显示
			if(stTask.taskstat() == vmsg::TASK_STAT_FINISH) { // 已完成的排在前面
				DailyTaskCfg *pDailyTaskCfg = DAILY_TASK_CFG_MASTER->getDailyTaskCfgByID(stTask.taskid());
				
				if(pDailyTaskCfg != NULL) {
					DailyTaskItem *pTaskItem = DailyTaskItem::create();
					pTaskItem->updateUI(stTask);
					m_pDailyTaskListView->pushBackCustomItem(pTaskItem);
					uTotalAwardCnt++;
				} else {
					CCLOG("Error! Not found the daily task in DailyTaskCfg. taskid=%u", stTask.taskid());
				}
			} else {
				stDailyTasks.push_back(stTask);
			}
		}
	}

	/*if(uEndTime <= uSvrTime) { // 月卡已过期
		DailyTaskItem *pTaskItem = DailyTaskItem::create();
		pTaskItem->updateUI();
		m_pDailyTaskListView->pushBackCustomItem(pTaskItem);
	}*/
	//TEST
	/*
	vmsg::CSTask stTask_add;
	stTask_add.set_taskid(21);
	stTask_add.set_taskstat(vmsg::TASK_STAT_RECEIVED);
	stTask_add.set_r1(1);
	stDailyTasks.push_back(stTask_add);
	*/
	
	for(unsigned int i = 0; i < stDailyTasks.size(); i++) {
		const vmsg::CSTask &stTask = stDailyTasks[i];
		DailyTaskCfg *pDailyTaskCfg = DAILY_TASK_CFG_MASTER->getDailyTaskCfgByID(stTask.taskid());

		if(pDailyTaskCfg != NULL) {
			bool isModuleOpen = false;

			switch(pDailyTaskCfg->systype()) {
				case vmsg::DAILY_SYS_WARMAP:
					isModuleOpen = true;
					break;
				case vmsg::DAILY_SYS_INFINIT:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_NOTHINGLAND);
					break;
				case vmsg::DAILY_SYS_EQUIP:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_EQUIPLVUP);
					break;
				case vmsg::DAILY_SYS_WEAPON:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_WEAPONLIB);
					break;
				case vmsg::DAILY_SYS_FRIEND:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_FRIEND);
					break;
				case vmsg::DAILY_SYS_GOLD:
					isModuleOpen = true;
					break;
				case vmsg::DAILY_SYS_GUILD:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_GUILD);
					break;
				case vmsg::DAILY_SYS_ATKDEF:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_ATTRSTRENGTHEN);
					break;
				case vmsg::DAILY_SYS_EXPLORE:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_LOTTERY);
					break;
				case vmsg::DAILY_SYS_ARENA:
					isModuleOpen = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_ARENA);
					break;
				case vmsg::DAILY_SYS_ELITE:
					isModuleOpen = P_WAR_MAP_CTRL->isEliteWarOpen();
					break;
				case vmsg::DAILY_SYS_SHARE:
					isModuleOpen = true;
					break;
				default:;
			}

			if(isModuleOpen) {
				DailyTaskItem *pTaskItem = DailyTaskItem::create();
				pTaskItem->updateUI(stTask);
				m_pDailyTaskListView->pushBackCustomItem(pTaskItem);
			}
		} else {
			CCLOG("Error! Not found the daily task in DailyTaskCfg. taskid=%u", stTask.taskid());
		}
	}

	UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(TAB_DAILYTASK));
	float offsetX = pTabBtn->getSize().width * 0.36f;
	float offsetY = pTabBtn->getSize().height * 0.18f;
	CToolUtils::badgeWidget(pTabBtn, uTotalAwardCnt, offsetX, offsetY);

	this->scheduleOnce(schedule_selector(TaskPanel::scheduleGuideShow), 0.5);
	closeLoading();
}

void TaskPanel::scheduleGuideShow(float dt)
{
	GUIDE_SHOW(GUIDE_UI_DAILYTASK);
}

void TaskPanel::updateMilestone() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	m_stTaskLst.clear();
	const vmsg::CSMilestoneInfo &stMilestoneInfo = P_TASK_CTRL->getMilestoneInfo();
	vector<vmsg::CSTask> stMilestones;
	unsigned int uTotalAwardCnt = 0;

	for(int i = 0; i < stMilestoneInfo.milestones_size(); i++) {
		const vmsg::CSTask &stTask = stMilestoneInfo.milestones(i);

		if(stTask.taskstat() != vmsg::TASK_STAT_OVER) { // 已结束的不显示
			if(stTask.taskstat() == vmsg::TASK_STAT_FINISH) { // 已完成的排在前面
				MilestoneCfg *pMilestoneCfg = MILESTONE_CFG_MASTER->getMilestoneCfgByID(stTask.taskid());

				if(pMilestoneCfg != NULL){
					m_stTaskLst.push_back(stTask);
					uTotalAwardCnt++;
				} else {
					CCLOG("[error]MilestonePanel::updateUI,Not found the milestone in MilestoneCfg. taskid=%u", stTask.taskid());
				}
			} else {
				stMilestones.push_back(stTask);
			}
		}
	}

	for(unsigned int i = 0; i < stMilestones.size(); i++) {
		const vmsg::CSTask &stTask = stMilestones[i];
		MilestoneCfg *pMilestoneCfg = MILESTONE_CFG_MASTER->getMilestoneCfgByID(stTask.taskid());
		if(pMilestoneCfg != NULL) {
			m_stTaskLst.push_back(stTask);
		} else {
			CCLOG("[error]MilestonePanel::updateUI,Not found the milestone in MilestoneCfg. taskid=%u", stTask.taskid());
		}
	}

	UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(TAB_MILESTONE));
	float offsetX = pTabBtn->getSize().width * 0.36f;
	float offsetY = pTabBtn->getSize().height * 0.18f;
	CToolUtils::badgeWidget(pTabBtn, uTotalAwardCnt, offsetX, offsetY);

	m_pTableView->reloadData();
	closeLoading();
}

CCSize TaskPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 144);
}

unsigned int TaskPanel::numberOfCellsInTableView(CCTableView *table){
	return m_stTaskLst.size();
}

CCTableViewCell* TaskPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){

	CCTableViewCell* cell = table->dequeueCell();
	MilestoneItem *pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = MilestoneItem::create();
		pItem->setPosition(ccp(0, 12));
		cell->addChild(pItem, 0, TASK_ITEM_TAG);
	}else{
		pItem = dynamic_cast<MilestoneItem*>(cell->getChildByTag(TASK_ITEM_TAG));
	}


	if(idx < m_stTaskLst.size()) {
		pItem->updateUI(m_stTaskLst[idx]);
	}

	return cell;
}

void TaskPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){

	MilestoneItem *pItem = dynamic_cast<MilestoneItem*>(cell->getChildByTag(TASK_ITEM_TAG));
	if(NULL == pItem) {
		return;
	}
	pItem->onTouch();

}

void TaskPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/TaskPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_uSoundType = TASK_PANEL_TYPE;

	m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(TaskPanel::touchEventHandler));

	m_pTabListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("tab_list"));
	m_pTabListView->addEventListenerListView(this, listvieweventselector(TaskPanel::listViewEventHandler));

	UILayout *pTaskLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("task_layout"));
	m_pTaskLayout = pTaskLayout;

	m_pTaskNameTxt = dynamic_cast<UILabel*>(pTaskLayout->getChildByName("task_name_txt"));

	m_pTaskDescTxt = dynamic_cast<UILabel*>(pTaskLayout->getChildByName("task_desc_txt"));

	m_pRecvNPCNameValTxt = dynamic_cast<UILabel*>(pTaskLayout->getChildByName("recv_npc_txt"));

	m_pCommitNPCNameValTxt = dynamic_cast<UILabel*>(pTaskLayout->getChildByName("commit_npc_txt"));

	m_pCoinValTxt = dynamic_cast<UILabel*>(pTaskLayout->getChildByName("coin_val_txt"));

	m_pExpValTxt = dynamic_cast<UILabel*>(pTaskLayout->getChildByName("exp_val_txt"));

	m_pBgAwardImg = dynamic_cast<UIImageView*>(pTaskLayout->getChildByName("award_item_img"));

	IconCell *pCell = IconCell::create();
	pCell->setName("award_cell");
	pCell->showNumTxt();
	pCell->setPositionX(m_pBgAwardImg->getSize().width / 2);
	m_pBgAwardImg->addChild(pCell);

	m_pTraceBtn = dynamic_cast<UIButton*>(pTaskLayout->getChildByName("trace_btn"));
	m_pTraceBtn->addTouchEventListener(this, toucheventselector(TaskPanel::touchEventHandler));

	m_pDailyTaskListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("daily_task_list"));
	m_pDailyTaskListView->addEventListenerListView(this, listvieweventselector(TaskPanel::listViewEventHandler));

	m_pMilestoneListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("milestone_list"));
	m_pMilestoneListView->addEventListenerListView(this, listvieweventselector(TaskPanel::listViewEventHandler));
	m_pMilestoneListView->setEnabled(false);

	m_pTableView = CCTableView::create(this, CCSizeMake(720, 420));
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(m_pMilestoneListView->getPosition());
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pRootLayout->addNode(m_pTableView);

	switchTab(m_uTabType);
	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		
		if(m_uTabType == TAB_MAINTASK) {
			updateTask();
		} else if(m_uTabType == TAB_DAILYTASK) {
			updateDailyTask();
		} else if(m_uTabType == TAB_MILESTONE) {
			updateMilestone();
		}
	}
	
}

void TaskPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				close();
			} else if(std::strcmp(pszWidgetName, m_pTraceBtn->getName()) == 0) {
				close();
				P_TASK_CTRL->autoTask();
			}
			break;
		default:;
	}
}

void TaskPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(strcmp(pszListViewName, m_pTabListView->getName()) == 0) {
				int nSelectedIdx = m_pTabListView->getCurSelectedIndex();
				switchTab(nSelectedIdx);

				if(m_uTabType == TAB_MAINTASK) {
					updateTask();
				} else if(m_uTabType == TAB_DAILYTASK) {
					updateDailyTask();
				} else if(m_uTabType == TAB_MILESTONE) {
					updateMilestone();
				}
			}
			break;
		default:;
	}
}

CCNode* TaskPanel::getGuideNodeByName(const std::string &name)
{
	CCNode* pRet = NULL;
	if (name == "daily_task_get_btn")
	{
		pRet = dynamic_cast<DailyTaskItem*>( m_pDailyTaskListView->getItem(0))->getGoToButton();
	}
	return pRet;
}

