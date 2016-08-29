#include "WeaponRefitLayout.h"
#include "WeaponCfgMaster.h"
#include "defines.h"
#include "IconCell.h"
#include "firearms/FireArmsObj.h"
#include "WeaponRefitCfgMaster.h"
#include "WeaponRefitAdditionCfgMaster.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "WeaponUtils.h"
#include "bag/BagControl.h"
#include "WeaponLibControl.h"
#include "tip/TipControl.h"

using namespace std;

void WeaponRefitLayout::initWithLayout(UILayout *pRefLayout) {
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    m_pRefLayout->retain();
    m_pRefLayout->removeFromParent();

    //from equip info
    m_pFromColorValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("color_txt_0"));
    m_pFromAtkValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_val_0"));
    m_pFromAtkAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_attr_val_0"));

    //to equip info
    m_pToColorValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("color_txt_1"));
    m_pToAtkValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_val_1"));
    m_pToAtkAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_attr_val_1"));

    m_pDoRefitBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("do_refit_btn"));
    m_pDoRefitBtn->addTouchEventListener(this, toucheventselector(WeaponRefitLayout::touchEventHandler));

    UIImageView* pImg;
    IconCell* pCell;
    for(int i = 0; i < MAX_REFIT_ITEM_NUM; ++i) {
        pImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName(string("cell_" + intToString(i)).c_str() ));

        m_pCellBgLst[i] = (pImg);

        pCell = IconCell::create();
        pCell->showNumTxt();
        pImg->addChild(pCell);
        m_pItemCellLst[i] = pCell;

        pImg->addTouchEventListener(this, toucheventselector(WeaponRefitLayout::touchEventHandler));
        pImg->setTouchEnabled(true);
    }
}

void WeaponRefitLayout::touchEventHandler(CCObject* pSender, TouchEventType type) {
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    string str(pszWidgetName);
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "do_refit_btn") == 0) {
            doRefit();
        } else if(str.find_first_of("cell_") == 0) {
            string sub = str.substr(str.find_first_not_of("cell_"));
            int iIdx = atoi(sub.c_str());
            if(iIdx >= 0 && iIdx < MAX_REFIT_ITEM_NUM) {
                IconCell* pCell = m_pItemCellLst[iIdx];
                //显示材料掉落提示面板
                if(!(pCell->getItemCfgid() == 0 && pCell->getItemType() == 0 && pCell->getItemNum() == 0)){
					P_TIP_CTRL->showItemTipPanel(pCell->getItemType(), pCell->getItemCfgid(), pCell->getItemNum());
                }
            }
        }
    }
}

