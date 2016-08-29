#include "ArenaDailyAwardCfgMaster.h"
#include <set>

int CArenaDailyAwardCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    std::set<unsigned int> stSet;
    
    for(int iIdx = 0; iIdx < m_stCfg.arenadailyawardcfg_size(); iIdx++)
    {
        unsigned int uiLevel = m_stCfg.arenadailyawardcfg(iIdx).level();
        if (stSet.insert(uiLevel).second == false
            || static_cast<unsigned int>(iIdx + 1) != m_stCfg.arenadailyawardcfg(iIdx).level())
        {
            CV_WARNING(("%s, invalid level %u", __PRETTY_FUNCTION__, uiLevel));
            return -1;
        }
    }

    return iRet;
}


const ArenaDailyAwardCfg* CArenaDailyAwardCfgMaster::GetOneCfg(const int iLv)
{
    if(iLv <= 0 || iLv > m_stCfg.arenadailyawardcfg_size() )
    {
        CV_WARNING(("%s, invalid lv %u", __PRETTY_FUNCTION__, iLv));
        return NULL;
    }

    return &(m_stCfg.arenadailyawardcfg(iLv - 1));
}



