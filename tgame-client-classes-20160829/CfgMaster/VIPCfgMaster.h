#ifndef VIPCfgMaster_h__
#define VIPCfgMaster_h__

#include "CfgMasterBase.h"
#include "VIPCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class VIPCfgMaster: public CCfgMasterBase<VIPCfgSet>
{
public:
	VIPCfgMaster() {};
	~VIPCfgMaster() {};

	VIPCfg* getVIPCfg(const unsigned int uVIPModuleID);
	
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

#define VIP_CFG_MASTER CVLib::CVSingleton<VIPCfgMaster>::Instance()

#endif // VIPCfgMaster_h__
