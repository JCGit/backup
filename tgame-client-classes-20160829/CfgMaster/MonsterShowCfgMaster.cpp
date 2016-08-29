#include "MonsterShowCfgMaster.h"
#include <set>
#include "vlib/CVRand.h"

using std::set;
using namespace std;
using namespace CVLib;

int CMonsterShowCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<string> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.monstershowcfg_size(); iIdx++)
	{
		string uiCfgID = m_stCfg.monstershowcfg(iIdx).avatar();
		if(stItemIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %s is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID.c_str()));
			return -1;//id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CMonsterShowCfgMaster::SetCfg()
{
	int iRet = 0;

	map<int, vector<int> >::iterator stIter;
	for(int iIdx = 0; iIdx < m_stCfg.monstershowcfg_size(); iIdx++)
	{
		string uiCfgID = m_stCfg.monstershowcfg(iIdx).avatar();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %s is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID.c_str()));
			return -1;//id重复
		}

		unsigned int type = m_stCfg.monstershowcfg(iIdx).monstertype();
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

MonsterShowCfg* CMonsterShowCfgMaster::GetMonsterShowCfgByShowID(const std::string showID)
{
	map<std::string, int>::const_iterator it = m_stID2IDX.find(showID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %s", __PRETTY_FUNCTION__, showID.c_str()));
		return NULL; //找不到该物品配置
	}

	return m_stCfg.mutable_monstershowcfg(it->second);
}

MonsterShowCfg* CMonsterShowCfgMaster::GetMonsterShowCfgRand(const unsigned int type)
{
	map<int, vector<int> >::iterator stIter = m_stTypeToIDX.find(type);

	if(stIter == m_stTypeToIDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, type));
		return NULL; //找不到该物品配置
	}

	unsigned int totalSize = stIter->second.size();
	unsigned int idx = CRandom::GetRand(totalSize);

	return m_stCfg.mutable_monstershowcfg(stIter->second.at(idx));
}