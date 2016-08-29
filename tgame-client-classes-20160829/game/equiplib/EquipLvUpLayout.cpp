#include "EquipLvUpLayout.h"
#include "EquipLibControl.h"
#include "defines.h"
#include "IconCell.h"
#include "EquipLevelCfgMaster.h"
#include "GlobalCfgMaster.h"
#include "CommonDialog.h"
#include "UserObj.h"
#include "EquipExpItemCfgMaster.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "firearms/StarProgressBar.h"
#include "game/guide/GuideControl.h"
#include "PopTipLayer.h"
#include "Global.h"
#include "ToolUtils.h"
#include "control/AudioManager.h"
#include "EquipmentCfgMaster.h"
#include "ItemUtils.h"
#include "tip/TipControl.h"
#include "../city/CityControl.h"
#include "../city/EntryControl.h"
#include "../../ProtoDescCpp/ModuleID.pb.h"

using namespace std;

void EquipLvUpLayout::initWithLayout(UILayout *pRefLayout) {
    CC_SAFE_RELEASE_NULL(m_pRefLayout);
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    m_pRefLayout->retain();
    m_pRefLayout->removeFromParent();
    initUI();
}

void EquipLvUpLayout::initUI() {

    m_pFullRemainExp = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("full_remain_exp"));
    m_pTotalGetExp = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("total_get_exp"));
    m_pCoinTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("coin_txt"));

    UIImageView* pImg;
    IconCell* pCell;
    for(int i = 0; i < EQUIP_SEL_MAT_MAX_NUM; ++i) {
        pImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName(string("cell_" + intToString(i)).c_str() ));
        pImg->addTouchEventListener(this, toucheventselector(EquipLvUpLayout::touchCellEventHandler));
        pImg->setTouchEnabled(true);
        pImg->setTag(i);
        m_pCellBgLst[i] = (pImg);

        pCell = IconCell::create();
        //pCell->showNumTxt();
        pImg->addChild(pCell);
        m_pItemCellLst[i] = pCell;
    }

    m_pAutoSelBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("auto_btn"));
    m_pAutoSelBtn->addTouchEventListener(this, toucheventselector(EquipLvUpLayout::touchCellEventHandler));

    m_pDoLvUpBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("do_lv_up_btn"));
    m_pDoLvUpBtn->addTouchEventListener(this, toucheventselector(EquipLvUpLayout::touchCellEventHandler));

    resetUI();
}


void EquipLvUpLayout::touchCellEventHandler(CCObject* pSender, TouchEventType type) {
    //点击了材料图标
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    string strWidgetName(pWidget->getName());
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strWidgetName.find_first_of("cell_") == 0) {
            if(m_stEquip.equipguid() <= 0) {
                POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_SEL_EQUIP_TIP));
                break;
            }
            P_EQUIP_LIB_CTRL->getEquipSelMatPanel()->open();
        } else if(strWidgetName == "auto_btn") {
			GUIDE_TOUCH(pWidget);
            doAutoSel();
			GUIDE_SHOW(GUIDE_UI_EQUIP_LIB);
        }  else if(strWidgetName == "do_lv_up_btn") {
			GUIDE_TOUCH(pWidget);
            doLvUp();
			GUIDE_SHOW(GUIDE_UI_EQUIP_LIB);
        } 
        break;
    }
}



void EquipLvUpLayout::setInfo(const CSEquipInfo& stEquip, const int iBagIdx) {
    m_stEquip = stEquip;
    m_iEquipBagIdx = iBagIdx;

    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquip.equipid() );
    if(NULL == pstCfg) {
        return;
    }

    const int iMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(stEquip.equipid());
    //升级还需经验
    int iNeedExp = P_EQUIP_LIB_CTRL->getObj()->getFullLvNeedExp(m_stEquip.equipid(), m_stEquip.level()); //升级满所需要的经验
    if(iNeedExp >= m_stEquip.exp()) {
        iNeedExp = iNeedExp - m_stEquip.exp(); //要减去当前等级已经有的经验
    }
    
    m_pFullRemainExp->setText(intToString(iNeedExp));

}

