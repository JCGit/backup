#include "PlayerGrowCfgMaster.h"

#include <set>

const PlayerGrowCfg* PlayerGrowCfgMaster::getPlayerGrowCfg(const unsigned int uLevel) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uLevel);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! Level=%u", __PRETTY_FUNCTION__, uLevel));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_playergrowcfg(iter->second);
}

int PlayerGrowCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.playergrowcfg_size(); i++)
	{
		const PlayerGrowCfg &stPlayerGrowCfg = m_stCfg.playergrowcfg(i);
		unsigned int key = stPlayerGrowCfg.level();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! Level=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int PlayerGrowCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.playergrowcfg_size(); i++)
	{
		const PlayerGrowCfg &stPlayerGrowCfg = m_stCfg.playergrowcfg(i);
		unsigned int key = stPlayerGrowCfg.level();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! Level=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}
