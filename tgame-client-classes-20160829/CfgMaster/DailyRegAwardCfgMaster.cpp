#include "DailyRegAwardCfgMaster.h"

int CDailyRegAwardCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.dailyregawardcfg_size(); iIdx++)
	{
		if(static_cast<unsigned int>(iIdx + 1) != m_stCfg.dailyregawardcfg(iIdx).day())
		{
			CV_ERROR(("%s, id %u is dup cfg id !",
				__PRETTY_FUNCTION__, m_stCfg.dailyregawardcfg(iIdx).day()));
			return ERET_SYS_ERR;//id÷ÿ∏¥
		}
	}
	return iRet;
}


const DailyRegAwardCfg* CDailyRegAwardCfgMaster::GetCfg(unsigned int uiDay)const
{
	if (uiDay <= 0
		|| uiDay > static_cast<unsigned int>(m_stCfg.dailyregawardcfg_size()))
	{
		return NULL;
	}

	return &(m_stCfg.dailyregawardcfg(uiDay-1));
}

unsigned int CDailyRegAwardCfgMaster::GetMaxDay()const
{
	return m_stCfg.dailyregawardcfg_size();
}