void EquipLvUpLayout::resetUI() {

    m_pFullRemainExp->setText("0");
    m_pTotalGetExp->setText("0");
    m_pCoinTxt->setText("0");

    m_pAutoSelBtn->setTouchEnabled(true);
    m_pDoLvUpBtn->setTouchEnabled(true);
    
    resetMatCell();
}

UIButton* EquipLvUpLayout::getAutoSelBtn() {
	if(m_pAutoSelBtn != NULL && m_pAutoSelBtn->isVisible() && m_pAutoSelBtn->isTouchEnabled()) {
		return m_pAutoSelBtn;
	}

	return NULL;
}

UIButton* EquipLvUpLayout::getLvUpBtn() {
	if(m_pDoLvUpBtn != NULL && m_pDoLvUpBtn->isVisible() && m_pDoLvUpBtn->isTouchEnabled()) {
		return m_pDoLvUpBtn;
	}

	return NULL;
}

void EquipLvUpLayout::doAutoSel() {
    if(m_stEquip.equipguid() <= 0) {
        //COMMON_DIALOG->show(TextStr(TEXT_SEL_EQUIP_TIP));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_SEL_EQUIP_TIP));
        return;
    }

    const int iMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(m_stEquip.equipid());
    if(m_stEquip.level() >= (unsigned int)iMaxLv) {
        //COMMON_DIALOG->show(TextStr(WEAPON_FULL_LEVEL));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(WEAPON_FULL_LEVEL));
        return;
    }

    const vector<EquipSelMatOneInfo>& stMatLst = P_EQUIP_LIB_CTRL->getEquipSelMatPanel()->findMat();
    int iMatNum = stMatLst.size();
    iMatNum = (iMatNum < EQUIP_SEL_MAT_MAX_NUM) ? iMatNum : EQUIP_SEL_MAT_MAX_NUM;
    if(iMatNum <= 0) {
        //POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_EQUIP_MAT_NULL));
		//P_TIP_CTRL->getItemLackPanel()->promptBuyItem(vmsg::RMB_STORE_ITEM_EQUIP_EXP_PKG);
		COMMON_DIALOG->show(TextStr(WEAPON_NO_ENOUGH_GOTOBUY), this, NounStr(NOUN_GOTO), "", dialogbtneventselector(EquipLvUpLayout::onClickDialogGotoBtn));
        return;
    }
    int iNeedExp = P_EQUIP_LIB_CTRL->getObj()->getFullLvNeedExp(m_stEquip.equipid(), m_stEquip.level()); //升级满所需要的经验
    iNeedExp = iNeedExp - m_stEquip.exp(); //要减去当前等级已经有的经验
    if(iNeedExp <= 0) {
        CCLOG("[warn]EquipLvUpLayout::doAutoSel, need exp is 0!");
        return;
    }

    P_EQUIP_LIB_CTRL->getObj()->clearLvUpMat();
    int iTotalExp = 0;
    for(int i = 0; i < iMatNum; ++i) {
		if(stMatLst[i].itemCfgID <= 0) {  //装备不加入自动加入材料
			continue;
		}

        P_EQUIP_LIB_CTRL->getObj()->addOneLvUpMat( stMatLst[i] );
        //现在一次只选择升一级所需的材料就停止吧
        if(stMatLst[i].itemCfgID > 0 ) {
            const EquipExpItemCfg* pExpCfg = EQUIPEXPITEM_CFG_MASTER->GetEquipExpItemCfg( stMatLst[i].itemCfgID );
            if(pExpCfg) {
                iTotalExp += pExpCfg->equipexp();
            } 
        } else {
            iTotalExp += stMatLst[i].stEquip.equipInfo.totalexp();
        }
        if(iTotalExp >= iNeedExp) {
            break;
        }
    }
    updateSelMat();
}

void EquipLvUpLayout::onClickDialogGotoBtn()
{
	if(P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_BLACKMARKET) || P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_PANIC_BUYING_STORE)
		)
	{
		P_ENTRY_CTRL->enter(ENTRY_BUSINESS);
	}else
	{
		POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_MODULE_NOOPEN));
	}
}


