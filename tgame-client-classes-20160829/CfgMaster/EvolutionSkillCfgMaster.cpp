#include "EvolutionSkillCfgMaster.h"

#include <set>

using std::set;

EvolutionSkillCfg* EvolutionSkillCfgMaster::getEvolutionSkillCfgByID(const unsigned int uCfgID) {
	map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(uCfgID);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, uCfgID));

		return NULL; // �Ҳ�����Ӧ����
	}

	return m_stCfg.mutable_evolutionskillcfg(iter->second);
}

int EvolutionSkillCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.evolutionskillcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.evolutionskillcfg(iIdx).evolutionid();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id�ظ�
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int EvolutionSkillCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.evolutionskillcfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.evolutionskillcfg(iIdx).evolutionid();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id�ظ�
		}
	}

	return iRet;
}
