#ifndef SceneCfgMaster_h__
#define SceneCfgMaster_h__

#include "CfgMasterBase.h"
#include "SceneCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class SceneCfgMaster: public CCfgMasterBase<SceneCfgSet>
{
public:
	SceneCfgMaster() {};
	~SceneCfgMaster() {};

	SceneCfg* getSceneCfgByID(const unsigned int uCfgID);
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
};

#define SCENE_CFG_MASTER CVLib::CVSingleton<SceneCfgMaster>::Instance()

#endif // SceneCfgMaster_h__
