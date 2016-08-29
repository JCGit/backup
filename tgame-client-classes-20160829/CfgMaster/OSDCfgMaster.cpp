#include "OSDCfgMaster.h"

#include <set>

using std::set;

OSDCfg* OSDCfgMaster::getOSDCfgByID(const unsigned int uSceneID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uSceneID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, uSceneID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_osdcfg(iter->second);
}

OSDCfg* OSDCfgMaster::getPrevOSDCfg(unsigned int uCurSceneID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uCurSceneID);

	OSDCfg *pRet = NULL;

	if(iter != m_stID2Idx.end())
	{
		if(iter->second > 0) {
			pRet = m_stCfg.mutable_osdcfg(iter->second - 1);
		} else {
			pRet = m_stCfg.mutable_osdcfg(iter->second);
		}
	}

	return pRet;
}

OSDCfg* OSDCfgMaster::getNextOSDCfg(unsigned int uCurSceneID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uCurSceneID);

	OSDCfg *pRet = NULL;

	if(iter != m_stID2Idx.end())
	{
		if(iter->second < m_stCfg.osdcfg_size() - 1) {
			pRet = m_stCfg.mutable_osdcfg(iter->second + 1);
		} else {
			pRet = m_stCfg.mutable_osdcfg(iter->second);
		}
	}

	return pRet;
}

int OSDCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.osdcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.osdcfg(iIdx).sceneid();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int OSDCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.osdcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.osdcfg(iIdx).sceneid();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}
