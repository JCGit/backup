#include "EquipRefitCfgMaster.h"
#include <set>

using std::set;

int CEquipRefitCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	set<pair<unsigned int, unsigned int> > stIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.equiprefitcfg_size(); iIdx++)
	{
		unsigned int uiPart = m_stCfg.equiprefitcfg(iIdx).part();
		unsigned int uiColor = m_stCfg.equiprefitcfg(iIdx).color();
		pair<unsigned int, unsigned int> pairKey = make_pair(uiPart, uiColor);
		if(stIDSet.insert(pairKey).second == false)
		{
			CV_ERROR(("%s, part%u color %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiPart, uiColor));
			return ERET_SYS_ERR;//id÷ÿ∏¥
		}

	}
	return iRet;
}

int CEquipRefitCfgMaster::SetCfg()
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.equiprefitcfg_size(); iIdx++)
	{
		pair<unsigned int, unsigned int> pairKey = make_pair(m_stCfg.equiprefitcfg(iIdx).part(), m_stCfg.equiprefitcfg(iIdx).color());
		if(m_stID2IDX.insert(make_pair(pairKey, iIdx)).second == false)
		{
			CV_ERROR(("%s, part%u color %u is dup cfg id !",
				__PRETTY_FUNCTION__, m_stCfg.equiprefitcfg(iIdx).part(), m_stCfg.equiprefitcfg(iIdx).color()));
			return ERET_SYS_ERR;
		}
	}

	return iRet;
}

const EquipRefitCfg* CEquipRefitCfgMaster::GetCfg(unsigned int uiPart, unsigned int uiColor)const
{
	map<pair<unsigned int, unsigned int>, int>::const_iterator it = m_stID2IDX.find(make_pair(uiPart, uiColor));

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find part%u color %u item", __PRETTY_FUNCTION__, uiPart, uiColor));
		return NULL; 
	}

	return &(m_stCfg.equiprefitcfg(it->second));
}
