#include "TaskControl.h"

#include "Macros.pb.h"

#include "CfgMaster/DailyTaskCfgMaster.h"
#include "CfgMaster/TaskCfgMaster.h"
#include "CfgMaster/DailyTaskAwardCfgMaster.h"
#include "CfgMaster/MilestoneCfgMaster.h"

#include "basic/PopTipLayer.h"
#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "basic/NounHelper.h"

#include "game/Global.h"
#include "game/UserObj.h"
#include "game/city/CityControl.h"
#include "game/bag/BagControl.h"
#include "game/city/CityScreenView.h"
#include "evtlog/EvtLog.h"
#include "TaskPanel.h"
#include "game/notification/NotificationControl.h"
#include "game/tip/TipControl.h"
#include "game/warmap/WarMapControl.h"

TaskControl::TaskControl()
:m_bInitialized(false)
,m_pTaskPanel(NULL)
,m_bRecvTaskReceived(true)
,m_bCommitTaskReceived(true)
{
    
}

TaskControl::~TaskControl()
{
	finalize();
}

void TaskControl::init() {
	if(!m_bInitialized) {
		m_pTaskPanel = TaskPanel::create();
		CC_SAFE_RETAIN(m_pTaskPanel);

		m_uCommitTaskID = 0;
		m_uCommitDailyTaskID = 0;
		m_uCommitMilestoneTaskID = 0;

		m_bInitialized = true;
	}
}

void TaskControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pTaskPanel);

		m_bInitialized = false;
	}
}

TaskPanel* TaskControl::getTaskPanel() {
	return m_pTaskPanel;
}

const vmsg::CSTask& TaskControl::getCurTask() {
	return m_stTaskObj.getCurTask();
}

const vmsg::CSDailyTaskInfo& TaskControl::getDailyTaskInfo() {
	return m_stTaskObj.getDailyTaskInfo();
}

const vmsg::CSMilestoneInfo& TaskControl::getMilestoneInfo() {
	return m_stTaskObj.getMilestoneInfo();
}

unsigned int TaskControl::getMaxTaskID() {
	const TaskCfgSet &stTaskCfgSet = TASK_CFG_MASTER->GetCfg();
	int nTaskCfgSize = stTaskCfgSet.taskcfg_size();

	if(nTaskCfgSize > 0) {
		return stTaskCfgSet.taskcfg(nTaskCfgSize - 1).taskid();
	} else {
		return 0;
	}
}

void TaskControl::autoTask() {
	if(P_GUIDE_CTRL->isGuideDuring() && P_GUIDE_CTRL->getGuideID() != GUIDE_ID_FIRST_TASK) {
		CCLOG("auto task guide retun");
		return;
	}

	const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

	if(stTask.has_taskid() && TASK_CFG_MASTER->isValid(stTask.taskid())) {
		const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(stTask.taskid());

		unsigned int uTaskStat = stTask.taskstat();
		unsigned int uTaskTargetType = pTaskCfg->tasktarget().targettype();
		
		if(uTaskTargetType == vmsg::TASK_TARGET_KILL_MONSTER) { // 杀怪

		} else if(uTaskTargetType == vmsg::TASK_TARGET_CLEAR_WAR) { // 通关副本
			unsigned int uPassID = pTaskCfg->tasktarget().r1();

			if(uTaskStat == vmsg::TASK_STAT_ORG) {
				if(P_USER_OBJ->getUserInfo()->level >= pTaskCfg->level()) {
					unsigned int uNPCID = pTaskCfg->getnpcid();
					P_CITY_CTRL->moveToNPC(uNPCID);

                    // first task
                    if(stTask.taskid() == TASK_CFG_MASTER->GetCfg().taskcfg(0).taskid()) {
                        ADD_EVT_LOG(EVT_LOG_GO_NPC_GET_TASK_FIRST);
                    }
				} else {
					P_TIP_CTRL->getLevelNotEnoughPanel()->openWithLimitLevel(pTaskCfg->level());
				}
			} else if(uTaskStat == vmsg::TASK_STAT_RECEIVED) {
				CCLOG("move to battle");
				P_WAR_MAP_CTRL->getWarMapPanel()->matchTask(true);
				P_CITY_CTRL->moveToBattlePortal();
			} else if(uTaskStat == vmsg::TASK_STAT_FINISH) {
				unsigned int uNPCID = pTaskCfg->commitnpcid();
				P_CITY_CTRL->moveToNPC(uNPCID);

                // first task
                if(stTask.taskid() == TASK_CFG_MASTER->GetCfg().taskcfg(0).taskid()) {
                    ADD_EVT_LOG(EVT_LOG_GO_NPC_COMMIT_TASK_FIRST);
                }
			}
		} else if(uTaskTargetType == vmsg::TASK_TARGET_TALK_NPC) { // npc对话
			if(uTaskStat == vmsg::TASK_STAT_ORG) {
				unsigned int uNPCID = pTaskCfg->getnpcid();
				P_CITY_CTRL->moveToNPC(uNPCID);
			} else if(uTaskStat == vmsg::TASK_STAT_FINISH) {
				unsigned int uNPCID = pTaskCfg->commitnpcid();
				P_CITY_CTRL->moveToNPC(uNPCID);

                // first task
                if(stTask.taskid() == TASK_CFG_MASTER->GetCfg().taskcfg(0).taskid()) {
                    ADD_EVT_LOG(EVT_LOG_GO_NPC_COMMIT_TASK_FIRST);
                }
			}
		}
	}
}

