#include "ModuleOpenCfgMaster.h"

#include <set>

using std::set;

ModuleOpenCfg* ModuleOpenCfgMaster::getModuleOpenCfg(const unsigned int uModuleID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uModuleID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find ModuleID = %u !", __PRETTY_FUNCTION__, uModuleID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_moduleopencfg(iter->second);
}

ModuleOpenCfg* ModuleOpenCfgMaster::getModuleOpenCfgByIndex( int uIndex)
{
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.begin();
	while (uIndex>0)
	{
		uIndex--;
		iter++;
	}
	if (iter == m_stID2Idx.end())
	{
		return NULL;
	}
	return m_stCfg.mutable_moduleopencfg(iter->second);;
}

int ModuleOpenCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.moduleopencfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.moduleopencfg(iIdx).moduleid();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int ModuleOpenCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.moduleopencfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.moduleopencfg(iIdx).moduleid();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}

int ModuleOpenCfgMaster::getModuleOpenCfgNumb()
{
	return m_stID2Idx.size();
}