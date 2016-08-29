#ifndef WeaponExpItemCfgMaster_h__
#define WeaponExpItemCfgMaster_h__

#include "CfgMasterBase.h"
#include "WeaponExpItemCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CWeaponExpItemCfgMaster : public CCfgMasterBase<WeaponExpItemCfgSet>
{
public:
	CWeaponExpItemCfgMaster(){}
	~CWeaponExpItemCfgMaster(){}

	WeaponExpItemCfg* GetWeaponExpItemCfgByID(const unsigned int uiCfgID);

	bool CheckItemIsExpItem(unsigned int uiCfgID);		//检查item是否为可用于升级的物件
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stAllExpItemID.clear();
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

	//所有升级物件的id
	vector<unsigned int> m_stAllExpItemID;
};

#define WEAPON_EXP_CFG (CVSingleton<CWeaponExpItemCfgMaster>::Instance())

#endif // WeaponExpItemCfgMaster_h__