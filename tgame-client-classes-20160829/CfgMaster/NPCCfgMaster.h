#ifndef NPCCfgMaster_h__
#define NPCCfgMaster_h__

#include "CfgMasterBase.h"
#include "NPCCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class NPCCfgMaster: public CCfgMasterBase<NPCCfgSet>
{
public:
	NPCCfgMaster() {};
	~NPCCfgMaster() {};

	NPCCfg* getNPCCfgByID(const unsigned int uCfgID);
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
};

#define NPC_CFG_MASTER CVLib::CVSingleton<NPCCfgMaster>::Instance()

#endif // NPCCfgMaster_h__
