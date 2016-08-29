#include "WeaponAdvanceLayout.h"
#include "WeaponCfgMaster.h"
#include "WeaponTypeCfgMaster.h"
#include "defines.h"
#include "WeaponUtils.h"
#include "firearms/FireArmsObj.h"
#include "CSEquipItemMsg.pb.h"
#include "WeaponStarStepCfgMaster.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "defines.h"
#include "IconCell.h"
#include "UserObj.h"
#include "bag/BagControl.h"
#include "TextHelper.h"
#include "WeaponLibControl.h"
#include "ToolUtils.h"
#include "tip/TipControl.h"

#include "game/guide/GuideControl.h"

using namespace std;

bool compareWeaponLv(const vmsg::CSWeapon& stInfoA, const vmsg::CSWeapon& stInfoB) {
    return (stInfoA.level() < stInfoB.level() );
}

void WeaponAdvanceLayout::initWithLayout(UILayout *pRefLayout) {
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    m_pRefLayout->retain();
    m_pRefLayout->removeFromParent();

    //from equip info
    m_pFromAtkValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_val_0"));
    m_pFromAtkAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_attr_val_0"));
    m_pFromAtkGrowValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_growup_val_0"));
    m_pFromAtkAttrGrowValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("attr_growup_val_0"));
    m_pFromMaxLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("max_lv_val_0"));

    //to equip info
    m_pToAtkValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_val_1"));
    m_pToAtkAttrTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_attr_val_1"));
    m_pToAtkGrowValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_growup_val_1"));
    m_pToAtkAttrGrowValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("attr_growup_val_1"));
    m_pToMaxLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("max_lv_val_1"));

    m_pAdvTipTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("adv_tip_txt"));

    m_pDoAdvBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("do_adv_btn"));

    m_pDoAdvBtn->addTouchEventListener(this, toucheventselector(WeaponAdvanceLayout::touchEventHandler));

    m_pCoinTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("coin_txt"));

    UIImageView* pImg;
    IconCell* pCell;
    for(int i = 0; i < MAX_ITEM_NUM; ++i) {
        pImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName(string("cell_" + intToString(i)).c_str() ));

        m_pCellBgLst[i] = (pImg);

        pCell = IconCell::create();
        pCell->showNumTxt();
        pImg->setTag(i);
        pImg->addChild(pCell);
        m_pItemCellLst[i] = pCell;

        pImg->addTouchEventListener(this, toucheventselector(WeaponAdvanceLayout::touchEventHandler));
        pImg->setTouchEnabled(true);
    }
}


