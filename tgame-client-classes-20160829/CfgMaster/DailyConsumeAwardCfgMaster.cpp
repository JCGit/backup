#include "DailyConsumeAwardCfgMaster.h"
#include <set>

using std::set;

const DailyConsumeAwardCfg* DailyConsumeAwardCfgMaster::getDailyConsumeActCfg(unsigned int uLevel)
{
	map<unsigned int, int>::const_iterator iter = m_stID2IDX.find(uLevel);
	if (iter == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfg. StoryID=%u", __PRETTY_FUNCTION__, uLevel));
		return NULL; //找不到相应的配置
	}

	return &(m_stCfg.dailyconsumeawardcfg(iter->second));
}

int DailyConsumeAwardCfgMaster::ValidateCfg()const
{
	int iRet = 0;

	set<unsigned int> stFightGeekAwardSet;

	for(int i = 0; i < m_stCfg.dailyconsumeawardcfg_size(); i++)
	{
		unsigned int uLevel = m_stCfg.dailyconsumeawardcfg(i).level();

		if (stFightGeekAwardSet.insert(uLevel).second == false)
		{
			CV_ERROR(("%s, met a dup key! StoryID=%u", __PRETTY_FUNCTION__, uLevel));
			iRet = -1; // 重复了
			break;
		}
	}

	return iRet;
}

int DailyConsumeAwardCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int i = 0; i < m_stCfg.dailyconsumeawardcfg_size(); i++)
	{
		unsigned int uLevel = m_stCfg.dailyconsumeawardcfg(i).level();

		if (m_stID2IDX.insert(make_pair(uLevel, i)).second == false)
		{
			CV_ERROR(("%s, met a dup key! StoryID=%u", __PRETTY_FUNCTION__, uLevel));
			iRet = -1; // 重复了
			break;
		}
	}

	return iRet;
}

unsigned int DailyConsumeAwardCfgMaster::getDailyConsumeTypeNum()
{
	return m_stID2IDX.size();
}
