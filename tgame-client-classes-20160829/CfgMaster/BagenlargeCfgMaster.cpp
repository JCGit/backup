#include "BagenlargeCfgMaster.h"
//#include "BagRetVal.h"
#include <set>


unsigned int CBagenlargeCfgMaster::GetGold(const unsigned uiIdxFrom, const unsigned int uiIdxTo)
{
    unsigned int uiNeedCostCoin = 0;
    
    if (uiIdxFrom > uiIdxTo
        && uiIdxTo < static_cast<unsigned int>(m_stCfg.bagenlarge1_size()))
    {
        CV_WARNING(("%s, invalid param", __FUNCTION__));
        return 0;
    }

    for (unsigned int uiIdx = uiIdxFrom; uiIdx <= uiIdxTo; uiIdx++)
    {
        uiNeedCostCoin += m_stCfg.bagenlarge1(uiIdx).price();
    }
    
    return uiNeedCostCoin;
   
}

int CBagenlargeCfgMaster::ValidateCfg()const
{
    int iRet = 0;
    
    std::set<unsigned int> stSet;

    for(int iIdx = 0; iIdx < m_stCfg.bagenlarge1_size(); iIdx++)
    {
        unsigned int uiBagIdx = m_stCfg.bagenlarge1(iIdx).bagidx();
        if (stSet.insert(uiBagIdx).second == false
            || static_cast<unsigned int>(iIdx) != m_stCfg.bagenlarge1(iIdx).bagidx())
        {
            CV_WARNING(("%s, invalid bag idx %u", __FUNCTION__, uiBagIdx));
            return -1;
        }
    }
    
    return iRet;
}