void WeaponAdvanceLayout::setInfo(const UINT64_t weaponGUID) {
    
    const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);
    m_iNowWeaponCfgID = stWeaponInfo.cfgid();

    const WeaponCfg* pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeaponInfo.cfgid());
    const WeaponStarStepCfg* pStarCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pWeaponCfg->initstar(), pWeaponCfg->initstep());


	unsigned int uFromWeaponAddCoe = WeaponUtils::getWeaponAddCoe(stWeaponInfo.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality());
	m_pFromAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(stWeaponInfo.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality()) + uFromWeaponAddCoe));
	m_pFromAtkAttrTxt->setText(intToString(WeaponUtils::getWeaponAtkAttr(stWeaponInfo.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality())));
	m_pFromAtkGrowValTxt->setText(intToString(  WeaponUtils::getWeaponAtkAddVal(stWeaponInfo.cfgid())  ));
	m_pFromAtkAttrGrowValTxt->setText(intToString(pWeaponCfg->atkattrvaladd()));
	const unsigned int uFromEquipMaxLv = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pWeaponCfg->initstar(), 
		pWeaponCfg->initstep())->needlevel();
	m_pFromMaxLvTxt->setText(intToString(uFromEquipMaxLv));

    if(pStarCfg->star() == pStarCfg->nextstar() && pStarCfg->step() == pStarCfg->nextstep()){  //星级、阶段不再升级
        COMMON_DIALOG->show(TextStr(WEAPON_STAR_UP_ENOUGH), NULL, NounStr(OK));
        //reset
        for(int i = 0; i < MAX_ITEM_NUM; ++i) {
            m_pCellBgLst[i]->setVisible(false);
        }

        m_pToAtkValTxt->setVisible(false);
        m_pToAtkAttrTxt->setVisible(false);
        m_pToAtkGrowValTxt->setVisible(false);
        m_pToAtkAttrGrowValTxt->setVisible(false);
        m_pToMaxLvTxt->setVisible(false);
        m_pCoinTxt->setVisible(false);

		m_pAdvTipTxt->setVisible(false);
        return;
    }
    char tmpStr[50] = {0};
    snprintf(tmpStr, 50, TextStr(TEXT_WEAPON_ADV_LV_TIP), pStarCfg->needlevel());
    m_pAdvTipTxt->setText(tmpStr);

    WeaponCfg pUpCfg;
    int iRet = WEAPON_CFG_MASTER->GetWeaponCfgByStarAndStep(pWeaponCfg->weaponid(), pStarCfg->nextstar(), pStarCfg->nextstep(), pUpCfg);
    if(iRet != 0){
        CCLOG("WeaponAdvanceLayout::updateWeaponInfo, can not find weapon cfg,ret %d", iRet);
        return;
    }
    m_iToWeaponCfgID = pUpCfg.cfgid();

    //升星后
	unsigned int uToWeaponAddCoe = WeaponUtils::getWeaponAddCoe(pUpCfg.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality());
    m_pToAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(pUpCfg.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality()) + uToWeaponAddCoe));
    m_pToAtkAttrTxt->setText(intToString(WeaponUtils::getWeaponAtkAttr(pUpCfg.cfgid(), stWeaponInfo.level(), stWeaponInfo.quality())));
    m_pToAtkGrowValTxt->setText(intToString( WeaponUtils::getWeaponAtkAddVal(pUpCfg.cfgid())));
    m_pToAtkAttrGrowValTxt->setText(intToString(pUpCfg.atkattrvaladd()));
    const unsigned int uToEquipMaxLv = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pUpCfg.initstar(), 
        pUpCfg.initstep())->needlevel();
    m_pToMaxLvTxt->setText(intToString(uToEquipMaxLv));

    //显示材料
    
    //3. set need mat
    int iNowCell = 0;
    UIImageView* pImg;
    IconCell* pCell;
    m_bIsMatEnough = true; //默认为true
    if(pStarCfg->needweaponcnt() > 0) {
        pImg = m_pCellBgLst[iNowCell];
        pImg->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
        pImg->setSize(CCSizeMake(188, 100));
        pImg->setVisible(true);
        pCell = m_pItemCellLst[iNowCell];
        pCell->setInfo(vmsg::THING_TYPE_WEAPON, pStarCfg->needweaponcnt(), stWeaponInfo.cfgid() );
        setMaterialNumTxt(vmsg::THING_TYPE_WEAPON, pStarCfg->needweaponcnt(), stWeaponInfo.cfgid(), pCell->getNumTxt());
        iNowCell++;
    }
    for(int i = 0; i < pStarCfg->needs_size(); ++i) {
        if(iNowCell >= MAX_ITEM_NUM) {
            break;
        }
        pImg = m_pCellBgLst[iNowCell];
        pImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
        pImg->setSize(CCSizeMake(100, 100));
        pImg->setVisible(true);
        pCell = m_pItemCellLst[iNowCell];
        pCell->setInfo(pStarCfg->needs(i).type(), pStarCfg->needs(i).cfgcnt(), pStarCfg->needs(i).cfgid() );
        setMaterialNumTxt(pStarCfg->needs(i).type(), pStarCfg->needs(i).cfgcnt(), 
            pStarCfg->needs(i).cfgid(), pCell->getNumTxt());

        iNowCell++;
    }
    for(; iNowCell < MAX_ITEM_NUM; ++iNowCell) {
        pImg = m_pCellBgLst[iNowCell]; //其他的不显示
        pImg->setVisible(false);
    }

    //set coin 
    char str[50];
    snprintf(str, 50, "%d", pStarCfg->costcoin());
    m_pCoinTxt->setText( str );

    m_pDoAdvBtn->setTouchEnabled(true);
}

void WeaponAdvanceLayout::updateWeaponInfo(const UINT64_t weaponGUID, const bool bIsPlayEff){
    if(weaponGUID <= 0) {
        return;
    }
    m_uWeaponGuid = weaponGUID;
    
    if(bIsPlayEff) {
        showEff();
    } else {
        setInfo(weaponGUID);
    }
}

