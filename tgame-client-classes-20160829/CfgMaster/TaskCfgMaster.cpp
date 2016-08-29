#include "TaskCfgMaster.h"

#include <set>

using std::set;

bool TaskCfgMaster::isValid(unsigned int uTaskID) {
	return m_stID2Idx.find(uTaskID) != m_stID2Idx.end();
}

const TaskCfg* TaskCfgMaster::getTaskCfgByID(unsigned int uTaskID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uTaskID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u task", __PRETTY_FUNCTION__, uTaskID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_taskcfg(iter->second);
}

int TaskCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stTaskIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.taskcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.taskcfg(iIdx).taskid();

		if(stTaskIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int TaskCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.taskcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.taskcfg(iIdx).taskid();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}
