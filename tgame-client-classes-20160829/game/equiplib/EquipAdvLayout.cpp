#include "EquipAdvLayout.h"
#include "IconCell.h"
#include "defines.h"
#include "EquipLibControl.h"
#include "EquipStarStepCfgMaster.h"
#include "bag/BagControl.h"
#include "CommonDialog.h"
#include "weaponlib/WeaponLibControl.h"
#include "firearms/StarProgressBar.h"
#include "UserObj.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "PopTipLayer.h"
#include "EquipmentCfgMaster.h"
#include "ItemUtils.h"
#include "tip/TipControl.h"

using namespace std;

bool compareEquipLv(const CSEquipInfo& stInfoA, const CSEquipInfo& stInfoB) {
    return (stInfoA.level() < stInfoB.level() );
}


void EquipAdvLayout::initWithLayout(UILayout *pRefLayout) {
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

void EquipAdvLayout::initUI() {
    if(NULL == m_pRefLayout) {
        return;
    }
    
    m_pFromMaxLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("from_max_lv_txt"));
    m_pFromAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("from_attr_txt"));
    m_pFromGrowupAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("from_growup_attr_txt"));

    m_pAttrNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("attr_name_txt"));

    m_pToMaxLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("to_max_lv_txt"));
    m_pToAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("to_attr_txt"));
    m_pToGrowupAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("to_growup_attr_txt"));

    UIButton* pDoAdvBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("do_adv_btn"));
    pDoAdvBtn->addTouchEventListener(this, toucheventselector(EquipAdvLayout::touchEventHandler));

    UIImageView* pImg;
    IconCell* pCell;
    for(int i = 0; i < MAX_EQUIP_ADV_ITEM_NUM; ++i) {
        pImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName(string("cell_" + intToString(i)).c_str() ));
        pImg->addTouchEventListener(this, toucheventselector(EquipAdvLayout::touchCellEventHandler));
        pImg->setTouchEnabled(true);

        m_pCellBgLst[i] = (pImg);

        pCell = IconCell::create();
        pCell->showNumTxt();
        pImg->addChild(pCell);
        m_pItemCellLst[i] = pCell;
    }

    m_pCoinTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("coin_txt")); //coin_txt

}

void EquipAdvLayout::touchCellEventHandler(CCObject* pSender, TouchEventType type) {
    //点击了材料图标
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    string strWidgetName(pWidget->getName());
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strWidgetName.find_first_of("cell_") == 0) {
            string sub = strWidgetName.substr(strWidgetName.find_first_not_of("cell_"));
            int iIdx = atoi(sub.c_str());
            if(iIdx >= 0 && iIdx < MAX_EQUIP_ADV_ITEM_NUM) {
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

void EquipAdvLayout::touchEventHandler(CCObject* pSender, TouchEventType type) {
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "do_adv_btn") == 0) {
            doAdv();
        }
    }
}

