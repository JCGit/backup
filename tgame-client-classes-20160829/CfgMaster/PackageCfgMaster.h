#ifndef PackageCfgMaster_h__
#define PackageCfgMaster_h__

#include "CfgMasterBase.h"
#include "PackageCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CPackageCfgMaster : public CCfgMasterBase<PackageCfgSet>
{
public:
	CPackageCfgMaster(){}
	~CPackageCfgMaster(){}

	const PackageCfg* GetPackageCfgByID(const unsigned int uiCfgID);
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

#define PACKAGE_CFG_MASTER (CVSingleton<CPackageCfgMaster>::Instance())

#endif // PackageCfgMaster_h__