#include "QStageEntityObj.h"
#include "WeaponCfgMaster.h"
#include "HeroCfgMaster.h"
#include "Macros.pb.h"

void CQStageEntityObj::setInfoByMonsterCfg(MonsterCfg* cfg){
	hp = cfg->hp();
	ep = 0;
	level = cfg->level();
	walkSpeed = MONSTER_CFG_MASTER->GetRandSpeed(*cfg);
	runSpeed = cfg->highspeed();
	backSpeed = 0;
	atk = cfg->attack();
	totalAtkSpeed = 0;
	atkSpeed = 0;
	originalAtk = atk;
	def = cfg->defence();
	atkType = cfg->atktype();
	defType = cfg->deftype();
	roleParty = cfg->camp();
	roleType = cfg->monstertype();
	hurtResPath = cfg->hurtanimation();
	dieResPath = cfg->dieanimation();
	emergeVoice = cfg->emergevoice();
	hurtVoice = cfg->hurtvoice();
	dieVoice = cfg->dievoice();
	hurtColor = cfg->hurtcolor();
	atkAttr.set_attrtype(cfg->atkatttype());
	atkAttr.set_attrval(cfg->atkatt());
	hit = 0;  //怪物必定命中
	initScale = cfg->initscale();
	isBlock = (cfg->isblock() > 0);
	dodgeDist = 0;
	dodgeCD = 0;
	weaponCfgID = 0;
	evolutionSkillID = 0;

	//动作名
	stMovements.emergeAct = cfg->actions().emerge();

	weapon.set_weaponguid(0);
	weapon.set_cfgid(0);
	weapon.set_level(0);
	weapon.set_exp(0);

	stMagicDefRatio[0] = 0;	//下标为0的代表无属性，值为0
	for(int i = 0; i < MAX_MAGIC_CNT - 1; i++){
		if(i < cfg->defatts_size()){
			stMagicDefRatio[i + 1] = cfg->defatts(i).defatt();
		}else{
			stMagicDefRatio[i + 1] = 0;
		}
	}

	stMagicAtkRatio[0] = 0;	//下标为0的代表无属性，值为0
	for(int i = 0; i < MAX_MAGIC_CNT - 1; i++){
		if(i == cfg->atkatttype()){
			stMagicAtkRatio[i + 1] = cfg->atkatt();
		}else{
			stMagicAtkRatio[i + 1] = 0;
		}
	}
}

void CQStageEntityObj::setInfoByUserInfo(const UserInfo* userInfo){
	hp = userInfo->hp;
	ep = userInfo->mp;
	level = userInfo->level;
	walkSpeed = userInfo->moveSpeed;
	runSpeed = userInfo->runSpeed;
	backSpeed = userInfo->backSpeed;
	atk = userInfo->atk;
	totalAtkSpeed = userInfo->atkSpeed;
	atkSpeed = userInfo->atkSpeed - WEAPON_CFG_MASTER->GetWeaponCfgByID(userInfo->weapon.cfgid())->shotspeed();
	def = userInfo->def;
	atkType = userInfo->atkType;
	defType = userInfo->defType;
	roleParty = ROLE_PARTY_FRIEND;
	roleType = ROLE_TYPE_PLAYER;
	hurtResPath.assign("");
	dieResPath.assign("");
	emergeVoice = HERO_CFG_MASTER->GetHeroCfgByID(userInfo->showID)->emergevoice();
	hurtVoice = HERO_CFG_MASTER->GetHeroCfgByID(userInfo->showID)->hurtvoice();
	dieVoice = HERO_CFG_MASTER->GetHeroCfgByID(userInfo->showID)->dievoice();
	hurtColor = HERO_CFG_MASTER->GetHeroCfgByID(userInfo->showID)->hurtcolor();
	dodgeDist = userInfo->dodgeDist;
	dodgeCD = ceil(userInfo->dodgeCD/1000);
	weaponCfgID = userInfo->weaponCfgID;
	evolutionSkillID = userInfo->evolutionSkillID;
	hit = userInfo->hit;  
	initScale = 0;
	isBlock = false;

	//动作名
	stMovements.emergeAct = "emerge";

	weapon.set_weaponguid(userInfo->weapon.weaponguid());
	weapon.set_cfgid(userInfo->weapon.cfgid());
	weapon.set_level(userInfo->weapon.level());
	weapon.set_exp(userInfo->weapon.exp());
	weapon.set_quality(userInfo->weapon.quality());

	const WeaponCfg* cfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(weaponCfgID);
	originalAtk = atk - WEAPON_CFG_MASTER->GetWeaponAtkAdd(weapon.cfgid(), weapon.quality(), weapon.level());
	
	stMagicDefRatio[0] = 0;	//下标为0的代表无属性，值为0
	for(unsigned int i = 0; i < userInfo->stDefAttrs.size(); i++){
		stMagicDefRatio[i + 1] = userInfo->stDefAttrs[i].attrval();
	}

	stMagicAtkRatio[0] = 0;	//下标为0的代表无属性，值为0
	vmsg::CSPlayerAttr useAtkAtt;
	for(unsigned int i = 0; i < userInfo->stAtkAttrs.size(); i++){
		stMagicAtkRatio[i + 1] = userInfo->stAtkAttrs[i].attrval();

		if(userInfo->stAtkAttrs[i].attrtype() == cfg->atkattrtype()){
			useAtkAtt.set_attrtype(userInfo->stAtkAttrs[i].attrtype());
			useAtkAtt.set_attrval(userInfo->stAtkAttrs[i].attrval());
			atkAttr = useAtkAtt;
		}
	}
}

