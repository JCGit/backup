#include "EquipMixLayout.h"
#include "IconCell.h"
#include "defines.h"
#include "EquipLibControl.h"
#include "weaponlib/WeaponLibControl.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "ItemUtils.h"
#include "EquipRefitCfgMaster.h"
#include "EquipStarStepCfgMaster.h"
#include "bag/BagControl.h"
#include "UserObj.h"
#include "EquipRefitAdditionCfgMaster.h"
#include "PopTipLayer.h"
#include "EquipmentCfgMaster.h"
#include "tip/TipControl.h"

using namespace std;

void EquipMixLayout::initWithLayout(UILayout *pRefLayout) {
    CC_SAFE_RELEASE_NULL(m_pRefLayout);
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    m_pRefLayout->retain();
    m_pRefLayout->removeFromParent();

    initUI();
    resetUI();
}


void EquipMixLayout::initUI() {
    if(NULL == m_pRefLayout) {
        return;
    }

    m_pAttrNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("attr_name_txt"));

    m_pFromMixLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("from_mix_lv_txt"));
    m_pFromAttrValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("from_attr_val_txt"));

    m_pToAttrValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("to_attr_val_txt"));
    m_pToMixLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("to_mix_lv_txt"));

    m_pCoinTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("coin_txt"));

    UIButton* pDoBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("do_mix_btn"));
    pDoBtn->addTouchEventListener(this, toucheventselector(EquipMixLayout::touchEventHandler));

    UIImageView* pImg;
    IconCell* pCell;
    for(int i = 0; i < MAX_EQUIP_MIX_ITEM_NUM; ++i) {
        pImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName(string("cell_" + intToString(i)).c_str() ));
        pImg->addTouchEventListener(this, toucheventselector(EquipMixLayout::touchCellEventHandler));
        pImg->setTouchEnabled(true);

        m_pCellBgLst[i] = (pImg);

        pCell = IconCell::create();
        pCell->showNumTxt();
        pImg->addChild(pCell);
        m_pItemCellLst[i] = pCell;
    }
    m_pCoinTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("coin_txt")); //coin_txt
}

void EquipMixLayout::touchCellEventHandler(CCObject* pSender, TouchEventType type) {
    //点击了材料图标
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    string strWidgetName(pWidget->getName());
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strWidgetName.find_first_of("cell_") == 0) {
            string sub = strWidgetName.substr(strWidgetName.find_first_not_of("cell_"));
            int iIdx = atoi(sub.c_str());
            if(iIdx >= 0 && iIdx < MAX_EQUIP_MIX_ITEM_NUM) {
                IconCell* pCell = m_pItemCellLst[iIdx];
                //显示材料掉落提示面板
                if(!(pCell->getItemCfgid() == 0 && pCell->getItemType() == 0 && pCell->getItemNum() == 0)){
					P_TIP_CTRL->showItemTipPanel(pCell->getItemType(), pCell->getItemCfgid(), 1);
                }
            }
        }
        break;
    }
}

void EquipMixLayout::touchEventHandler(CCObject* pSender, TouchEventType type) {
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "do_mix_btn") == 0) {
            doMix();
        }
    }
}

const char* EquipMixLayout::getCellColorImg(const int iColor) {
    switch (iColor)
    {
    case vmsg::EQUIP_GRADE_WHITE:
        return "bag/color_white.png";
        break;
    case vmsg::EQUIP_GRADE_GREEN:
        return "bag/color_green.png";
        break;
    case vmsg::EQUIP_GRADE_BLUE:
        return "bag/color_blue.png";
        break;
    case vmsg::EQUIP_GRADE_PURPLE:
        return "bag/color_purple.png";
        break;
    case vmsg::EQUIP_GRADE_ORANGE:
        return "bag/color_orange.png";
        break;
    default:
        return "bag/color_white.png";
        break;
    }
}

void EquipMixLayout::resetUI(){
    m_pAttrNameTxt->setText("");
    m_pFromMixLvTxt->setText("0");
    m_pFromAttrValTxt->setText("0");

    m_pToAttrValTxt->setText("0");
    m_pToMixLvTxt->setText("0");
    m_pCoinTxt->setText("0");

    m_pCoinTxt->setText("0");

    for(int i = 0; i < MAX_EQUIP_MIX_ITEM_NUM; ++i) {
        m_pItemCellLst[i]->clearCell();
        m_pCellBgLst[i]->setVisible(true);
    }

}

void EquipMixLayout::doMix() {
    if(m_uEquipGuid <= 0) {
        //COMMON_DIALOG->show(TextStr(TEXT_SEL_EQUIP_TIP));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_SEL_EQUIP_TIP));
        return;
    }
    
    if(!m_bIsMatEnough) {
        //COMMON_DIALOG->show(TextStr(WEAPON_NO_ENOUGH));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(WEAPON_NO_ENOUGH));
        return;
    }

    //check coin
    const int iNeedCoin = atoi(m_pCoinTxt->getStringValue());
    if(iNeedCoin > 0) {
        //联邦币不足
        if(!P_TIP_CTRL->CoinCostTip(iNeedCoin, this)){
            return;
        }
    }

    //send
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->showLoading(); //显示loading
    P_EQUIP_LIB_CTRL->getProxy()->sendEquipMixRqst(m_iEquipBagIdx, m_uEquipGuid);
}


