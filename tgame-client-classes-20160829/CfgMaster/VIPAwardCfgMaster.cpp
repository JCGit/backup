#include "VIPAwardCfgMaster.h"

#include <set>

#include "defines.h"

using std::set;

VIPAwardCfg* VIPAwardCfgMaster::getVIPAwardCfg(const unsigned int uPlayerLv, const unsigned int uVIPLv) {
	string key = intToString(uPlayerLv) + "_" + intToString(uVIPLv);
	map<string, int>::const_iterator iter = m_stID2Idx.find(key);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfg PlayerLv = %u VIPLv = %u !", __PRETTY_FUNCTION__, uPlayerLv, uVIPLv));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_vipawardcfg(iter->second);
}

int VIPAwardCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<string> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.vipawardcfg_size(); iIdx++)
	{
		const VIPAwardCfg &stVIPAwardCfg = m_stCfg.vipawardcfg(iIdx);
		string key = intToString(stVIPAwardCfg.playerlv()) + "_" + intToString(stVIPAwardCfg.viplv());

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, key = %s is dup cfg key !", __PRETTY_FUNCTION__, key.c_str()));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int VIPAwardCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.vipawardcfg_size(); iIdx++)
	{
		const VIPAwardCfg &stVIPAwardCfg = m_stCfg.vipawardcfg(iIdx);
		string key = intToString(stVIPAwardCfg.playerlv()) + "_" + intToString(stVIPAwardCfg.viplv());

		if(m_stID2Idx.insert(make_pair(key, iIdx)).second == false)
		{
			CV_ERROR(("%s, key = %s is dup cfg key !", __PRETTY_FUNCTION__, key.c_str()));

			return -1;//id重复
		}
	}

	return iRet;
}
