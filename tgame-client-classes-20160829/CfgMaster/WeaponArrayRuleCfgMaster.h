#ifndef WeaponArrayRuleCfg_H__
#define WeaponArrayRuleCfg_H__

#include "WeaponArrayRuleCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWeaponArrayRuleCfgMaster : public CCfgMasterBase<WeaponArrayRuleCfgSet>
{
public:

	CWeaponArrayRuleCfgMaster(){}
	~CWeaponArrayRuleCfgMaster(){}

	const WeaponArrayRuleCfg* GetRuleCfgByID(const unsigned int uiCfgID);

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

#define WEAPON_ARR_RULE_CFG_MASTER (CVSingleton<CWeaponArrayRuleCfgMaster>::Instance())

#endif

