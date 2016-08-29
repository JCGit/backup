#include "NameCfgMaster.h"

#include <set>
#include "vlib/CVRand.h"
using namespace CVLib;
using std::set;

int CNameCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.namecfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.namecfg(iIdx).id();
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

int CNameCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.namecfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.namecfg(iIdx).id();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
	}

	return iRet;
}

const NameCfg* CNameCfgMaster::GetNameCfgByID(const unsigned int uiCfgID)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.namecfg(it->second));
}

const NameCfg* CNameCfgMaster::GetRandCfg(){
	int idx = CRandom::GetRand(m_stCfg.namecfg_size());

	return &(m_stCfg.namecfg(idx));
}

const string CNameCfgMaster::GetRandName(){
	int idx = CRandom::GetRand(2);
	string name = "";
	name += GetRandCfg()->firstname();
	if(idx == 0){
		name += GetRandCfg()->secondnamemale();
	}else{
		name += GetRandCfg()->secondnamefemale();
	}

	return name;
}