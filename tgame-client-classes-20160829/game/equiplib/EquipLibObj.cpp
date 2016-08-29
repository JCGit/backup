#include "EquipLibObj.h"
#include "UserObj.h"
#include "bag/BagControl.h"
#include "EquipmentCfgMaster.h"
//#include "EquipRefineCostCfgMaster.h"
#include "EquipRefitAdditionCfgMaster.h"
#include "EquipStarStepCfgMaster.h"
#include "EquipExpItemCfgMaster.h"
#include "EquipLevelCfgMaster.h"
#include "GlobalCfgMaster.h"

EquipLibObj::EquipLibObj(){
    m_iSelEquipGuid = 0;
    m_iNowSelLvUpMatNum = 0;
    m_stCfgIDMap.clear();
}

const EquipLibOneInfo EquipLibObj::getEquipInfo(const UINT64_t uEquipGuid) {

    //role equip
    const std::vector<vmsg::CSEquipInfo>& stRoleEquipLst = P_USER_OBJ->getUserInfo()->stEquipLst;
    for (int i = 0; i < (int)stRoleEquipLst.size(); ++i) {
        if( uEquipGuid == stRoleEquipLst[i].equipguid() ) {
            return EquipLibOneInfo(stRoleEquipLst[i], true, -1);
        }
    }

    //备用防具
    const vmsg::CSEquipInfo& stSpareEquip = P_USER_OBJ->getUserInfo()->spareEquip;
    if(uEquipGuid == stSpareEquip.equipguid()) {
        return EquipLibOneInfo(stSpareEquip, false, -1);
    }

    //bag equip
    const vmsg::CSBagInfo& stBag = P_BAG_CTRL->getBagInfo();
    for (int i = 0; i < stBag.celllst_size(); ++i) {
        if (stBag.celllst(i).has_equipinfo() && uEquipGuid == stBag.celllst(i).equipinfo().equipguid() ) {
            return EquipLibOneInfo(stBag.celllst(i).equipinfo(), false, stBag.celllst(i).index());
        }
    }

    //not found!
    return EquipLibOneInfo();
}


int EquipLibObj::getEquipMaxLvUpLv(const int iEquipCfgID) {
    const EquipmentCfg* pstEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iEquipCfgID );
    if(NULL == pstEquipCfg) {
        CCLOG("[warn]EquipLibObj::getEquipMaxLvUpLv,fail to get equip cfg,cfgid=%d", iEquipCfgID);
        return 0;
    }
    const EquipStarStepCfg* pStarCfg = EQUIPSTARSTEP_CFG_MASTER->GetCfg(pstEquipCfg->initstar(), pstEquipCfg->initstep());
    if(NULL == pStarCfg) {
        return EQUIPLEVEL_CFG_MASTER->GetMaxLevel();
    }
    return pStarCfg->needlevel();
}

int EquipLibObj::getFullLvNeedExp(const int iEquipCfgID, const int iNowLv) {
    const int iMaxLv = getEquipMaxLvUpLv(iEquipCfgID);
    if(iNowLv >= iMaxLv) {
        return 0;
    }
    
    const int iDeltaLv = iMaxLv - iNowLv;
    int uTotalExp = 0;
    unsigned int uAddExp = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iEquipCfgID)->expmulti();
    for(int i = 0; i < iDeltaLv; i++){
        const EquipLevelCfg* pCfg = EQUIPLEVEL_CFG_MASTER->GetCfg(iNowLv + i);
        if(pCfg) {
            uTotalExp += pCfg->exp();
        }
        
    }

    uTotalExp = (unsigned int)(1.0*uTotalExp*uAddExp/10000);

    return uTotalExp;
}

bool EquipLibObj::addOneLvUpMat(const EquipSelMatOneInfo& info){
    if(isLvUpMatFull()) {
        return false;
    }
    UINT64_t uGuid = 0;
    int iType = 0;
    int iCfgID = 0;
    int iAddExp = 0;

    if(info.itemCfgID > 0) {
        uGuid = info.itemCfgID;
        iCfgID = info.itemCfgID;
        iType = vmsg::THING_TYPE_ITEM;
        const EquipExpItemCfg* pExpCfg = EQUIPEXPITEM_CFG_MASTER->GetEquipExpItemCfg( info.itemCfgID );
        if(pExpCfg) {
            iAddExp += pExpCfg->equipexp();
        }
    } else {
        iCfgID = info.stEquip.equipInfo.equipid();
        uGuid = info.stEquip.equipInfo.equipguid();
        iType = vmsg::THING_TYPE_EQUIP;
        //装备增加的经验要单独计算
        iAddExp += getEquipEatExp(info.stEquip.equipInfo.totalexp(), iCfgID);
    }
    m_stCfgIDMap[uGuid] = iCfgID;
    if(m_stLvUpMat.count(uGuid) <= 0) {
        vmsg::CSEquipLvUpConsume stConsume;
        stConsume.set_cnt(1);
        stConsume.set_id(uGuid);
        stConsume.set_type(iType);
        m_stLvUpMat[uGuid] = stConsume;
        m_iNowSelLvUpMatNum++;
        m_iTotalAddExp += iAddExp;
        return true;
    } 
    if(info.itemCfgID > 0) {
        vmsg::CSEquipLvUpConsume& stConsume = m_stLvUpMat[uGuid];
        stConsume.set_cnt( 1 + stConsume.cnt() );
        m_iNowSelLvUpMatNum++;
        m_iTotalAddExp += iAddExp;
        return true;
    }

    return false;
}

