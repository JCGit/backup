#ifndef DailyRegAwardCfgMaster_h__
#define DailyRegAwardCfgMaster_h__

#include "CfgMasterBase.h"
#include "DailyRegAwardCfg.pb.h"

using namespace com::cfg::vo;
using namespace std;

class CDailyRegAwardCfgMaster : public CCfgMasterBase<DailyRegAwardCfgSet>
{
public:

	CDailyRegAwardCfgMaster(){}
	~CDailyRegAwardCfgMaster(){}

	const DailyRegAwardCfg* GetCfg(unsigned int uiDay)const;
	unsigned int GetMaxDay()const;

protected:

	virtual int ValidateCfg()const;

};

#define DAILYREGAWARD_CFG_MASTER (CVSingleton<CDailyRegAwardCfgMaster>::Instance())


#endif // DailyRegAwardCfgMaster_h__
