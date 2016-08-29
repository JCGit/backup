#ifndef GenTypeCfg_H__
#define GenTypeCfg_H__

#include "GenTypeCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CGenTypeCfgMaster : public CCfgMasterBase<GenTypeCfgSet>
{
public:

	CGenTypeCfgMaster(){}
	~CGenTypeCfgMaster(){}

	const GenTypeCfg* GetCfgByType(const unsigned int uiCfgID);

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

#define GEN_TYPE_CFG_MASTER (CVSingleton<CGenTypeCfgMaster>::Instance())

#endif

