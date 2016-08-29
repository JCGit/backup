#ifndef ArenaMapLibCfg_H__
#define ArenaMapLibCfg_H__

#include "ArenaMapLibCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CArenaMapLibCfgMaster : public CCfgMasterBase<ArenaMapLibCfgSet>
{
public:

	CArenaMapLibCfgMaster(){}
	~CArenaMapLibCfgMaster(){}

	const ArenaMapLibCfg* GetArenaMapLibCfgByID(const unsigned int uiCfgID);

	const ArenaMapLibCfg* GetArenaMapByLv(const unsigned int uLv);
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

#define ARENA_MAP_LIB_CFG_MASTER (CVSingleton<CArenaMapLibCfgMaster>::Instance())

#endif

