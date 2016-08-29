#include "GuideCfgMaster.h"

#include <set>
#include "defines.h"

using std::set;

bool GuideCfgMaster::isValid(const unsigned int uGuideID) {
	return m_stID2Idx.find(uGuideID) != m_stID2Idx.end();
}

const GuideCfg* GuideCfgMaster::getGuideCfg(const unsigned int uGuideID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uGuideID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, uGuideID));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_guidecfg(iter->second);
}

int GuideCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.guidecfg_size(); iIdx++)
	{
		const GuideCfg &stGuideCfg = m_stCfg.guidecfg(iIdx);
		unsigned int uGuideID = stGuideCfg.guideid();
		
		if(stCfgIDSet.insert(uGuideID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uGuideID));

			return -1; // id重复
		}

	}
	return iRet;
}

int GuideCfgMaster::SetCfg() {
	int iRet = 0;
	
	for(int iIdx = 0; iIdx < m_stCfg.guidecfg_size(); iIdx++)
	{
		const GuideCfg &stGuideCfg = m_stCfg.guidecfg(iIdx);
		unsigned int uGuideID = stGuideCfg.guideid();

		if(m_stID2Idx.insert(make_pair(uGuideID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uGuideID));

			return -1;//id重复
		}

	}

	return iRet;
}
