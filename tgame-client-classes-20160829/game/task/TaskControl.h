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

	unsigned int getMaxOpenWarID(); // ��ȡ�ѿ��ŵ����ؿ�id

	void resetOnConnClose();

	bool hasFinishedDailyTask(); // ������ɵ��ճ�

	bool hasFinishedMilestone(); // ������ɵĳɾ�

	void notifyMilestoneFinished();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendTalkWithNPC(unsigned int uNPCID); // npc�Ի�

	void sendRecvTaskRqst(); // ��ȡ��������

	void sendCommitTaskRqst(); // �ύ��������

	void sendQueryTaskRqst(); // ��ѯ��������

	void sendCommitDailyTaskRqst(unsigned int uTaskID); // �ύ�ճ�����

	void sendQueryDailyTaskRqst(); // ��ѯ�ճ�����

	void sendRefreshDailyTaskRqst(); // ˢ���ճ�����

	void sendCommitMilestoneRqst(unsigned int uTaskID); // �ύ�ɾ�

	void sendQueryMilestoneRqst(); // ��ѯ�ɾ�

	void sendTriggerDailyTaskRqst(unsigned int uCmd); //��������
private:
	bool m_bInitialized;

	TaskProxy m_stTaskProxy;

	TaskObj m_stTaskObj;

	TaskPanel *m_pTaskPanel;

	unsigned int m_uCommitTaskID; //��ʱ��¼����ύ������ID
	unsigned int m_uCommitDailyTaskID; //��ʱ��¼����ύ���ճ�����
	unsigned int m_uCommitMilestoneTaskID; //��ʱ��¼����ύ����̱�

	bool m_bRecvTaskReceived;

	bool m_bCommitTaskReceived;

};

#define P_TASK_CTRL CVLib::CVSingleton<TaskControl>::Instance()

#endif // TaskControl_h__
