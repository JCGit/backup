#ifndef __CFG_LOADER_H__
#define __CFG_LOADER_H__

#include "SceneItemCfgMaster.h"
#include "BagenlargeCfgMaster.h"
#include "EquipmentCfgMaster.h"
#include "ItemCfgMaster.h"
#include "BattleSysCfgMaster.h"
#include "BattleDefRatioCfgMaster.h"
#include "EffectSoundCfgMaster.h"
#include "WarMapCfgMaster.h"
#include "MapCfgMaster.h"
#include "BuffCfgMaster.h"
#include "BulletCfgMaster.h"
#include "MonsterCfgMaster.h"
#include "BuildItemCfgMaster.h"
#include "ScoreCfgMaster.h"
#include "CardCfgMaster.h"
#include "TaskCfgMaster.h"
#include "SceneCfgMaster.h"
#include "OSDCfgMaster.h"
#include "NounCfgMaster.h"
#include "TextCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "WeaponLevelCfgMaster.h"
#include "WeaponExpItemCfgMaster.h"
#include "WeaponStarStepCfgMaster.h"
#include "GlobalCfgMaster.h"
#include "HeroCfgMaster.h"
#include "WeaponRefitAdditionCfgMaster.h"
#include "WeaponRefitCfgMaster.h"
#include "WeaponLotteryCfgMaster.h"
#include "EvolutionSkillCfgMaster.h"
#include "EvolutionSkillExpCfgMaster.h"
#include "EvolutionSkillGrowCfgMaster.h"
#include "EndlessCfgMaster.h"
#include "NPCCfgMaster.h"
#include "MonsterShowCfgMaster.h"
#include "MonsterLevCfgMaster.h"
#include "DailyTaskCfgMaster.h"
#include "ExpCfgMaster.h"
#include "SceneEffCfgMaster.h"
#include "GuildActCfgMaster.h"
#include "GuildActHpCfgMaster.h"
#include "AttrStrengthenCfgMaster.h"
#include "CVZMonsterWaveCfgMaster.h"
#include "CVZMonsterCfgMaster.h"
#include "WeaponTypeCfgMaster.h"
#include "NameCfgMaster.h"
#include "SvrNotifyMaster.h"
#include "BubbleLibCfgMaster.h"
#include "VIPAwardCfgMaster.h"
#include "VIPCfgMaster.h"
#include "VIPLvCfgMaster.h"
#include "PackageCfgMaster.h"
#include "ArenaBuyNumCfgMaster.h"
#include "ArenaDailyAwardCfgMaster.h"
#include "ArenaFightAwardCfgMaster.h"
#include "ArenaLvCfgMaster.h"
#include "ArenaMatchCostCfgMaster.h"
#include "ArenaRuleCfgMaster.h"
#include "PlayerInitCfgMaster.h"
#include "PanelSoundCfgMaster.h"
#include "StoreCfgMaster.h"
#include "ModuleOpenCfgMaster.h"
#include "QiangHunCfgMaster.h"
#include "SkillCfgMaster.h"
#include "DailyTaskAwardCfgMaster.h"
#include "MilestoneCfgMaster.h"
#include "FloorBuffCfgMaster.h"
#include "WeaponBuffCfgMaster.h"
#include "TrapCfgMaster.h"
#include "StoryCfgMaster.h"
#include "ErrorCodeCfgMaster.h"
#include "FightGuideCfgMaster.h"
#include "NihilityMapLibCfgMaster.h"
#include "NihilityMonsterShowCfgMaster.h"
#include "EquipExpItemCfgMaster.h"
#include "EquipLevelCfgMaster.h"
#include "EquipRefitAdditionCfgMaster.h"
#include "EquipRefitCfgMaster.h"
#include "EquipStarStepCfgMaster.h"
#include "ConstantCfgMaster.h"
#include "NihilityPackageCfgMaster.h"
#include "ArenaMapLibCfgMaster.h"
#include "EliteWarMapCfgMaster.h"
#include "EliteAwardLibCfgMaster.h"
#include "FightGuideMapCfgMaster.h"
#include "NothinglandSweepItemCfgMaster.h"
#include "DebrisCfgMaster.h"
#include "ItemDropTipCfgMaster.h"
#include "ArenaBossLevCfgMaster.h"
#include "DailyRegAwardCfgMaster.h"
#include "NotificationCfgMaster.h"
#include "NihilityScoreCfgMaster.h"
#include "AIParamCfgMaster.h"
#include "DailyActCfgMaster.h"
#include "GenStrengthenCfgMaster.h"
#include "ArenaRandHeroCfgMaster.h"
#include "FightKillNumCfgMaster.h"
#include "DotaItemCfgMaster.h"
#include "DotaLvCfgMaster.h"
#include "ArenaKillNumCfgMaster.h"
#include "ActCfgMaster.h"
#include "WorldBossLvCfgMaster.h"
#include "WorldBossShowLibCfgMaster.h"
#include "BossChallengeInstanceCfgMaster.h"
#include "SpringActCfgMaster.h"
#include "DailyInstanceCfgMaster.h"
#include "WeaponArrayOpenCfgMaster.h"
#include "WeaponArrayEffCfgMaster.h"
#include "WeaponArrayRuleCfgMaster.h"
#include "EntryCfgMaster.h"
#include "EntryGroupCfgMaster.h"
#include "PlayerGrowCfgMaster.h"
#include "WarmPromptCfgMaster.h"
#include "CostCfgMaster.h"
#include "BuyInstanceCfgMaster.h"
#include "PlayerEnforceCfgMaster.h"
#include "GenTypeCfgMaster.h"
#include "WarAwardConditionCfgMaster.h"
#include "SumChargeActCfgMaster.h"
#include "DailyChargeActCfgMaster.h"
#include "DailyLoginActCfgMaster.h"
#include "FightGeekAwardCfgMaster.h"
#include "GradeGeekAwardCfgMaster.h"
#include "FBShareCfgMaster.h"
#include "VipGiftCfgMaster.h"
#include "DailyConsumeAwardCfgMaster.h"
#include "SumConsumeActCfgMaster.h"
#include "CfgMaster/GuideCfgMaster.h"
#include "CfgMaster/GuideStepCfgMaster.h"
#include "CfgMaster/OnlineAwardCfgMaster.h"
#include "CfgMaster/NextDayAwardCfgMaster.h"

