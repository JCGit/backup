#include "ActCfgMaster.h"
#include<iostream>
#include <set>

using std::set;
/*
typedef pair<unsigned int, int> PAIR; 
struct CmpByValue {  
	bool operator()(const PAIR& lhs, const PAIR& rhs) {  
		return lhs.second < rhs.second;  
	}  
};
*/

int CActCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.actcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.actcfg(iIdx).type();
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

int CActCfgMaster::SetCfg()
{
	int iRet = 0;
	
	for(int iIdx = 0; iIdx < m_stCfg.actcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.actcfg(iIdx).type();
		unsigned int pOrder = m_stCfg.actcfg(iIdx).order();
		if(m_stID2IDX.insert(make_pair(pOrder,uiCfgID)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
		m_stTypeID.insert(make_pair(uiCfgID, iIdx));
	}
	//std::sort(m_stID2IDX.begin(), m_stID2IDX.end(), CmpByValue() );
	return iRet;
}

const ActCfg* CActCfgMaster::GetActCfgByType(const unsigned int uType)
{
	/*
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uType);
	
	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u type", __PRETTY_FUNCTION__, uType));
		return NULL; //找不到该物品配置
	}
	*/
	for (auto it = m_stID2IDX.begin(); it != m_stID2IDX.end(); it++)
	{
		if (it->second == uType)
		{
			map<unsigned int, int>::const_iterator itType = m_stTypeID.find(uType);
			if (itType == m_stTypeID.end())
			{
				CV_ERROR(("%s, can not find cfgid = %u type", __PRETTY_FUNCTION__, uType));
				return NULL;
			}
			return &(m_stCfg.actcfg(itType->second));
		}
	}

	CV_ERROR(("%s, can not find cfgid = %u type", __PRETTY_FUNCTION__, uType));
	return NULL; //找不到该物品配置
}

int CActCfgMaster::getActCfgNumber()
{
	return m_stID2IDX.size();
}