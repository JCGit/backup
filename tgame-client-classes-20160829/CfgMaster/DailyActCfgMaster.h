#ifndef DailyActCfg_H__
#define DailyActCfg_H__

#include "DailyActCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CDailyActCfgMaster : public CCfgMasterBase<DailyActCfgSet>
{
public:

	CDailyActCfgMaster(){}
	~CDailyActCfgMaster(){}

	const DailyActCfg* GetDailyActCfgByID(const unsigned int uiCfgID);

protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;
};

#define DAILY_ACT_CFG_MASTER (CVSingleton<CDailyActCfgMaster>::Instance())

#endif

