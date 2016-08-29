#ifndef GuideCfgMaster_h__
#define GuideCfgMaster_h__

#include "CfgMasterBase.h"
#include "GuideCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class GuideCfgMaster: public CCfgMasterBase<GuideCfgSet>
{
public:
	GuideCfgMaster() {};
	~GuideCfgMaster() {};

	bool isValid(const unsigned int uGuideID);
	const GuideCfg* getGuideCfg(const unsigned int uGuideID);
	
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

#define GUIDE_CFG_MASTER CVLib::CVSingleton<GuideCfgMaster>::Instance()

#endif // GuideCfgMaster_h__