bool TaskControl::isTaskNPC(unsigned int uNPCID) {
	bool bRet = false;

	const vmsg::CSTask &stTask = m_stTaskObj.getCurTask();
	const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(stTask.taskid());

	if(pTaskCfg != NULL) {
		unsigned int uTaskStat = stTask.taskstat();

		if(uTaskStat == vmsg::TASK_STAT_ORG) {
			bRet = uNPCID == pTaskCfg->getnpcid();
		} else if(uTaskStat == vmsg::TASK_STAT_RECEIVED) {
			
		} else if(uTaskStat == vmsg::TASK_STAT_FINISH) {
			bRet = uNPCID == pTaskCfg->commitnpcid();
		}
	}

	return bRet;
}

unsigned int TaskControl::getMaxOpenWarID() {
	unsigned int uWarID = 0;
	const vmsg::CSTask &stTask = getCurTask();

	if(stTask.has_taskid()) {
		unsigned int uTaskID = stTask.taskid();
		unsigned int uTaskStat = stTask.taskstat();
		uTaskID = uTaskID > P_TASK_CTRL->getMaxTaskID() ? P_TASK_CTRL->getMaxTaskID() : uTaskID;

		while(uTaskID > 0) {
			const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(uTaskID);
			unsigned int uTaskTargetType = pTaskCfg->tasktarget().targettype();

			if(uTaskTargetType == vmsg::TASK_TARGET_CLEAR_WAR) {
				if(uTaskID == stTask.taskid() && uTaskStat >= vmsg::TASK_STAT_RECEIVED) {
					uWarID = pTaskCfg->tasktarget().r1();
					break;
				} else if(uTaskID < stTask.taskid()) {
					uWarID = pTaskCfg->tasktarget().r1();
					break;
				}

			}

			uTaskID--;
		}
	}

	return uWarID;
}

void TaskControl::resetOnConnClose() {
    //断线时重置数据
    CCLOG("[trace]TaskControl,reset data on connection close.");
    TaskObj stUserInfo;
    m_stTaskObj = stUserInfo;

    m_uCommitTaskID = 0;
    m_uCommitDailyTaskID = 0;
    m_uCommitMilestoneTaskID = 0;
}

bool TaskControl::hasFinishedDailyTask() {
	bool bRet = false;
	const vmsg::CSDailyTaskInfo &stDailyTaskInfo = m_stTaskObj.getDailyTaskInfo();

	for(int i = 0; i < stDailyTaskInfo.dailytasks_size(); i++) {
		const vmsg::CSTask &stTask = stDailyTaskInfo.dailytasks(i);

		if(stTask.taskstat() == vmsg::TASK_STAT_FINISH) {
			bRet = true;
			break;
		}
	}

	/*const vmsg::CSMonthCardInfo &stMonthCardInfo = P_USER_OBJ->getUserInfo()->stMonthCardInfo;
	unsigned int uSvrTime = P_GLOBAL->getSvrTime();
	unsigned int uEndTime = stMonthCardInfo.endtime();
	unsigned int uAwardTime = stMonthCardInfo.lastawardtime();

	if(uEndTime > uSvrTime && !CVTimeUtils::IsSameDay(uAwardTime, uSvrTime)) { // 月卡没过期且今天还没领奖
		bRet = true;
	}*/

	return bRet;
}

