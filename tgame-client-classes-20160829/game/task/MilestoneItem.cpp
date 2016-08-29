#include "MilestoneItem.h"

#include "defines.h"
#include "Resources.h"

#include "CfgMaster/DailyTaskCfgMaster.h"
#include "CfgMaster/DailyTaskAwardCfgMaster.h"
#include "CfgMaster/MilestoneCfgMaster.h"
#include "control/AudioManager.h"

#include "ClientSvrMsg.pb.h"

#include "basic/CCColorUtil.h"
#include "basic/NounHelper.h"

#include "game/UserObj.h"

#include "TaskControl.h"

MilestoneItem::MilestoneItem()
:m_pIconImg(NULL)
,m_pTaskNameTxt(NULL)
,m_pTaskDescTxt(NULL)
,m_pTaskIntroTxt(NULL)
,m_pGoldValTxt(NULL)
,m_pCoinValTxt(NULL)
,m_pMilestoneProgress(NULL)
,m_pProgressValTxt(NULL)
,m_pStampGetImg(NULL)
,m_uTaskID(0)
,m_uTaskStat(0)
{
}

MilestoneItem::~MilestoneItem()
{
}

MilestoneItem* MilestoneItem::create() {
	MilestoneItem *pRet = new MilestoneItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool MilestoneItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/MilestoneItemUI.ExportJson");
		addChild(m_pRootLayout);

		setName("milestone_item");
		m_pIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_img"));

		m_pTaskNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("task_name_txt"));

		m_pTaskDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("task_desc_txt"));

		//m_pTaskIntroTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("task_intro_txt"));

		m_pGoldValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("gold_val_txt"));

		m_pCoinValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_val_txt"));

		m_pMilestoneProgress = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("milestone_progress"));

		m_pProgressValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("progress_val_txt"));

		m_pStampGetImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("stamp_get_img"));
		
		bRet = true;
	} while(0);

	return bRet;
}

void MilestoneItem::updateUI(const vmsg::CSTask& stTask) {
	m_uTaskID = stTask.taskid();
	m_uTaskStat = stTask.taskstat();

	MilestoneCfg *pMilestoneCfg = MILESTONE_CFG_MASTER->getMilestoneCfgByID(stTask.taskid());
    if(NULL == pMilestoneCfg) {
        CCLOG("[error]MilestoneItem::updateUI,fail to get MilestoneCfg, taskid=%d", stTask.taskid());
        return;
    }
	std::string url = ICON_PATH + pMilestoneCfg->milestoneicon() + ".png";
	m_pIconImg->loadTexture(url.c_str());
	m_pTaskNameTxt->setText(pMilestoneCfg->milestonename());
	m_pTaskDescTxt->setText(pMilestoneCfg->milestonedesc());
	//m_pTaskIntroTxt->setText(pMilestoneCfg->milestonedesc());

	m_pGoldValTxt->setText(intToString(pMilestoneCfg->award().gold()));
	m_pCoinValTxt->setText(intToString(pMilestoneCfg->award().coin()));

	bool isTaskFinished = false;
	bool isTaskOver = false;

	int percent = stTask.r1() < pMilestoneCfg->target().r1() ? stTask.r1() * 100 / pMilestoneCfg->target().r1() : 100;
	m_pMilestoneProgress->setPercent(percent);
	m_pProgressValTxt->setText(intToString(percent) + "%");

	isTaskFinished = stTask.r1() >= pMilestoneCfg->target().r1() && stTask.taskstat() == vmsg::TASK_STAT_FINISH;
	isTaskOver = stTask.r1() >= pMilestoneCfg->target().r1() && stTask.taskstat() == vmsg::TASK_STAT_OVER;

	m_pStampGetImg->setVisible(isTaskFinished);
	
}

bool MilestoneItem::isTaskFinished() {
	std::string text = m_pProgressValTxt->getStringValue();

	std::string::size_type pos = text.find("/");

	return std::atoi(text.substr(0, pos).c_str()) >= std::atoi(text.substr(pos + 1, text.size() - pos - 1).c_str());
}

unsigned int MilestoneItem::getTaskID() {
	return m_uTaskID;
}

unsigned int MilestoneItem::getTaskStat() {
	return m_uTaskStat;
}


void MilestoneItem::onTouch() {
    if(m_uTaskStat == vmsg::TASK_STAT_FINISH) {
		string url = SOUND_RES_PATH + "ut00028.ogg";
		AudioManager::preloadEffect(url.c_str());
		AudioManager::playEffect("ut00028.ogg");
        P_TASK_CTRL->sendCommitMilestoneRqst(m_uTaskID);
    }
}

