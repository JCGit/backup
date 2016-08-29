#include "PanelSoundCfgMaster.h"

#include <set>

using std::set;

int CPanelSoundCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.panelsoundcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.panelsoundcfg(iIdx).id();
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

int CPanelSoundCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.panelsoundcfg_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.panelsoundcfg(iIdx).id();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiCfgID));
			return -1;//id重复
		}
	}

	return iRet;
}

const PanelSoundCfg* CPanelSoundCfgMaster::GetPanelSoundCfgByID(const unsigned int uiCfgID)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.panelsoundcfg(it->second));
}

string CPanelSoundCfgMaster::getPanelOpenSound(const unsigned int uiSoundType){
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiSoundType);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, invalid type %u", __PRETTY_FUNCTION__, uiSoundType));
		return "";
	}

	return m_stCfg.panelsoundcfg(it->second).opensound();
}

string CPanelSoundCfgMaster::getPanelCloseSound(const unsigned int uiSoundType){
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiSoundType);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, invalid type %u", __PRETTY_FUNCTION__, uiSoundType));
		return "";
	}

	return m_stCfg.panelsoundcfg(it->second).closesound();
}
