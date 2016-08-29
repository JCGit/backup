#ifndef TaskCfgMaster_h__
#define TaskCfgMaster_h__

#include "CfgMasterBase.h"
#include "TaskCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class TaskCfgMaster: public CCfgMasterBase<TaskCfgSet>
{
public:
	TaskCfgMaster() {};
	~TaskCfgMaster() {};

	bool isValid(unsigned int uTaskID);

	const TaskCfg* getTaskCfgByID(unsigned int uTaskID);

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

#define TASK_CFG_MASTER CVLib::CVSingleton<TaskCfgMaster>::Instance()

#endif // TaskCfgMaster_h__
