#include "NounCfgMaster.h"


int CNounCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stEquipIDSet;
	
	for(int iIdx = 0; iIdx < m_stCfg.nouncfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nouncfg(iIdx).key();
		if(stEquipIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__FUNCTION__, uiCfgID));
			return -1;//idÖØ¸´
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CNounCfgMaster::SetCfg()
{
	int iRet = 0;
	
	for(int iIdx = 0; iIdx < m_stCfg.nouncfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.nouncfg(iIdx).key();
        m_stMap[uiCfgID] = m_stCfg.nouncfg(iIdx).value();
		
	}
	
	return iRet;
}

std::string CNounCfgMaster::GetStr(const int iKey)
{
    if(0 == m_stMap.count(iKey)) {
        return "";
    }
    return m_stMap[iKey];
}