void EquipLvUpLayout::doLvUp() {
    if(m_stEquip.equipguid() <= 0) {
        //COMMON_DIALOG->show(TextStr(TEXT_SEL_EQUIP_TIP));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_SEL_EQUIP_TIP));
        return;
    }
    const int iMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(m_stEquip.equipid());
    if(m_stEquip.level() >= (unsigned int)iMaxLv) {
        //COMMON_DIALOG->show(TextStr(WEAPON_FULL_LEVEL), NULL, NounStr(OK));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(WEAPON_FULL_LEVEL));
        return;
    }
    
    //联邦币不足
    if(!P_TIP_CTRL->CoinCostTip(m_uCostCoin, this)){
        return;
    }

    const map<UINT64_t, vmsg::CSEquipLvUpConsume>& stMat = P_EQUIP_LIB_CTRL->getObj()->getLvUpMat();
    if(stMat.size() <= 0){
        //COMMON_DIALOG->show(TextStr(WEAPON_CHOOSE_MATERIAL));
        POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(WEAPON_CHOOSE_MATERIAL));
        return;
    }
    //如果消耗材料里包含装备，需要二次确认
    bool bNeedConfirm = false;
    for(auto iter = stMat.begin(); iter != stMat.end(); ++iter) {
        const vmsg::CSEquipLvUpConsume& stOne = iter->second;
        if(vmsg::THING_TYPE_EQUIP == stOne.type() ) {
            bNeedConfirm = true;
            break;
        }
    }
    if(bNeedConfirm) {
        COMMON_DIALOG->show(TextStr(TEXT_EAT_EQUIP_TIP), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(EquipLvUpLayout::onConfiremLvUp) );
    } else {
        onConfiremLvUp();
    }
}

void EquipLvUpLayout::onConfiremLvUp() {

    vector<vmsg::CSEquipLvUpConsume> stMatLst;
    const map<UINT64_t, vmsg::CSEquipLvUpConsume>& stMat = P_EQUIP_LIB_CTRL->getObj()->getLvUpMat();
    for(auto iter = stMat.begin(); iter != stMat.end(); ++iter) {
        const vmsg::CSEquipLvUpConsume& stOne = iter->second;
        stMatLst.push_back(stOne);
    }
    if(stMatLst.size() <= 0) {
        CCLOG("[trace]EquipLvUpLayout::onConfiremLvUp, material num is 0 !!!!");
        return;
    }
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->showLoading();
    P_EQUIP_LIB_CTRL->getProxy()->sendEquipLvUpRqst(m_iEquipBagIdx, m_stEquip.equipguid(), stMatLst);
    CCLOG("[trace]EquipLvUpLayout::onConfiremLvUp,equip bag idx %d, guid %llu, material num %d", m_iEquipBagIdx, m_stEquip.equipguid(), stMatLst.size());
}

void EquipLvUpLayout::updateSelMat() {
    const map<UINT64_t, vmsg::CSEquipLvUpConsume>& stMat = P_EQUIP_LIB_CTRL->getObj()->getLvUpMat();
    int iCellIdx = 0;
    IconCell* pCell;
    UIImageView* pBgImg;
    int iCfgID = 0;
    for(auto iter = stMat.begin(); iter != stMat.end(); ++iter) {
        const vmsg::CSEquipLvUpConsume& stOne = iter->second;
        
        for(int i = 0; i < stOne.cnt(); ++i) {
            if(iCellIdx >= EQUIP_SEL_MAT_MAX_NUM) {
                break;
            }
            pCell = m_pItemCellLst[iCellIdx];
            pBgImg = m_pCellBgLst[iCellIdx];
            iCfgID = P_EQUIP_LIB_CTRL->getObj()->getCfgIDByGuid(stOne.id());
            pCell->setInfo(stOne.type(), 1, iCfgID );
            
            pBgImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);

            iCellIdx++;
        }  

    }
    const int iMatNum = iCellIdx;
    for(; iCellIdx < EQUIP_SEL_MAT_MAX_NUM; ++iCellIdx) {
        pCell = m_pItemCellLst[iCellIdx];
        pBgImg = m_pCellBgLst[iCellIdx];
        pCell->clearCell();
        pBgImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
    }


    m_uCostCoin = iMatNum * GLOBAL_CFG_MASTER->GetCfg().equipexpcoincost();
    m_pCoinTxt->setText(intToString(m_uCostCoin));
    //m_pCoinTxt->setVisible(true);
    const int iTotalAddExp = P_EQUIP_LIB_CTRL->getObj()->getMatTotalExp();
    m_pTotalGetExp->setText(intToString(iTotalAddExp));

    //预览
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->previewLvUp();
}