void WeaponRefitLayout::updateWeaponInfo(const UINT64_t weaponGUID){
    m_uWeaponGuid = weaponGUID;
    m_bIsMaterialEnough = false;
    const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);

    //get cfg
    const WeaponCfg* pstWeaponCfg =  WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeaponInfo.cfgid());
    if(NULL == pstWeaponCfg) {
        CCLOG("[error]WeaponRefitLayout::updateWeaponInfo,fail to get weapon cfg,cfgid=%d", stWeaponInfo.cfgid());
        return;
    }
    const WeaponRefitAdditionCfg* pstRefitAdditionCfg = WEAPONREFITADDITION_CFG_MASTER->GetCfg(stWeaponInfo.quality());
    if(NULL == pstRefitAdditionCfg) {
        CCLOG("[error]WeaponRefitLayout::updateWeaponInfo,fail to getWeaponRefitAdditionCfg,color=%d", stWeaponInfo.quality());
        return;
    }
    const int iToColor = stWeaponInfo.quality() + 1;
    const WeaponRefitAdditionCfg* pstNextRefitAdditionCfg = WEAPONREFITADDITION_CFG_MASTER->GetCfg(iToColor);
    if(NULL == pstNextRefitAdditionCfg) {
        //获取改装后的配置，如果失败，提示改装满了
        CCLOG("[warn]WeaponRefitLayout::updateWeaponInfo,fail to getWeaponRefitAdditionCfg,color=%d, may be color is max", 
            stWeaponInfo.quality() + 1);
        COMMON_DIALOG->show(TextStr(WEAPON_REFIT_ENOUGH), NULL, NounStr(OK));
        return;
    }
    //获取消耗物品配置
    const WeaponRefitCfg* pstRefitCfg =  WEAPONREFIT_CFG_MASTER->GetCfg(pstWeaponCfg->weapontype(), iToColor);
    if(NULL == pstRefitCfg) {
        CCLOG("[error]WeaponRefitLayout::updateWeaponInfo,fail to WeaponRefitCfg,weaponid=%d,color=%d",
            pstWeaponCfg->weaponid(),  stWeaponInfo.quality());
        return;
    }

    //set from equip
	unsigned int uFromWeaponAddCoe = WeaponUtils::getWeaponAddCoe(stWeaponInfo.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality());
    m_pFromColorValTxt->setText( intToString(stWeaponInfo.quality()) );
    m_pFromAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(stWeaponInfo.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality()) + uFromWeaponAddCoe)); //强化攻击力+ Atk*颜色加成
    m_pFromAtkAttrTxt->setText( intToString(WeaponUtils::getWeaponAtkAttr(stWeaponInfo.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality()) ) );


    //set to equip
	unsigned int uToWeaponAddCoe = WeaponUtils::getWeaponAddCoe(stWeaponInfo.cfgid(), stWeaponInfo.level(), iToColor);
    m_pToColorValTxt->setText(intToString(iToColor));
    m_pToAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(stWeaponInfo.cfgid(), stWeaponInfo.level(), iToColor) + uToWeaponAddCoe)); //强化攻击力+ Atk*颜色加成
    m_pToAtkAttrTxt->setText( intToString(WeaponUtils::getWeaponAtkAttr(stWeaponInfo.cfgid(), stWeaponInfo.level(), iToColor ) ) );


    //show cost items
    UIImageView* pImg;
    IconCell* pCell;
    m_bIsMaterialEnough = true; //默认为true吧
    int i = 0;
    for(i = 0; i < pstRefitCfg->elmtlist_size(); i++){
        if(i >= MAX_REFIT_ITEM_NUM){
            break;
        }
        const RefitConsumeElement& stElmt = pstRefitCfg->elmtlist(i);
        pImg = m_pCellBgLst[i];
        pImg->setVisible(true);
        pCell = m_pItemCellLst[i];
        pCell->setInfo(stElmt.elementtype(), stElmt.elementcnt(), stElmt.elementid());
        setMaterialNumTxt(stElmt.elementid(), stElmt.elementcnt(), pCell->getNumTxt());
    }
    for(; i < MAX_REFIT_ITEM_NUM; ++i) {
        pImg = m_pCellBgLst[i]; //其他的不显示
        pImg->setVisible(false);
    }
    if(pstRefitCfg->elmtlist_size() == 0) {
        m_bIsMaterialEnough = true;
    }
}


void WeaponRefitLayout::setMaterialNumTxt(const int iCfgID, const int iCnt, UILabel* pTxt) {
    if(NULL == pTxt) {
        return;
    }
    const int iBagItemNum = P_BAG_CTRL->getItemCnt(iCfgID);
    if(iBagItemNum >= iCnt) {
        //够了
        pTxt->setColor(ccc3(0x00, 0xFF, 0x00));
    } else {
        pTxt->setColor(ccc3(0xFF, 0x00, 0x00));
        m_bIsMaterialEnough = false; //只要有一个不够就不行
    }
    char tmp[30];
    snprintf(tmp, 30, "%d/%d", iBagItemNum, iCnt);
    pTxt->setText(tmp);

}
void WeaponRefitLayout::doRefit() {
    if(m_uWeaponGuid <= 0) {
        CCLOG("[error]WeaponRefitLayout::doRefit, invalid weapon guid!");
        COMMON_DIALOG->show(TextStr(WEAPON_CHOOSE_WEAPON), NULL, NounStr(OK));
        return;
    }

    if(m_bIsMaterialEnough == false){
        COMMON_DIALOG->show(TextStr(WEAPON_NO_ENOUGH), NULL, NounStr(OK));
        return;
    }

    int bagIdx = P_ARM_OBJ->getWeaponIdx(m_uWeaponGuid);

    P_WEAPON_CTRL->getWeaponInfoPanel()->showLoading();
    P_WEAPON_CTRL->sendWeaponRefitRqst(bagIdx, m_uWeaponGuid);
}


void WeaponRefitLayout::resetUI(){

}


