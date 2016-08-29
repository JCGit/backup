#include "EquipInfoLayout.h"
#include "IconCell.h"
#include "EquipLibControl.h"
#include "defines.h"
#include "EquipLevelCfgMaster.h"
#include "EquipmentCfgMaster.h"
#include "ItemUtils.h"
#include "firearms/StarProgressBar.h"
#include "NounHelper.h"
#include "ToolUtils.h"
#include "control/AudioManager.h"


using namespace std;

void EquipInfoLayout::initWithLayout(UILayout *pRefLayout) {
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    initUI();
}


void EquipInfoLayout::initUI() {
    if(NULL == m_pRefLayout) {
        return;
    }
    
    m_pNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("name_txt"));
    m_pImgInUse = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("img_in_use"));

    m_pCellBg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("cell_bg"));;
    m_pEquipImg = UIImageView::create();
    m_pEquipImg->setPosition(m_pCellBg->getPosition());
    m_pRefLayout->addChild(m_pEquipImg);
    m_pCellBg->setZOrder(1);

    m_pDescTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("desc_txt"));
    m_pMaxLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("max_lv_txt"));
    m_pLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("lv_txt"));
    m_pPosTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("pos_txt"));

    m_pAttrValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("attr_val_txt"));
    m_pAttrNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("attr_name_txt"));

    m_pExpBar = dynamic_cast<UILoadingBar*>(m_pRefLayout->getChildByName("exp_bar"));
    m_pExpBarValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("exp_bar_val_txt"));


    m_pEquipStar = CStarProgressBar::create("share/little_star_dark.png", 
        "share/little_star_light.png", "ui_v2/share.png", MAX_EQUIP_STAR);
    m_pEquipStar->setAnchorPoint(ccp(0.5f, 0.0f));
    CCPoint pt = m_pCellBg->getPosition();
    pt.y -= m_pCellBg->getSize().height - 10;
    m_pEquipStar->setPosition(pt);
    m_pEquipStar->setStarMargin(1);
    m_pRefLayout->addChild(m_pEquipStar);
}


void EquipInfoLayout::doUpdateUI(const bool bIsRsp) {
    if(NULL == m_pRefLayout) {
        CCLOG("[warn]EquipInfoLayout::doUpdateUI, layout is null.");
        return;
    }
    const UINT64_t& uEquipGuid = P_EQUIP_LIB_CTRL->getObj()->getSelEquipGuid();
    if(uEquipGuid <= 0) {
        //暂时没有选择装备,不提示
        CCLOG("[warn]EquipInfoLayout::doUpdateUI, equip guid is 0.");
        return;
    }
    const EquipLibOneInfo stEquip = P_EQUIP_LIB_CTRL->getObj()->getEquipInfo(uEquipGuid);
    if(stEquip.equipInfo.equipguid() <= 0) {
        CCLOG("[warn]EquipInfoLayout::doUpdateUI, fail to find equip guid %llu", uEquipGuid);
        return;
    }
    resetUI(); //reset

    //check is lv up
    int iDeltaLv = 0;
    if(bIsRsp && (stEquip.equipInfo.equipguid() == m_stEquipLibInfo.equipInfo.equipguid()) ) {
        iDeltaLv = (stEquip.equipInfo.level() - m_stEquipLibInfo.equipInfo.level());
    }

    m_stEquipLibInfo = stEquip; //set

    m_pImgInUse->setVisible(stEquip.bIsWear);

    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquip.equipInfo.equipid() );
    if(NULL == pstCfg) {
        return;
    }
    m_pCellBg->loadTexture(ItemUtils::getColorBgImg(pstCfg->color()).c_str(),  UI_TEX_TYPE_PLIST );

    m_pEquipImg->loadTexture(ItemUtils::getItemIcon(vmsg::THING_TYPE_EQUIP, stEquip.equipInfo.equipid() ).c_str() );
    m_pNameTxt->setText(pstCfg->equipname());
    m_pDescTxt->setText(pstCfg->equipdesc());
    m_pLvTxt->setText(intToString(stEquip.equipInfo.level()));

    const int iMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(stEquip.equipInfo.equipid());
    m_pMaxLvTxt->setText( intToString(stEquip.equipInfo.level()) + "/" + intToString(iMaxLv) );
    m_pEquipStar->setStarNum(pstCfg->initstar(), MAX_EQUIP_STAR);

    int iAttrVal = ItemUtils::getEquipAttrVal(stEquip.equipInfo.equipid(), stEquip.equipInfo.level(), stEquip.equipInfo.quality());
    m_pAttrNameTxt->setText(AttrTypeStr(pstCfg->attr().attrid() ) );
    m_pAttrValTxt->setText( "+" + intToString( iAttrVal ));

    m_pPosTxt->setText(getEquipPosStr(pstCfg->part()));

    //升级还需经验
    unsigned int upExp = 0;
    const EquipLevelCfg* pLvCfg = EQUIPLEVEL_CFG_MASTER->GetCfg(stEquip.equipInfo.level());
    if(pLvCfg) {
        int iPer = stEquip.equipInfo.exp() * 100 / (pLvCfg->exp() * 1.0f * pstCfg->expmulti()/ 10000 );
        iPer = iPer > 100 ? 100 : iPer;
        m_pExpBarValTxt->setText(intToString(iPer) + "%");
        m_pExpBar->setPercent(iPer);
    } else {
        CCLOG("[warn]EquipInfoLayout::setInfo, fail to get EquipLevelCfg,lv=%d", stEquip.equipInfo.level());
        m_pExpBarValTxt->setText("");
        m_pExpBar->setPercent(0);
    }

    m_iRealExpPercent = m_pExpBar->getPercent();
    m_strRealMaxLvTxt = m_pLvTxt->getStringValue(); //记下升级前的值
    m_strRealAttrTxt = m_pAttrValTxt->getStringValue(); //记下升级前的值

    if(bIsRsp && iDeltaLv > 0) {
        playLvUpEff(iDeltaLv);
    }

}


