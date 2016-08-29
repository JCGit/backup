#ifndef WeaponTypeCfgMaster_h__
#define WeaponTypeCfgMaster_h__

#include "CfgMasterBase.h"
#include "WeaponTypeCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CWeaponTypeCfgMaster : public CCfgMasterBase<WeaponTypeCfgSet>
{
public:
	CWeaponTypeCfgMaster(){}
	~CWeaponTypeCfgMaster(){}

    const WeaponTypeCfg* GetWeaponTypeCfgByID(const unsigned int uiCfgID);
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//id到下标的映射
    map<unsigned int, int> m_stID2IDX; //<type, index>
};

#define WEAPON_TYPE_CFG (CVSingleton<CWeaponTypeCfgMaster>::Instance())

#endif // WeaponTypeCfgMaster_h__