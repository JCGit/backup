#include "ItemUtils.h"
#include "EquipRefitAdditionCfgMaster.h"
#include "defines.h"
#include "NounHelper.h"
#include "TextHelper.h"
#include "BasicTypes.h"
#include "ClientSvrMsg.pb.h"

#include "ItemCfgMaster.h"
#include "EquipmentCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "DebrisCfgMaster.h"
#include "GenStrengthenCfgMaster.h"
#include "DotaItemCfgMaster.h"

using namespace std;
using namespace vmsg;

string ItemUtils::getItemIcon(const int iType, const int iCfgID) {
    switch(iType) {
    case THING_TYPE_ITEM:
        return "item/" + string(ITEM_CFG_MASTER->GetItemIcon(iCfgID)) + ".png";
    case THING_TYPE_EQUIP:
        return EQUIPMENT_CFG_MASTER->GetIconPath(iCfgID);
    case THING_TYPE_COIN:
        return "item/icon_coin.png";
    case THING_TYPE_GOLD:
        return "item/icon_diamond.png";
    case THING_TYPE_ARENA_MONEY:
        return "item/icon_coin.png";
    case THING_TYPE_WEAPON:
        return "weaponicon/" + WEAPON_CFG_MASTER->getIcon(iCfgID) + ".png";
    case THING_TYPE_DEBRIS:
        return DEBRIS_CFG_MASTER->getIconPath(iCfgID);
    case THING_TYPE_GEN:
        if(const GenStrengthenCfg* cfg = GEN_STRENGTHEN_CFG_MASTER->GetGenStrengthenCfgByID(iCfgID)) {
            return "icon/" + cfg->icon() + ".png";
        }
        break;
	case THING_TYPE_DOTAITEM:
		if(const DotaItemCfg* pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(iCfgID)) {
			return "item/" + pDotaItemCfg->dotaitemicon() + ".png";
		}
		break;
    case THING_TYPE_MONTHCARD:
        return "icon/ib10001.png";
        break;
	case THING_TYPE_MANUAL:
		return "item/" + string(ITEM_CFG_MASTER->GetItemIcon((ITEM_CFG_MASTER->GetItemsByType(iType)->at(0)))) + ".png";
		break;
    default:
        CCLOG("[error]ItemUtils::getItemIcon,unknow type %d,cfgid %d", iType, iCfgID);
    }
    return "item/default.png";
}

string ItemUtils::getItemName(const int iType, const int iCfgID) {
    switch(iType) {
    case THING_TYPE_ITEM:
        return ITEM_CFG_MASTER->GetItemName(iCfgID);
    case THING_TYPE_EQUIP:
        return EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iCfgID)->equipname();
    case THING_TYPE_COIN:
        return NounStr(NOUN_COIN);
    case THING_TYPE_GOLD:
        return NounStr(NOUN_GOLD);
    case THING_TYPE_EXP:
        return NounStr(NOUN_EXP);
    case THING_TYPE_ARENA_MONEY:
        return NounStr(ARENA_COIN_TXT);
    case THING_TYPE_WEAPON:
        return WEAPON_CFG_MASTER->getName(iCfgID);
    case THING_TYPE_DEBRIS:
        if(DEBRIS_CFG_MASTER->GetDebrisCfgByID(iCfgID)) {
            return DEBRIS_CFG_MASTER->GetDebrisCfgByID(iCfgID)->debrisname();
        }
        break;
    case THING_TYPE_MONTHCARD:
        return NounStr(TXT_THING_TYPE_MONTHCARD);
    case THING_TYPE_GEN:
        if(const GenStrengthenCfg* cfg = GEN_STRENGTHEN_CFG_MASTER->GetGenStrengthenCfgByID(iCfgID)) {
            return cfg->name();
        }
        break;
	case THING_TYPE_DOTAITEM:
		if(const DotaItemCfg* pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(iCfgID)) {
			return pDotaItemCfg->dotaitemname();
		}
		break;
	case THING_TYPE_MANUAL:
		return ITEM_CFG_MASTER->GetItemName(ITEM_CFG_MASTER->GetItemsByType(iType)->at(0));
		break;
    default:
        CCLOG("[error]ItemUtils::getItemName,unknow type %d", iType);
    }
    return "";
}

