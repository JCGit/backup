#include "CfgMaster/FBShareCfgMaster.h"

#include <set>
using std::set;

const FBShareCfg* FBShareCfgMaster::GetFBShareCfg(unsigned int uShareNum)
{
	map<unsigned int, int>::const_iterator iter = m_stID2IDX.find(uShareNum);

	if (iter == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, uShareNum));

		return NULL; // 找不到相应配置
	}

	return &m_stCfg.fbsharecfg(iter->second);
}

int FBShareCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for (int iIdx = 0; iIdx < m_stCfg.fbsharecfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.fbsharecfg(iIdx).id();

		if (stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}
		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int FBShareCfgMaster::SetCfg() {
	int iRet = 0;

	for (int iIdx = 0; iIdx < m_stCfg.fbsharecfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.fbsharecfg(iIdx).id();

		if (m_stID2IDX.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}
