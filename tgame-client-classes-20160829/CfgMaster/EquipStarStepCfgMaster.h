#ifndef EquipStarStepCfgMaster_h__
#define EquipStarStepCfgMaster_h__

#include "CfgMasterBase.h"
#include "EquipStarStepCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CEquipStarStepCfgMaster : public CCfgMasterBase<EquipStarStepCfgSet>
{
public:

	CEquipStarStepCfgMaster(){}
	~CEquipStarStepCfgMaster(){}

	const EquipStarStepCfg* GetCfg(unsigned int uiStar, unsigned int uiStep)const;

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

private:
	//id到下标的映射
	map<pair<unsigned int, unsigned int>, int> m_stID2IDX;

};

#define EQUIPSTARSTEP_CFG_MASTER (CVSingleton<CEquipStarStepCfgMaster>::Instance())



#endif // EquipStarStepCfgMaster_h__
