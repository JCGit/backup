#include "AIParamCfgMaster.h"

#include <set>

using std::set;

int CAIParamCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.aiparamcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.aiparamcfg(iIdx).id();
		if(stItemIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id�ظ�
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CAIParamCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.aiparamcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.aiparamcfg(iIdx).id();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id�ظ�
		}
	}

	return iRet;
}

const AIParamCfg* CAIParamCfgMaster::GetAICfgByID(const unsigned int uCfgID)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uCfgID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find AI = %u cfgID", __PRETTY_FUNCTION__, uCfgID));
		return NULL; //�Ҳ�������Ʒ����
	}

	return &(m_stCfg.aiparamcfg(it->second));
}
