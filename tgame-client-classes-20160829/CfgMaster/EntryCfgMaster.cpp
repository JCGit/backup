#include "EntryCfgMaster.h"

#include <set>

const EntryCfg* EntryCfgMaster::getEntryCfg(const unsigned int uEntryID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uEntryID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! EntryID=%u", __PRETTY_FUNCTION__, uEntryID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_entrycfg(iter->second);
}

unsigned int EntryCfgMaster::getParentEntryID(unsigned int uEntryID) {
	map<unsigned int, unsigned int>::const_iterator iter = m_stChild2Parent.find(uEntryID);

	if(iter == m_stChild2Parent.end()) {
		return 0;
	}
	
	return iter->second;
}

unsigned int EntryCfgMaster::getEntryIDByModuleID(unsigned int uModuleID) {
	map<unsigned int, unsigned int>::const_iterator iter = m_stModule2Entry.find(uModuleID);

	if(iter == m_stModule2Entry.end()) {
		return 0;
	}

	return iter->second;
}

pair<unsigned int, vector<unsigned int> > EntryCfgMaster::getLevelEntrys(unsigned int uLevel) {
	pair<unsigned int, vector<unsigned int> > stLevelEntrysPair;
	map<unsigned int, vector<unsigned int> >::const_iterator iter = m_stLevel2Entrys.begin();

	while(iter != m_stLevel2Entrys.end()) {
		if(iter->first == uLevel) { // 这个等级会开放的
			stLevelEntrysPair = *iter;
			break;
		}

		iter++;
	}

	return stLevelEntrysPair;
}

int EntryCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.entrycfg_size(); i++)
	{
		const EntryCfg &stEntryCfg = m_stCfg.entrycfg(i);
		unsigned int key = stEntryCfg.entryid();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! EntryID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int EntryCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.entrycfg_size(); i++)
	{
		const EntryCfg &stEntryCfg = m_stCfg.entrycfg(i);
		unsigned int key = stEntryCfg.entryid();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! EntryID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}

		unsigned int uNeedLevel = stEntryCfg.needlevel();

		if(uNeedLevel > 0) {
			m_stLevel2Entrys[uNeedLevel].push_back(key);
		}

		for(int j = 0; j < stEntryCfg.childentryid_size(); j++) {
			unsigned int uChildEntryID = stEntryCfg.childentryid(j);
			m_stChild2Parent.insert(make_pair(uChildEntryID, key));
		}

		for(int k = 0; k < stEntryCfg.moduleid_size(); k++) {
			unsigned int uModuleID = stEntryCfg.moduleid(k);
			m_stModule2Entry.insert(make_pair(uModuleID, key));
		}
	}

	return iRet;
}


unsigned int EntryCfgMaster::getChildEntryIDSize(unsigned int uEntryID)
{
	unsigned int uChildSize = 0;
	const EntryCfg* uEntry = getEntryCfg(uEntryID);
	if (uEntry)
	{
		uChildSize = uEntry->childentryid_size();
	}
	
	return uChildSize;
}