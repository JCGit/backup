#include "EquipExpItemCfgMaster.h"
#include <set>

using std::set;

int CEquipExpItemCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	set<unsigned int> stIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.equipexpitemcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.equipexpitemcfg(iIdx).itemcfgid();
		if(stIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return ERET_SYS_ERR;//idÖØ¸´
		}
	}
	return iRet;
}

int CEquipExpItemCfgMaster::SetCfg()
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.equipexpitemcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.equipexpitemcfg(iIdx).itemcfgid();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return ERET_SYS_ERR;
		}
	}

	return iRet;
}

const EquipExpItemCfg* CEquipExpItemCfgMaster::GetEquipExpItemCfg(const unsigned int uiCfgID)const
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		//CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; 
	}

	return &(m_stCfg.equipexpitemcfg(it->second));
}
