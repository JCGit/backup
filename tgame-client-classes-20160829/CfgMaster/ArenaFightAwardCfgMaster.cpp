#include "ArenaFightAwardCfgMaster.h"
#include <set>

int CArenaFightAwardCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    std::set<unsigned int> stSet;
    
    for(int iIdx = 0; iIdx < m_stCfg.arenafightawardcfg_size(); iIdx++)
    {
        unsigned int uiLevel = m_stCfg.arenafightawardcfg(iIdx).level();
        if (stSet.insert(uiLevel).second == false
            || static_cast<unsigned int>(iIdx + 1) != m_stCfg.arenafightawardcfg(iIdx).level())
        {
            CV_WARNING(("%s, invalid level %u", __PRETTY_FUNCTION__, uiLevel));
            return -1;
        }
    }

    return iRet;
}


const ArenaFightAwardCfg* CArenaFightAwardCfgMaster::GetAwardCfg(const unsigned int uiLv)
{
    if (uiLv <= 0)
    {
        CV_WARNING(("%s, invalid lv %u", __PRETTY_FUNCTION__, uiLv));
        return NULL;
    }
    unsigned int iTmpLv = static_cast<unsigned int>(m_stCfg.arenafightawardcfg_size());
    if (uiLv <= iTmpLv)
    {
        iTmpLv = uiLv;
    }


    return &(m_stCfg.arenafightawardcfg(uiLv - 1));
}


