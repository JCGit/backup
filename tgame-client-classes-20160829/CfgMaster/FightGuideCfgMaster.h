#ifndef __FIGHT_GUIDE_CFG_H__
#define __FIGHT_GUIDE_CFG_H__

#include "FightGuideCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CFightGuideCfgMaster : public CCfgMasterBase<FightGuideCfgSet>
{
public:

	CFightGuideCfgMaster(){}
	~CFightGuideCfgMaster(){}

	const FightGuideCfg* GetFightGuideCfgByID(const unsigned int uiCfgID);

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

#define FIGHT_GUIDE_CFG (CVSingleton<CFightGuideCfgMaster>::Instance())

#endif

