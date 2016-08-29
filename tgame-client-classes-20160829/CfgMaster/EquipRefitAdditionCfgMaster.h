#ifndef EquipRefitAdditionCfgMaster_h__
#define EquipRefitAdditionCfgMaster_h__

#include "vlib/base/CVSingleton.h"
#include "EquipRefitAdditionCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CEquipRefitAdditionCfgMaster : public CCfgMasterBase<EquipRefitAdditionCfgSet>
{
public:
	CEquipRefitAdditionCfgMaster(){

    }
	~CEquipRefitAdditionCfgMaster(){}

	const EquipRefitAdditionCfg*  GetCfg(const unsigned int uiColor)const;
    const unsigned int GetMaxQuality();
protected:

	virtual int ValidateCfg()const;
};

#define EQUIPREFITADDITION_CFG_MASTER (CVSingleton<CEquipRefitAdditionCfgMaster>::Instance())

#endif // EquipRefitAdditionCfgMaster_h__
