#ifndef ArenaKillNumCfg_H__
#define ArenaKillNumCfg_H__

#include "ArenaKillNumCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CArenaKillNumCfgMaster : public CCfgMasterBase<ArenaKillNumCfgSet>
{
public:

	CArenaKillNumCfgMaster(){}
	~CArenaKillNumCfgMaster(){}

	const ArenaKillNumCfg* GetCfgByKillNum(const unsigned int uKillNum);

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

#define ARENA_KILL_NUM_CFG (CVSingleton<CArenaKillNumCfgMaster>::Instance())

#endif