#define CHECK_LOAD_CFG_RET(iRet) do { if(iRet) return iRet; } while(0)

class CCfgLoader
{
public:
    CCfgLoader(){}
    ~CCfgLoader(){}
    
    int LoadInitCfg() {
        //在检查更新前需要读入的配置
        int iRet = NOUN_CFG_MASTER->Init("xlscfg/NounCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = TEXT_CFG_MASTER->Init("xlscfg/TextCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = PANEL_SOUND_CFG->Init("xlscfg/PanelSoundCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = SCENE_CFG_MASTER->Init("xlscfg/SceneCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = ERROR_CODE_CFG_MASTER->Init("xlscfg/ErrorCode.cfg");
        CHECK_LOAD_CFG_RET(iRet);
        
        return iRet;
    }

    int LoadAllCfg()
    {
        int iRet = 0;
        iRet = SKILL_CFG_MASTER->Init("xlscfg/SkillCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = EFFECT_SOUND_CFG_MASTER->Init("xlscfg/EffectSoundCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = BATTLE_SYS_CFG_MASTER->Init("xlscfg/BattleSysCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = BATTLE_DEF_RATIO_CFG_MASTER->Init("xlscfg/BattleDefRatioCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);
        
        iRet = SCENE_ITEM_CFG->Init("xlscfg/SceneItemCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = BAGENLARGE_CFG_MASTER->Init("xlscfg/BagenlargeCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = EQUIPMENT_CFG_MASTER->Init("xlscfg/EquipmentCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = ITEM_CFG_MASTER->Init("xlscfg/ItemCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = MAP_CFG_MASTER->Init("xlscfg/MapCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = BUFF_CFG_MASTER->Init("xlscfg/BuffCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = BULLET_CFG_MASTER->Init("xlscfg/BulletCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = MONSTER_CFG_MASTER->Init("xlscfg/MonsterCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = WAR_MAP_CFG->Init("xlscfg/WarMapCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = BUILD_ITEM_CFG->Init("xlscfg/BuildItemCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);
        
		iRet = SCORE_CFG_MASTER->Init("xlscfg/ScoreCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = CARD_CFG_MASTER->Init("xlscfg/CardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_CFG_MASTER->Init("xlscfg/WeaponCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_EXP_CFG->Init("xlscfg/WeaponExpItemCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_LEVEL_CFG->Init("xlscfg/WeaponLevelCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_STAR_CFG->Init("xlscfg/WeaponStarStepCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);


		iRet = OSD_CFG_MASTER->Init("xlscfg/OSDCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        ///
        iRet = LoadInitCfg();
        CHECK_LOAD_CFG_RET(iRet);

		iRet = GLOBAL_CFG_MASTER->Init("xlscfg/GlobalCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = HERO_CFG_MASTER->Init("xlscfg/HeroCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = WEAPONREFITADDITION_CFG_MASTER->Init("xlscfg/WeaponRefitAdditionCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = END_LESS_CFG->Init("xlscfg/EndlessCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = WEAPONREFIT_CFG_MASTER->Init("xlscfg/WeaponRefitCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = WEAPON_LOT_CFG_MASTER->Init("xlscfg/WeaponLotteryCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = EVOLUTION_SKILL_CFG_MASTER->Init("xlscfg/EvolutionSkillCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = EVOLUTION_SKILL_EXP_CFG_MASTER->Init("xlscfg/EvolutionSkillExpCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = EVOLUTION_SKILL_GROW_CFG_MASTER->Init("xlscfg/EvolutionSkillGrowCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = TASK_CFG_MASTER->Init("xlscfg/TaskCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = NPC_CFG_MASTER->Init("xlscfg/NPCCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = MONSTER_SHOW_CFG->Init("xlscfg/MonsterShowCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = MONSTER_LEV_CFG->Init("xlscfg/MonsterLevCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DAILY_TASK_CFG_MASTER->Init("xlscfg/DailyTaskCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = EXP_CFG_MASTER->Init("xlscfg/ExpCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = SCENE_EFFECT_CFG->Init("xlscfg/SceneEffCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ATTR_STRENGTHEN_CFG_MASTER->Init("xlscfg/AttrStrengthenCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = GUILD_ACT_CFG->Init("xlscfg/GuildActCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = GUILD_HP_CFG->Init("xlscfg/GuildActHpCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = CVZ_MONSTER_WAVE_CFG_MASTER->Init("xlscfg/CVZMonsterWaveCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = CVZMONSTER_CFG_MASTER->Init("xlscfg/CVZMonsterCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_TYPE_CFG->Init("xlscfg/WeaponTypeCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = NAME_CFG_MASTER->Init("xlscfg/NameCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = SVR_NOTIFY_CFG_MASTER->Init("xlscfg/ServerNotifyCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = BUBBLE_LIB_CFG->Init("xlscfg/BubbleLibCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = VIP_AWARD_CFG_MASTER->Init("xlscfg/VIPAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = VIP_CFG_MASTER->Init("xlscfg/VIPCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = VIP_LV_CFG_MASTER->Init("xlscfg/VIPLvCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = PACKAGE_CFG_MASTER->Init("xlscfg/PackageCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_BUY_NUM_CFG_MASTER->Init("xlscfg/ArenaBuyNumCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_DAILY_AWARD_CFG_MASTER->Init("xlscfg/ArenaDailyAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_FIGHT_AWARD_CFG_MASTER->Init("xlscfg/ArenaFightAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_LV_CFG_MASTER->Init("xlscfg/ArenaLvCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_MATCH_COST_CFG_MASTER->Init("xlscfg/ArenaMatchCostCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_RULE_CFG->Init("xlscfg/ArenaRuleCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = PLAYER_INIT_CFG->Init("xlscfg/PlayerInitCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);


		iRet = STORE_CFG_MASTER->Init("xlscfg/StoreCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = MODULE_OPEN_CFG_MASTER->Init("xlscfg/ModuleOpenCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = QIANGHUN_CFG_MASTER->Init("xlscfg/QiangHunCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DAILY_TASK_AWARD_CFG_MASTER->Init("xlscfg/DailyTaskAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = MILESTONE_CFG_MASTER->Init("xlscfg/MilestoneCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = FLOOR_BUFF_CFG->Init("xlscfg/FloorBuffCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_BUFF_CFG->Init("xlscfg/WeaponBuffCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = TRAP_CFG_MASTER->Init("xlscfg/TrapCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = STORY_CFG_MASTER->Init("xlscfg/StoryCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = FIGHT_GUIDE_CFG->Init("xlscfg/FightGuideCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);
        
        iRet = EQUIPEXPITEM_CFG_MASTER->Init("xlscfg/EquipExpItemCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = NIHILITY_MAP_LIB_CFG->Init("xlscfg/NihilityMapLibCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = NIHILITY_MONSTER_SHOW_CFG->Init("xlscfg/NihilityMonsterShowCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = EQUIPLEVEL_CFG_MASTER->Init("xlscfg/EquipLevelCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = EQUIPREFITADDITION_CFG_MASTER->Init("xlscfg/EquipRefitAdditionCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = EQUIPREFIT_CFG_MASTER->Init("xlscfg/EquipRefitCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = EQUIPSTARSTEP_CFG_MASTER->Init("xlscfg/EquipStarStepCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = CONSTANT_CFG_MASTER->Init("xlscfg/ConstantCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = NIHILITY_PACKAGE_CFG_MASTER->Init("xlscfg/NihilityPackageCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_MAP_LIB_CFG_MASTER->Init("xlscfg/ArenaMapLibCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ELITE_WAR_MAP_CFG_MASTER->Init("xlscfg/EliteWarMapCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ELITE_AWARD_LIB_CFG_MASTER->Init("xlscfg/EliteAwardLibCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = FIGHT_GUIDE_MAP_CFG_MASTER->Init("xlscfg/FightGuideMapCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = NIHLITY_SWEEP_ITEM_CFG_MASTER->Init("xlscfg/NothinglandSweepItemCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = DEBRIS_CFG_MASTER->Init("xlscfg/DebrisCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

        iRet = ITEM_DROP_TIP_MASTER->Init("xlscfg/ItemDropTipCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_BOSS_LEV_CFG->Init("xlscfg/ArenaBossLevCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = DAILYREGAWARD_CFG_MASTER->Init("xlscfg/DailyRegAwardCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = NOTIFICATION_CFG_MASTER->Init("xlscfg/NotificationCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = NIHILITY_SCORE_CFG->Init("xlscfg/NihilityScoreCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = AI_PARAM_CFG_MASTER->Init("xlscfg/AIParamCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DAILY_ACT_CFG_MASTER->Init("xlscfg/DailyActCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = GEN_STRENGTHEN_CFG_MASTER->Init("xlscfg/GenStrengthenCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_RAND_HERO_CFG_MASTER->Init("xlscfg/ArenaRandHeroCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = FIGHT_KILL_NUM_CFG->Init("xlscfg/FightKillNumCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DOTA_ITEM_CFG_MASTER->Init("xlscfg/DotaItemCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DOTA_LV_CFG_MASTER->Init("xlscfg/DotaLvCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ARENA_KILL_NUM_CFG->Init("xlscfg/ArenaKillNumCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ACT_CFG_MASTER->Init("xlscfg/ActCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WORLD_BOSS_LV_CFG->Init("xlscfg/WorldBossLvCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WORLD_BOSS_SHOW_LIB_CFG->Init("xlscfg/WorldBossShowLibCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = BOSS_CHALLENGE_CFG->Init("xlscfg/BossChallengeInstanceCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = SPRING_ACT_CFG_MASTER->Init("xlscfg/SpringActCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        iRet = DAILY_INST_CFG_MASTER->Init("xlscfg/DailyInstanceCfg.cfg");
        CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_ARR_OPEN_CFG_MASTER->Init("xlscfg/WeaponArrayOpenCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_ARR_EFF_CFG_MASTER->Init("xlscfg/WeaponArrayEffCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WEAPON_ARR_RULE_CFG_MASTER->Init("xlscfg/WeaponArrayRuleCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ENTRY_CFG_MASTER->Init("xlscfg/EntryCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = ENTRY_GROUP_CFG_MASTER->Init("xlscfg/EntryGroupCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = PLAYER_GROW_CFG_MASTER->Init("xlscfg/PlayerGrowCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WARM_PROMPT_CFG_MASTER->Init("xlscfg/WarmPromptCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = COST_CFG_MASTER->Init("xlscfg/CostCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = BUY_INSTANCE_CFG_MASTER->Init("xlscfg/BuyInstanceCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = PLAYER_ENFORCE_CFG_MASTER->Init("xlscfg/PlayerEnforceCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = GEN_TYPE_CFG_MASTER->Init("xlscfg/GenTypeCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = WAR_AWARD_CONDITION_CFG->Init("xlscfg/WarAwardConditionCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = SumChargeAct_CFG_MASTER->Init("xlscfg/SumChargeActCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DailyChargeAct_CFG_MASTER->Init("xlscfg/DailyChargeAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DailyLoginAct_CFG_MASTER->Init("xlscfg/DailyLoginAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = FightGeekAward_CFG_MASTER->Init("xlscfg/FightGeekAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);
		
		iRet = GradeGeekAward_CFG_MASTER->Init("xlscfg/GradeGeekAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = VIPGIFT_CFG_MASTER->Init("xlscfg/VipGiftCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = DAILYCONSUME_CFG_MASTER->Init("xlscfg/DailyConsumeAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = SumConsumeAct_CFG_MASTER->Init("xlscfg/SumConsumeActCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = FBShare_CFG->Init("xlscfg/FBShareCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = GUIDE_CFG_MASTER->Init("xlscfg/GuideCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = GUIDE_STEP_CFG_MASTER->Init("xlscfg/GuideStepCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet  = OnlineAward_CFG_MASTER->Init("xlscfg/OnlinePKGAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

		iRet = NextDayAward_CFG_MASTER->Init("xlscfg/NextDayAwardCfg.cfg");
		CHECK_LOAD_CFG_RET(iRet);

        return iRet;
    }

}; //end class


#endif //__CFG_LOADER_H__

