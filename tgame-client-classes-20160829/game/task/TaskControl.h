#ifndef TaskControl_h__
#define TaskControl_h__

#include "vlib/base/CVSingleton.h"

#include "TaskProxy.h"
#include "TaskObj.h"
#include "TaskPanel.h"

class TaskControl
{
public:
	TaskControl();
	~TaskControl();

	void init();

	void finalize();

	TaskPanel* getTaskPanel();

	const vmsg::CSTask& getCurTask();

	const vmsg::CSDailyTaskInfo& getDailyTaskInfo();

	const vmsg::CSMilestoneInfo& getMilestoneInfo();

	unsigned int getMaxTaskID();

	void autoTask();

	bool isTaskNPC(unsigned int uNPCID);

	unsigned int getMaxOpenWarID(); // 获取已开放的最大关卡id

	void resetOnConnClose();

	bool hasFinishedDailyTask(); // 有已完成的日常

	bool hasFinishedMilestone(); // 有已完成的成就

	void notifyMilestoneFinished();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendTalkWithNPC(unsigned int uNPCID); // npc对话

	void sendRecvTaskRqst(); // 接取主线任务

	void sendCommitTaskRqst(); // 提交主线任务

	void sendQueryTaskRqst(); // 查询主线任务

	void sendCommitDailyTaskRqst(unsigned int uTaskID); // 提交日常任务

	void sendQueryDailyTaskRqst(); // 查询日常任务

	void sendRefreshDailyTaskRqst(); // 刷新日常任务

	void sendCommitMilestoneRqst(unsigned int uTaskID); // 提交成就

	void sendQueryMilestoneRqst(); // 查询成就

	void sendTriggerDailyTaskRqst(unsigned int uCmd); //触发任务
private:
	bool m_bInitialized;

	TaskProxy m_stTaskProxy;

	TaskObj m_stTaskObj;

	TaskPanel *m_pTaskPanel;

	unsigned int m_uCommitTaskID; //临时记录这次提交的任务ID
	unsigned int m_uCommitDailyTaskID; //临时记录这次提交的日常任务
	unsigned int m_uCommitMilestoneTaskID; //临时记录这次提交的里程碑

	bool m_bRecvTaskReceived;

	bool m_bCommitTaskReceived;

};

#define P_TASK_CTRL CVLib::CVSingleton<TaskControl>::Instance()

#endif // TaskControl_h__
