#ifndef NameCfgMaster_h__
#define NameCfgMaster_h__

#include "CfgMasterBase.h"
#include "NameCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
using namespace std;
class CNameCfgMaster : public CCfgMasterBase<NameCfgSet>
{
public:
	CNameCfgMaster(){}
	~CNameCfgMaster(){}

	const NameCfg* GetNameCfgByID(const unsigned int uiCfgID);

	const NameCfg* GetRandCfg();

	const string GetRandName();
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

#define NAME_CFG_MASTER (CVSingleton<CNameCfgMaster>::Instance())

#endif // NameCfgMaster_h__