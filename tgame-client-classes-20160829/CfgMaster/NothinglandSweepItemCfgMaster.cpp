#include "NothinglandSweepItemCfgMaster.h"

#include <set>

using std::set;

int CNothinglandSweepItemCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.nothinglandsweepitemcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nothinglandsweepitemcfg(iIdx).itemid();
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

int CNothinglandSweepItemCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.nothinglandsweepitemcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nothinglandsweepitemcfg(iIdx).itemid();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
	}

	return iRet;
}

const NothinglandSweepItemCfg* CNothinglandSweepItemCfgMaster::GetNothinglandSweepItemCfgByID(const unsigned int uiCfgID)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.nothinglandsweepitemcfg(it->second));
}