std::string EquipInfoLayout::getEquipPosStr(const int iEquipPos)
{
    std::string typeName;
    if(iEquipPos == vmsg::EQUIP_POS_ARMOR) {
        typeName = NounStr(ARMOR);
    } else if(iEquipPos == vmsg::EQUIP_POS_RING) {
        typeName = NounStr(RING);
    } else if(iEquipPos == vmsg::EQUIP_POS_SHOE) {
        typeName = NounStr(SHOE);
    } else if(iEquipPos == vmsg::EQUIP_POS_CLOTH) {
        typeName = NounStr(CLOTH);
    }
    return typeName;
}


void EquipInfoLayout::previewLvUpExp() {
    const int iTotalAddExp = P_EQUIP_LIB_CTRL->getObj()->getMatTotalExp();
    if(iTotalAddExp <= 0) {
        return;
    }
    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(m_stEquipLibInfo.equipInfo.equipid() );
    if(NULL == pstCfg) {
        return;
    }
    bool bIsShowTxtEff = true;
    const int iDeltaLv = P_EQUIP_LIB_CTRL->getObj()->getLevelsByExp(m_stEquipLibInfo.equipInfo.equipid(), 
        m_stEquipLibInfo.equipInfo.exp(), m_stEquipLibInfo.equipInfo.level(), iTotalAddExp);
    if(iDeltaLv < 0) {
        return;
    } else if(iDeltaLv == 0) {
        //当前没升满，只显示经验条变化吧
        bIsShowTxtEff = false;
    }

    const unsigned int iMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(m_stEquipLibInfo.equipInfo.equipid());
    unsigned int iToLv = m_stEquipLibInfo.equipInfo.level() + iDeltaLv;
    iToLv = iToLv > iMaxLv ? iMaxLv : iToLv;

    m_strPreviewMaxLvTxt = intToString( iToLv );  //升级后的值
    m_pLvTxt->setText( m_strPreviewMaxLvTxt );

    int iAttrVal = ItemUtils::getEquipAttrVal(m_stEquipLibInfo.equipInfo.equipid(), iToLv, m_stEquipLibInfo.equipInfo.quality());
    m_strPreviewAttrTxt = "+" + intToString( iAttrVal ); //升级后的值
    m_pAttrValTxt->setText( m_strPreviewAttrTxt);

    
    //exp bar
    int iNowPercent = m_iRealExpPercent;
    iNowPercent = iNowPercent > 100 ? 100 : iNowPercent;

    int iToPercent = 100;
    if(m_stEquipLibInfo.equipInfo.level() == iToLv) {
        int iToExp = m_stEquipLibInfo.equipInfo.exp() + iTotalAddExp;
        const EquipLevelCfg* pLvCfg = EQUIPLEVEL_CFG_MASTER->GetCfg(iToLv); //当前级满时所需要的经验
        if(pLvCfg) {
            iToPercent = iToExp * 100 / (pLvCfg->exp() * 1.0f * pstCfg->expmulti()/ 10000 );
            iToPercent = iToPercent > 100 ? 100 : iToPercent;
        } 

    }
    
    //呼吸效果
    m_pLvTxt->stopAllActions();
    m_pAttrValTxt->stopAllActions();
    m_pExpBar->stopAllActions();

    m_pLvTxt->setOpacity(255);
    m_pAttrValTxt->setOpacity(255);
    m_pExpBar->setOpacity(255);

    CCCallFuncND *fun1 = CCCallFuncND::create(this, callfuncND_selector(EquipInfoLayout::txtPreviewCallback), (void *)iToPercent );
    CCCallFuncND *fun2 = CCCallFuncND::create(this, callfuncND_selector(EquipInfoLayout::txtRealCallback),  (void *)iNowPercent );
    CCSequence* seq = NULL;
    if(bIsShowTxtEff) {
        seq  = CCSequence::create( CCFadeTo::create(1.0f, 30), fun1, CCFadeTo::create(1.0f, 255),  
            CCFadeTo::create(1.0f, 30), fun2, CCFadeTo::create(1.0f, 255),  NULL );
        m_pLvTxt->runAction( CCRepeatForever::create(seq) );

        seq  = CCSequence::create( CCFadeTo::create(1.0f, 30), fun1, CCFadeTo::create(1.0f, 255),  
            CCFadeTo::create(1.0f, 30), fun2, CCFadeTo::create(1.0f, 255),  NULL ); //不能共用
        m_pAttrValTxt->runAction( CCRepeatForever::create(seq) );
    }
    seq  = CCSequence::create( CCFadeTo::create(1.0f, 30), fun1, CCFadeTo::create(1.0f, 255),  
        CCFadeTo::create(1.0f, 30), fun2, CCFadeTo::create(1.0f, 255),  NULL ); //不能共用
    m_pExpBar->runAction( CCRepeatForever::create(seq) );
}

