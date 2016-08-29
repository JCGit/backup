#ifndef SumConsumeActCfgMaster_h__
#define SumConsumeActCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "SumConsumeActCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class SumConsumeActCfgMaster : public CCfgMasterBase<SumConsumeActCfgSet>
{
public:
	SumConsumeActCfgMaster(){}
	~SumConsumeActCfgMaster(){}

	const SumConsumeActCfg* getSumConsumeActCfg(unsigned int uLevel);
	unsigned int	getSumConsumeAwardTypeNum();


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

#define SumConsumeAct_CFG_MASTER (CVSingleton<SumConsumeActCfgMaster>::Instance())

#endif // SumConsumeActCfgMaster_h__