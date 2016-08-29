#ifndef MonsterCfgMaster_h__
#define MonsterCfgMaster_h__

#include "CfgMasterBase.h"
#include "MonsterCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CMonsterCfgMaster : public CCfgMasterBase<MonsterCfgSet>
{
public:
	CMonsterCfgMaster(){}
	~CMonsterCfgMaster(){}

	MonsterCfg* GetMonsterCfgByID(const unsigned int uiCfgID);

	int GetRandSpeed(MonsterCfg& stCfg);
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

#define  MONSTER_CFG_MASTER (CVSingleton<CMonsterCfgMaster>::Instance())

#endif // MonsterCfgMaster_h__
