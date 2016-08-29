#ifndef EvolutionSkillExpCfgMaster_h__
#define EvolutionSkillExpCfgMaster_h__

#include "CfgMasterBase.h"
#include "EvolutionSkillExpCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class EvolutionSkillExpCfgMaster: public CCfgMasterBase<EvolutionSkillExpCfgSet>
{
public:
	EvolutionSkillExpCfgMaster() {};
	~EvolutionSkillExpCfgMaster() {};

	EvolutionSkillExpCfg* getEvolutionSkillExpCfgByID(const unsigned int uCfgID);
	
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

#define EVOLUTION_SKILL_EXP_CFG_MASTER CVLib::CVSingleton<EvolutionSkillExpCfgMaster>::Instance()

#endif // EvolutionSkillExpCfgMaster_h__
