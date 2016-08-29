#ifndef DailyChargeActCfgMaster_h__
#define DailyChargeActCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "DailyChargeAwardCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class DailyChargeActCfgMaster : public CCfgMasterBase<DailyChargeAwardCfgSet>
{
public:
	DailyChargeActCfgMaster(){}
	~DailyChargeActCfgMaster(){}

	const DailyChargeAwardCfg* getDailyChargeActCfg(unsigned int uLevel);
	unsigned int	getDailyChargeTypeNum();


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

#define DailyChargeAct_CFG_MASTER (CVSingleton<DailyChargeActCfgMaster>::Instance())

#endif // DailyChargeActCfgMaster_h__