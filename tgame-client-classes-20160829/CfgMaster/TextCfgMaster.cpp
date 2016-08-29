#include "TextCfgMaster.h"


int CTextCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stEquipIDSet;
	
	for(int iIdx = 0; iIdx < m_stCfg.textcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.textcfg(iIdx).key();
		if(stEquipIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__FUNCTION__, uiCfgID));
			return -1;//id�ظ�
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CTextCfgMaster::SetCfg()
{
	int iRet = 0;
	
	for(int iIdx = 0; iIdx < m_stCfg.textcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.textcfg(iIdx).key();
        m_stMap[uiCfgID] = m_stCfg.textcfg(iIdx).value();
		
	}
	
	return iRet;
}
/*
std::string CTextCfgMaster::GetStr(const int iKey)
{
    if(0 == m_stMap.count(iKey)) {
        return "";
    }
    return m_stMap[iKey];
}
*/

//����Ҫƴ���ַ����Ļ������������������û���ַ�������
const char* CTextCfgMaster::GetCstr(const int iKey)
{
    if(0 == m_stMap.count(iKey)) {
        return "";
    }
    return m_stMap[iKey].c_str();
}

