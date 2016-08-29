#include "EliteAwardLibCfgMaster.h"

#include <set>

const EliteAwardLibCfg* EliteAwardLibCfgMaster::getEliteAwardLibCfg(const unsigned int uID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s. Not found the cfg! ID=%u", __PRETTY_FUNCTION__, uID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_eliteawardlibcfg(iter->second);
}

const vector<EliteAwardLibCfg*>& EliteAwardLibCfgMaster::getChapterAwards(const unsigned int uEliteWarChapterID) {
	return m_stChapterAwards[uEliteWarChapterID];
}

int EliteAwardLibCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int i = 0; i < m_stCfg.eliteawardlibcfg_size(); i++)
	{
		const EliteAwardLibCfg &stEliteAwardLibCfg = m_stCfg.eliteawardlibcfg(i);
		unsigned int key = stEliteAwardLibCfg.id();

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, The key is already exist! ID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}
	}

	return iRet;
}

int EliteAwardLibCfgMaster::SetCfg() {
	int iRet = 0;
	unsigned int uEliteWarChapterID = 0;

	for(int i = 0; i < m_stCfg.eliteawardlibcfg_size(); i++)
	{
		const EliteAwardLibCfg &stEliteAwardLibCfg = m_stCfg.eliteawardlibcfg(i);
		unsigned int key = stEliteAwardLibCfg.id();

		if(m_stID2Idx.insert(make_pair(key, i)).second == false)
		{
			CV_ERROR(("%s, The key is already exist! ID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}

		if(stEliteAwardLibCfg.warid() != uEliteWarChapterID) {
			uEliteWarChapterID = stEliteAwardLibCfg.warid();
			vector<EliteAwardLibCfg*> chapter;
			m_stChapterAwards.insert(make_pair(uEliteWarChapterID, chapter));
		}

		m_stChapterAwards.at(uEliteWarChapterID).push_back(m_stCfg.mutable_eliteawardlibcfg(i));
	}

	return iRet;
}
