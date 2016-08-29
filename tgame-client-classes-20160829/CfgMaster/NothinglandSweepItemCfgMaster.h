#ifndef NothinglandSweepItemCfg_H__
#define NothinglandSweepItemCfg_H__

#include "NothinglandSweepItemCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CNothinglandSweepItemCfgMaster : public CCfgMasterBase<NothinglandSweepItemCfgSet>
{
public:

	CNothinglandSweepItemCfgMaster(){}
	~CNothinglandSweepItemCfgMaster(){}

	const NothinglandSweepItemCfg* GetNothinglandSweepItemCfgByID(const unsigned int uiCfgID);

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

#define NIHLITY_SWEEP_ITEM_CFG_MASTER (CVSingleton<CNothinglandSweepItemCfgMaster>::Instance())

#endif