string ItemUtils::getItemDesc(const int iType, const int iCfgID){
    switch(iType) {
    case THING_TYPE_ITEM:
        return ITEM_CFG_MASTER->GetItemCfgByID(iCfgID)->itemdesc();
    case THING_TYPE_EQUIP:
        return EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iCfgID)->equipdesc();
    case THING_TYPE_COIN:
        return TextStr(COIN_DESC_TXT);
    case THING_TYPE_GOLD:
        return TextStr(GOLD_DESC_TXT);
    case THING_TYPE_ARENA_MONEY:
        return TextStr(ARENA_COIN_DESC_TXT);
    case THING_TYPE_WEAPON:
        return WEAPON_CFG_MASTER->getWeaponDesc(iCfgID);
    case THING_TYPE_DEBRIS:
        if(DEBRIS_CFG_MASTER->GetDebrisCfgByID(iCfgID)) {
            return DEBRIS_CFG_MASTER->GetDebrisCfgByID(iCfgID)->debrisdesc();
        }
        break;
    case THING_TYPE_MONTHCARD:
        return NounStr(TXT_TGAME_DESC_MONTHCARD);
    case THING_TYPE_GEN:
        if(const GenStrengthenCfg* cfg = GEN_STRENGTHEN_CFG_MASTER->GetGenStrengthenCfgByID(iCfgID)) {
            return cfg->desc();
        }
        break;
	case THING_TYPE_DOTAITEM:
		if(const DotaItemCfg* pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(iCfgID)) {
			return pDotaItemCfg->dotaitemdesc();
		}
		break;
	case THING_TYPE_MANUAL:
		return ITEM_CFG_MASTER->GetItemCfgByID(ITEM_CFG_MASTER->GetItemsByType(iType)->at(0))->itemdesc();
		break;
    default:
        CCLOG("[error]ItemUtils::getItemDesc,unknow type %d", iType);
    }
    return "";
}

unsigned int ItemUtils::getItemColor(const int iType, const int iCfgID){
    switch(iType) {
    case THING_TYPE_ITEM:
        return ITEM_CFG_MASTER->GetItemCfgByID(iCfgID)->itemcolor();
    case THING_TYPE_EQUIP:
        return EQUIPMENT_CFG_MASTER->GetEquipColor(iCfgID);
    case THING_TYPE_COIN:
        return WEAPON_COLOR_WHITE;
    case THING_TYPE_GOLD:
        return WEAPON_COLOR_GOLD;
	case THING_TYPE_DOTAITEM:
		if(const DotaItemCfg* pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(iCfgID)) {
			return pDotaItemCfg->dotaitemcolor();
		}
		break;
    default:
        CCLOG("[error]ItemUtils::getItemColor,unknow type %d", iType);
    }
    return 1;
}

UINT64_t ItemUtils::getEquipFightPower(const int iCfgID, const int iRefineLv, const int iPart){
    const EquipmentCfg* stCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iCfgID);
    if(stCfg == NULL){
        return 0;
    }/*
    const EquipRefineCostCfg* pstRefineCfg = EQUIP_REFINE_COST_CFG_MASTER->GetCfg(iRefineLv, iPart);
    if(NULL == pstRefineCfg) {
        CCLOG("[error]getEquipFightPower,fail to get EquipRefineCostCfg");
        return 0;
    }*/
    int iAddVal = 0;//pstRefineCfg->colorattradd( (stCfg->color() - 1) ) * (1 + iRefineLv);  //TODO:EQUIP
    iAddVal += stCfg->attr().attrval();

    UINT64_t fightPower = 0;
    switch (stCfg->attr().attrid())
    {
    case ATTR_HP:
        fightPower = iAddVal*2;
        break;
    case ATTR_DEF:
        fightPower = iAddVal*6;
        break;
    case ATTR_ATK:
        fightPower = iAddVal*4;
        break;
    case ATTR_HIT:
        fightPower = iAddVal*0.1;
        break;
    default:
        break;
    }

    return fightPower;
}

UINT64_t ItemUtils::getWeaponFightPower(const int iCfgID, const int iLv, const int iColor){
    const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(iCfgID);
    UINT64_t fightPower = 0;
    if(stCfg == NULL){
        return 0;
    }

	//枪械模块公式 = （枪械地位– 1） * 5000 + （枪械星级– 1） * 5000 + 枪械阶数 * 500 + 枪械等级 * 100
	fightPower = (stCfg->weapondegree() - 1) * 5000 + (stCfg->initstar() - 1) * 5000 + stCfg->initstep() * 500 + iLv * 100;

    return fightPower;
}

