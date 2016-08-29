#ifndef DotaItemCfgMaster_h__
#define DotaItemCfgMaster_h__

#include "CfgMasterBase.h"
#include "DotaItemCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class DotaItemCfgMaster: public CCfgMasterBase<DotaItemCfgSet>
{
public:
	DotaItemCfgMaster() {};
	virtual ~DotaItemCfgMaster() {};

	const DotaItemCfg* getDotaItemCfg(unsigned int uDotaItemID);
	
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;

};

#define DOTA_ITEM_CFG_MASTER CVLib::CVSingleton<DotaItemCfgMaster>::Instance()

#endif // DotaItemCfgMaster_h__