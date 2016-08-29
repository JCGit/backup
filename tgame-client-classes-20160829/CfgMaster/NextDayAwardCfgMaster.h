#ifndef NextDayAwardCfgMaster_h__
#define NextDayAwardCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "NextDayAwardCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class NextDayAwardCfgMaster : public CCfgMasterBase<NextDayCfgSet>
{
public:
	NextDayAwardCfgMaster(){}
	~NextDayAwardCfgMaster(){}

	const NextDayCfg* getNextDayAwardActCfg(unsigned int uLevel);
	unsigned int	getNextDayAwardTypeNum();


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

#define NextDayAward_CFG_MASTER (CVSingleton<NextDayAwardCfgMaster>::Instance())

#endif // NextDayAwardCfgMaster_h__