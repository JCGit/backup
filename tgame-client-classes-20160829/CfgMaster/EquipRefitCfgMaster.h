#ifndef EquipRefitCfgMaster_h__
#define EquipRefitCfgMaster_h__

#include "CfgMasterBase.h"
#include "EquipRefitCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CEquipRefitCfgMaster : public CCfgMasterBase<EquipRefitCfgSet>
{
public:

	CEquipRefitCfgMaster(){}
	~CEquipRefitCfgMaster(){}

	const EquipRefitCfg* GetCfg(unsigned int uiEquipTypeID, unsigned int uiColor)const;

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

private:
	map<pair<unsigned int, unsigned int>, int> m_stID2IDX;

};

#define EQUIPREFIT_CFG_MASTER (CVSingleton<CEquipRefitCfgMaster>::Instance())



#endif // EquipRefitCfgMaster_h__
