#include "WeaponRefitCfgMaster.h"
#include <set>

using std::set;

int CWeaponRefitCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	set<pair<unsigned int, unsigned int> > stIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.weaponrefitcfg_size(); iIdx++)
	{
		unsigned int uiWeaponTypeID = m_stCfg.weaponrefitcfg(iIdx).weapontypeid();
		unsigned int uiColor = m_stCfg.weaponrefitcfg(iIdx).color();
		pair<unsigned int, unsigned int> pairKey = make_pair(uiWeaponTypeID, uiColor);
		if(stIDSet.insert(pairKey).second == false)
		{
			CV_ERROR(("%s, weapontypeid %u color %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiWeaponTypeID, uiColor));
			return -1;//id÷ÿ∏¥
		}

	}
	return iRet;
}

int CWeaponRefitCfgMaster::SetCfg()
{
	int iRet = ERET_OK;

	map<int, vector<int> >::iterator stIter;

	for(int iIdx = 0; iIdx < m_stCfg.weaponrefitcfg_size(); iIdx++)
	{
		pair<unsigned int, unsigned int> pairKey = make_pair(m_stCfg.weaponrefitcfg(iIdx).weapontypeid(), m_stCfg.weaponrefitcfg(iIdx).color());
		if(m_stID2IDX.insert(make_pair(pairKey, iIdx)).second == false)
		{
			CV_ERROR(("%s, weapontypeid %u color %u is dup cfg id !",
				__PRETTY_FUNCTION__, m_stCfg.weaponrefitcfg(iIdx).weapontypeid(), m_stCfg.weaponrefitcfg(iIdx).color()));
			return -1;
		}

		unsigned int type = m_stCfg.weaponrefitcfg(iIdx).weapontypeid();
		stIter = m_stTypeToIDX.find(type);

		if(stIter == m_stTypeToIDX.end()){
			m_stTypeToIDX[type].clear();
			m_stTypeToIDX[type].push_back(iIdx);
		}else{
			m_stTypeToIDX[type].push_back(iIdx);
		}
	}

	return iRet;
}

const WeaponRefitCfg* CWeaponRefitCfgMaster::GetCfg(unsigned int uiWeaponTypeID, unsigned int uiColor)const
{
	map<pair<unsigned int, unsigned int>, int>::const_iterator it = m_stID2IDX.find(make_pair(uiWeaponTypeID, uiColor));

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find weapontypeid %u color %u item", __PRETTY_FUNCTION__, uiWeaponTypeID, uiColor));
		return NULL; 
	}

	return &(m_stCfg.weaponrefitcfg(it->second));
}

vector<int> CWeaponRefitCfgMaster::getRefitArrByType(const unsigned int type){
	map<int, vector<int> >::iterator stIter = m_stTypeToIDX.find(type);
	vector<int> stVec;
	stVec.clear();

	if(stIter == m_stTypeToIDX.end()){
		CV_ERROR(("%s, can not find type = %u refit", __PRETTY_FUNCTION__, type));
		return stVec;
	}

	return stIter->second;
}

const WeaponRefitCfg* CWeaponRefitCfgMaster::GetCfgByIdx(unsigned int idx){
	if(idx < 0 || idx > (unsigned int)m_stCfg.weaponrefitcfg_size()){
		return NULL;
	}

	return &(m_stCfg.weaponrefitcfg(idx));
}

