#ifndef ArenaDailyAwardCfgMaster_h__
#define ArenaDailyAwardCfgMaster_h__

#include "ArenaDailyAwardCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CArenaDailyAwardCfgMaster : public CCfgMasterBase<ArenaDailyAwardCfgSet>
{
public:
    CArenaDailyAwardCfgMaster(){}
    ~CArenaDailyAwardCfgMaster(){}

    const ArenaDailyAwardCfg* GetOneCfg(const int iLv);

protected:
    virtual int ValidateCfg()const;

};

#define ARENA_DAILY_AWARD_CFG_MASTER (CVSingleton<CArenaDailyAwardCfgMaster>::Instance())


#endif // ArenaDailyAwardCfgMaster_h__
