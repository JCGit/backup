#include "NihilityScoreCfgMaster.h"

#include <set>

using std::set;

int CNihilityScoreCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.nihilityscorecfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nihilityscorecfg(iIdx).lev();
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

int CNihilityScoreCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.nihilityscorecfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nihilityscorecfg(iIdx).lev();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
	}

	return iRet;
}

const NihilityScoreCfg* CNihilityScoreCfgMaster::GetNihilityScoreCfgByLev(const unsigned int uiLev)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiLev);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u lev", __PRETTY_FUNCTION__, uiLev));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.nihilityscorecfg(it->second));
}
