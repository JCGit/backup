#ifndef DailyConsumeAwardCfgMaster_h__
#define DailyConsumeAwardCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "DailyConsumeAwardCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class DailyConsumeAwardCfgMaster : public CCfgMasterBase<DailyConsumeAwardCfgSet>
{
public:
	DailyConsumeAwardCfgMaster(){}
	~DailyConsumeAwardCfgMaster(){}

	const DailyConsumeAwardCfg* getDailyConsumeActCfg(unsigned int uLevel);
	unsigned int	getDailyConsumeTypeNum();


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

#define DAILYCONSUME_CFG_MASTER (CVSingleton<DailyConsumeAwardCfgMaster>::Instance())

#endif // DailyConsumeAwardCfgMaster_h__