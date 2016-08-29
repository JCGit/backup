#ifndef WeaponUtils_h__
#define WeaponUtils_h__

#include "BasicTypes.h"
#include "UserObj.h"

#include "WeaponCfgMaster.h"
#include "WeaponRefitAdditionCfgMaster.h"

class WeaponUtils
{
public:
	//获取枪械攻击力
	static unsigned int getWeaponAtk(unsigned int cfgID, unsigned int level, unsigned int color){
		const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);
		unsigned int iLevelAddAtkVal = weaponCfg->atkadd() * (level - 1); //强化攻击力
		unsigned int iAtkVal = 1.0 * (weaponCfg->atk() + iLevelAddAtkVal) * WEAPONREFITADDITION_CFG_MASTER->GetCfg(color)->multi() / EGAME_DENOM;

        //2014-10-29, TGAM-604,攻击力改为显示秒伤：子弹伤害 * 攻速/1270
        iAtkVal = iAtkVal * weaponCfg->shotspeed() / 1270;
		iAtkVal = iAtkVal * (1 + 1.0*P_USER_OBJ->getUserInfo()->atkAddEff/EGAME_DENOM);		//百分比加成

		return iAtkVal;
	}

    //计算攻击成长
    static unsigned int getWeaponAtkAddVal(const unsigned int cfgID) {
        const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);
        if(NULL == weaponCfg) {
            return 0;
        }
        unsigned int iAtkVal = weaponCfg->atkadd() * weaponCfg->shotspeed() / 1270;

        return iAtkVal;
    }

	//获取枪械攻击属性值
	static unsigned int getWeaponAtkAttr(unsigned int cfgID, unsigned int level, unsigned int color){
		const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);
        if(!weaponCfg) {
            CCLOG("[warn]WeaponUtil::getWeaponAddCoe, fail to get weapon cfg, cfg id %d", cfgID);
            return 0;
        }
		unsigned int iLevelAddAtkAttrVal = weaponCfg->atkattrvaladd() * (level - 1); //强化攻击力
		unsigned int atkAttr = 1.0 * (weaponCfg->atkattrval() + iLevelAddAtkAttrVal) * WEAPONREFITADDITION_CFG_MASTER->GetCfg(color)->multi() / EGAME_DENOM;

		return atkAttr;
	}

	static unsigned int getWeaponAddCoe(unsigned int cfgID, unsigned int level, unsigned int color){
		const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);
        if(!weaponCfg) {
            CCLOG("[warn]WeaponUtil::getWeaponAddCoe, fail to get weapon cfg, cfg id %d" , cfgID);
            return 0;
        }
		unsigned int addCoe = getWeaponAtk(cfgID, level, color);
		return addCoe*(1.0f*weaponCfg->fightpowercoe()/EGAME_DENOM);
	}

    static const char* getWeaponBigIconColorBg(const int iColor) {
        switch (iColor) {
        case 1:
            return "weapon_lib/weapon_big_bg_white.png";
        case 2:
            return "weapon_lib/weapon_big_bg_green.png";
        case 3:
		case 4:
            return "weapon_lib/weapon_big_bg_blue.png";
        case 5:
		case 6:
            return "weapon_lib/weapon_big_bg_purple.png";
		case 7:
		case 8:
		case 9:
            return "weapon_lib/weapon_big_bg_gold.png";
        default:
            return "weapon_lib/weapon_big_bg_white.png";
        }
    }
};

#endif // WeaponUtils_h__
