#include "VIPCfgMaster.h"

#include <set>

using std::set;

VIPCfg* VIPCfgMaster::getVIPCfg(const unsigned int uVIPModuleID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uVIPModuleID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find VIPModuleID = %u !", __PRETTY_FUNCTION__, uVIPModuleID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_vipcfg(iter->second);
}

int VIPCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.vipcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.vipcfg(iIdx).vipmoduleid();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int VIPCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.vipcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.vipcfg(iIdx).vipmoduleid();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}
