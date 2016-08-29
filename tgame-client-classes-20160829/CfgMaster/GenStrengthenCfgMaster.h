#ifndef GenStrengthenCfg_H__
#define GenStrengthenCfg_H__

#include "GenStrengthenCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CGenStrengthenCfgMaster : public CCfgMasterBase<GenStrengthenCfgSet>
{
public:

	CGenStrengthenCfgMaster(){}
	~CGenStrengthenCfgMaster(){}

	const GenStrengthenCfg* GetGenStrengthenCfgByID(const unsigned int uiCfgID);

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

#define GEN_STRENGTHEN_CFG_MASTER (CVSingleton<CGenStrengthenCfgMaster>::Instance())

#endif

