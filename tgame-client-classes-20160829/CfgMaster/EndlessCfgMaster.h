#ifndef EndlessCfgMaster_h__
#define EndlessCfgMaster_h__

#include "CfgMasterBase.h"
#include "EndlessCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CEndlessCfgMaster : public CCfgMasterBase<EndlessCfgSet>
{
public:
	CEndlessCfgMaster(){}
	virtual ~CEndlessCfgMaster(){}

public:
	const EndlessCfg& GetCfg()const
	{
		return m_stCfg.endlesscfg(0);
	}
};

#define END_LESS_CFG (CVSingleton<CEndlessCfgMaster>::Instance())

#endif // EndlessCfgMaster_h__