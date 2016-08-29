#include "DailyTaskCfgMaster.h"

#include <set>

using std::set;

DailyTaskCfg* DailyTaskCfgMaster::getDailyTaskCfgByID(const unsigned int uCfgID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uCfgID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, uCfgID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_dailytaskcfg(iter->second);
}

int DailyTaskCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.dailytaskcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.dailytaskcfg(iIdx).id();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int DailyTaskCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.dailytaskcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.dailytaskcfg(iIdx).id();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}
