#ifndef NihilityUtils_h__
#define NihilityUtils_h__

#include "MonsterCfgMaster.h"

#define INIT_MONSTER_LEVEL 5
#define NIHILITY_FIGHT_SCENE 70001
#define NIHILITY_GLOD_SCENE 80001

class CNihilityUtils
{
public:
	static void genMonsterCfg(unsigned int cfgID, MonsterCfg& cfg, int level = 1);	

	static void genBossCfg(unsigned int cfgID, MonsterCfg& cfg, int level = 1);
protected:
private:
};

#endif // NihilityUtils_h__
