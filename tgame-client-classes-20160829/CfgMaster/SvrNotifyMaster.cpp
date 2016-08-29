#include "SvrNotifyMaster.h"

#include <set>

using std::set;

int SvrNotifyMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.servernotifycfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.servernotifycfg(iIdx).notifyid();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // idÖØ¸´
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int SvrNotifyMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.servernotifycfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.servernotifycfg(iIdx).notifyid();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//idÖØ¸´
		}
	}

	return iRet;
}


const ServerNotifyCfg* SvrNotifyMaster::getNotify(const int iNotifyID) {
    if(m_stID2Idx.count(iNotifyID) <= 0) {
        CV_WARNING(("[warn]SvrNotifyMaster::getNotify,fail to find cfg for notify id %d", iNotifyID));
        return NULL;
    }
    const int idx = m_stID2Idx[iNotifyID];
    return &(m_stCfg.servernotifycfg(idx));
}


