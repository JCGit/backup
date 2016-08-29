#ifndef SumChargeActCfgMaster_h__
#define SumChargeActCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "SumChargeActCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class SumChargeActCfgMaster : public CCfgMasterBase<SumChargeActCfgSet>
{
public:
	SumChargeActCfgMaster(){}
	~SumChargeActCfgMaster(){}

	const SumChargeActCfg* getSumChargeActCfg(unsigned int uLevel);
	unsigned int	getSumChargeTypeNum();


protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2IDX.clear(); 
	}

private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

};

#define SumChargeAct_CFG_MASTER (CVSingleton<SumChargeActCfgMaster>::Instance())

#endif // SumChargeActCfgMaster_h__