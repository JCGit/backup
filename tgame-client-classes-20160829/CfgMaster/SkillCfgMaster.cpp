#include "SkillCfgMaster.h"

#include <set>

using std::set;

int CSkillCfgMaster::ValidateCfg()const
{
    int iRet = 0;


    set<unsigned int> stItemIDSet;

    for(int iIdx = 0; iIdx < m_stCfg.skillcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.skillcfg(iIdx).skillid();
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

int CSkillCfgMaster::SetCfg()
{
    int iRet = 0;

    for(int iIdx = 0; iIdx < m_stCfg.skillcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.skillcfg(iIdx).skillid();
        if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return -1;//id重复
        }
    }

    return iRet;
}

const SkillCfg* CSkillCfgMaster::GetSkillCfgByID(const unsigned int uiCfgID)
{
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_WARNING(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return NULL; //找不到该物品配置
    }

    return &(m_stCfg.skillcfg(it->second));
}
