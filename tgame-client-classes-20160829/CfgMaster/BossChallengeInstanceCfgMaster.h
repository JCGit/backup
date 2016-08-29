#ifndef BossChallengeInstanceCfg_H__
#define BossChallengeInstanceCfg_H__

#include "BossChallengeInstanceCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CBossChallengeInstanceCfgMaster : public CCfgMasterBase<BossChallengeCfgSet>
{
public:

	CBossChallengeInstanceCfgMaster(){}
	~CBossChallengeInstanceCfgMaster(){}

	const BossChallengeCfg* GetBossChallengeCfgByType(const unsigned int uiCfgID);

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

#define BOSS_CHALLENGE_CFG (CVSingleton<CBossChallengeInstanceCfgMaster>::Instance())

#endif

