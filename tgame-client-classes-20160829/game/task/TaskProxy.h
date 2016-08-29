#ifndef TaskProxy_h__
#define TaskProxy_h__

#include "QBaseProxy.h"

class TaskProxy: public QBaseProxy
{
public:
	TaskProxy();
	virtual ~TaskProxy();

	// override start

    virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);
    virtual void fault(const vmsg::CSCommonErrMsg &err);

	// override end

	void sendTalkWithNPC(unsigned int uNPCID); // npc对话

	void sendRecvTaskRqst(); // 接取主线任务

	void sendCommitTaskRqst(); // 提交主线任务

	void sendQueryTaskRqst(); // 查询主线任务

	void sendCommitDailyTaskRqst(unsigned int uTaskID); // 提交日常任务

	void sendQueryDailyTaskRqst(); // 查询日常任务

	void sendRefreshDailyTaskRqst(); // 刷新日常任务

	void sendCommitMilestoneRqst(unsigned int uTaskID); // 提交成就

	void sendQueryMilestoneRqst(); // 查询成就

	void sendTriggerDailyTaskRqst(unsigned int uCmd);//触发任务
private:

};

#endif // TaskProxy_h__
