#ifndef WeaponArrayOpenCfg_H__
#define WeaponArrayOpenCfg_H__

#include "WeaponArrayOpenCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWeaponArrayOpenCfgMaster : public CCfgMasterBase<WeaponArrayOpenCfgSet>
{
public:

	CWeaponArrayOpenCfgMaster(){}
	~CWeaponArrayOpenCfgMaster(){}

	const WeaponArrayOpenCfg* GetWeaponArrayOpenCfgByID(const unsigned int uiCfgID);

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

#define WEAPON_ARR_OPEN_CFG_MASTER (CVSingleton<CWeaponArrayOpenCfgMaster>::Instance())

#endif

