#ifndef ArenaMatchCostCfgMaster_h__
#define ArenaMatchCostCfgMaster_h__

#include "ArenaMatchCostCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CArenaMatchCostCfgMaster : public CCfgMasterBase<ArenaMatchCostCfgSet>
{
public:
    CArenaMatchCostCfgMaster(){}
    ~CArenaMatchCostCfgMaster(){}


    int GetCostCoin(const int uiNum, unsigned int& uiCoin);
protected:

    virtual int ValidateCfg()const;

};

#define ARENA_MATCH_COST_CFG_MASTER (CVSingleton<CArenaMatchCostCfgMaster>::Instance())


#endif // ArenaMatchCostCfgMaster_h__
