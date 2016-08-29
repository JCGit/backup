#include "ExpCfgMaster.h"

#include <set>

using std::set;

ExpCfg* ExpCfgMaster::getExpCfgByID(const unsigned int uCfgID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uCfgID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, uCfgID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_expcfg(iter->second);
}

unsigned int ExpCfgMaster::getNextTotalExp(unsigned int uCurLevel) {
	map<unsigned int, unsigned int>::const_iterator iter = m_stExpNeedMap.find(uCurLevel);

	if(iter == m_stExpNeedMap.end()) {
		return 0;
	}

	return iter->second;
}

int ExpCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.expcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.expcfg(iIdx).level();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int ExpCfgMaster::SetCfg() {
	int iRet = 0;
	unsigned int uTotalExp = 0;

	for(int iIdx = 0; iIdx < m_stCfg.expcfg_size(); iIdx++)
	{
		const ExpCfg &stExpCfg = m_stCfg.expcfg(iIdx);
		unsigned int uLevel = stExpCfg.level();

		if(m_stID2Idx.insert(make_pair(uLevel, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uLevel));

			return -1;//id重复
		}

		uTotalExp += stExpCfg.exp();
		m_stExpNeedMap.insert(make_pair(uLevel, uTotalExp));
	}

	return iRet;
}
