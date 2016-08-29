#ifndef WorldBossShowLibCfg_H__
#define WorldBossShowLibCfg_H__

#include "WorldBossShowLibCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWorldBossShowLibCfgMaster : public CCfgMasterBase<WorldBossShowLibCfgSet>
{
public:

	CWorldBossShowLibCfgMaster(){}
	~CWorldBossShowLibCfgMaster(){}

	const WorldBossShowLibCfg* GetWorldBossCfgByDay(const unsigned int uDay);

protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;
};

#define WORLD_BOSS_SHOW_LIB_CFG (CVSingleton<CWorldBossShowLibCfgMaster>::Instance())

#endif

