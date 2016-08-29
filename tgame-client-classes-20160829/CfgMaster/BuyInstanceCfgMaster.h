#ifndef BuyInstanceCfg_H__
#define BuyInstanceCfg_H__

#include "BuyInstanceCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CBuyInstanceCfgMaster : public CCfgMasterBase<BuyInstanceCfgSet>
{
public:

	CBuyInstanceCfgMaster(){}
	~CBuyInstanceCfgMaster(){}

	const BuyInstanceCfg* GetBuyCostCfgByCnt(const unsigned int uiCfgID);

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

#define BUY_INSTANCE_CFG_MASTER (CVSingleton<CBuyInstanceCfgMaster>::Instance())

#endif

