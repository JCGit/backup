#ifndef WorldBossLvCfg_H__
#define WorldBossLvCfg_H__

#include "WorldBossLvCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWorldBossLvCfgMaster : public CCfgMasterBase<WorldBossLvCfgSet>
{
public:

	CWorldBossLvCfgMaster(){}
	~CWorldBossLvCfgMaster(){}

	const WorldBossLvCfg* GetWorldBossCfgByLv(const unsigned int uLv);

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

#define WORLD_BOSS_LV_CFG (CVSingleton<CWorldBossLvCfgMaster>::Instance())

#endif

