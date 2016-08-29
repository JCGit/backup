#include "ArenaKillNumCfgMaster.h"

#include <set>

using std::set;

int CArenaKillNumCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.arenakillnumcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.arenakillnumcfg(iIdx).lev();
		if(stItemIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//idÖØ¸´
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CArenaKillNumCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.arenakillnumcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.arenakillnumcfg(iIdx).lev();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//idÖØ¸´
		}
	}

	return iRet;
}

const ArenaKillNumCfg* CArenaKillNumCfgMaster::GetCfgByKillNum(const unsigned int uKillNum){
	unsigned int maxNum = m_stCfg.arenakillnumcfg(m_stCfg.arenakillnumcfg_size() - 1).killnum();

	if(uKillNum > maxNum){
		return &m_stCfg.arenakillnumcfg(m_stCfg.arenakillnumcfg_size() - 1);
	}

	for(int iIdx = 0; iIdx < m_stCfg.arenakillnumcfg_size(); iIdx++){
		unsigned int killNum = m_stCfg.arenakillnumcfg(iIdx).killnum();
		if(uKillNum <= killNum){
			return &m_stCfg.arenakillnumcfg(iIdx);
		}
	}

	return &m_stCfg.arenakillnumcfg(0);
}
