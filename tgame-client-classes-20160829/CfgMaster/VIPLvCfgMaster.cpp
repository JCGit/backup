#include "VIPLvCfgMaster.h"

#include <set>

using std::set;

VIPLvCfg* VIPLvCfgMaster::getVIPLvCfg(const unsigned int uVIPLv) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uVIPLv);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find VIPLv = %u !", __PRETTY_FUNCTION__, uVIPLv));

		return NULL; // �Ҳ�����Ӧ����
	}

	return m_stCfg.mutable_viplvcfg(iter->second);
}

int VIPLvCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.viplvcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.viplvcfg(iIdx).viplv();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id�ظ�
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int VIPLvCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.viplvcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.viplvcfg(iIdx).viplv();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id�ظ�
		}
	}

	return iRet;
}
