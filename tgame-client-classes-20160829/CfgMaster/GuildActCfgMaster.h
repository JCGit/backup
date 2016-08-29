#ifndef GuildActCfgMaster_h__
#define GuildActCfgMaster_h__

#include "CfgMasterBase.h"
#include "GuildActCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CGuildActCfgMaster : public CCfgMasterBase<GuildActCfgSet>
{
public:
	CGuildActCfgMaster(){}
	~CGuildActCfgMaster(){}

	const GuildActCfg* GetGuildActCfgByID(const unsigned int uiCfgID);
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

#define GUILD_ACT_CFG (CVSingleton<CGuildActCfgMaster>::Instance())

#endif // GuildActCfgMaster_h__