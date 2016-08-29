#ifndef ConstantCfg_H__
#define ConstantCfg_H__

#include "ConstantCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CConstantCfgMaster : public CCfgMasterBase<ConstantCfgSet>
{
public:

	CConstantCfgMaster(){}
	~CConstantCfgMaster(){}

	const ConstantCfg* GetConstantCfgByID(const unsigned int uiCfgID);

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

#define CONSTANT_CFG_MASTER (CVSingleton<CConstantCfgMaster>::Instance())

#endif

