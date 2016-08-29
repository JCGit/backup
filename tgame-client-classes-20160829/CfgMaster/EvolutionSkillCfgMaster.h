#ifndef EvolutionSkillCfgMaster_h__
#define EvolutionSkillCfgMaster_h__

#include "CfgMasterBase.h"
#include "EvolutionSkillCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class EvolutionSkillCfgMaster: public CCfgMasterBase<EvolutionSkillCfgSet>
{
public:
	EvolutionSkillCfgMaster() {};
	~EvolutionSkillCfgMaster() {};

	EvolutionSkillCfg* getEvolutionSkillCfgByID(const unsigned int uCfgID);
	
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

#define EVOLUTION_SKILL_CFG_MASTER CVLib::CVSingleton<EvolutionSkillCfgMaster>::Instance()

#endif // EvolutionSkillCfgMaster_h__
