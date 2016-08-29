#ifndef MilestoneCfgMaster_h__
#define MilestoneCfgMaster_h__

#include "CfgMasterBase.h"
#include "MilestoneCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class MilestoneCfgMaster: public CCfgMasterBase<MilestoneCfgSet>
{
public:
	MilestoneCfgMaster() {};
	~MilestoneCfgMaster() {};

	MilestoneCfg* getMilestoneCfgByID(const unsigned int uCfgID);
	
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

#define MILESTONE_CFG_MASTER CVLib::CVSingleton<MilestoneCfgMaster>::Instance()

#endif // MilestoneCfgMaster_h__