void EquipAdvLayout::doUpdateUI() {
    if(NULL == m_pRefLayout) {
        return;
    }

    const UINT64_t& uEquipGuid = P_EQUIP_LIB_CTRL->getObj()->getSelEquipGuid();
    if(uEquipGuid <= 0) {
        //暂时没有选择装备
        return;
    }
    const EquipLibOneInfo stEquip = P_EQUIP_LIB_CTRL->getObj()->getEquipInfo(uEquipGuid);
    if(stEquip.equipInfo.equipguid() <= 0) {
        CCLOG("[warn]EquipAdvLayout::doUpdateUI, fail to find equip guid %llu", uEquipGuid);
        return;
    }

    const int iEquipCfgID = stEquip.equipInfo.equipid();

    m_uEquipGuid = uEquipGuid;
    m_iEquipBagIdx = stEquip.iBagIdx;
    m_iEquipCfgID = iEquipCfgID;
    m_iEquipLv = stEquip.equipInfo.level();

    //update ui
    //1. set from equip info
    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(iEquipCfgID );
    const EquipStarStepCfg* pstStarCfg = EQUIPSTARSTEP_CFG_MASTER->GetCfg(pstCfg->initstar(), pstCfg->initstep());
    if(NULL == pstStarCfg) {
        CCLOG("[warn]EquipAdvLayout::doUpdateUI, fail to get equip star step cfg,star=%d,step=%d", pstCfg->initstar(), pstCfg->initstep());
        return;
    }

	const int iFromMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(iEquipCfgID);
	m_pFromMaxLvTxt->setText(intToString( stEquip.equipInfo.level() ) + "/" + intToString(iFromMaxLv) );
	int iAttrVal = ItemUtils::getEquipAttrVal(iEquipCfgID, stEquip.equipInfo.level(),  stEquip.equipInfo.quality());
	m_pFromAttrTxt->setText( "+" + intToString( iAttrVal ));
	m_pAttrNameTxt->setText(AttrTypeStr(pstCfg->attr().attrid()) );
	m_pFromGrowupAttrTxt->setText(intToString(pstCfg->attradd()) );

    if(pstStarCfg->star() == pstStarCfg->nextstar() && pstStarCfg->step() == pstStarCfg->nextstep()){  
        //星级、阶段不再升级
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(WEAPON_STAR_UP_ENOUGH));
		//隐藏多余信息
		CCArray* i_childrenArray =  m_pRefLayout->getChildren();
		CCObject* i_obj = nullptr;
		CCARRAY_FOREACH(i_childrenArray, i_obj)
		{
			Widget* i_widget = dynamic_cast<Widget*>(i_obj);
			if (i_widget)
			{
				if(!(strcmp( i_widget->getName() ,"title" ) ==0 ||  strcmp( i_widget->getName() , "Label_72" ) == 0 ||
					 strcmp(i_widget->getName() , "Label_72_0" ) == 0||  strcmp (i_widget->getName() , "Label_72_1") == 0
					|| i_widget == m_pFromAttrTxt || i_widget == m_pFromGrowupAttrTxt || i_widget == m_pFromMaxLvTxt || i_widget == m_pAttrNameTxt))
				{
					i_widget->setEnabled(false);
				}
			}
		}
        return;
    }

    

    //2.set to equip info
    const EquipmentCfg* pstToEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByStarStep(pstCfg->equipid(), pstStarCfg->nextstar(), pstStarCfg->nextstep() );
    if(NULL == pstToEquipCfg) {
        CCLOG("[warn]EquipAdvLayout::doUpdateUI, fail to get to equip cfg,star=%d,step=%d", pstStarCfg->nextstar(), pstStarCfg->nextstep() );
        return;
    }

    const int iToMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(pstToEquipCfg->equipcfgid() );
    m_pToMaxLvTxt->setText(intToString( stEquip.equipInfo.level() ) + "/" + intToString(iToMaxLv) );
    iAttrVal = ItemUtils::getEquipAttrVal(pstToEquipCfg->equipcfgid(), stEquip.equipInfo.level(), stEquip.equipInfo.quality());
    m_pToAttrTxt->setText( "+" + intToString( iAttrVal ));
    m_pToGrowupAttrTxt->setText(intToString(pstToEquipCfg->attradd()) );
    
    //3. set need mat
    int iNowCell = 0;
    UIImageView* pImg;
    IconCell* pCell;
    m_bIsMatEnough = true; //默认为true
    if(pstStarCfg->needequipcnt() > 0) {
        pImg = m_pCellBgLst[iNowCell];
        pImg->setVisible(true);
        pCell = m_pItemCellLst[iNowCell];
        pCell->setInfo(vmsg::THING_TYPE_EQUIP, pstStarCfg->needequipcnt(), iEquipCfgID );
        setMaterialNumTxt(vmsg::THING_TYPE_EQUIP, pstStarCfg->needequipcnt(), iEquipCfgID, pCell->getNumTxt());
        iNowCell++;
    }
    for(int i = 0; i < pstStarCfg->needs_size(); ++i) {
        if(iNowCell >= MAX_EQUIP_ADV_ITEM_NUM) {
            break;
        }
        pImg = m_pCellBgLst[iNowCell];
        pImg->setVisible(true);
        pCell = m_pItemCellLst[iNowCell];
        pCell->setInfo(pstStarCfg->needs(i).type(), pstStarCfg->needs(i).cfgcnt(), pstStarCfg->needs(i).cfgid() );
        setMaterialNumTxt(pstStarCfg->needs(i).type(), pstStarCfg->needs(i).cfgcnt(), 
            pstStarCfg->needs(i).cfgid(), pCell->getNumTxt());

        iNowCell++;
    }
    for(; iNowCell < MAX_EQUIP_ADV_ITEM_NUM; ++iNowCell) {
        pImg = m_pCellBgLst[iNowCell]; //其他的不显示
        pImg->setVisible(false);
    }

    //4.set cost coin
    m_pCoinTxt->setText(intToString(pstStarCfg->costcoin()));

}

