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

	void sendTalkWithNPC(unsigned int uNPCID); // npc�Ի�

	void sendRecvTaskRqst(); // ��ȡ��������

	void sendCommitTaskRqst(); // �ύ��������

	void sendQueryTaskRqst(); // ��ѯ��������

	void sendCommitDailyTaskRqst(unsigned int uTaskID); // �ύ�ճ�����

	void sendQueryDailyTaskRqst(); // ��ѯ�ճ�����

	void sendRefreshDailyTaskRqst(); // ˢ���ճ�����

	void sendCommitMilestoneRqst(unsigned int uTaskID); // �ύ�ɾ�

	void sendQueryMilestoneRqst(); // ��ѯ�ɾ�

	void sendTriggerDailyTaskRqst(unsigned int uCmd);//��������
private:

};

#endif // TaskProxy_h__
