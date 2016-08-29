#ifndef ArenaRuleCfgMaster_h__
#define ArenaRuleCfgMaster_h__

#include "CfgMasterBase.h"
#include "ArenaRuleCfg.pb.h"
#include "vlib/base/CVSingleton.h"

using namespace ::com::cfg::vo;
class CArenaRuleCfgMaster : public CCfgMasterBase<ArenaRuleCfgSet>
{
public:
	CArenaRuleCfgMaster(){}
	~CArenaRuleCfgMaster(){}

	const ArenaRuleCfg& GetCfg()const
	{
		return m_stCfg.arenarulecfg(0);
	}
};

#define ARENA_RULE_CFG (CVSingleton<CArenaRuleCfgMaster>::Instance())

#endif // ArenaRuleCfgMaster_h__