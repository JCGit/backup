#include "CVZMonsterCfgMaster.h"


int CCVZMonsterCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.cvzmonstercfg_size(); iIdx++)
	{
		unsigned int uiLevel= m_stCfg.cvzmonstercfg(iIdx).level();
		if(uiLevel != static_cast<unsigned int>(iIdx + 1))
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiLevel));
			return -1;//id÷ÿ∏¥
		}
	}
	return iRet;
}

const CVZMonsterCfg* CCVZMonsterCfgMaster::GetCfg(unsigned int uiLevel)const
{
	if (uiLevel <= 0
		|| uiLevel > static_cast<unsigned int>(m_stCfg.cvzmonstercfg_size()))
	{
		CV_ERROR(("%s, id %u is not exist !",
			__PRETTY_FUNCTION__, uiLevel));
		return  NULL;
	}

	return &(m_stCfg.cvzmonstercfg(uiLevel-1));
}
