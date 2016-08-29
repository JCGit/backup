#ifndef WeaponLevelCfgMaster_h__
#define WeaponLevelCfgMaster_h__

#include "CfgMasterBase.h"
#include "WeaponLevelCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CWeaponLevelCfgMaster : public CCfgMasterBase<WeaponLevelCfgSet>
{
public:
	CWeaponLevelCfgMaster(){
        m_iMaxLv = 0;
    }
	~CWeaponLevelCfgMaster(){}

	WeaponLevelCfg* GetWeaponLevelCfgByID(const unsigned int uiCfgID);
    const int GetMaxLevel();
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

    int m_iMaxLv;
};

#define WEAPON_LEVEL_CFG (CVSingleton<CWeaponLevelCfgMaster>::Instance())

#endif // WeaponLevelCfgMaster_h__