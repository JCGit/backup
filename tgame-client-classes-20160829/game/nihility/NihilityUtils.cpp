#include "NihilityUtils.h"
#include "Macros.pb.h"

#include "WorldBossLvCfgMaster.h"
#include "MonsterLevCfgMaster.h"
#include "NihilityMonsterShowCfgMaster.h"

void CNihilityUtils::genMonsterCfg(unsigned int cfgID, MonsterCfg& cfg, int level /* = 1 */){

	CCLOG("[trace]CNihilityUtils::genMonsterCfg cfgID:%d, level:%d.", cfgID, level);

	const NihilityMonsterShowCfg* showCfg = NIHILITY_MONSTER_SHOW_CFG->GetNihilityMonsterShowCfgByID(cfgID);
	MonsterLevCfg* levCfg = MONSTER_LEV_CFG->GetMonsterLevCfgByLev(level);

	cfg.set_id(showCfg->cfgid());
	cfg.set_monstername(showCfg->monstername());
	cfg.set_desc(showCfg->desc());
	cfg.set_avatar(showCfg->avatar());
	cfg.set_camp(ROLE_PARTY_HOSTILE);
	cfg.set_monstertype(showCfg->monstertype());
	cfg.set_level(level);
	cfg.set_aiid(showCfg->aiid());
	cfg.set_patrolarea(showCfg->patrolarea());
	cfg.set_floorbuffid(showCfg->floorbuffid());
	cfg.set_hurtcolor(showCfg->hurtcolor());
	cfg.set_initscale(showCfg->initscale());
	cfg.set_isblock(showCfg->isblock());
	cfg.set_headimg(showCfg->headimg());

	cfg.mutable_actions()->set_emerge(showCfg->actions().emerge());

	unsigned int attIdx = showCfg->monstertype() - 1; 
	cfg.set_hp(levCfg->monsteratt(attIdx).hp());
	cfg.set_attack(levCfg->monsteratt(attIdx).atk());
	cfg.set_defence(levCfg->monsteratt(attIdx).def());
	cfg.set_atkatt(levCfg->monsteratt(attIdx).atkatt());

	cfg.set_atktype(showCfg->atktype());
	cfg.set_deftype(showCfg->deftype());
	cfg.set_ballistic(showCfg->ballistic());

	for(int i =0; i < showCfg->skills_size(); i++){
		cfg.add_skills(showCfg->skills(i));
	}

	cfg.set_dropid(0);
	cfg.set_minwalkspeed(showCfg->minwalkspeed());
	cfg.set_maxwalkspeed(showCfg->maxwalkspeed());
	cfg.set_highspeed(showCfg->highspeed());
	cfg.set_emergevoice(showCfg->emergevoice());
	cfg.set_hurtvoice(showCfg->hurtvoice());
	cfg.set_dievoice(showCfg->dievoice());
	cfg.set_dieanimation(showCfg->dieanimation());
	cfg.set_ailuafile("");
	cfg.set_atkatttype(showCfg->atkatttype());

	for(int i =0; i < levCfg->defatts_size(); i++){
		DefAttElement* pstDef = cfg.add_defatts();
		pstDef->set_defatt(levCfg->defatts(i).defatt());
		pstDef->set_defatttype(levCfg->defatts(i).defatttype());
	}

	cfg.set_walktype(showCfg->walktype());
	cfg.set_hurtactioncd(showCfg->hurtactioncd());
}

void CNihilityUtils::genBossCfg(unsigned int cfgID, MonsterCfg& cfg, int level /* = 1 */){
	const NihilityMonsterShowCfg* showCfg = NIHILITY_MONSTER_SHOW_CFG->GetNihilityMonsterShowCfgByID(cfgID);
	const WorldBossLvCfg* levCfg = WORLD_BOSS_LV_CFG->GetWorldBossCfgByLv(level);

	cfg.set_id(showCfg->cfgid());
	cfg.set_monstername(showCfg->monstername());
	cfg.set_desc(showCfg->desc());
	cfg.set_avatar(showCfg->avatar());
	cfg.set_camp(ROLE_PARTY_HOSTILE);
	cfg.set_monstertype(showCfg->monstertype());
	cfg.set_level(level);
	cfg.set_aiid(showCfg->aiid());
	cfg.set_patrolarea(showCfg->patrolarea());
	cfg.set_floorbuffid(showCfg->floorbuffid());
	cfg.set_hurtcolor(showCfg->hurtcolor());
	cfg.set_initscale(showCfg->initscale());
	cfg.set_isblock(showCfg->isblock());
	cfg.set_headimg(showCfg->headimg());

	cfg.mutable_actions()->set_emerge(showCfg->actions().emerge());

	cfg.set_hp(levCfg->hp());
	cfg.set_attack(levCfg->atk());
	cfg.set_defence(levCfg->def());
	cfg.set_atkatt(levCfg->atkatt());
	cfg.set_minwalkspeed(levCfg->minwalkspeed());
	cfg.set_maxwalkspeed(levCfg->maxwalkspeed());

	cfg.set_atktype(showCfg->atktype());
	cfg.set_deftype(showCfg->deftype());
	cfg.set_ballistic(showCfg->ballistic());

	for(int i =0; i < showCfg->skills_size(); i++){
		cfg.add_skills(showCfg->skills(i));
	}

	cfg.set_dropid(0);
	cfg.set_highspeed(showCfg->highspeed());
	cfg.set_emergevoice(showCfg->emergevoice());
	cfg.set_hurtvoice(showCfg->hurtvoice());
	cfg.set_dievoice(showCfg->dievoice());
	cfg.set_dieanimation(showCfg->dieanimation());
	cfg.set_ailuafile("");
	cfg.set_atkatttype(showCfg->atkatttype());

	for(int i =0; i < levCfg->defatts_size(); i++){
		DefAttElement* pstDef = cfg.add_defatts();
		pstDef->set_defatt(levCfg->defatts(i).defatt());
		pstDef->set_defatttype(levCfg->defatts(i).defatttype());
	}

	cfg.set_walktype(showCfg->walktype());
	cfg.set_hurtactioncd(showCfg->hurtactioncd());
}




