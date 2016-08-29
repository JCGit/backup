#include "AttrStrengthenCfgMaster.h"

#include <set>
#include "defines.h"

using std::set;

AttrStrengthenCfg* AttrStrengthenCfgMaster::getAttrStrengthenCfgByLevel(const unsigned int uAttrType, const unsigned int uLevel) {
	string key = intToString(uAttrType) + "_" + intToString(uLevel);
	map<string, int>::const_iterator iter = m_stID2Idx.find(key);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %s !", __PRETTY_FUNCTION__, key.c_str()));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_attrstrengthencfg(iter->second);
}

int AttrStrengthenCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<string> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.attrstrengthencfg_size(); iIdx++)
	{
		const AttrStrengthenCfg &stAttrStrengthenCfg = m_stCfg.attrstrengthencfg(iIdx);
		string key = intToString(stAttrStrengthenCfg.type()) + "_" + intToString(stAttrStrengthenCfg.level());

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, cfgid = %s is dup cfg id !", __PRETTY_FUNCTION__, key.c_str()));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int AttrStrengthenCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.attrstrengthencfg_size(); iIdx++)
	{
		const AttrStrengthenCfg &stAttrStrengthenCfg = m_stCfg.attrstrengthencfg(iIdx);
		string key = intToString(stAttrStrengthenCfg.type()) + "_" + intToString(stAttrStrengthenCfg.level());

		if(m_stID2Idx.insert(make_pair(key, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %s is dup cfg id !", __PRETTY_FUNCTION__, key.c_str()));

			return -1;//id重复
		}
	}

	return iRet;
}
