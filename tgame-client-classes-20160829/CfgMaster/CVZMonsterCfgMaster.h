#ifndef CVZMonsterCfgMaster_h__
#define CVZMonsterCfgMaster_h__

#include "CfgMasterBase.h"
#include "CVZMonsterCfg.pb.h"

using namespace com::cfg::vo;
using namespace std;

class CCVZMonsterCfgMaster : public CCfgMasterBase<CVZMonsterCfgSet>
{
public:

	CCVZMonsterCfgMaster(){}
	~CCVZMonsterCfgMaster(){}

	const CVZMonsterCfg* GetCfg(const unsigned int uiLevel)const;
	unsigned int GetMaxLv()const
	{
		return m_stCfg.cvzmonstercfg_size();
	}

protected:

	virtual int ValidateCfg()const;
};

#define CVZMONSTER_CFG_MASTER (CVSingleton<CCVZMonsterCfgMaster>::Instance())

#endif // CVZMonsterCfgMaster_h__
