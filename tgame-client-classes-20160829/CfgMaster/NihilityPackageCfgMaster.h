#ifndef NihilityPackageCfg_H__
#define NihilityPackageCfg_H__

#include "NihilityPackageCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CNihilityPackageCfgMaster : public CCfgMasterBase<NihilityPackageCfgSet>
{
public:

	CNihilityPackageCfgMaster(){}
	~CNihilityPackageCfgMaster(){}

	const NihilityPackageCfg* GetNihilityPackageCfgByID(const unsigned int uiCfgID);
	const NihilityPackageCfg* GetNihilityPackageCfgByLv(const unsigned int uLv);
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

#define NIHILITY_PACKAGE_CFG_MASTER (CVSingleton<CNihilityPackageCfgMaster>::Instance())

#endif

