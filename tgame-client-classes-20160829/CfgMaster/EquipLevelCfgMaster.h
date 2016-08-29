#ifndef EquipLevelCfgMaster_h__
#define EquipLevelCfgMaster_h__

#include "CfgMasterBase.h"
#include "EquipLevelCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CEquipLevelCfgMaster : public CCfgMasterBase<EquipLevelCfgSet>
{
public:

	CEquipLevelCfgMaster(){}
	~CEquipLevelCfgMaster(){}

	const EquipLevelCfg* GetCfg(const unsigned int uiEquipLevel)const;
	unsigned int GetMaxLevel()const
	{
		return m_stCfg.equiplevelcfg_size();
	}

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

#define EQUIPLEVEL_CFG_MASTER (CVSingleton<CEquipLevelCfgMaster>::Instance())

#endif // EquipLevelCfgMaster_h__
