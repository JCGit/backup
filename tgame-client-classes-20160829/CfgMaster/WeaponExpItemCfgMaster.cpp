#include "WeaponExpItemCfgMaster.h"

#include <set>

using std::set;

int CWeaponExpItemCfgMaster::ValidateCfg()const
{
    int iRet = 0;


    set<unsigned int> stItemIDSet;

    for(int iIdx = 0; iIdx < m_stCfg.weaponexpitemcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.weaponexpitemcfg(iIdx).itemcfgid();
        if(stItemIDSet.insert(uiCfgID).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return -1;//id重复
        }

        //to add other validate check, such as skill etc.
    }
    return iRet;
}

int CWeaponExpItemCfgMaster::SetCfg()
{
    int iRet = 0;

    for(int iIdx = 0; iIdx < m_stCfg.weaponexpitemcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.weaponexpitemcfg(iIdx).itemcfgid();
        if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return -1;//id重复
        }

		m_stAllExpItemID.push_back(m_stCfg.weaponexpitemcfg(iIdx).itemcfgid());
    }

    return iRet;
}

WeaponExpItemCfg* CWeaponExpItemCfgMaster::GetWeaponExpItemCfgByID(const unsigned int uiCfgID)
{
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return NULL; //找不到该物品配置
    }

    return m_stCfg.mutable_weaponexpitemcfg(it->second);
}

bool CWeaponExpItemCfgMaster::CheckItemIsExpItem(unsigned int uiCfgID){
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end()){
		return false;
	}

	return true;
}