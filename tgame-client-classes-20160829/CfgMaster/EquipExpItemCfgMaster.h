#ifndef EquipExpItemCfgMaster_h__
#define EquipExpItemCfgMaster_h__

#include "CfgMasterBase.h"
#include "EquipExpItemCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CEquipExpItemCfgMaster : public CCfgMasterBase<EquipExpItemCfgSet>
{
public:

	CEquipExpItemCfgMaster(){}
	~CEquipExpItemCfgMaster(){}

	const EquipExpItemCfg* GetEquipExpItemCfg(const unsigned int uiCfgID)const;

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

};

#define EQUIPEXPITEM_CFG_MASTER (CVSingleton<CEquipExpItemCfgMaster>::Instance())


#endif // EquipExpItemCfgMaster_h__
