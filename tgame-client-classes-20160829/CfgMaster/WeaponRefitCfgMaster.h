#ifndef WeaponRefitCfgMaster_h__
#define WeaponRefitCfgMaster_h__

#include "CfgMasterBase.h"
#include "WeaponRefitCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWeaponRefitCfgMaster : public CCfgMasterBase<WeaponRefitCfgSet>
{
public:

	CWeaponRefitCfgMaster(){}
	~CWeaponRefitCfgMaster(){}

	const WeaponRefitCfg* GetCfg(unsigned int uiWeaponTypeID, unsigned int uiColor)const;

	const WeaponRefitCfg* GetCfgByIdx(unsigned int idx);

	vector<int> getRefitArrByType(const unsigned int type);

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stTypeToIDX.clear();
	}
protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

private:
	map<pair<unsigned int, unsigned int>, int> m_stID2IDX;

	//类型对应下标数组
	map<int, vector<int> > m_stTypeToIDX;
};

#define WEAPONREFIT_CFG_MASTER (CVSingleton<CWeaponRefitCfgMaster>::Instance())

#endif // WeaponRefitCfgMaster_h__
