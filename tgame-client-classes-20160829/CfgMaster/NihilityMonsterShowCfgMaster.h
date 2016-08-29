#ifndef NihilityMonsterShowCfg_H__
#define NihilityMonsterShowCfg_H__

#include "NihilityMonsterShowCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CNihilityMonsterShowCfgMaster : public CCfgMasterBase<NihilityMonsterShowCfgSet>
{
public:

	CNihilityMonsterShowCfgMaster(){}
	~CNihilityMonsterShowCfgMaster(){}

	const NihilityMonsterShowCfg* GetNihilityMonsterShowCfgByID(const unsigned int uiCfgID);
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

#define NIHILITY_MONSTER_SHOW_CFG (CVSingleton<CNihilityMonsterShowCfgMaster>::Instance())

#endif

