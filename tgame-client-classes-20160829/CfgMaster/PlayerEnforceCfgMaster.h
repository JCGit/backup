#ifndef PlayerEnforceCfg_H__
#define PlayerEnforceCfg_H__

#include "PlayerEnforceCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CPlayerEnforceCfgMaster : public CCfgMasterBase<PlayerEnforceCfgSet>
{
public:

	CPlayerEnforceCfgMaster(){}
	~CPlayerEnforceCfgMaster(){}

	const PlayerEnforceCfg* GetCfgByLev(const unsigned int uiCfgID);

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

#define PLAYER_ENFORCE_CFG_MASTER (CVSingleton<CPlayerEnforceCfgMaster>::Instance())

#endif

