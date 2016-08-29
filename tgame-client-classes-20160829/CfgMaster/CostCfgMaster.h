#ifndef CostCfg_H__
#define CostCfg_H__

#include "CostCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CCostCfgMaster : public CCfgMasterBase<CostCfgSet>
{
public:

	CCostCfgMaster(){}
	~CCostCfgMaster(){}

	const CostCfg* GetCostCfgByCount(const unsigned int uiCfgID);

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

#define COST_CFG_MASTER (CVSingleton<CCostCfgMaster>::Instance())

#endif

