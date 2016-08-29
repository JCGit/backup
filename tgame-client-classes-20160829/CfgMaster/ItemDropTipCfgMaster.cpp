#include "ItemDropTipCfgMaster.h"
#include <set>
#include "WarMapCfgMaster.h"
#include "EliteWarMapCfgMaster.h"

int ItemDropTipCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    std::set<unsigned int> stSet;
    int iThingType = 0;
    int iThingID = 0;

    set<pair<unsigned int, unsigned int> > stIDSet;

    for(int iIdx = 0; iIdx < m_stCfg.itemdroptipcfg_size(); iIdx++)
    {
        iThingID = m_stCfg.itemdroptipcfg(iIdx).thingid();
        iThingType = m_stCfg.itemdroptipcfg(iIdx).thingtype();

        pair<int, int> pairKey = make_pair(iThingID, iThingType);
        if(stIDSet.insert(pairKey).second == false)
        {
            CV_ERROR(("%s,id %d's thing id %u,type %u is dup cfg id !",
                __PRETTY_FUNCTION__, m_stCfg.itemdroptipcfg(iIdx).id(), iThingID, iThingType  ));
            return ERET_SYS_ERR;//id÷ÿ∏¥
        }
    }

    return iRet;
}

int ItemDropTipCfgMaster::SetCfg()
{
    int iRet = ERET_OK;

    int iThingType = 0;
    int iThingID = 0;

    for(int iIdx = 0; iIdx < m_stCfg.itemdroptipcfg_size(); iIdx++)
    {
        iThingID = m_stCfg.itemdroptipcfg(iIdx).thingid();
        iThingType = m_stCfg.itemdroptipcfg(iIdx).thingtype();

        pair<int, int> pairKey = make_pair(iThingID, iThingType);
        if(m_stCfgMap.insert(make_pair(pairKey, iIdx)).second == false)
        {
            CV_ERROR(("%s,id %d's thing id %u,type %u is dup cfg id !",
                __PRETTY_FUNCTION__, m_stCfg.itemdroptipcfg(iIdx).id(), iThingID, iThingType  ));
            return ERET_SYS_ERR;//id÷ÿ∏¥
        }
    }


    return iRet;
}

const ItemDropTipCfg* ItemDropTipCfgMaster::GetOneCfg(const int iThingID, const int iThingType) 
{

    map<pair<int, int>, int>::const_iterator it = m_stCfgMap.find(make_pair(iThingID, iThingType));

    if(it == m_stCfgMap.end())
    {
        CV_ERROR(("%s, can not find thingid %u thingtype %u item", __PRETTY_FUNCTION__, iThingID, iThingType));
        return NULL; 
    }

    return &(m_stCfg.itemdroptipcfg(it->second));
}

string ItemDropTipCfgMaster::getWarName(const int iWarType, const int iWarID) {
    if(WAR_TYPE_NORMAL == iWarType) {
        const WarMapCfg* pCfg = WAR_MAP_CFG->GetWarMapCfgByID(iWarID);
        if(pCfg) {
            return pCfg->name();
        }
    } else if(WAR_TYPE_ELITE == iWarType) {
        const EliteWarMapCfg* pCfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(iWarID);
        if(pCfg) {
            return pCfg->name();
        }
    }
    return "";
}

