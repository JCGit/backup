#include "ArenaBuyNumCfgMaster.h"
#include <set>

int CArenaBuyNumCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    std::set<unsigned int> stSet;

    for(int iIdx = 0; iIdx < m_stCfg.arenabuynumcfg_size(); iIdx++)
    {
        unsigned int uiLevel = m_stCfg.arenabuynumcfg(iIdx).num();
        if (stSet.insert(uiLevel).second == false
            || static_cast<unsigned int>(iIdx + 1) != m_stCfg.arenabuynumcfg(iIdx).num())
        {
            CV_WARNING(("%s, invalid num %u", __PRETTY_FUNCTION__, uiLevel));
            return -1;
        }
    }

    return iRet;
}


int CArenaBuyNumCfgMaster::GetGold(const int uiNum, unsigned int& uiGold)
{
    if (uiNum <= 0)
    {
        CV_WARNING(("%s, invalid uiNum %u", __PRETTY_FUNCTION__, uiNum));
        return -1;
    }

    int iMaxSize = m_stCfg.arenabuynumcfg_size();
    if (uiNum >= iMaxSize)
    {
        uiGold = m_stCfg.arenabuynumcfg(iMaxSize - 1).gold();
    }
    else 
    {
        uiGold = m_stCfg.arenabuynumcfg(uiNum - 1).gold();
    }

    return ERET_OK;
}