void WeaponAdvanceLayout::setMaterialNumTxt(const int iType, const int iCnt, const int iCfgID, UILabel* pTxt) {
    if(NULL == pTxt) {
        m_bIsMatEnough = false;
        return;
    }
    int iBagItemNum = 0;
    if(vmsg::THING_TYPE_WEAPON == iType) {
        iBagItemNum = P_ARM_OBJ->getWeaponIdleNumByCfgID(iCfgID, m_uWeaponGuid);
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


void WeaponAdvanceLayout::touchEventHandler(CCObject* pSender, TouchEventType type){
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    const string str(pszWidgetName);
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "do_adv_btn") == 0) {
			GUIDE_TOUCH(pWidget);
            doAdvance();	
        } else if(str.find_first_of("cell_") == 0) {
			GUIDE_TOUCH(pWidget);
            string sub = str.substr(str.find_first_not_of("cell_"));
            int iIdx = atoi(sub.c_str());
            if(iIdx >= 0 && iIdx < MAX_ITEM_NUM) {
                IconCell* pCell = m_pItemCellLst[iIdx];
                //显示材料掉落提示面板
                if(!(pCell->getItemCfgid() == 0 && pCell->getItemType() == 0 && pCell->getItemNum() == 0)){
                    P_TIP_CTRL->showItemTipPanel(pCell->getItemType(), pCell->getItemCfgid(), pCell->getItemNum());
                }
            }
        }
    }
}

void WeaponAdvanceLayout::doAdvance() {
    const vmsg::CSWeapon* pstWeapon = P_ARM_OBJ->getWeaponByGUID(m_uWeaponGuid);
    if(NULL == pstWeapon) {
        return;
    }
    const WeaponCfg* pCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_iNowWeaponCfgID);
    const WeaponStarStepCfg* pStarCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pCfg->initstar(), pCfg->initstep());
    if(pStarCfg->star() == pStarCfg->nextstar() && pStarCfg->step() == pStarCfg->nextstep()){  
        //星级、阶段不再升级
        COMMON_DIALOG->show(TextStr(WEAPON_STAR_UP_ENOUGH), NULL, NounStr(OK));
        return;
    }

    if(pstWeapon->level() < pStarCfg->needlevel()){
        char tmpStr[50] = {0};
        snprintf(tmpStr, 50, TextStr(TEXT_WEAPON_ADV_LV_TIP), pStarCfg->needlevel());
        COMMON_DIALOG->show(tmpStr, NULL, NounStr(OK));
        return;
    }

    if(pStarCfg->star() == pStarCfg->nextstar() && pStarCfg->step() == pStarCfg->nextstep()){  
        //星级、阶段不再升级
        COMMON_DIALOG->show(TextStr(WEAPON_STAR_UP_ENOUGH));
        return;
    }

    if(!m_bIsMatEnough) {
        COMMON_DIALOG->show(TextStr(WEAPON_NO_ENOUGH), NULL, NounStr(OK));
        return;
    }
    
    if(!P_TIP_CTRL->CoinCostTip(pStarCfg->costcoin(), this)){
        return;
    }

    //如果消耗的有装备，优先使用背包中CfgID相同的低等级装备
    vector<vmsg::CSWeapon> stNeedWeaponInfoList;

    if(pStarCfg->needweaponcnt() > 0) {
        const vmsg::CSWeaponBag& stBag = P_ARM_OBJ->getWeaponBag();
        for (int i = 0; i < stBag.weapons_size(); ++i ) {
            const vmsg::CSWeapon& oneInfo = stBag.weapons(i);
            if(m_iNowWeaponCfgID == oneInfo.cfgid() && m_uWeaponGuid != oneInfo.weaponguid() && oneInfo.state() == vmsg::WEAPON_IDLE) {
                stNeedWeaponInfoList.push_back(oneInfo);
            }
        }

        if(stNeedWeaponInfoList.size() < pStarCfg->needweaponcnt()) {
            COMMON_DIALOG->show(TextStr(WEAPON_NO_ENOUGH));
            return;
        }

        //sort
        sort(stNeedWeaponInfoList.begin(), stNeedWeaponInfoList.end(), compareWeaponLv);
    }



    vector<UINT64_t> stGuidLst;
    unsigned int i = 0;
    for(; i < stNeedWeaponInfoList.size(); ++i) {
        stGuidLst.push_back(stNeedWeaponInfoList[i].weaponguid());
        if(stGuidLst.size() >= pStarCfg->needweaponcnt()) {
            break;
        }
    }

    //发协议前提前记录一下结果吧
    P_WEAPON_CTRL->getWeaponAdvDonePanel()->setOneWeaponInfo(true, m_iNowWeaponCfgID, pstWeapon->level(), pstWeapon->quality());
    P_WEAPON_CTRL->getWeaponAdvDonePanel()->setOneWeaponInfo(false, m_iToWeaponCfgID, pstWeapon->level(), pstWeapon->quality());


    const int bagIdx = P_ARM_OBJ->getWeaponIdx(m_uWeaponGuid);

    //全部完成后统一处理
    m_pDoAdvBtn->setTouchEnabled(false);
    P_WEAPON_CTRL->getWeaponInfoPanel()->showLoading();
    P_WEAPON_CTRL->sendWeaponStarStepRqst(bagIdx, m_uWeaponGuid, stGuidLst);
}



