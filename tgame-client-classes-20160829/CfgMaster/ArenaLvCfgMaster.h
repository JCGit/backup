#ifndef ArenaLvCfgMaster_h__
#define ArenaLvCfgMaster_h__

#include "ArenaLvCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class CArenaLvCfgMaster : public CCfgMasterBase<ArenaLvCfgSet>
{
public:
    CArenaLvCfgMaster(){}
    ~CArenaLvCfgMaster(){}

    int GetLvByScore(const int iInScore, int& iRetLv);
	const ArenaLvCfg* GetCfgByScore(const int iInScore);
	const ArenaLvCfg* GetOneCfg(const int lev);
    int GetLvRange(const int iInScore, int& iRetBeginScore, int& iRetEndScore);//获取匹配对手时，搜索的分数段
protected:

    virtual int ValidateCfg()const;

};

#define ARENA_LV_CFG_MASTER (CVSingleton<CArenaLvCfgMaster>::Instance())


#endif // ArenaLvCfgMaster_h__
