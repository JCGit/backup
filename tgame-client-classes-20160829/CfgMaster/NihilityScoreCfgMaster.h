#ifndef NihilityScoreCfg_H__
#define NihilityScoreCfg_H__

#include "NihilityScoreCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CNihilityScoreCfgMaster : public CCfgMasterBase<NihilityScoreCfgSet>
{
public:

	CNihilityScoreCfgMaster(){}
	~CNihilityScoreCfgMaster(){}

	const NihilityScoreCfg* GetNihilityScoreCfgByLev(const unsigned int uiLev);

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

#define NIHILITY_SCORE_CFG (CVSingleton<CNihilityScoreCfgMaster>::Instance())

#endif

