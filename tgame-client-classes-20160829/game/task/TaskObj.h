#ifndef TaskObj_h__
#define TaskObj_h__

#include "CSTaskMsg.pb.h"

class TaskObj
{
public:
	TaskObj();
	~TaskObj();

	const vmsg::CSTask& getCurTask();

	void setCurTask(const vmsg::CSTask& stTask);

	const vmsg::CSDailyTaskInfo& getDailyTaskInfo();

	void setDailyTaskInfo(const vmsg::CSDailyTaskInfo& stDailyTaskInfo);

	const vmsg::CSMilestoneInfo& getMilestoneInfo();

	void setMilestoneInfo(const vmsg::CSMilestoneInfo& stMilestoneInfo);

private:
	vmsg::CSTask m_stTask; // ��������

	vmsg::CSDailyTaskInfo m_stDailyTaskInfo; // �ճ�����

	vmsg::CSMilestoneInfo m_stMilestoneInfo; // �ɾ�

};

#endif // TaskObj_h__
