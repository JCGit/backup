#include "FightKillNumCfgMaster.h"

#include <set>

using std::set;

int CFightKillNumCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.fightkillnumcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.fightkillnumcfg(iIdx).lev();
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

int CFightKillNumCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.fightkillnumcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.fightkillnumcfg(iIdx).lev();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//idÖØ¸´
		}
	}

	return iRet;
}

const FightKillNumCfg* CFightKillNumCfgMaster::GeCfgByKillNum(const unsigned int uNum){
	if(uNum == 0){
		return NULL;
	}

	unsigned int maxNum = m_stCfg.fightkillnumcfg(m_stCfg.fightkillnumcfg_size() - 1).killnum();
	unsigned int remainNum = uNum%maxNum;

	for(int iIdx = 0; iIdx < m_stCfg.fightkillnumcfg_size(); iIdx++){
		unsigned int killNum = m_stCfg.fightkillnumcfg(iIdx).killnum();
		if(remainNum == killNum){
			return &m_stCfg.fightkillnumcfg(iIdx);
		}
	}

	return NULL;
}
