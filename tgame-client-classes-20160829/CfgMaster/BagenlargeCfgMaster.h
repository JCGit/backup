#ifndef BagenlargeCfgMaster_h__
#define BagenlargeCfgMaster_h__

#include "CfgMasterBase.h"
#include "BagenlargeCfg.pb.h"

using namespace com::cfg::vo;

class CBagenlargeCfgMaster : public CCfgMasterBase<BagenlargeCfgSet>
{
public:

    CBagenlargeCfgMaster(){}
    ~CBagenlargeCfgMaster(){}

    unsigned int GetGold(const unsigned uiIdxFrom, const unsigned int uiIdxTo);

protected:

    int ValidateCfg()const;

};

#define BAGENLARGE_CFG_MASTER (CVSingleton<CBagenlargeCfgMaster>::Instance())


#endif // BagenlargeCfgMaster_h__
