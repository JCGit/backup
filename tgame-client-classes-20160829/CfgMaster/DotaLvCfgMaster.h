#ifndef DotaLvCfgMaster_h__
#define DotaLvCfgMaster_h__

#include "CfgMasterBase.h"
#include "DotaLvCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class DotaLvCfgMaster: public CCfgMasterBase<DotaLvCfgSet>
{
public:
	DotaLvCfgMaster() {};
	virtual ~DotaLvCfgMaster() {};

	const DotaLvCfg* getDotaLvCfg(unsigned int uLevel, unsigned int uPart);
	
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

private:
	// id到下标的映射
	//map<unsigned int, int> m_stID2Idx;
	map<string, int> m_stID2Idx;

};

#define DOTA_LV_CFG_MASTER CVLib::CVSingleton<DotaLvCfgMaster>::Instance()

#endif // DotaLvCfgMaster_h__