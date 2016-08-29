#ifndef VIPLvCfgMaster_h__
#define VIPLvCfgMaster_h__

#include "CfgMasterBase.h"
#include "VIPLvCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class VIPLvCfgMaster: public CCfgMasterBase<VIPLvCfgSet>
{
public:
	VIPLvCfgMaster() {};
	~VIPLvCfgMaster() {};

	VIPLvCfg* getVIPLvCfg(const unsigned int uVIPLv);
	
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

#define VIP_LV_CFG_MASTER CVLib::CVSingleton<VIPLvCfgMaster>::Instance()

#endif // VIPLvCfgMaster_h__
