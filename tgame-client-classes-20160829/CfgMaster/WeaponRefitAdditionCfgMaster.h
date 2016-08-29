#ifndef WeaponRefitAdditionCfgMaster_h__
#define WeaponRefitAdditionCfgMaster_h__

#include "vlib/base/CVSingleton.h"
#include "WeaponRefitAdditionCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CWeaponRefitAdditionCfgMaster : public CCfgMasterBase<WeaponRefitAdditionCfgSet>
{
public:
	CWeaponRefitAdditionCfgMaster(){}
	~CWeaponRefitAdditionCfgMaster(){}

	const WeaponRefitAdditionCfg*  GetCfg(const unsigned int uiColor)const;

	int GetCfgSize(){
		return m_stCfg.weaponrefitadditioncfg_size();
	}

protected:

	virtual int ValidateCfg()const;

};

#define WEAPONREFITADDITION_CFG_MASTER (CVSingleton<CWeaponRefitAdditionCfgMaster>::Instance())

#endif // WeaponRefitAdditionCfgMaster_h__
