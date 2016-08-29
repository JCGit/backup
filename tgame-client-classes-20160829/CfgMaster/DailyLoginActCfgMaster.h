#ifndef DailyLoginActCfgMaster_h__
#define DailyLoginActCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "DailyLoginAwardCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class DailyLoginActCfgMaster : public CCfgMasterBase<DailyLoginAwardCfgSet>
{
public:
	DailyLoginActCfgMaster(){}
	~DailyLoginActCfgMaster(){}

	const DailyLoginAwardCfg* getDailyLoginActCfg(unsigned int uLevel);
	const DailyLoginAwardCfg* getBestDailyLoginActCfg();

	unsigned int	getDailyLoginTypeNum();



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

#define DailyLoginAct_CFG_MASTER (CVSingleton<DailyLoginActCfgMaster>::Instance())

#endif // DailyLoginActCfgMaster_h__