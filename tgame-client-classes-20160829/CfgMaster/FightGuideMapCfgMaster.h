#ifndef FightGuideMapCfg_H__
#define FightGuideMapCfg_H__

#include "FightGuideMapCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CFightGuideMapCfgMaster : public CCfgMasterBase<FightGuideMapCfgSet>
{
public:

	CFightGuideMapCfgMaster(){}
	~CFightGuideMapCfgMaster(){}

	const FightGuideMapCfg* GetFightGuideMapCfgByID(const unsigned int uiCfgID);

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

#define FIGHT_GUIDE_MAP_CFG_MASTER (CVSingleton<CFightGuideMapCfgMaster>::Instance())

#endif