void EquipAdvLayout::setMaterialNumTxt(const int iType, const int iCnt, const int iCfgID, UILabel* pTxt) {
    if(NULL == pTxt) {
        m_bIsMatEnough = false;
        return;
    }
    int iBagItemNum = 0;
    if(vmsg::THING_TYPE_EQUIP == iType) {
        iBagItemNum = P_BAG_CTRL->getEquipCnt(iCfgID, m_uEquipGuid );
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

void EquipAdvLayout::doAdv() {
    if(m_uEquipGuid <= 0) {
        //COMMON_DIALOG->show(TextStr(TEXT_SEL_EQUIP_TIP));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_SEL_EQUIP_TIP));
        return;
    }

    const int iMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(m_iEquipCfgID);
    if(m_iEquipLv < iMaxLv) {
        //COMMON_DIALOG->show(TextStr(TEXT_EQUIP_ADV_TIP));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_EQUIP_ADV_TIP));
        return;
    }
    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(m_iEquipCfgID );
    const EquipStarStepCfg* pstStarCfg = EQUIPSTARSTEP_CFG_MASTER->GetCfg(pstCfg->initstar(), pstCfg->initstep());
    if(NULL == pstStarCfg) {
        CCLOG("[warn]EquipAdvLayout::doUpdateUI, fail to get equip star step cfg,star=%d,step=%d", pstCfg->initstar(), pstCfg->initstep());
        return;
    }
    if(pstStarCfg->star() == pstStarCfg->nextstar() && pstStarCfg->step() == pstStarCfg->nextstep()){  
        //星级、阶段不再升级
        //COMMON_DIALOG->show(TextStr(WEAPON_STAR_UP_ENOUGH));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(WEAPON_STAR_UP_ENOUGH));
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

    //如果消耗的有装备，优先使用背包中CfgID相同的低等级装备
    
    vector<CSEquipInfo> stNeedEquipLst;
    if(pstStarCfg->needequipcnt() > 0) {
        const vmsg::CSBagInfo& stBag = P_BAG_CTRL->getBagInfo();
        for (int i = 0; i < stBag.celllst_size(); ++i) {
            if (!stBag.celllst(i).has_equipinfo() ) {
                continue;
            }
            const vmsg::CSEquipInfo& stOneEquip = stBag.celllst(i).equipinfo();
            //排除掉当前选择的装备
            if (stOneEquip.equipid() == m_iEquipCfgID && stOneEquip.equipguid() != m_uEquipGuid ) {
                stNeedEquipLst.push_back( stOneEquip );
            }
        }
        if(stNeedEquipLst.size() < pstStarCfg->needequipcnt()) {
            COMMON_DIALOG->show(TextStr(WEAPON_NO_ENOUGH));
            return;
        }

        //sort
        sort(stNeedEquipLst.begin(), stNeedEquipLst.end(), compareEquipLv);
    }
    


    vector<UINT64_t> stGuidLst;
    unsigned int i = 0;
    for(; i < stNeedEquipLst.size(); ++i) {
        stGuidLst.push_back(stNeedEquipLst[i].equipguid() );
        if(stGuidLst.size() >= pstStarCfg->needequipcnt()) {
            break;
        }
    }

    //只放装备的GUID就可以了，后台会根据配置扣材料

    //send
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->showLoading(); //显示loading
    P_EQUIP_LIB_CTRL->getProxy()->sendEquipAdvRqst(m_iEquipBagIdx, m_uEquipGuid, stGuidLst);
}

 void EquipAdvLayout::resetUI(){

     m_pFromMaxLvTxt->setText("0");
     m_pFromAttrTxt->setText("0");
     m_pFromGrowupAttrTxt->setText("0");
     m_pAttrNameTxt->setText("0");

     m_pToMaxLvTxt->setText("0");
     m_pToAttrTxt->setText("0");
     m_pToGrowupAttrTxt->setText("0");

     m_pCoinTxt->setText("0");

     for(int i = 0; i < MAX_EQUIP_ADV_ITEM_NUM; ++i) {
         m_pItemCellLst[i]->clearCell();
         m_pCellBgLst[i]->setVisible(true);
     }

     m_iEquipBagIdx = -1;
     m_uEquipGuid = 0;
     m_iEquipCfgID = 0;
     m_iEquipLv = 0;

 }
