#include "EntryGroupCfgMaster.h"

#include <set>

const EntryGroupCfg* EntryGroupCfgMaster::getEntryGroupCfg(const unsigned int uEntryGroupID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uEntryGroupID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! EntryGroupID=%u", __PRETTY_FUNCTION__, uEntryGroupID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_entrygroupcfg(iter->second);
}

unsigned int EntryGroupCfgMaster::getEntryGroupID(unsigned int uEntryID) {
	map<unsigned int, unsigned int>::const_iterator iter = m_stEntry2Group.find(uEntryID);

	if(iter == m_stEntry2Group.end()) {
		return 0;
	}

	return iter->second;
}

int EntryGroupCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.entrygroupcfg_size(); i++)
	{
		const EntryGroupCfg &stEntryGroupCfg = m_stCfg.entrygroupcfg(i);
		unsigned int key = stEntryGroupCfg.entrygroupid();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! EntryGroupID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int EntryGroupCfgMaster::SetCfg() {
	int iRet = 0;

	for(int i = 0; i < m_stCfg.entrygroupcfg_size(); i++)
	{
		const EntryGroupCfg &stEntryGroupCfg = m_stCfg.entrygroupcfg(i);
		unsigned int key = stEntryGroupCfg.entrygroupid();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! EntryGroupID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}

		for(int j = 0; j < stEntryGroupCfg.entryid_size(); j++) {
			unsigned int uEntryID = stEntryGroupCfg.entryid(j);
			
			if(m_stEntry2Group.insert(make_pair(uEntryID, key)).second == false) {
				CV_ERROR(("%s, The key is already exist! EntryGroupID=%u EntryID=%u", __PRETTY_FUNCTION__, key, uEntryID));

				return -1; // id重复
			}
		}
	}

	return iRet;
}
