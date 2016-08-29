#include "WeaponDetailInfoLayout.h"
#include "WeaponCfgMaster.h"
//#include "WeaponStarStepCfgMaster.h"
#include "WeaponTypeCfgMaster.h"
#include "defines.h"
#include "WeaponUtils.h"
#include "firearms/FireArmsObj.h"
#include "CSEquipItemMsg.pb.h"
#include "ToolUtils.h"
using namespace std;

void WeaponDetailInfoLayout::initWithLayout(UILayout *pRefLayout) {
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    m_pRefLayout->retain();
    m_pRefLayout->removeFromParent();
    


    m_DescTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("desc_txt")); //desc_txt

    m_pAtkGrowValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_grow_val_txt"));
    m_pAtkAttrGrowValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_attr_grow_val_txt"));
    m_pAtkValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_val_txt"));
    m_pAtkSpTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_sp_txt"));

    m_pAtkTypeTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_type_txt"));
    m_pAtkAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_attr_txt"));
    m_pChuantouliTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("chuantouli_txt"));

    m_pWeightTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("weight_txt"));
    m_pAtkDistTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_dist_txt"));
    m_pAddPerTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("add_per_txt"));
}


void WeaponDetailInfoLayout::updateWeaponDetailInfo(const unsigned int cfgID, const UINT64_t weaponGUID){
    const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);
    if(!weaponCfg) {
        return;
    }

    int iWeaponLv = 1;
    int iWeaponColor = 1;
    bool bIsInUse = false;
    if(weaponGUID > 0) {
        const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);
        iWeaponLv = stWeaponInfo.level();
        iWeaponColor = stWeaponInfo.quality();
        bIsInUse = (weaponGUID == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid());
    }

    m_pAtkGrowValTxt->setText(intToString( WeaponUtils::getWeaponAtkAddVal(cfgID) ));//攻击成长
    m_pAtkAttrGrowValTxt->setText(intToString(weaponCfg->atkattrvaladd())); // 攻击属性成长

    m_DescTxt->setText( weaponCfg->weapondesc() );

	unsigned int weaponAddCoe = WeaponUtils::getWeaponAddCoe(cfgID, iWeaponLv, iWeaponColor);
    m_pAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(cfgID, iWeaponLv, iWeaponColor) + weaponAddCoe));

    string attrStr = WEAPON_CFG_MASTER->getAtkAttrTypeDesc(weaponCfg->atkattrtype()) 
        + "+" + intToString(WeaponUtils::getWeaponAtkAttr(cfgID, iWeaponLv, iWeaponColor));
    m_pAtkAttrTxt->setText(attrStr);

    m_pAtkTypeTxt->setText(WEAPON_TYPE_CFG->GetWeaponTypeCfgByID(weaponCfg->weapontype())->desc());
    m_pAtkSpTxt->setText(intToString(weaponCfg->shotspeed()));
    m_pChuantouliTxt->setText(intToString(weaponCfg->penetrateval()));
    m_pAtkDistTxt->setText(intToString(weaponCfg->shotrange()));
    m_pAddPerTxt->setText("0%");
    m_pWeightTxt->setText(intToString(weaponCfg->decmovespeed()));
}


