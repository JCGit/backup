#ifndef WeaponLotteryCfgMaster_h__
#define WeaponLotteryCfgMaster_h__

#include "WeaponLotteryCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;
using namespace std;

class CWeaponLotteryCfgMaster : public CCfgMasterBase<WeaponLotteryCfgSet>
{
public:

    CWeaponLotteryCfgMaster(){}
    ~CWeaponLotteryCfgMaster(){}

    const WeaponLotteryCfg* GetWeaponLotteryCfg(const int iLotType)const;

protected:

    virtual int ValidateCfg()const;

    virtual int SetCfg();


private:

};

//探秘必然获得物品配置
#define WEAPON_LOT_CFG_MASTER (CVSingleton<CWeaponLotteryCfgMaster>::Instance())

#endif // WeaponLotteryCfgMaster_h__
