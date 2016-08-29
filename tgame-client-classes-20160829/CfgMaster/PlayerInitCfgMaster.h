#ifndef PlayerInitCfgMaster_h__
#define PlayerInitCfgMaster_h__

#include "CfgMasterBase.h"
#include "PlayerInitCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CPlayerInitCfgMaster : public CCfgMasterBase<PlayerInitCfgSet>
{
public:
	CPlayerInitCfgMaster(){}
	~CPlayerInitCfgMaster(){}

	const PlayerInitCfg* GetPlayerCfgByID(const unsigned int uiCfgID);
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

#define PLAYER_INIT_CFG (CVSingleton<CPlayerInitCfgMaster>::Instance())

#endif // PlayerInitCfgMaster_h__