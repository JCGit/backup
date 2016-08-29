#ifndef GlobalCfgMaster_h__
#define GlobalCfgMaster_h__

#include "GlobalCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CGlobalCfg : public CCfgMasterBase<GlobalCfgSet>
{
public:
	CGlobalCfg(){}
	virtual ~CGlobalCfg(){}

public:
	const GlobalCfg& GetCfg()const
	{
		return m_stCfg.globalcfg(0);
	}
};

#define GLOBAL_CFG_MASTER (CVSingleton<CGlobalCfg>::Instance())
#endif // GlobalCfgMaster_h__