#ifndef ArenaBuyNumCfgMaster_h__
#define ArenaBuyNumCfgMaster_h__

#include "ArenaBuyNumCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CArenaBuyNumCfgMaster : public CCfgMasterBase<ArenaBuyNumCfgSet>
{
public:
    CArenaBuyNumCfgMaster(){}
    ~CArenaBuyNumCfgMaster(){}

    int GetGold(const int uiNum, unsigned int& uiGold);

protected:

    virtual int ValidateCfg()const;

};

#define ARENA_BUY_NUM_CFG_MASTER (CVSingleton<CArenaBuyNumCfgMaster>::Instance())


#endif // ArenaBuyNumCfgMaster_h__
