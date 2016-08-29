#include "ArenaMatchCostCfgMaster.h"
#include <set>

int CArenaMatchCostCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    std::set<unsigned int> stSet;
    
    for(int iIdx = 0; iIdx < m_stCfg.arenamatchcostcfg_size(); iIdx++)
    {
        unsigned int uiNum = m_stCfg.arenamatchcostcfg(iIdx).num();
        if (stSet.insert(uiNum).second == false
            || static_cast<unsigned int>(iIdx + 1) != m_stCfg.arenamatchcostcfg(iIdx).num())
        {
            CV_WARNING(("%s, invalid num %u", __PRETTY_FUNCTION__, uiNum));
            return -1;
        }
    }

    return iRet;
}


int CArenaMatchCostCfgMaster::GetCostCoin(const int uiNum, unsigned int& uiCoin)
{
    if (uiNum <= 0)
    {
        CV_WARNING(("%s, invalid uiNum %u", __PRETTY_FUNCTION__, uiNum));
        return -1;
    }

    int iMaxSize = m_stCfg.arenamatchcostcfg_size();
    if (uiNum >= iMaxSize)
    {
        uiCoin = m_stCfg.arenamatchcostcfg(iMaxSize - 1).coin();
    }
    else 
    {
        uiCoin = m_stCfg.arenamatchcostcfg(uiNum - 1).coin();
    }

    return ERET_OK;
}

