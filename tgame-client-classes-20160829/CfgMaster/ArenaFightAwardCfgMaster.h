#ifndef ArenaFightAwardCfgMaster_h__
#define ArenaFightAwardCfgMaster_h__

#include "ArenaFightAwardCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CArenaFightAwardCfgMaster : public CCfgMasterBase<ArenaFightAwardCfgSet>
{
public:
    CArenaFightAwardCfgMaster(){}
    ~CArenaFightAwardCfgMaster(){}

    const ArenaFightAwardCfg* GetAwardCfg(const unsigned int uiLv); 

protected:

    virtual int ValidateCfg()const;

};

#define ARENA_FIGHT_AWARD_CFG_MASTER (CVSingleton<CArenaFightAwardCfgMaster>::Instance())


#endif // ArenaFightAwardCfgMaster_h__
