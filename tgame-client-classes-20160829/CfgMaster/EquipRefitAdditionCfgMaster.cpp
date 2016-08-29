#include "EquipRefitAdditionCfgMaster.h"

const EquipRefitAdditionCfg*  CEquipRefitAdditionCfgMaster::GetCfg(const unsigned int uiColor)const
{
	if (uiColor <= 0
		|| uiColor > static_cast<unsigned int>(m_stCfg.equiprefitadditioncfg_size()))
	{
		return NULL;
	}

	return &(m_stCfg.equiprefitadditioncfg(uiColor-1));
}

int CEquipRefitAdditionCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.equiprefitadditioncfg_size(); iIdx++)
	{
		if (static_cast<unsigned int>(iIdx + 1) != m_stCfg.equiprefitadditioncfg(iIdx).quality())
		{
			CV_WARNING(("%s,must be sorted! index %d", __PRETTY_FUNCTION__, iIdx));
			return ERET_SYS_ERR;
		}
	}

	return iRet;
}

const unsigned int CEquipRefitAdditionCfgMaster::GetMaxQuality() {
    return static_cast<unsigned int>(m_stCfg.equiprefitadditioncfg_size() );
}
