#include "EliteWarMapCfgMaster.h"

#include <set>

const EliteWarMapCfg* EliteWarMapCfgMaster::getEliteWarMapCfg(const unsigned int uID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! ID=%u", __PRETTY_FUNCTION__, uID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_elitewarmapcfg(iter->second);
}

const vector<EliteWarMapCfg*>& EliteWarMapCfgMaster::getEliteWarChapter(const unsigned int uChapterID) {
	return m_stEliteWarChapters[uChapterID];
}

int EliteWarMapCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.elitewarmapcfg_size(); i++)
	{
		const EliteWarMapCfg &stEliteWarMapCfg = m_stCfg.elitewarmapcfg(i);
		unsigned int key = stEliteWarMapCfg.id();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! ID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int EliteWarMapCfgMaster::SetCfg() {
	int iRet = 0;
	unsigned int uEliteWarChapterID = 0;

	for(int i = 0; i < m_stCfg.elitewarmapcfg_size(); i++)
	{
		const EliteWarMapCfg &stEliteWarMapCfg = m_stCfg.elitewarmapcfg(i);
		unsigned int key = stEliteWarMapCfg.id();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! ID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}

		if(stEliteWarMapCfg.warid() != uEliteWarChapterID) {
			uEliteWarChapterID = stEliteWarMapCfg.warid();
			vector<EliteWarMapCfg*> chapter;
			m_stEliteWarChapters.insert(make_pair(uEliteWarChapterID, chapter));
		}

		m_stEliteWarChapters.at(uEliteWarChapterID).push_back(m_stCfg.mutable_elitewarmapcfg(i));
	}

	return iRet;
}
