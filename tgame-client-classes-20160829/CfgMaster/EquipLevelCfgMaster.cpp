#include "EquipLevelCfgMaster.h"
#include <set>

using std::set;

int CEquipLevelCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	set<unsigned int> stIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.equiplevelcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.equiplevelcfg(iIdx).level();
		if(stIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return ERET_SYS_ERR;//idÖØ¸´
		}

	}
	return iRet;
}

int CEquipLevelCfgMaster::SetCfg()
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.equiplevelcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.equiplevelcfg(iIdx).level();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return ERET_SYS_ERR;
		}
	}

	return iRet;
}

const EquipLevelCfg* CEquipLevelCfgMaster::GetCfg(const unsigned int uiEquipLevel)const
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiEquipLevel);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiEquipLevel));
		return NULL; 
	}

	return &(m_stCfg.equiplevelcfg(it->second));
}
