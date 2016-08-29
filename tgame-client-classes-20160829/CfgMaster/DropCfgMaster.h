#ifndef DropCfgMaster_h__
#define DropCfgMaster_h__

#include "DropCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CDropCfgMaster : public CCfgMasterBase<DropCfgSet>
{
public:

	CDropCfgMaster(){}
	~CDropCfgMaster(){}

	DropCfg* GetDropCfgByID(const unsigned int uiCfgID);

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

#define DROP_CFG_MASTER (CVSingleton<CDropCfgMaster>::Instance())

#endif // DropCfgMaster_h__
