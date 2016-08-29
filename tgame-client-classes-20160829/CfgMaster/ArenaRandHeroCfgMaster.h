#ifndef ArenaRandHeroCfg_H__
#define ArenaRandHeroCfg_H__

#include "ArenaRandHeroCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CArenaRandHeroCfgMaster : public CCfgMasterBase<ArenaRandHeroCfgSet>
{
public:

	CArenaRandHeroCfgMaster(){}
	~CArenaRandHeroCfgMaster(){}

public:
	const ArenaRandHeroCfg& GetCfg()const
	{
		return m_stCfg.arenarandherocfg(0);
	}
};

#define ARENA_RAND_HERO_CFG_MASTER (CVSingleton<CArenaRandHeroCfgMaster>::Instance())

#endif