void CQStageEntityObj::setInfoByDetaiInfo(const vmsg::CSPlayerDetailInfo& detaiInfo, unsigned int uParty){
	hp = detaiInfo.hp();
	ep = detaiInfo.mp();
	level = detaiInfo.level();		
	walkSpeed = detaiInfo.movespeed();
	runSpeed = detaiInfo.runspeed();
	backSpeed = detaiInfo.backspeed();
	atk = detaiInfo.atk();
	totalAtkSpeed = detaiInfo.atkspeed();
	atkSpeed = detaiInfo.atkspeed() - WEAPON_CFG_MASTER->GetWeaponCfgByID(detaiInfo.weapon().cfgid())->shotspeed();
	def = detaiInfo.def();
	atkType = detaiInfo.atktype();
	defType = detaiInfo.deftype();
	roleParty = uParty;
	roleType = ROLE_TYPE_PLAYER;
	hurtResPath = "";
	dieResPath = "";
	emergeVoice = HERO_CFG_MASTER->GetHeroCfgByID(detaiInfo.showid())->emergevoice();
	hurtVoice = HERO_CFG_MASTER->GetHeroCfgByID(detaiInfo.showid())->hurtvoice();
	dieVoice = HERO_CFG_MASTER->GetHeroCfgByID(detaiInfo.showid())->dievoice();
	hurtColor = HERO_CFG_MASTER->GetHeroCfgByID(detaiInfo.showid())->hurtcolor();
	dodgeDist = detaiInfo.dodgedist();
	dodgeCD = ceil(detaiInfo.dodgecd()/1000);
	weaponCfgID = detaiInfo.weapon().cfgid();
	evolutionSkillID = detaiInfo.evolutionskillid();
	hit = detaiInfo.hit();  
	initScale = 0;
	isBlock = false;

	//动作名
	stMovements.emergeAct = "emerge";

	weapon.set_weaponguid(detaiInfo.weapon().weaponguid());
	weapon.set_cfgid(detaiInfo.weapon().cfgid());
	weapon.set_level(detaiInfo.weapon().level());
	weapon.set_exp(detaiInfo.weapon().exp());
	weapon.set_quality(detaiInfo.weapon().quality());

	const WeaponCfg* cfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(detaiInfo.weapon().cfgid());
	originalAtk = atk - WEAPON_CFG_MASTER->GetWeaponAtkAdd(weapon.cfgid(), weapon.quality(), weapon.level());

	stMagicDefRatio[0] = 0;	//下标为0的代表无属性，值为0
	for(int i = 0; i < detaiInfo.defattrs_size(); i++){
		stMagicDefRatio[i + 1] = detaiInfo.defattrs(i).attrval();
	}

	stMagicAtkRatio[0] = 0;	//下标为0的代表无属性，值为0
	vmsg::CSPlayerAttr useAtkAtt;
	for(int i = 0; i < detaiInfo.atkattrs_size(); i++){
		stMagicAtkRatio[i + 1] = detaiInfo.atkattrs(i).attrval();

		if(detaiInfo.atkattrs(i).attrtype() == cfg->atkattrtype()){
			useAtkAtt.set_attrtype(detaiInfo.atkattrs(i).attrtype());
			useAtkAtt.set_attrval(detaiInfo.atkattrs(i).attrval());
			atkAttr = useAtkAtt;
		}
	}
}