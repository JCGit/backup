#include "DailyLoginActCfgMaster.h"
#include <set>

using std::set;

const DailyLoginAwardCfg* DailyLoginActCfgMaster::getDailyLoginActCfg(unsigned int uLevel)
{
	map<unsigned int, int>::const_iterator iter = m_stID2IDX.find(uLevel);
	if (iter == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfg. StoryID=%u", __PRETTY_FUNCTION__, uLevel));
		return NULL; //找不到相应的配置
	}

	return &(m_stCfg.dailyloginawardcfg(iter->second));
}

int DailyLoginActCfgMaster::ValidateCfg()const
{
	int iRet = 0;

	set<unsigned int> stDailyLoginSet;

	for(int i = 0; i < m_stCfg.dailyloginawardcfg_size(); i++)
	{
		unsigned int uDay = m_stCfg.dailyloginawardcfg(i).day();

		if (stDailyLoginSet.insert(uDay).second == false)
		{
			CV_ERROR(("%s, met a dup key! StoryID=%u", __PRETTY_FUNCTION__, uDay));
			iRet = -1; // 重复了
			break;
		}
	}

	return iRet;
}

int DailyLoginActCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int i = 0; i < m_stCfg.dailyloginawardcfg_size(); i++)
	{
		unsigned int uDay = m_stCfg.dailyloginawardcfg(i).day();

		if (m_stID2IDX.insert(make_pair(uDay, i)).second == false)
		{
			CV_ERROR(("%s, met a dup key! StoryID=%u", __PRETTY_FUNCTION__, uDay));
			iRet = -1; // 重复了
			break;
		}
	}

	return iRet;
}

const DailyLoginAwardCfg* DailyLoginActCfgMaster::getBestDailyLoginActCfg()
{
	int _size = m_stCfg.dailyloginawardcfg_size();
	return getDailyLoginActCfg(_size);
}

unsigned int DailyLoginActCfgMaster::getDailyLoginTypeNum()
{
	return m_stID2IDX.size();
}
