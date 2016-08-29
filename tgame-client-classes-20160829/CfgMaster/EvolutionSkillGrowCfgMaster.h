#ifndef EvolutionSkillGrowCfgMaster_h__
#define EvolutionSkillGrowCfgMaster_h__

#include "CfgMasterBase.h"
#include "EvolutionSkillGrowCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class EvolutionSkillGrowCfgMaster: public CCfgMasterBase<EvolutionSkillGrowCfgSet>
{
public:
	EvolutionSkillGrowCfgMaster() {};
	~EvolutionSkillGrowCfgMaster() {};

	EvolutionSkillGrowCfg* getEvolutionSkillGrowCfgByID(const unsigned int uCfgID);
	
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

#define EVOLUTION_SKILL_GROW_CFG_MASTER CVLib::CVSingleton<EvolutionSkillGrowCfgMaster>::Instance()

#endif // EvolutionSkillGrowCfgMaster_h__
