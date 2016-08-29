#include "EquipStarStepCfgMaster.h"
#include <set>

using std::set;

int CEquipStarStepCfgMaster::ValidateCfg()const
{
	int iRet = ERET_OK;

	set<pair<unsigned int, unsigned int> > stIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.equipstarstepcfg_size(); iIdx++)
	{
		unsigned int uiStar = m_stCfg.equipstarstepcfg(iIdx).star();
		unsigned int uiStep = m_stCfg.equipstarstepcfg(iIdx).step();
		pair<unsigned int, unsigned int> pairKey = make_pair(uiStar, uiStep);
		if(stIDSet.insert(pairKey).second == false)
		{
			CV_ERROR(("%s, star %u step %u is dup cfg id !",
				__PRETTY_FUNCTION__, uiStar, uiStep));
			return ERET_SYS_ERR;//id÷ÿ∏¥
		}
	}
	return iRet;
}

int CEquipStarStepCfgMaster::SetCfg()
{
	int iRet = ERET_OK;

	for(int iIdx = 0; iIdx < m_stCfg.equipstarstepcfg_size(); iIdx++)
	{
		pair<unsigned int, unsigned int> pairKey = make_pair(m_stCfg.equipstarstepcfg(iIdx).star(), m_stCfg.equipstarstepcfg(iIdx).step());
		if(m_stID2IDX.insert(make_pair(pairKey, iIdx)).second == false)
		{
			CV_ERROR(("%s, star %u step %u is dup cfg id !",
				__PRETTY_FUNCTION__, m_stCfg.equipstarstepcfg(iIdx).star(), m_stCfg.equipstarstepcfg(iIdx).step()));
			return ERET_SYS_ERR;
		}
	}

	return iRet;
}

const EquipStarStepCfg* CEquipStarStepCfgMaster::GetCfg(unsigned int uiStar, unsigned int uiStep)const
{
	map<pair<unsigned int, unsigned int>, int>::const_iterator it = m_stID2IDX.find(make_pair(uiStar, uiStep));

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find star %u step %u item", __PRETTY_FUNCTION__, uiStar, uiStep));
		return NULL; 
	}

	return &(m_stCfg.equipstarstepcfg(it->second));
}