void EquipLibObj::clearLvUpMat() {
    m_stLvUpMat.clear();
    m_stCfgIDMap.clear();
    m_iNowSelLvUpMatNum = 0;
    m_iTotalAddExp = 0;
}


int EquipLibObj::getLevelsByExp(const int iEquipCfgID, const UINT64_t uEquipExp, const int iLv, const unsigned int uiExpAdd) {
    //neil,从后台拿的代码，修改一下
    
    unsigned int uiOldLv = iLv;
    unsigned int iFinalLv = uiOldLv; //最终等级
    UINT64_t uFinalExp = uEquipExp;

    UINT64_t u64ExpHas = uiExpAdd + uEquipExp;
    UINT64_t u64ExpAll = 0;
    unsigned int uiMaxLevel = EQUIPLEVEL_CFG_MASTER->GetMaxLevel();
    const EquipmentCfg* pstEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iEquipCfgID);
    if (pstEquipCfg == NULL)
    {
        return 0;
    }
    const EquipStarStepCfg* pstStarStepCfg = EQUIPSTARSTEP_CFG_MASTER->GetCfg(pstEquipCfg->initstar(), pstEquipCfg->initstep());
    if (pstStarStepCfg == NULL)
    {
        return 0;
    }
    for (unsigned int uiLevel = uiOldLv; uiLevel <= uiMaxLevel; uiLevel++)
    {

        const EquipLevelCfg* pstLvCfg = EQUIPLEVEL_CFG_MASTER->GetCfg(uiLevel);
        if (pstLvCfg == NULL)
        {
            CV_WARNING(("%s, equip level cfg cant find", __PRETTY_FUNCTION__));
            return 0;
        }

        if (uiLevel < uiMaxLevel)
        {
            const EquipLevelCfg* pstNextLvCfg = EQUIPLEVEL_CFG_MASTER->GetCfg(uiLevel+1);
            if (pstNextLvCfg == NULL)
            {
                CV_WARNING(("%s, equip level cfg cant find", __PRETTY_FUNCTION__));
                return 0;
            }
            if (pstStarStepCfg->needlevel() < uiLevel + 1)
            {
                if (uFinalExp > 0)
                {
                    iFinalLv = uiLevel;
                    uFinalExp = 0;
                }
                goto equiplvup_wb;
            }
        }

        UINT64_t uiExpLv = static_cast<UINT64_t>(pstLvCfg->exp() * 1.0 * pstEquipCfg->expmulti() / EGAME_DENOM);

        u64ExpAll += uiExpLv;

        if (u64ExpAll > u64ExpHas)//只能升到当前等级啦
        {
            iFinalLv = uiLevel;
            uFinalExp = u64ExpHas + uiExpLv - u64ExpAll;
            break;
        }

        //满级满经验
        if (uiLevel == uiMaxLevel
            && u64ExpAll <= u64ExpHas)
        {
            iFinalLv = uiMaxLevel;
            uFinalExp = uiExpLv;
            //pstEquip->m_ui64TotalExp += uiExpLv;
            break;
        }

        iFinalLv = uiLevel;
        uFinalExp = uiExpLv;
        //pstEquip->m_ui64TotalExp += pstEquip->m_ui64Exp;
    }
equiplvup_wb:
    int iDeltaLv = iFinalLv - uiOldLv;
    iDeltaLv = (iDeltaLv >= 0) ? iDeltaLv : 0;
    return iDeltaLv;
}

UINT64_t EquipLibObj::getEquipEatExp(const UINT64_t uTotalExp, const int iEquipCfgID) {
    UINT64_t uEatExp = uTotalExp;
    uEatExp = uEatExp * GLOBAL_CFG_MASTER->GetCfg().equipeatradio() / 10000.0;
    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID( iEquipCfgID );
    if(pstCfg) {
        uEatExp += pstCfg->baseexp();
    }

    return uEatExp;
}
