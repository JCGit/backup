#include "NPCCfgMaster.h"

#include <set>

using std::set;

NPCCfg* NPCCfgMaster::getNPCCfgByID(const unsigned int uCfgID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uCfgID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, uCfgID));

		return NULL; // �Ҳ�����Ӧ����
	}

	return m_stCfg.mutable_npccfg(iter->second);
}

int NPCCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.npccfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.npccfg(iIdx).npcid();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id�ظ�
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int NPCCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.npccfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.npccfg(iIdx).npcid();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id�ظ�
		}
	}

	return iRet;
}
