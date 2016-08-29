#ifndef __BATTLE_SYS_CFG_MASTER_H__
#define __BATTLE_SYS_CFG_MASTER_H__

#include "BattleSysCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CBattleSysCfgMaster : public CCfgMasterBase<BattleSysCfgSet>
{
public:

    CBattleSysCfgMaster(){}
    ~CBattleSysCfgMaster(){}

    BattleSysCfg* GetBattleCfgByID(const unsigned int uiCfgID);

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

#define BATTLE_SYS_CFG_MASTER (CVSingleton<CBattleSysCfgMaster>::Instance())

#endif