void EquipInfoLayout::resetUI() {
    m_pLvTxt->stopAllActions();
    m_pAttrValTxt->stopAllActions();
    m_pExpBar->stopAllActions();

    m_pLvTxt->setOpacity(255);
    m_pAttrValTxt->setOpacity(255);
    m_pExpBar->setOpacity(255);

    m_pLvTxt->setText(m_strRealMaxLvTxt);
    m_pAttrValTxt->setText(m_strRealAttrTxt);
    m_pExpBar->setPercent(m_iRealExpPercent);
    m_pExpBarValTxt->setText(intToString(m_iRealExpPercent) + "%");

    m_pRefLayout->removeNodeByTag(LV_UP_EFF_TAG);
    m_pRefLayout->removeNodeByTag(ADV_EFF_TAG);

    m_pEquipImg->setVisible(true); //visible
    m_pCellBg->setVisible(true);
}

void EquipInfoLayout::txtPreviewCallback(CCNode* pSender, void* pData) {
    UILoadingBar* pbar = dynamic_cast<UILoadingBar*>(pSender);
    if(pbar) {
        int val = (long)pData;
        m_pExpBar->setPercent(val);
        m_pExpBarValTxt->setText( intToString(val) + "%" );
        return;
    }

    UILabel* ptxt = dynamic_cast<UILabel*>(pSender);
    if(NULL == ptxt){
        return;
    }
    if(m_pLvTxt == ptxt) {
        m_pLvTxt->setText(m_strPreviewMaxLvTxt);
    } else if(m_pAttrValTxt == ptxt) {
        m_pAttrValTxt->setText(m_strPreviewAttrTxt);
    } 
}

