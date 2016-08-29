#include "WorldBossLvCfgMaster.h"

#include <set>

using std::set;

int CWorldBossLvCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.worldbosslvcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.worldbosslvcfg(iIdx).level();
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

int CWorldBossLvCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.worldbosslvcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.worldbosslvcfg(iIdx).level();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
	}

	return iRet;
}

const WorldBossLvCfg* CWorldBossLvCfgMaster::GetWorldBossCfgByLv(const unsigned int uLv)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uLv);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find world boss lv = %u", __PRETTY_FUNCTION__, uLv));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.worldbosslvcfg(it->second));
}
