#include "NihilityMapLibCfgMaster.h"
#include <set>
#include "vlib/CVRand.h"

using namespace CVLib;

using std::set;

int CNihilityMapLibCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.nihilitymaplibcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nihilitymaplibcfg(iIdx).id();
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

int CNihilityMapLibCfgMaster::SetCfg()
{
	int iRet = 0;

	map<unsigned int, vector<int> >::iterator stIter;
	for(int iIdx = 0; iIdx < m_stCfg.nihilitymaplibcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nihilitymaplibcfg(iIdx).id();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}

		unsigned int type = m_stCfg.nihilitymaplibcfg(iIdx).type();
		stIter = m_stTypeToIDX.find(type);

		if(stIter == m_stTypeToIDX.end()){
			m_stTypeToIDX[type].clear();
			m_stTypeToIDX[type].push_back(iIdx);
		}else{
			m_stTypeToIDX[type].push_back(iIdx);
		}
	}

	return iRet;
}

const NihilityMapLibCfg* CNihilityMapLibCfgMaster::GetNihilityMapLibCfgByID(const unsigned int uiCfgID)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.nihilitymaplibcfg(it->second));
}

const NihilityMapLibCfg* CNihilityMapLibCfgMaster::GetNihilityMapRandByType(const unsigned int uType, const unsigned int floor){
	map<unsigned int, vector<int> >::iterator stIter = m_stTypeToIDX.find(uType);

	if(stIter == m_stTypeToIDX.end()){
		CV_ERROR(("%s, can not find type = %u nihility map cfg", __PRETTY_FUNCTION__, uType));
		return NULL; //找不到该物品配置
	}

	vector<int> stVector = stIter->second;
	vector<int> stNewVec;
	vector<int>::iterator stVecIter;
	for(stVecIter = stVector.begin(); stVecIter != stVector.end(); stVecIter++){
		unsigned int minLev = m_stCfg.nihilitymaplibcfg(*stVecIter).minlevel();
		unsigned int maxLev = m_stCfg.nihilitymaplibcfg(*stVecIter).maxlevel();
		if(floor >= minLev && floor <= maxLev){
			stNewVec.push_back(*stVecIter);
		}
	}

	unsigned int totalSize = stNewVec.size();
	unsigned int idx = CRandom::GetRand(totalSize);

	return &(m_stCfg.nihilitymaplibcfg(stNewVec.at(idx)));
}

const NihilityMapLibCfg* CNihilityMapLibCfgMaster::GetNihilityMapRandByTypeAndRes(const unsigned int uType, const unsigned int floor, const string stRes){
	map<unsigned int, vector<int> >::iterator stIter = m_stTypeToIDX.find(uType);

	if(stIter == m_stTypeToIDX.end()){
		CV_ERROR(("%s, can not find type = %u nihility map cfg", __PRETTY_FUNCTION__, uType));
		return NULL; //找不到该物品配置
	}

	vector<int> stVector = stIter->second;
	vector<int> stNewVec;
 	vector<int>::iterator stVecIter;
	for(stVecIter = stVector.begin(); stVecIter != stVector.end(); stVecIter++){
		unsigned int minLev = m_stCfg.nihilitymaplibcfg(*stVecIter).minlevel();
		unsigned int maxLev = m_stCfg.nihilitymaplibcfg(*stVecIter).maxlevel();
		if(m_stCfg.nihilitymaplibcfg(*stVecIter).foreid() == stRes){
			if(floor >= minLev && floor <= maxLev){
				stNewVec.push_back(*stVecIter);
			}
		}
	}

	unsigned int totalSize = stNewVec.size();
	if(totalSize <= 0){
		CV_ERROR(("%s, can not find type = %u, foreid = %s nihility map cfg", __PRETTY_FUNCTION__, uType, stRes.c_str()));
		return NULL; //找不到该物品配置
	}

	unsigned int idx = CRandom::GetRand(totalSize);
	return &(m_stCfg.nihilitymaplibcfg(stNewVec.at(idx)));
}

