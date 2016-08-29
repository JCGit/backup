#include "WeaponStarStepCfgMaster.h"

#include <set>

using std::set;

int CWeaponStarStepCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<pair<unsigned int, unsigned int> > stItemIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.weaponstarstepcfg_size(); iIdx++)
	{
		pair<unsigned int, unsigned int> m_stIDX = make_pair(m_stCfg.weaponstarstepcfg(iIdx).star(), m_stCfg.weaponstarstepcfg(iIdx).step());
		if(stItemIDSet.insert(m_stIDX).second == false)
		{
			CV_ERROR(("%s, star = %u & step = %u is dup cfg id !",
				__PRETTY_FUNCTION__, m_stIDX.first, m_stIDX.second));
			return -1;//id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CWeaponStarStepCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.weaponstarstepcfg_size(); iIdx++)
	{
		pair<unsigned int, unsigned int> m_stIDX = make_pair(m_stCfg.weaponstarstepcfg(iIdx).star(), m_stCfg.weaponstarstepcfg(iIdx).step());
		if(m_stID2IDX.insert(make_pair(m_stIDX, iIdx)).second == false)
		{
			CV_ERROR(("%s, star = %u & step = %u is dup cfg id !",
				__PRETTY_FUNCTION__, m_stIDX.first, m_stIDX.second));
			return -1;//id重复
		}

		//得到星级对应的阶级数
		unsigned int uiStar = m_stCfg.weaponstarstepcfg(iIdx).star();
		map<unsigned int, unsigned int>::const_iterator it = m_stStarSteps.find(uiStar);
		if(it == m_stStarSteps.end()){
			m_stStarSteps[uiStar] = 0;
		}else{
			unsigned int uiStep = it->second;
			uiStep++;
			m_stStarSteps[uiStar] = uiStep;
		}

		//记录升星对应的下标
		if(m_stCfg.weaponstarstepcfg(iIdx).nextstar() != uiStar){
			m_stStar2IDX[uiStar] = iIdx;
		}
	}

	return iRet;
}

WeaponStarStepCfg* CWeaponStarStepCfgMaster::GetWeaponStarStepCfgByStarAndStep(const unsigned int star, const unsigned int step)
{
	pair<unsigned int, unsigned int> m_stIDX = make_pair(star, step);
	map<pair<unsigned int, unsigned int>, int>::const_iterator it = m_stID2IDX.find(m_stIDX);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfg by star = %u & step = %u", __PRETTY_FUNCTION__, m_stIDX.first, m_stIDX.second));
		return NULL; //找不到该物品配置
	}

	return m_stCfg.mutable_weaponstarstepcfg(it->second);
}


const WeaponStarStepCfg* CWeaponStarStepCfgMaster::GetWeaponStarMaterialsByStar(const unsigned int star){
	map<unsigned int, unsigned int>::const_iterator it = m_stStar2IDX.find(star);

	if(it == m_stStar2IDX.end())
	{
		CV_ERROR(("%s, can not find material by star = %u", __PRETTY_FUNCTION__, star));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.weaponstarstepcfg(it->second));
}