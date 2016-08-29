#include "DotaItemCfgMaster.h"

#include <set>

const DotaItemCfg* DotaItemCfgMaster::getDotaItemCfg(unsigned int uDotaItemID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uDotaItemID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! DotaItemID=%u", __PRETTY_FUNCTION__, uDotaItemID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_dotaitemcfg(iter->second);
}

int DotaItemCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.dotaitemcfg_size(); i++)
	{
		const DotaItemCfg &stDotaItemCfg = m_stCfg.dotaitemcfg(i);
		unsigned int key = stDotaItemCfg.dotaitemid();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! DotaItemID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int DotaItemCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.dotaitemcfg_size(); i++)
	{
		const DotaItemCfg &stDotaItemCfg = m_stCfg.dotaitemcfg(i);
		unsigned int key = stDotaItemCfg.dotaitemid();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! DotaItemID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}
