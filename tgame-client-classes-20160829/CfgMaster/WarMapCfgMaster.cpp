#include "WarMapCfgMaster.h"
#include <set>

using std::set;

const vector<WarMapCfg*>* CWarMapCfgMaster::getWarChapter(unsigned int uChapterID) {
	vector<WarMapCfg*> *pChapter = NULL;

	if(m_stWarChapters.find(uChapterID) != m_stWarChapters.end()) {
		pChapter = &m_stWarChapters[uChapterID];
	}

	return pChapter;
}

int CWarMapCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.warmapcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.warmapcfg(iIdx).id();
		if(stItemIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CWarMapCfgMaster::SetCfg()
{
	int iRet = 0;
	unsigned int uChapterID = 0;

	map<unsigned int, pair<unsigned int, unsigned int> >::iterator stIDIter;

	for(int i = 0; i < m_stCfg.warmapcfg_size(); i++)
	{
		const WarMapCfg &stWarMapCfg = m_stCfg.warmapcfg(i);
		unsigned int key = m_stCfg.warmapcfg(i).id();
		if(m_stID2IDX.insert(make_pair(key, i)).second == false) {
			CV_ERROR(("%s, The key is already exist! ID=%u", __PRETTY_FUNCTION__, key));

			return -1; // id重复
		}

		if(stWarMapCfg.warid() != uChapterID) {
			uChapterID = stWarMapCfg.warid();
			vector<WarMapCfg*> chapter;
			m_stWarChapters.insert(make_pair(uChapterID, chapter));
		}

		m_stWarChapters.at(uChapterID).push_back(m_stCfg.mutable_warmapcfg(i));

		unsigned int uiWarID = m_stCfg.warmapcfg(i).warid();
		pair<unsigned int, unsigned int> m_stIDX;
		stIDIter = m_stWarID2IDx.find(uiWarID);

		if(stIDIter == m_stWarID2IDx.end()){
			m_stWarID2IDx[uiWarID] = make_pair(key, key);
		}else{
			m_stIDX = stIDIter->second;
			unsigned int first = m_stIDX.first;
			unsigned int second = m_stIDX.second;
			m_stIDX.first = MIN(key, first);
			m_stIDX.second = MAX(key, second);
			m_stWarID2IDx[uiWarID] = m_stIDX;
			second = m_stIDX.second;
		}

		m_stTotalMonsterNum = 0;
		for(int i = 0; i < m_stCfg.warmapcfg(i).monsters_size(); i++){
			m_stTotalMonsterNum += m_stCfg.warmapcfg(i).monsters(i).monsternum();
		}

		m_stTotalItemNum = 0;
		for(int i = 0; i < m_stCfg.warmapcfg(i).items_size(); i++){
			m_stTotalItemNum += m_stCfg.warmapcfg(i).items(i).itemnum();
		}
	}

	return iRet;
}

WarMapCfg* CWarMapCfgMaster::GetWarMapCfgByID(const unsigned int uiCfgID)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		//CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; //找不到该物品配置
	}

	return m_stCfg.mutable_warmapcfg(it->second);
}

pair<unsigned int, unsigned int> CWarMapCfgMaster::GetIDXByWarID(const unsigned int uiWarID){
	map<unsigned int, pair<unsigned int, unsigned int> >::iterator stIDIter;
	stIDIter = m_stWarID2IDx.find(uiWarID);
	if(stIDIter == m_stWarID2IDx.end()){
		CV_ERROR(("%s, warid = %u have none cfg id !", __PRETTY_FUNCTION__, uiWarID));
		return make_pair(0, 0);
	}

	return stIDIter->second;
}

unsigned int CWarMapCfgMaster::GetTotalMonsterNum(){
	return m_stTotalMonsterNum;
}

unsigned int CWarMapCfgMaster::GetTotalItemNum(){
	return m_stTotalItemNum;
}

unsigned int CWarMapCfgMaster::GetNormalSize()
{
	return  m_stCfg.warmapcfg(m_stCfg.warmapcfg_size()-1).id();
}