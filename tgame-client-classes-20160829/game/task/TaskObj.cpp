#include "TaskObj.h"

TaskObj::TaskObj()
{
	m_stTask.Clear();
}

TaskObj::~TaskObj()
{
}

const vmsg::CSTask& TaskObj::getCurTask() {
	return m_stTask;
}

void TaskObj::setCurTask(const vmsg::CSTask& uCurTask) {
	m_stTask = uCurTask;
}

const vmsg::CSDailyTaskInfo& TaskObj::getDailyTaskInfo() {
	return m_stDailyTaskInfo;
}

void TaskObj::setDailyTaskInfo(const vmsg::CSDailyTaskInfo& stDailyTaskInfo) {
	m_stDailyTaskInfo = stDailyTaskInfo;
}

const vmsg::CSMilestoneInfo& TaskObj::getMilestoneInfo() {
	return m_stMilestoneInfo;
}

void TaskObj::setMilestoneInfo(const vmsg::CSMilestoneInfo& stMilestoneInfo) {
	m_stMilestoneInfo = stMilestoneInfo;
}
