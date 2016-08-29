#include "GuideStepCfgMaster.h"

#include <set>
#include "defines.h"

using std::set;

const vector<GuideStepCfg*>& GuideStepCfgMaster::getSteps(const unsigned int uGuideID) {
	return m_stGuides.at(uGuideID);
}

const GuideStepCfg* GuideStepCfgMaster::getGuideStepCfg(const unsigned int uGuideID, const unsigned int uStepID) {
	string key = intToString(uGuideID) + "_" + intToString(uStepID);

	map<string, int>::const_iterator iter = m_stID2Idx.find(key);

	if(iter == m_stID2Idx.end())
	{
		CV_ERROR(("%s, can not find cfgid = %s !", __PRETTY_FUNCTION__, key.c_str()));

		return NULL; // 找不到相应配置
	}

	return m_stCfg.mutable_guidestepcfg(iter->second);
}

int GuideStepCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<string> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.guidestepcfg_size(); iIdx++)
	{
		const GuideStepCfg &stGuideStepCfg = m_stCfg.guidestepcfg(iIdx);
		string key = intToString(stGuideStepCfg.guideid()) + "_" + intToString(stGuideStepCfg.stepid());

		if(stCfgIDSet.insert(key).second == false)
		{
			CV_ERROR(("%s, cfgid = %s is dup cfg id !", __PRETTY_FUNCTION__, key.c_str()));

			return -1; // id重复
		}

	}
	return iRet;
}

int GuideStepCfgMaster::SetCfg() {
	int iRet = 0;
	unsigned int uGuideID = 0;

	for(int iIdx = 0; iIdx < m_stCfg.guidestepcfg_size(); iIdx++)
	{
		const GuideStepCfg &stGuideStepCfg = m_stCfg.guidestepcfg(iIdx);
		string key = intToString(stGuideStepCfg.guideid()) + "_" + intToString(stGuideStepCfg.stepid());

		if(m_stID2Idx.insert(make_pair(key, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %s is dup cfg id !", __PRETTY_FUNCTION__, key.c_str()));

			return -1;//id重复
		}

		if(stGuideStepCfg.guideid() != uGuideID) {
			uGuideID = stGuideStepCfg.guideid();
			vector<GuideStepCfg*> steps;
			m_stGuides.insert(make_pair(uGuideID, steps));
		}

		m_stGuides.at(uGuideID).push_back(m_stCfg.mutable_guidestepcfg(iIdx));
	}

	return iRet;
}
