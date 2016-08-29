#ifndef WarmPromptCfgMaster_h__
#define WarmPromptCfgMaster_h__

#include "CfgMasterBase.h"
#include "WarmPromptCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class WarmPromptCfgMaster: public CCfgMasterBase<WarmPromptCfgSet>
{
public:
	WarmPromptCfgMaster() {};
	virtual ~WarmPromptCfgMaster() {};

	const WarmPromptCfg* getWarmPromptCfg(const unsigned int uID);

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

#define WARM_PROMPT_CFG_MASTER CVLib::CVSingleton<WarmPromptCfgMaster>::Instance()

#endif // WarmPromptCfgMaster_h__