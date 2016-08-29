#ifndef MonsterLevCfgMaster_h__
#define MonsterLevCfgMaster_h__

#include "CfgMasterBase.h"
#include "MonsterLevCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CMonsterLevCfgMaster : public CCfgMasterBase<MonsterLevCfgSet>
{
public:
	CMonsterLevCfgMaster(){}
	~CMonsterLevCfgMaster(){}

	MonsterLevCfg* GetMonsterLevCfgByLev(const unsigned int level);
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

#define MONSTER_LEV_CFG (CVSingleton<CMonsterLevCfgMaster>::Instance())

#endif // MonsterLevCfgMaster_h__