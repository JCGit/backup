#include "WarmPromptCfgMaster.h"

#include <set>

const WarmPromptCfg* WarmPromptCfgMaster::getWarmPromptCfg(const unsigned int uID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! ID=%u", __PRETTY_FUNCTION__, uID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_warmpromptcfg(iter->second);
}

int WarmPromptCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.warmpromptcfg_size(); i++)
	{
		const WarmPromptCfg &stWarmPromptCfg = m_stCfg.warmpromptcfg(i);
		unsigned int key = stWarmPromptCfg.id();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! ID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int WarmPromptCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.warmpromptcfg_size(); i++)
	{
		const WarmPromptCfg &stWarmPromptCfg = m_stCfg.warmpromptcfg(i);
		unsigned int key = stWarmPromptCfg.id();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! ID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}
