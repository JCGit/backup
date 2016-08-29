#ifndef GuildActHpCfgMaster_h__
#define GuildActHpCfgMaster_h__

#include "CfgMasterBase.h"
#include "GuildActHpCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CGuildActHpCfgMaster : public CCfgMasterBase<GuildActHpCfgSet>
{
public:
	CGuildActHpCfgMaster(){}
	~CGuildActHpCfgMaster(){}

	const GuildActHpCfg* GetGuildActHpCfgByID(const unsigned int uiCfgID);
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

#define GUILD_HP_CFG (CVSingleton<CGuildActHpCfgMaster>::Instance())

#endif // GuildActHpCfgMaster_h__