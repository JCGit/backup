#ifndef VIPAwardCfgMaster_h__
#define VIPAwardCfgMaster_h__

#include "CfgMasterBase.h"
#include "VIPAwardCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class VIPAwardCfgMaster: public CCfgMasterBase<VIPAwardCfgSet>
{
public:
	VIPAwardCfgMaster() {};
	~VIPAwardCfgMaster() {};

	VIPAwardCfg* getVIPAwardCfg(const unsigned int uPlayerLv, const unsigned int uVIPLv);
	
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
	//map<unsigned int, int> m_stID2Idx;
	map<string, int> m_stID2Idx;
};

#define VIP_AWARD_CFG_MASTER CVLib::CVSingleton<VIPAwardCfgMaster>::Instance()

#endif // VIPAwardCfgMaster_h__