void EquipMixLayout::doUpdateUI() {
    if(NULL == m_pRefLayout) {
        return;
    }

    const UINT64_t& uEquipGuid = P_EQUIP_LIB_CTRL->getObj()->getSelEquipGuid();
    if(uEquipGuid <= 0) {
        //暂时没有选择装备,不提示
        return;
    }
    const EquipLibOneInfo stEquip = P_EQUIP_LIB_CTRL->getObj()->getEquipInfo(uEquipGuid);
    if(stEquip.equipInfo.equipguid() <= 0) {
        CCLOG("[warn]EquipAdvLayout::doUpdateUI, fail to find equip guid %llu", uEquipGuid);
        return;
    }

    //TODO:要改
    const unsigned int iMaxQuality = EQUIPREFITADDITION_CFG_MASTER->GetMaxQuality();
    if(stEquip.equipInfo.quality() >= iMaxQuality) {
        //COMMON_DIALOG->show(TextStr(TEXT_EQUIP_MIX_FULL_TIP));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_EQUIP_MIX_FULL_TIP));
        return;
    }
    
    m_uEquipGuid = uEquipGuid;
    m_iEquipBagIdx = stEquip.iBagIdx;
    m_iEquipCfgID = stEquip.equipInfo.equipid();
    m_iEquipLv = stEquip.equipInfo.level();

    const int iToEquipColor = stEquip.equipInfo.quality() + 1; //融合后品级
    //1. set from equip info
    const EquipmentCfg* pstEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(m_iEquipCfgID );
    const EquipRefitCfg* pRefitCfg = EQUIPREFIT_CFG_MASTER->GetCfg(pstEquipCfg->part(), iToEquipColor);
    if(NULL == pRefitCfg) {
        CCLOG("[warn]EquipAdvLayout::doUpdateUI, fail to get EquipRefitCfg,part=%d,to color=%d", pstEquipCfg->part(), iToEquipColor );
        return;
    }

    m_pAttrNameTxt->setText( AttrTypeStr(pstEquipCfg->attr().attrid()) );
   
    int iShowAttrVal = ItemUtils::getEquipShowAttrVal(stEquip.equipInfo.equipid(), stEquip.equipInfo.level(), stEquip.equipInfo.quality() ); //强化属性显示
    m_pFromAttrValTxt->setText("+" + intToString(iShowAttrVal) );
    m_pFromMixLvTxt->setText(intToString(stEquip.equipInfo.quality()) + "/" + intToString(iMaxQuality) );

    //2. set to equip info
    iShowAttrVal = ItemUtils::getEquipShowAttrVal(stEquip.equipInfo.equipid(), stEquip.equipInfo.level(), iToEquipColor ); //强化属性显示
    m_pToAttrValTxt->setText("+" + intToString(iShowAttrVal) );
    m_pToMixLvTxt->setText(intToString(iToEquipColor)  + "/" + intToString(iMaxQuality) );

    //3. set coin
    m_pCoinTxt->setText( intToString(pRefitCfg->costcoin()) );

    //4. set mat
    int iNowCell = 0;
    UIImageView* pImg;
    IconCell* pCell;
    m_bIsMatEnough = true; //默认为true

    for(int i = 0; i < pRefitCfg->elmtlist_size(); ++i) {
        if(iNowCell >= MAX_EQUIP_MIX_ITEM_NUM) {
            break;
        }
        pImg = m_pCellBgLst[iNowCell];
        pImg->setVisible(true);
        pCell = m_pItemCellLst[iNowCell];
        pCell->setInfo(pRefitCfg->elmtlist(i).elementtype(), pRefitCfg->elmtlist(i).elementcnt(), pRefitCfg->elmtlist(i).elementid() );
        setMaterialNumTxt(pRefitCfg->elmtlist(i).elementtype(), pRefitCfg->elmtlist(i).elementcnt(), 
            pRefitCfg->elmtlist(i).elementid(), pCell->getNumTxt());

        iNowCell++;
    }
    for(; iNowCell < MAX_EQUIP_MIX_ITEM_NUM; ++iNowCell) {
        pImg = m_pCellBgLst[iNowCell]; //其他的不显示
        pImg->setVisible(false);
    }

}


void EquipMixLayout::setMaterialNumTxt(const int iType, const int iCnt, const int iCfgID, UILabel* pTxt) {
    if(NULL == pTxt) {
        m_bIsMatEnough = false;
        return;
    }
    int iBagItemNum = 0;
    if(vmsg::THING_TYPE_EQUIP == iType) {
        iBagItemNum = P_BAG_CTRL->getEquipCnt(iCfgID, m_uEquipGuid);
    } else {
        iBagItemNum = P_BAG_CTRL->getItemCnt(iCfgID); //此时，cnt代表需要的材料个数
    }
    if(iBagItemNum >= iCnt) {
        //够了
        iBagItemNum = iCnt;
        pTxt->setColor(ccc3(0x00, 0xFF, 0x00)); 
    } else {
        m_bIsMatEnough = false;
        pTxt->setColor(ccc3(0xFF, 0x00, 0x00));

    }
    char tmp[30];
    snprintf(tmp, 30, "%d/%d", iBagItemNum, iCnt);
    pTxt->setText(tmp);

}
