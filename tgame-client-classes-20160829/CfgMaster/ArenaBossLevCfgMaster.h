#ifndef ArenaBossLevCfg_H__
#define ArenaBossLevCfg_H__

#include "ArenaBossLevCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CArenaBossLevCfgMaster : public CCfgMasterBase<ArenaBossLevCfgSet>
{
public:

	CArenaBossLevCfgMaster(){}
	~CArenaBossLevCfgMaster(){}

	const ArenaBossLevCfg* GetArenaBossLevCfgByLv(const unsigned int uiLv);

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

#define ARENA_BOSS_LEV_CFG (CVSingleton<CArenaBossLevCfgMaster>::Instance())

#endif

