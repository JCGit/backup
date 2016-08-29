#include "WeaponRefitAdditionCfgMaster.h"

const WeaponRefitAdditionCfg*  CWeaponRefitAdditionCfgMaster::GetCfg(const unsigned int uiColor)const
{
	if (uiColor <= 0
		|| uiColor > static_cast<unsigned int>(m_stCfg.weaponrefitadditioncfg_size()))
	{
		return NULL;
	}

	return &(m_stCfg.weaponrefitadditioncfg(uiColor-1));
}

int CWeaponRefitAdditionCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.weaponrefitadditioncfg_size(); iIdx++)
	{
		if (static_cast<unsigned int>(iIdx + 1) != m_stCfg.weaponrefitadditioncfg(iIdx).quality())
		{
			CV_WARNING(("%s", __PRETTY_FUNCTION__));
			return -1;
		}
	}

	return iRet;
}
