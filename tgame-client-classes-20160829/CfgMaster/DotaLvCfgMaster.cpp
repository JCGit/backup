#include "DotaLvCfgMaster.h"

#include <set>

#include "defines.h"

const DotaLvCfg* DotaLvCfgMaster::getDotaLvCfg(unsigned int uLevel, unsigned int uPart) {
	string key = intToString(uLevel) + "_" + intToString(uPart);
	map<string, int>::const_iterator iter = m_stID2Idx.find(key);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! DotaLv=%u DotaPart=%u", __PRETTY_FUNCTION__, uLevel, uPart));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_dotalvcfg(iter->second);
}

int DotaLvCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<string> stCfgIDSet;

	for(int i = 0; i < m_stCfg.dotalvcfg_size(); i++)
	{
		const DotaLvCfg &stDotaLvCfg = m_stCfg.dotalvcfg(i);
		unsigned int uLevel = stDotaLvCfg.dotalv();
		unsigned int uPart = stDotaLvCfg.dotapart();
		string key = intToString(uLevel) + "_" + intToString(uPart);

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! DotaLv=%u DotaPart=%u", __PRETTY_FUNCTION__, uLevel, uPart));

			return -1; // id重复
		}
	}

	return iRet;
}

int DotaLvCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.dotalvcfg_size(); i++)
	{
		const DotaLvCfg &stDotaLvCfg = m_stCfg.dotalvcfg(i);
		unsigned int uLevel = stDotaLvCfg.dotalv();
		unsigned int uPart = stDotaLvCfg.dotapart();
		string key = intToString(uLevel) + "_" + intToString(uPart);

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! DotaLv=%u DotaPart=%u", __PRETTY_FUNCTION__, uLevel, uPart));

			return -1; // id重复
		}
	}

	return iRet;
}
