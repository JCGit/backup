#include "StoryCfgMaster.h"
#include <set>

using std::set;

const StoryCfg* StoryCfgMaster::getStoryCfg(unsigned int uStoryID) {
	map<unsigned int, int>::const_iterator iter = m_stID2IDX.find(uStoryID);

	if(iter == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfg. StoryID=%u", __PRETTY_FUNCTION__, uStoryID));
		return NULL; //找不到相应的配置
	}

	return &(m_stCfg.storycfg(iter->second));
}

int StoryCfgMaster::ValidateCfg()const
{
	int iRet = 0;

	set<unsigned int> stStoryIDSet;

	for(int i = 0; i < m_stCfg.storycfg_size(); i++)
	{
		unsigned int uStoryID = m_stCfg.storycfg(i).storyid();

		if(stStoryIDSet.insert(uStoryID).second == false)
		{
			CV_ERROR(("%s, met a dup key! StoryID=%u", __PRETTY_FUNCTION__, uStoryID));
			iRet = -1; // 重复了
			break;
		}

	}

	return iRet;
}

int StoryCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int i = 0; i < m_stCfg.storycfg_size(); i++)
	{
		unsigned int uStoryID = m_stCfg.storycfg(i).storyid();

		if(m_stID2IDX.insert(make_pair(uStoryID, i)).second == false)
		{
			CV_ERROR(("%s, met a dup key! StoryID=%u", __PRETTY_FUNCTION__, uStoryID));
			iRet = -1; // 重复了
			break;
		}
	}

	return iRet;
}