void EquipLvUpLayout::doUpdateUI(const bool bIsRsp) {
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

    bool bIsLvUp = false;
    if(m_stEquip.equipguid() == stEquip.equipInfo.equipguid() && (stEquip.equipInfo.level() > m_stEquip.level() ) ) {
        bIsLvUp = true;
    }
    const int iDeltaLv = stEquip.equipInfo.level() - m_stEquip.level();
    if(bIsRsp) {
        showLvUpDoneEff(bIsLvUp);
    } else {
        resetMatCell();
    }
    setInfo(stEquip.equipInfo, stEquip.iBagIdx);
	GUIDE_SHOW(GUIDE_UI_WEAPON_INFO);
}

void EquipLvUpLayout::resetMatCell() {
    IconCell* pCell;
    UIImageView* pBgImg;
    for(int iCellIdx = 0; iCellIdx < EQUIP_SEL_MAT_MAX_NUM; ++iCellIdx) {
        pCell = m_pItemCellLst[iCellIdx];
        pBgImg = m_pCellBgLst[iCellIdx];
        pCell->clearCell();
        pCell->getIconImg()->setVisible(true);
        pCell->getIconImg()->loadTexture("other/img_sel_mat_tip.png", UI_TEX_TYPE_PLIST);

        pBgImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
        pBgImg->removeNodeByTag(LV_UP_EFF_NODE_TAG);
    }
    P_EQUIP_LIB_CTRL->getObj()->clearLvUpMat();

    m_pTotalGetExp->setText("0");
    m_pCoinTxt->setText("0");
}




void EquipLvUpLayout::showLvUpDoneEff(const bool bIsLvUp) {
    UIImageView* pBgImg;
    IconCell* pCell;
    m_iEffCellNum = 0;
    for(int iCellIdx = 0; iCellIdx < EQUIP_SEL_MAT_MAX_NUM; ++iCellIdx) {
        pCell = m_pItemCellLst[iCellIdx];
        pBgImg = m_pCellBgLst[iCellIdx];
        if( ! (pCell->getIconImg()->isVisible() ) ) {
            break;
        }

        CCArmature* pEff = CCArmature::create("ut00039");
        pEff->setAnchorPoint(ccp(0.5f, 0.5f));
        pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(EquipLvUpLayout::onEffDone));
        pBgImg->removeNodeByTag(LV_UP_EFF_NODE_TAG);
        pBgImg->addNode(pEff, 11, LV_UP_EFF_NODE_TAG);
        m_iEffCellNum++;
        pEff->getAnimation()->play("fly", -1, -1, 0); //not loop
    }
    if(m_iEffCellNum > 0) {
        m_pAutoSelBtn->setTouchEnabled(false);
        m_pDoLvUpBtn->setTouchEnabled(false);
    }
    if(!bIsLvUp) {
        return;
    }

}

void EquipLvUpLayout::playLvUpEffCallback(CCArmature* armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        armature->removeFromParentAndCleanup(true);
    }
}


void EquipLvUpLayout::onEffDone(CCArmature * armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        
        UIImageView* pBgImg = dynamic_cast<UIImageView*> (armature->getParent());
        if(pBgImg) {
            int iTagIdx = pBgImg->getTag();
            if(iTagIdx >= 0 && iTagIdx < EQUIP_SEL_MAT_MAX_NUM) {
                IconCell* pCell = m_pItemCellLst[iTagIdx];
                pCell->clearCell();
            }
        }
        armature->removeFromParentAndCleanup(true);

        m_iEffCellNum--;
        if(m_iEffCellNum <= 0) {
            resetMatCell();
            m_iEffCellNum = 0;
            m_pAutoSelBtn->setTouchEnabled(true);
            m_pDoLvUpBtn->setTouchEnabled(true);
        }
    } 
}
