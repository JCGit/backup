#ifndef WeaponArrayEffCfg_H__
#define WeaponArrayEffCfg_H__

#include "WeaponArrayEffCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWeaponArrayEffCfgMaster : public CCfgMasterBase<WeaponArrayEffCfgSet>
{
public:

	CWeaponArrayEffCfgMaster(){}
	~CWeaponArrayEffCfgMaster(){}

	const WeaponArrayEffCfg* GetWeaponArrayEffCfgByID(const unsigned int uiCfgID);

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

#define WEAPON_ARR_EFF_CFG_MASTER (CVSingleton<CWeaponArrayEffCfgMaster>::Instance())

#endif

