#ifndef FightKillNumCfg_H__
#define FightKillNumCfg_H__

#include "FightKillNumCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CFightKillNumCfgMaster : public CCfgMasterBase<FightKillNumCfgSet>
{
public:

	CFightKillNumCfgMaster(){}
	~CFightKillNumCfgMaster(){}

	const FightKillNumCfg* GeCfgByKillNum(const unsigned int uNum);

protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;
};

#define FIGHT_KILL_NUM_CFG (CVSingleton<CFightKillNumCfgMaster>::Instance())

#endif

