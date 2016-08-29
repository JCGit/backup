#ifndef DailyTaskCfgMaster_h__
#define DailyTaskCfgMaster_h__

#include "CfgMasterBase.h"
#include "DailyTaskCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class DailyTaskCfgMaster: public CCfgMasterBase<DailyTaskCfgSet>
{
public:
	DailyTaskCfgMaster() {};
	~DailyTaskCfgMaster() {};

	DailyTaskCfg* getDailyTaskCfgByID(const unsigned int uCfgID);
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
};

#define DAILY_TASK_CFG_MASTER CVLib::CVSingleton<DailyTaskCfgMaster>::Instance()

#endif // DailyTaskCfgMaster_h__
