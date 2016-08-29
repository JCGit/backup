#include "ArenaLvCfgMaster.h"
#include <set>

int CArenaLvCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    std::set<unsigned int> stSet;
    
    for(int iIdx = 0; iIdx < m_stCfg.arenalvcfg_size(); iIdx++)
    {
        unsigned int uiLevel = m_stCfg.arenalvcfg(iIdx).level();
        if (stSet.insert(uiLevel).second == false
            || static_cast<unsigned int>(iIdx + 1) != m_stCfg.arenalvcfg(iIdx).level())
        {
            CV_WARNING(("%s, invalid level %u", __PRETTY_FUNCTION__, uiLevel));
            return -1;
        }
    }

    return iRet;

}


int CArenaLvCfgMaster::GetLvByScore(const int iInScore, int& iRetLv)
{
    iRetLv = 1; //default
    for(int iIdx = 0; iIdx < m_stCfg.arenalvcfg_size(); iIdx++)
    {
        const int uiScore = m_stCfg.arenalvcfg(iIdx).score();
        if(iInScore <= uiScore) //???
        {
            iRetLv = m_stCfg.arenalvcfg(iIdx).level();
            return ERET_OK;
        }

    }
    //ret max lv
    int iSize = m_stCfg.arenalvcfg_size();
    iRetLv = m_stCfg.arenalvcfg(iSize - 1).level();
    return ERET_OK;
}

const ArenaLvCfg* CArenaLvCfgMaster::GetCfgByScore(const int iInScore)
{
	for(int iIdx = 0; iIdx < m_stCfg.arenalvcfg_size(); iIdx++)
	{
		const int uiScore = m_stCfg.arenalvcfg(iIdx).score();
		if(iInScore <= uiScore) //???
		{
			return &(m_stCfg.arenalvcfg(iIdx));
		}

	}

	//ret max lv
	int iSize = m_stCfg.arenalvcfg_size();
	return &(m_stCfg.arenalvcfg(iSize - 1));
}


int CArenaLvCfgMaster::GetLvRange(const int iInScore, int& iRetBeginScore, int& iRetEndScore)
{
    int iMyLv = 0;
    int iRet = ERET_OK;
    iRet = GetLvByScore(iInScore, iMyLv);
    if (1 == iMyLv)
    {
        iRetBeginScore = 0;
        iRetEndScore = m_stCfg.arenalvcfg(iMyLv - 1).score();
    }
    else if(m_stCfg.arenalvcfg_size() == iMyLv)
    {
        iRetBeginScore = m_stCfg.arenalvcfg(iMyLv - 1).score();
        iRetEndScore = 400;
    }
    else 
    {
        iRetBeginScore = m_stCfg.arenalvcfg(iMyLv - 2).score();
        iRetBeginScore = m_stCfg.arenalvcfg(iMyLv - 1).score();
    }


    return ERET_OK;
}

const ArenaLvCfg* CArenaLvCfgMaster::GetOneCfg(const int lev){
	if(lev <= 0 || lev > m_stCfg.arenalvcfg_size() )
	{
		CV_WARNING(("%s, invalid lv %u", __PRETTY_FUNCTION__, lev));
		return NULL;
	}

	return &(m_stCfg.arenalvcfg(lev - 1));
}
