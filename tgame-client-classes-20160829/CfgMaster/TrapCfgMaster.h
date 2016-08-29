#ifndef __TRAP_CFG_MASTER_H__
#define __TRAP_CFG_MASTER_H__

#include "TrapCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CTrapCfgMaster : public CCfgMasterBase<TrapCfgSet>
{
public:

	CTrapCfgMaster(){}
	~CTrapCfgMaster(){}

	const TrapCfg* GetTrapCfgByID(const unsigned int uiCfgID);

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

#define TRAP_CFG_MASTER (CVSingleton<CTrapCfgMaster>::Instance())

#endif

