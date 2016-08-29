#ifndef ActCfg_H__
#define ActCfg_H__

#include "ActCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CActCfgMaster : public CCfgMasterBase<ActCfgSet>
{
public:

	CActCfgMaster(){}
	~CActCfgMaster(){}

	const ActCfg* GetActCfgByType(const unsigned int uType);

	int getActCfgNumber();

	//id到下标的映射
	map<int ,unsigned int, CmpByKey> m_stID2IDX;
protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	map<unsigned int , int> m_stTypeID;
};

#define ACT_CFG_MASTER (CVSingleton<CActCfgMaster>::Instance())

#endif

