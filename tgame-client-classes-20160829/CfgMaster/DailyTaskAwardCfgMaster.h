#ifndef DailyTaskAwardCfgMaster_h__
#define DailyTaskAwardCfgMaster_h__

#include "CfgMasterBase.h"
#include "DailyTaskAwardCfg.pb.h"

using namespace com::cfg::vo;
using namespace std;

class DailyTaskAwardCfgMaster: public CCfgMasterBase<DailyTaskAwardCfgSet>
{
public:
	DailyTaskAwardCfgMaster() {};
	~DailyTaskAwardCfgMaster() {};

	DailyTaskAwardCfg* getDailyTaskAwardCfg(unsigned int uLevel, unsigned int uMainAwardType);
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	map<string, int> m_stID2Idx;

};

#define DAILY_TASK_AWARD_CFG_MASTER CVLib::CVSingleton<DailyTaskAwardCfgMaster>::Instance()

#endif // DailyTaskAwardCfgMaster_h__
