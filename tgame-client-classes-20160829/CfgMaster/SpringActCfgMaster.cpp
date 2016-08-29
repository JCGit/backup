#include "SpringActCfgMaster.h"

#include <set>

const SpringActCfg* SpringActCfgMaster::getSpringActCfg(const unsigned int uDay) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uDay);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! Day=%u", __PRETTY_FUNCTION__, uDay));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_springactcfg(iter->second);
}

int SpringActCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.springactcfg_size(); i++)
	{
		const SpringActCfg &stSpringActCfg = m_stCfg.springactcfg(i);
		unsigned int key = stSpringActCfg.day();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! Day=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int SpringActCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.springactcfg_size(); i++)
	{
		const SpringActCfg &stSpringActCfg = m_stCfg.springactcfg(i);
		unsigned int key = stSpringActCfg.day();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! Day=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}