bool TaskControl::hasFinishedMilestone() {
	bool bRet = false;
	const vmsg::CSMilestoneInfo &stMilestoneInfo = m_stTaskObj.getMilestoneInfo();

	for(int i = 0; i < stMilestoneInfo.milestones_size(); i++) {
		const vmsg::CSTask &stTask = stMilestoneInfo.milestones(i);

		if(stTask.taskstat() == vmsg::TASK_STAT_FINISH) {
			bRet = true;
			break;
		}
	}

	return bRet;
}

void TaskControl::notifyMilestoneFinished() {
	if(hasFinishedDailyTask() || hasFinishedMilestone()) {
		PUSH_NOTIFICATION(ENTRY_DAILY_TASK, NOTIFY_TAG_MILESTONE_FINISHED, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_DAILY_TASK, NOTIFY_TAG_MILESTONE_FINISHED, 0);
	}
}

void TaskControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	unsigned int uCmd = stMsg.msg_head().msg_id();

	if(stMsg.has_errmsg()) { //error

	} else if(stMsg.has_playerrespond()) {
		vmsg::CSPlayerRespond stPlayerRsp = stMsg.playerrespond();

		if(uCmd == vmsg::CS_CMD_PLAYER_TALKWITHNPC) { // npc对话

		}
	} else if(stMsg.has_taskrespond()) {
		vmsg::CSTaskRespond stTaskRsp = stMsg.taskrespond();

		if(stTaskRsp.has_receivetask_rsp()) { // 接取任务
            m_stTaskObj.setCurTask(stTaskRsp.receivetask_rsp());
			CCLOG("task recv");
			m_bRecvTaskReceived = true;
			P_GLOBAL->showPopEff("ut00028");
			P_CITY_CTRL->updateTask();
            P_CITY_CTRL->updateNPCTaskState(stTaskRsp.receivetask_rsp().taskid());

            // first task
            if(stTaskRsp.receivetask_rsp().taskid() == TASK_CFG_MASTER->GetCfg().taskcfg(0).taskid()) {
                ADD_EVT_LOG(EVT_LOG_GET_TASK_FIRST);
            }
            P_EVT_LOG->logByLv(EVT_LOG_GET_TASK, intToString(stTaskRsp.receivetask_rsp().taskid() ) );
            if(m_pTaskPanel) {
                m_pTaskPanel->closeLoading();
            }
		} else if(stTaskRsp.has_committask_rsp()) { // 提交任务
            m_stTaskObj.setCurTask(stTaskRsp.committask_rsp());
			m_bCommitTaskReceived = true;
			P_GLOBAL->showPopEff("ut00029");
            P_BAG_CTRL->sendQueryBagInfoRqst();
			P_CITY_CTRL->updateTask();
            P_CITY_CTRL->updateNPCTaskState(stTaskRsp.committask_rsp().taskid());

            P_EVT_LOG->logByLv(EVT_LOG_COMMIT_TASK, intToString(m_uCommitTaskID ) );

            const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(m_uCommitTaskID);
            if(pTaskCfg) {
				POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_EXP, 0, pTaskCfg->award().exp());
                //POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_COIN, 0, pTaskCfg->award().coin());
                if(vmsg::THING_TYPE_WEAPON == pTaskCfg->award().awardtype()) {
                    P_GLOBAL->showAdvItemEff(pTaskCfg->award().awardtype(), pTaskCfg->award().awardcfgid() ); //显示高级物品获得特效
                }
                POP_TIP_LAYER_MASTER->ShowAwardTip(pTaskCfg->award().awardtype(), 
                    pTaskCfg->award().awardcfgid(), pTaskCfg->award().awardcnt());
                
				bool isAutoTask = !P_CITY_CTRL->checkModuleOpen(m_uCommitTaskID);

				if(P_GUIDE_CTRL->isTaskTriggerGuide(m_uCommitTaskID)) {
					isAutoTask = false;
					P_CITY_CTRL->getCityScreenView()->updateEntry();
					P_CITY_CTRL->getCityScreenView()->unfoldEntry();
					GUIDE_TRIGGER(GUIDE_TRIGGER_TASK, m_uCommitTaskID);
				}

				if(isAutoTask) {
					autoTask();
				}
            }
            m_uCommitTaskID = 0;
            if(m_pTaskPanel) {
                m_pTaskPanel->closeLoading();
            }
		} else if(stTaskRsp.has_querytask_rsp()) { // 查询任务
			m_stTaskObj.setCurTask(stTaskRsp.querytask_rsp());
			P_CITY_CTRL->updateNPCTaskState(stTaskRsp.querytask_rsp().taskid());
			P_CITY_CTRL->updateTask();
			P_CITY_CTRL->getCityScreenView()->updateEntry();
		} else if(stTaskRsp.has_taskstat_notify()) { // 任务通知
			m_stTaskObj.setCurTask(stTaskRsp.taskstat_notify());

			P_CITY_CTRL->updateTask();
			
			P_CITY_CTRL->updateNPCTaskState(stTaskRsp.taskstat_notify().taskid());

		} else if(stTaskRsp.has_commitdailytask_rsp()) { // 提交日常任务
            m_stTaskObj.setDailyTaskInfo(stTaskRsp.commitdailytask_rsp());

			if(m_pTaskPanel != NULL && m_pTaskPanel->isOpen()) {
				m_pTaskPanel->updateDailyTask();
			}

			DailyTaskCfg *pDailyTaskCfg = DAILY_TASK_CFG_MASTER->getDailyTaskCfgByID(m_uCommitDailyTaskID);
			
			if(pDailyTaskCfg != NULL) {
				unsigned int uLevel = P_USER_OBJ->getUserInfo()->level;
				DailyTaskAwardCfg *pDailyTaskAwardCfg = DAILY_TASK_AWARD_CFG_MASTER->getDailyTaskAwardCfg(uLevel, pDailyTaskCfg->mainawardtype());

				if(pDailyTaskAwardCfg != NULL) {
					if(pDailyTaskCfg->mainawardtype() == vmsg::THING_TYPE_COIN) {
						//POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_COIN, 0, pDailyTaskAwardCfg->award().coin());
					} else if(pDailyTaskCfg->mainawardtype() == vmsg::THING_TYPE_GOLD) {
						POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_GOLD, 0, pDailyTaskAwardCfg->award().gold());
					} else if(pDailyTaskCfg->mainawardtype() == vmsg::THING_TYPE_EXP) {
						POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_EXP, 0, pDailyTaskAwardCfg->award().exp());
					}
				}else
				{
					POP_TIP_LAYER_MASTER->ShowAwardTip(pDailyTaskCfg->mainawardtype(), pDailyTaskCfg->award().awardcfgid(), pDailyTaskCfg->award().awardcnt());
					
				}
			}

            m_uCommitDailyTaskID = 0; //reset

			notifyMilestoneFinished();
		} else if(stTaskRsp.has_querydailytask_rsp()) { // 查询日常任务
			m_stTaskObj.setDailyTaskInfo(stTaskRsp.querydailytask_rsp());

			if(m_pTaskPanel != NULL && m_pTaskPanel->isOpen()) {
				m_pTaskPanel->updateDailyTask();
			}

			notifyMilestoneFinished();
		} else if(stTaskRsp.has_dailytaskstat_notify()) { // 日常任务通知
			m_stTaskObj.setDailyTaskInfo(stTaskRsp.dailytaskstat_notify());

			if(m_pTaskPanel != NULL && m_pTaskPanel->isOpen()) {
				m_pTaskPanel->updateDailyTask();
			}

			notifyMilestoneFinished();
		} else if(stTaskRsp.has_reflashdailytask_rsp()) { // 刷新日常任务
			m_stTaskObj.setDailyTaskInfo(stTaskRsp.reflashdailytask_rsp());

			if(m_pTaskPanel != NULL && m_pTaskPanel->isOpen()) {
				m_pTaskPanel->updateDailyTask();
			}

			notifyMilestoneFinished();
		} else if(stTaskRsp.has_commitmilestone_rsp()) { // 提交成就
			m_stTaskObj.setMilestoneInfo(stTaskRsp.commitmilestone_rsp());

			if(m_pTaskPanel != NULL && m_pTaskPanel->isOpen()) {
				m_pTaskPanel->updateMilestone();
			}

			notifyMilestoneFinished();

			MilestoneCfg *pMilestoneCfg = MILESTONE_CFG_MASTER->getMilestoneCfgByID(m_uCommitMilestoneTaskID);
            if(NULL == pMilestoneCfg) {
                CCLOG("[error]TaskControl::onRecvMsg,fail to get MilestoneCfg, taskid=%d", m_uCommitMilestoneTaskID);
                m_uCommitMilestoneTaskID = 0;
                return;
            }
            if(pMilestoneCfg != NULL) {
                POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_GOLD, 0, pMilestoneCfg->award().gold());
                POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_COIN, 0, pMilestoneCfg->award().coin());
            }
            m_uCommitMilestoneTaskID = 0;

		} else if(stTaskRsp.has_querymilestone_rsp()) { // 查询成就
			m_stTaskObj.setMilestoneInfo(stTaskRsp.querymilestone_rsp());

			if(m_pTaskPanel != NULL && m_pTaskPanel->isOpen()) {
				m_pTaskPanel->updateMilestone();
			}

			notifyMilestoneFinished();
		} else if(stTaskRsp.has_milestonestat_notify()) { // 成就通知
			m_stTaskObj.setMilestoneInfo(stTaskRsp.milestonestat_notify());

			if(m_pTaskPanel != NULL && m_pTaskPanel->isOpen()) {
				m_pTaskPanel->updateMilestone();
			}

			notifyMilestoneFinished();
		}
	}
}

