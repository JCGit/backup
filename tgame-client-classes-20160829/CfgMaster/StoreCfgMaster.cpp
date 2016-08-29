#include "StoreCfgMaster.h"

#include <set>
#include "defines.h"

using std::set;

StoreCfg* StoreCfgMaster::getStoreCfg(unsigned int uType, unsigned int uCfgID) {
	string key = intToString(uType) + "_" + intToString(uCfgID);
	map<string, int>::const_iterator iter = m_stID2Idx.find(key);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfg type=%u cfgid=%u !", __PRETTY_FUNCTION__, uType, uCfgID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_storecfg(iter->second);
}

int StoreCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<string> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.storecfg_size(); iIdx++)
	{
		const StoreCfg &stStoreCfg = m_stCfg.storecfg(iIdx);

		unsigned int uType = stStoreCfg.type();
		unsigned int uCfgID = stStoreCfg.cfgid();
		string key = intToString(uType) + "_" + intToString(uCfgID);
		
		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, type=%u cfgid=%u is dup cfg id !", __PRETTY_FUNCTION__, uType, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int StoreCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.storecfg_size(); iIdx++)
	{
		const StoreCfg &stStoreCfg = m_stCfg.storecfg(iIdx);

		unsigned int uType = stStoreCfg.type();
		unsigned int uCfgID = stStoreCfg.cfgid();
		string key = intToString(uType) + "_" + intToString(uCfgID);
		
		if(m_stID2Idx.insert(make_pair(key, iIdx)).second == false)
		{
			CV_ERROR(("%s, type=%u cfgid=%u is dup cfg id !", __PRETTY_FUNCTION__, uType, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}
