#ifndef __WEAPON_BUFF_CFG_H__
#define __WEAPON_BUFF_CFG_H__

#include "WeaponBuffCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWeaponBuffCfgMaster : public CCfgMasterBase<WeaponBuffCfgSet>
{
public:

	CWeaponBuffCfgMaster(){}
	~CWeaponBuffCfgMaster(){}

	const WeaponBuffCfg* GetWeaponBuffCfgByID(const unsigned int uiCfgID);

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

#define WEAPON_BUFF_CFG (CVSingleton<CWeaponBuffCfgMaster>::Instance())

#endif

