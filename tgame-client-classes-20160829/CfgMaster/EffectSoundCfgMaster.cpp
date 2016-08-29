#include "EffectSoundCfgMaster.h"

#include <set>

using std::set;

int CEffectSoundCfgMaster::ValidateCfg()const
{
    int iRet = 0;


    set<unsigned int> stItemIDSet;

    for(int iIdx = 0; iIdx < m_stCfg.effsoundcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.effsoundcfg(iIdx).effectid();
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

int CEffectSoundCfgMaster::SetCfg()
{
    int iRet = 0;

    for(int iIdx = 0; iIdx < m_stCfg.effsoundcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.effsoundcfg(iIdx).effectid();
	
        if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return -1;//id重复
        }

		
		if(m_stName2IDX.insert(make_pair(m_stCfg.effsoundcfg(iIdx).effecturl(), iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
    }

    return iRet;
}

EffectSoundCfg* CEffectSoundCfgMaster::GetSoundCfgByID(const unsigned int uiCfgID)
{
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return NULL; //找不到该物品配置
    }

    return m_stCfg.mutable_effsoundcfg(it->second);
}


EffectSoundCfg* CEffectSoundCfgMaster::GetSoundCfgBySkillUrl(string stSkillUrl){
	map<string, int>::const_iterator it = m_stName2IDX.find(stSkillUrl);

	if(it == m_stName2IDX.end())
	{
		CV_ERROR(("%s, can not find name = %s item", __PRETTY_FUNCTION__, stSkillUrl.c_str()));
		return NULL; //找不到该物品配置
	}

	return m_stCfg.mutable_effsoundcfg(it->second);
}