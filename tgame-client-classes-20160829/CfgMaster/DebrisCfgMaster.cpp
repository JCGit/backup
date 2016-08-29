#include "DebrisCfgMaster.h"
#include "BasicTypes.h"
#include <set>
#include "ClientSvrMsg.pb.h"
#include "WeaponCfgMaster.h"
#include "EquipmentCfgMaster.h"

using std::set;

int CDebrisCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    set<unsigned int> stIDSet;

    for(int iIdx = 0; iIdx < m_stCfg.debriscfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.debriscfg(iIdx).debrisid();
        if(stIDSet.insert(uiCfgID).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return ERET_SYS_ERR; 
        }

    }
    return iRet;
}

int CDebrisCfgMaster::SetCfg()
{
    int iRet = ERET_OK;
    
    m_stID2IDX.clear();
    m_stTypeIDListMap.clear();

    unsigned int uiCfgID = 0;
    int iThingType = 0;
    for(int iIdx = 0; iIdx < m_stCfg.debriscfg_size(); iIdx++)
    {
        uiCfgID = m_stCfg.debriscfg(iIdx).debrisid();
        if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return ERET_SYS_ERR; 
        }
        iThingType = m_stCfg.debriscfg(iIdx).composetype();
        m_stTypeIDListMap[iThingType].push_back(uiCfgID);
    }

    //sort
    for(auto iter = m_stTypeIDListMap.begin(); iter != m_stTypeIDListMap.end(); ++iter) {
        vector<int>& vec = iter->second;
        sort(vec.begin(), vec.end(), compareDebris);
    }

    return iRet;
}

const DebrisCfg* CDebrisCfgMaster::GetDebrisCfgByID(const unsigned int uiCfgID)
{
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return NULL; 
    }

    return m_stCfg.mutable_debriscfg(it->second);
}

const string CDebrisCfgMaster::getIconPath(const unsigned int uiCfgID) {
    const DebrisCfg* pCfg = GetDebrisCfgByID(uiCfgID);
    if(NULL == pCfg) {
        return "";
    }
    
    if(vmsg::THING_TYPE_WEAPON == pCfg->composetype()) {
        return "weaponicon/" + pCfg->debrisicon() + ".png";
    } else if(vmsg::THING_TYPE_EQUIP == pCfg->composetype()) {
        return ("item/" + pCfg->debrisicon() + ".png");
    } else {
        return "";
    }
    
}


const int CDebrisCfgMaster::getComposeThingStar(const unsigned int uiCfgID) {
    const DebrisCfg* pCfg = GetDebrisCfgByID(uiCfgID);
    if(NULL == pCfg) {
        return 0;
    }

    if(vmsg::THING_TYPE_WEAPON == pCfg->composetype()) {
        const WeaponCfg* pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(pCfg->composecfgid());
        if(pWeaponCfg) {
            return pWeaponCfg->initstar();
        }
    } else if(vmsg::THING_TYPE_EQUIP == pCfg->composetype()) {
        const EquipmentCfg* pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(pCfg->composecfgid());
        if(pEquipCfg) {
            return pEquipCfg->initstar();
        }
    } else {
        return 0;
    }


    return 0;
}

const vector<int>* CDebrisCfgMaster::getCfgIDListByType(const int iThingType) {
    if(m_stTypeIDListMap.count(iThingType)) {
        return &(m_stTypeIDListMap[iThingType]);
    }
    
    return NULL;
}


bool CDebrisCfgMaster::compareDebris(const int iACfgID, const int iBCfgID) {
    /*
    碎片对应枪械或装备星级越高，则碎片排序越靠前；
    */
    const int aStar = DEBRIS_CFG_MASTER->getComposeThingStar(iACfgID);
    const int bStar = DEBRIS_CFG_MASTER->getComposeThingStar(iBCfgID);

    return (aStar > bStar);
}


