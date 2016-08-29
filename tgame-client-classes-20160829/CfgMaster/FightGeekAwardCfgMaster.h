#ifndef FightGeedAwardCfgMaster_h__
#define FightGeedAwardCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "FightGeekAwardCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class FightGeekAwardCfgMaster : public CCfgMasterBase<FightGeekAwardCfgSet>
{
public:
	FightGeekAwardCfgMaster(){}
	~FightGeekAwardCfgMaster(){}

	const FightGeekAwardCfg* getFightGeekAwardActCfg(unsigned int uLevel);
	unsigned int	getFightGeekAwardTypeNum();


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

#define FightGeekAward_CFG_MASTER (CVSingleton<FightGeekAwardCfgMaster>::Instance())

#endif // FightGeedAwardCfgMaster_h__