void TaskControl::sendTalkWithNPC(unsigned int uNPCID) {
	m_stTaskProxy.sendTalkWithNPC(uNPCID);
}

void TaskControl::sendRecvTaskRqst() {
	const vmsg::CSTask &stTask = m_stTaskObj.getCurTask();

	if(//m_bRecvTaskReceived && 
		stTask.has_taskid() && stTask.taskstat() == vmsg::TASK_STAT_ORG) {
		m_bRecvTaskReceived = false;
        if(m_pTaskPanel) {
            m_pTaskPanel->showLoading();
        }
		m_stTaskProxy.sendRecvTaskRqst();
	}
}

void TaskControl::sendCommitTaskRqst() {
	const vmsg::CSTask &stTask = m_stTaskObj.getCurTask();

	if(//m_bCommitTaskReceived && 
		stTask.has_taskid() && stTask.taskstat() == vmsg::TASK_STAT_FINISH) {
		m_bCommitTaskReceived = false;
		m_uCommitTaskID = getCurTask().taskid();

        if(m_pTaskPanel) {
            m_pTaskPanel->showLoading();
        }
		m_stTaskProxy.sendCommitTaskRqst();
	}
}

void TaskControl::sendQueryTaskRqst() {
	m_stTaskProxy.sendQueryTaskRqst();
}

void TaskControl::sendCommitDailyTaskRqst(unsigned int uTaskID) {
    m_uCommitDailyTaskID = uTaskID;
	m_stTaskProxy.sendCommitDailyTaskRqst(uTaskID);
}

void TaskControl::sendQueryDailyTaskRqst() {
	m_stTaskProxy.sendQueryDailyTaskRqst();
}

void TaskControl::sendRefreshDailyTaskRqst() {
	m_stTaskProxy.sendRefreshDailyTaskRqst();
}

void TaskControl::sendCommitMilestoneRqst(unsigned int uTaskID) {
    m_uCommitMilestoneTaskID = uTaskID;
	m_stTaskProxy.sendCommitMilestoneRqst(uTaskID);
}

void TaskControl::sendQueryMilestoneRqst() {
	m_stTaskProxy.sendQueryMilestoneRqst();
}

void TaskControl::sendTriggerDailyTaskRqst(unsigned int uCmd)
{
	m_stTaskProxy.sendTriggerDailyTaskRqst(uCmd);
}
