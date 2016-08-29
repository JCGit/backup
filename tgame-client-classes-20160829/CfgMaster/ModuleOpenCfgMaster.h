#ifndef ModuleOpenCfgMaster_h__
#define ModuleOpenCfgMaster_h__

#include "CfgMasterBase.h"
#include "ModuleOpenCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class ModuleOpenCfgMaster: public CCfgMasterBase<ModuleOpenCfgSet>
{
public:
	ModuleOpenCfgMaster() {};
	~ModuleOpenCfgMaster() {};

	ModuleOpenCfg* getModuleOpenCfg(const unsigned int uModuleID);
	ModuleOpenCfg* getModuleOpenCfgByIndex( int uIndex);
	int getModuleOpenCfgNumb();
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

#define MODULE_OPEN_CFG_MASTER CVLib::CVSingleton<ModuleOpenCfgMaster>::Instance()

#endif // ModuleOpenCfgMaster_h__