void EquipInfoLayout::txtRealCallback(CCNode* pSender, void* pData) {
    UILoadingBar* pbar = dynamic_cast<UILoadingBar*>(pSender);
    if(pbar) {
        int val = ((long)pData);
        m_pExpBar->setPercent(val);
        m_pExpBarValTxt->setText( intToString(val) + "%" );
        return;
    }

    UILabel* ptxt = dynamic_cast<UILabel*>(pSender);
    if(NULL == ptxt){
        return;
    }
    if(m_pLvTxt == ptxt) {
        m_pLvTxt->setText(m_strRealMaxLvTxt);
    } else if(m_pAttrValTxt == ptxt) {
        m_pAttrValTxt->setText(m_strRealAttrTxt);
    } 
} 


void EquipInfoLayout::playLvUpEff(const int iDeltaLv) {
    int iPlayNum = iDeltaLv;
    if(iDeltaLv > 20) {
        iPlayNum = 20;
    }
    m_iLvUpEffPlayNum = iPlayNum;
    m_pExpBar->showRunEff(iPlayNum, 5, m_pExpBar->getPercent() );

    CToolUtils::loadArmatureRGBA4444("effect/ut00070/ut00070.ExportJson");
    m_pRefLayout->removeNodeByTag(LV_UP_EFF_TAG);

    AudioManager::playEffect( "ut00066.ogg" );

    CCArmature* pEff = CCArmature::create("ut00070");
    pEff->setAnchorPoint(ccp(0.5f, 0.5f));

    int xPos = m_pCellBg->getPositionX() - 2;
    int yPos = m_pCellBg->getPositionY() - 12;
    pEff->setPosition(xPos, yPos);
    pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(EquipInfoLayout::playLvUpEffCallback));
    m_pRefLayout->addNode(pEff, 8, LV_UP_EFF_TAG);
    pEff->getAnimation()->play("fly", -1, -1, 1); //not loop
}


void EquipInfoLayout::playLvUpEffCallback(CCArmature* armature, MovementEventType type, const char * name) {
    if (LOOP_COMPLETE == type){
        m_iLvUpEffPlayNum--;
        if(m_iLvUpEffPlayNum <= 0) {
            armature->getAnimation()->stop();
            armature->removeFromParentAndCleanup(true);
        }
    }
}



void EquipInfoLayout::playAdvEff() {

    CToolUtils::loadArmatureRGBA4444("effect/ut00069/ut00069.ExportJson");

    m_pRefLayout->removeNodeByTag(ADV_EFF_TAG);

    m_pEquipImg->setVisible(false); //invisible
    m_pCellBg->setVisible(false);

    AudioManager::playEffect( "ut00066.ogg" );

    CCArmature* pEff = CCArmature::create("ut00069");
    pEff->setAnchorPoint(ccp(0.5f, 0.5f));

    int xPos = m_pCellBg->getPositionX() - 5;
    int yPos = m_pCellBg->getPositionY() - 10;
    pEff->setPosition(xPos, yPos);

    pEff->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(EquipInfoLayout::onPlayAdvEffFrameEvt));
    pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(EquipInfoLayout::playAdvEffCallback));

    m_pRefLayout->addNode(pEff, 8, ADV_EFF_TAG);
    pEff->getAnimation()->play("fly", -1, -1, 0); //not loop
}


void EquipInfoLayout::onPlayAdvEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx) {
    string evtName(stEvent);
    if(evtName == "chuxian") {
        m_pEquipImg->setVisible(true); //visible
        m_pCellBg->setVisible(true);
    }
}

void EquipInfoLayout::playAdvEffCallback(CCArmature* armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        armature->removeFromParentAndCleanup(true);
        CToolUtils::removeArmatureFile("effect/ut00069/ut00069.ExportJson");
    }
}