UIButton* WeaponAdvanceLayout::getAdvanceBtn() {
	if(m_pDoAdvBtn != NULL && m_pDoAdvBtn->isVisible() && m_pDoAdvBtn->isTouchEnabled()) {
		return m_pDoAdvBtn;
	}

	return NULL;
}

void WeaponAdvanceLayout::movementCallbackToRelease(CCArmature * armature, MovementEventType type, const char * name) {
    if(type == COMPLETE){
        m_pAdvanceArmature->getAnimation()->play("fly_xh", -1, -1, 1);
        m_pAdvanceArmature->getAnimation()->setMovementEventCallFunc(NULL, NULL);
        CCFiniteTimeAction* removeFun = CCCallFunc::create(this, callfunc_selector(WeaponAdvanceLayout::removeAdvanceArmature));
        m_pRefLayout->runAction(CCSequence::create(CCDelayTime::create(0.5f), removeFun, NULL));
    }
}

void WeaponAdvanceLayout::removeAdvanceArmature(){
    if(m_pAdvanceArmature){
        m_pAdvanceArmature->removeFromParentAndCleanup(true);
        m_pAdvanceArmature = NULL;
    }

    CToolUtils::removeArmatureFile("effect/ut00014/ut00014.ExportJson");
}

void WeaponAdvanceLayout::resetUI(){

    m_pDoAdvBtn->setTouchEnabled(true);

    UIImageView* pImg;
    IconCell* pCell;
    for(int i = 0; i < MAX_ITEM_NUM; ++i) {
        pCell = m_pItemCellLst[i];
        pCell->clearCell();

        pImg = m_pCellBgLst[i]; //其他的不显示
        pImg->removeNodeByTag(LV_UP_EFF_NODE_TAG);
        pImg->setVisible(true);
    }
}


void WeaponAdvanceLayout::showEff() {
    UIImageView* pBgImg;
    IconCell* pCell;
    m_iEffCellNum = 0;
    CToolUtils::loadArmatureRGBA4444("effect/ut00039/ut00039.ExportJson");
    for(int iCellIdx = 0; iCellIdx < MAX_ITEM_NUM; ++iCellIdx) {
        pCell = m_pItemCellLst[iCellIdx];
        pBgImg = m_pCellBgLst[iCellIdx];
        if( ! (pCell->getIconImg()->isVisible() ) ) {
            break;
        }

        CCArmature* pEff = CCArmature::create("ut00039");
        pEff->setAnchorPoint(ccp(0.5f, 0.5f));
        pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(WeaponAdvanceLayout::onEffDone));
        pBgImg->removeNodeByTag(LV_UP_EFF_NODE_TAG);
        pBgImg->addNode(pEff, 11, LV_UP_EFF_NODE_TAG);
        m_iEffCellNum++;
        pEff->getAnimation()->play("fly", -1, -1, 0); //not loop
    }
    if(m_iEffCellNum > 0) {
        m_pDoAdvBtn->setTouchEnabled(false);
    }
}


void WeaponAdvanceLayout::onEffDone(CCArmature * armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){

        UIImageView* pBgImg = dynamic_cast<UIImageView*> (armature->getParent());
        if(pBgImg) {
            int iTagIdx = pBgImg->getTag();
            if(iTagIdx >= 0 && iTagIdx < MAX_ITEM_NUM) {
                IconCell* pCell = m_pItemCellLst[iTagIdx];
                pCell->clearCell();
            }
        }
        armature->removeFromParentAndCleanup(true);

        m_iEffCellNum--;
        if(m_iEffCellNum <= 0) {
            setInfo(m_uWeaponGuid);
            m_iEffCellNum = 0;
            m_pDoAdvBtn->setTouchEnabled(true);
            CToolUtils::removeArmatureFile("effect/ut00039/ut00039.ExportJson");
        }
    } 
}

