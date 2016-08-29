#include "DailyTaskAwardCfgMaster.h"

#include <set>

#include "defines.h"

using std::set;

DailyTaskAwardCfg* DailyTaskAwardCfgMaster::getDailyTaskAwardCfg(unsigned int uLevel, unsigned int uMainAwardType) {
	DailyTaskAwardCfg *pRet = NULL;

	for(int i = 0; i < m_stCfg.dailytaskawardcfg_size(); i++) {
		const DailyTaskAwardCfg &stDailyTaskAwardCfg = m_stCfg.dailytaskawardcfg(i);

		if(uLevel >= stDailyTaskAwardCfg.startlv()
		&& uLevel <= stDailyTaskAwardCfg.endlv()
		&& uMainAwardType == stDailyTaskAwardCfg.mainawardtype()) {
			pRet = m_stCfg.mutable_dailytaskawardcfg(i);
			break;
		}
	}
	
	return pRet;
}

int DailyTaskAwardCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<string> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.dailytaskawardcfg_size(); iIdx++)
	{
		const DailyTaskAwardCfg &stDailyTaskAwardCfg = m_stCfg.dailytaskawardcfg(iIdx);
		string key = intToString(stDailyTaskAwardCfg.startlv()) + "_" + intToString(stDailyTaskAwardCfg.endlv()) + "_" + intToString(stDailyTaskAwardCfg.mainawardtype());

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, startlv=%u endlv=%u is dup cfg key!", __PRETTY_FUNCTION__, stDailyTaskAwardCfg.startlv(), stDailyTaskAwardCfg.endlv()));

			return -1; // idÖØ¸´
		}

	}
	return iRet;
}

int DailyTaskAwardCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.dailytaskawardcfg_size(); iIdx++)
	{
		const DailyTaskAwardCfg &stDailyTaskAwardCfg = m_stCfg.dailytaskawardcfg(iIdx);
		string key = intToString(stDailyTaskAwardCfg.startlv()) + "_" + intToString(stDailyTaskAwardCfg.endlv()) + "_" + intToString(stDailyTaskAwardCfg.mainawardtype());

		if(m_stID2Idx.insert(make_pair(key, iIdx)).second == false)
		{
			CV_ERROR(("%s, startlv=%u endlv=%u is dup cfg key!", __PRETTY_FUNCTION__, stDailyTaskAwardCfg.startlv(), stDailyTaskAwardCfg.endlv()));

			return -1; // idÖØ¸´
		}
	}

	return iRet;
}