unsigned int ItemUtils::getEquipAttrVal(const int iEquipCfgID, const int iEquipLv, const int iQuality) {
    unsigned int uiVal = 0;

    const EquipmentCfg* pstEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iEquipCfgID );
    if(NULL == pstEquipCfg) {
        CCLOG("[warn]ItemUtils::getEquipAttrVal,fail to get equip cfg,cfgid=%d", iEquipCfgID);
        return 0;
    }
    //配置本有的属性值
    uiVal += pstEquipCfg->attr().attrval();
    /*
    //加上强化后提升的值
    const EquipRefineCostCfg* pstRefineCfg = EQUIP_REFINE_COST_CFG_MASTER->GetCfg(iRefineLv, pstEquipCfg->part());
    if (pstRefineCfg != NULL) {
        if (static_cast<unsigned int>(pstRefineCfg->colorattradd_size()) >= (unsigned int)iColor 
            && iColor > 0)
        {
            uiVal += EQUIP_REFINE_COST_CFG_MASTER->GetColorAttrAdd(iRefineLv, pstEquipCfg->part(), iColor);
        }
    }*/
    const EquipRefitAdditionCfg* pstAdditionCfg = EQUIPREFITADDITION_CFG_MASTER->GetCfg(iQuality);
    if (pstAdditionCfg == NULL) {
        CCLOG("[warn]ItemUtils::getEquipAttrVal,fail to get EquipRefitAdditionCfg,quality=%d,cfgid=%d", iQuality, iEquipCfgID);
        return 0;
    }
    uiVal += (iEquipLv - 1) * pstEquipCfg->attradd();
    uiVal = static_cast<unsigned int>(uiVal* 1.0 * pstAdditionCfg->multi() / EGAME_DENOM);

    return uiVal;
}

unsigned int ItemUtils::getEquipShowAttrVal(const int iEquipCfgID, const int iEquipLv,  const int iQuality)  {
    const EquipmentCfg* pstEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iEquipCfgID );
    if(NULL == pstEquipCfg) {
        CCLOG("[warn]ItemUtils::getEquipShowAttrVal,fail to get equip cfg,cfgid=%d", iEquipCfgID);
        return 0;
    }
    unsigned int iAttrVal = getEquipAttrVal(iEquipCfgID, iEquipLv, iQuality ); //强化加属性

    iAttrVal = iAttrVal - pstEquipCfg->attr().attrval(); //去掉基础属性值
    iAttrVal = iAttrVal >= 0 ? iAttrVal : 0;
    if(pstEquipCfg->attr().attrid() == vmsg::ATTR_HIT) { // 命中做系数处理
        iAttrVal = (int)(floor(iAttrVal * 2.3f + 0.5f));
    }
    return iAttrVal;
}

string ItemUtils::getColorBgImg(const int iColor) {

    switch (iColor)
    {
    case vmsg::EQUIP_GRADE_WHITE:
        return "share/item_frame_white.png";
        break;
    case vmsg::EQUIP_GRADE_GREEN:
        return "share/item_frame_green.png";
        break;
    case vmsg::EQUIP_GRADE_BLUE:
        return "share/item_frame_blue.png";
        break;
    case vmsg::EQUIP_GRADE_PURPLE:
        return "share/item_frame_purple.png";
        break;
    case vmsg::EQUIP_GRADE_ORANGE:
        return "share/item_frame_orange.png";
        break;
    default:
        return "share/item_frame.png";
        break;
    }
    return "share/item_frame.png";
}

string ItemUtils::getThingTypeName(const int iThingType) {
    switch(iThingType) {
    case THING_TYPE_EXP:
        return NounStr(EXP);
    case THING_TYPE_ITEM:
        return NounStr(ITEM);
    case THING_TYPE_EQUIP:
        return NounStr(EQUIP);
    case THING_TYPE_COIN:
        return NounStr(NOUN_COIN);
    case THING_TYPE_GOLD:
        return NounStr(NOUN_GOLD);
    case THING_TYPE_ARENA_MONEY:
        return NounStr(ARENA_COIN_TXT);
    case THING_TYPE_WEAPON:
        return NounStr(NOUN_WEAPON);
    case THING_TYPE_DEBRIS:
        return NounStr(TXT_THING_TYPE_DEBRIS);
    case THING_TYPE_MANUAL:
        return NounStr(TXT_THING_TYPE_MANUAL);
    case THING_TYPE_MONTHCARD:
        return NounStr(TXT_THING_TYPE_MONTHCARD);
    case THING_TYPE_GEN:
        return NounStr(TXT_THING_TYPE_GEN);
	case THING_TYPE_DOTAITEM:
		return NounStr(ITEM);
    default:
        CCLOG("[error]ItemUtils::getThingTypeName,unknow type %d", iThingType);
    }
    return "";
}

string ItemUtils::numbFormateW(int iNumb)
{
	char att[100];
	if (iNumb>10000)
	{
		snprintf(att, 100, "%d%s" ,iNumb/10000,"W"); 
	}else
	{
		snprintf(att, 100, "%d" ,iNumb);
	}
	
	
	return std::string(att);
	
}