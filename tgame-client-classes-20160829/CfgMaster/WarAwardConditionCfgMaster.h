#ifndef WarAwardConditionCfg_H__
#define WarAwardConditionCfg_H__

#include "WarAwardConditionCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CWarAwardConditionCfgMaster : public CCfgMasterBase<WarAwardConditionCfgSet>
{
public:

	CWarAwardConditionCfgMaster(){}
	~CWarAwardConditionCfgMaster(){}

	const WarAwardConditionCfg* GetWarAwardConditionCfgByID(const unsigned int uiCfgID);

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

#define WAR_AWARD_CONDITION_CFG (CVSingleton<CWarAwardConditionCfgMaster>::Instance())

#endif

