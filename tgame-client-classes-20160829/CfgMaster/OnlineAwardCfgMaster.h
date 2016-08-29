#ifndef OnlineAwardCfgMaster_h__
#define OnlineAwardCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "OnlinePKGAwardCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class OnlineAwardCfgMaster : public CCfgMasterBase<OnlinePKGCfgSet>
{
public:
	OnlineAwardCfgMaster(){}
	~OnlineAwardCfgMaster(){}

	const OnlinePKGCfg* getOnlinePKGCfg(unsigned int uLevel);
	unsigned int	getOnlineAwardTypeNum();


protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2IDX.clear(); 
	}

private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

};

#define OnlineAward_CFG_MASTER (CVSingleton<OnlineAwardCfgMaster>::Instance())

#endif // OnlineAwardCfgMaster_h__