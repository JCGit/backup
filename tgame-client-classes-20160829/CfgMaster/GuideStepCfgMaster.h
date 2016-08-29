#ifndef GuideStepCfgMaster_h__
#define GuideStepCfgMaster_h__

#include "CfgMasterBase.h"
#include "GuideStepCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class GuideStepCfgMaster: public CCfgMasterBase<GuideStepCfgSet>
{
public:
	GuideStepCfgMaster() {};
	~GuideStepCfgMaster() {};

	const vector<GuideStepCfg*>& getSteps(const unsigned int uGuideID);
	const GuideStepCfg* getGuideStepCfg(const unsigned int uGuideID, const unsigned int uStepID);

protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
        m_stGuides.clear();
	}

	// override end

private:
	// id到下标的映射
	//map<unsigned int, int> m_stID2Idx;
	map<string, int> m_stID2Idx;
	map<unsigned int, vector<GuideStepCfg*> > m_stGuides;
};

#define GUIDE_STEP_CFG_MASTER CVLib::CVSingleton<GuideStepCfgMaster>::Instance()

#endif // GuideStepCfgMaster_h__
