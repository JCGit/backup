#ifndef PlayerGrowCfgMaster_h__
#define PlayerGrowCfgMaster_h__

#include "CfgMasterBase.h"
#include "PlayerGrowCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class PlayerGrowCfgMaster: public CCfgMasterBase<PlayerGrowCfgSet>
{
public:
	PlayerGrowCfgMaster() {};
	virtual ~PlayerGrowCfgMaster() {};

	const PlayerGrowCfg* getPlayerGrowCfg(const unsigned int uLevel);
	
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

#define PLAYER_GROW_CFG_MASTER CVLib::CVSingleton<PlayerGrowCfgMaster>::Instance()

#endif // PlayerGrowCfgMaster_h__