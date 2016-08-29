#include "WeaponLvUpLayout.h"
#include "WeaponCfgMaster.h"
#include "defines.h"
#include "IconCell.h"
#include "firearms/FireArmsObj.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "WeaponUtils.h"
#include "WeaponStarStepCfgMaster.h"
#include "WeaponLevelCfgMaster.h"
#include "bag/BagControl.h"
#include "GlobalCfgMaster.h"
#include "vlib/CVSizeString.h"
#include "WeaponLibControl.h"
#include "UserObj.h"
#include "ToolUtils.h"
#include "WeaponExpItemCfgMaster.h"
#include "tip/TipControl.h"
#include "PopTipLayer.h"
#include "../city/CityControl.h"
#include "../city/EntryControl.h"
#include "../../ProtoDescCpp/ModuleID.pb.h"

using namespace std;

void WeaponLvUpLayout::initWithLayout(UILayout *pRefLayout) {
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    m_pRefLayout->retain();
    m_pRefLayout->removeFromParent();
    
    m_pExpTipTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("exp_tip_txt"));
    m_pCoinTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("coin_txt"));

    m_pTotalItemExpTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("total_item_exp_tip_txt"));

    m_pAutoSelBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("auto_sel_btn"));
    m_pAutoSelBtn->addTouchEventListener(this, toucheventselector(WeaponLvUpLayout::touchEventHandler));

    m_pDoLvUpBtn = dynamic_cast<UIButton*>(m_pRefLayout->getChildByName("do_lv_up_btn"));
    m_pDoLvUpBtn->addTouchEventListener(this, toucheventselector(WeaponLvUpLayout::touchEventHandler));
    
    UIImageView* pImg;
    IconCell* pCell;
    for(int i = 0; i < MAX_ITEM_NUM; ++i) {
        pImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName(string("cell_" + intToString(i)).c_str() ));
        pImg->addTouchEventListener(this, toucheventselector(WeaponLvUpLayout::touchCellEventHandler));
        pImg->setTouchEnabled(true);

        m_pCellBgLst[i] = (pImg);
        pImg->setTag(i);
        pCell = IconCell::create();
        pCell->showNumTxt();
        pImg->addChild(pCell);
        m_pItemCellLst[i] = pCell;
    }
}

void WeaponLvUpLayout::touchEventHandler(CCObject* pSender, TouchEventType type) {
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "do_lv_up_btn") == 0) {
			GUIDE_TOUCH(pWidget);

			if(m_bHasWeaponMaterial) {
				COMMON_DIALOG->show(TextStr(WEAPON_LEVEL_UP_SELECTED_WEAPON), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(WeaponLvUpLayout::doLvUp));
			} else {
				doLvUp();
			}
        } else if(strcmp(pszWidgetName, "auto_sel_btn") == 0) {
			GUIDE_TOUCH(pWidget);
            doAutoSel();
			GUIDE_SHOW(GUIDE_UI_WEAPON_INFO);
        }
    }
}

void WeaponLvUpLayout::touchCellEventHandler(CCObject* pSender, TouchEventType type) {
    //点击了材料图标
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    string strWidgetName(pWidget->getName());
    switch (type) {
    case TOUCH_EVENT_ENDED:
		GUIDE_TOUCH(pWidget);
        if(isLvFull()) {
            return;
        }

        string strIdx = strWidgetName.substr(5, 1);
        int iRet = 0;
        const int iIdx = CVLib::CVStringUtils::StrToInt<int>(strIdx.c_str(), strIdx.size(), &iRet);
        if(0 == iRet && iIdx >= 0 && iIdx < MAX_ITEM_NUM) {
            UIImageView* pImg = m_pCellBgLst[iIdx];
            if(!pImg->isVisible() || !m_pRefLayout->isVisible()) {
                return;
            }
            P_WEAPON_CTRL->getSelMatPanel()->open();
        }
    }
}


void WeaponLvUpLayout::setInfo(const UINT64_t weaponGUID) {
    m_uWeaponGuid = weaponGUID;

    const vmsg::CSWeapon& stWeapon = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);
    P_ARM_OBJ->setUpWeapon(stWeapon);

    //get cfg
    const WeaponCfg* cfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
    const WeaponStarStepCfg* starCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(cfg->initstar(), cfg->initstep());
    if(NULL == cfg || NULL == starCfg) {
        CCLOG("[error]WeaponLvUpLayout::updateWeaponInfo,fail to get weapon cfg,cfgid=%d", stWeapon.cfgid());
        return;
    }

    //升级还需经验
    unsigned int upExp = 0;
    if(stWeapon.level() >= starCfg->needlevel()){
        upExp = 0;
    }else{
        upExp = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(stWeapon.level())->exp() - stWeapon.exp();
    }
    m_pExpTipTxt->setText(intToString(upExp));

    //coin
    m_pCoinTxt->setText("0");
}
void WeaponLvUpLayout::updateWeaponInfo(const UINT64_t weaponGUID, const bool bIsRsp){


    setInfo(weaponGUID);
    if(bIsRsp) {
        //播放材料消失特效
        showLvUpDoneEff();
    } else {
        m_pAutoSelBtn->setTouchEnabled(true); //设置为可点击
        m_pDoLvUpBtn->setTouchEnabled(true);

        //如果在播放特效，先不重置
        UIImageView* pBgImg = m_pCellBgLst[0];
        if(pBgImg && pBgImg->getNodeByTag(LV_UP_EFF_NODE_TAG)) {
            //do nothing
        } else {
            resetCells(); //reset
        }
        

		//GUIDE_SHOW(GUIDE_UI_WEAPON_INFO); // 打开时
    }
    
	
}


void WeaponLvUpLayout::setMaterialNumTxt(const int iCfgID, const int iCnt, UILabel* pTxt) {
    if(NULL == pTxt) {
        return;
    }
    const int iBagItemNum = P_BAG_CTRL->getItemCnt(iCfgID);
    if(iBagItemNum >= iCnt) {
        //够了
        pTxt->setColor(ccc3(0x00, 0xFF, 0x00));
    } else {
        pTxt->setColor(ccc3(0xFF, 0x00, 0x00));
    }
    char tmp[30];
    snprintf(tmp, 30, "%d/%d", iBagItemNum, iCnt);
    pTxt->setText(tmp);

}


void WeaponLvUpLayout::doLvUp() {
    const int bagIdx = P_ARM_OBJ->getWeaponIdx(m_uWeaponGuid);
    
    const vmsg::CSWeapon& stWeapon = P_ARM_OBJ->getUpWeapon();
    if(stWeapon.weaponguid() <= 0){
         CCLOG("[error]WeaponLvUpLayout::doLvUp, invalid weapon guid!");
        COMMON_DIALOG->show(TextStr(WEAPON_CHOOSE_WEAPON), NULL, NounStr(OK));
        return;
    }

    const map<UINT64_t, vmsg::CSWeaponLvUpConsume>& stUpMaterials = P_ARM_OBJ->getUpMaterials();
    if(stUpMaterials.size() <= 0){
        COMMON_DIALOG->show(TextStr(WEAPON_CHOOSE_MATERIAL), NULL, NounStr(OK));
        return;
    }

    const WeaponCfg* pCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
    const WeaponStarStepCfg* pStarCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pCfg->initstar(), pCfg->initstep());
    if(stWeapon.level() >= pStarCfg->needlevel()){
        COMMON_DIALOG->show(TextStr(WEAPON_FULL_LEVEL), NULL, NounStr(OK));
        return;
    }

    //联邦币不足
    if(!P_TIP_CTRL->CoinCostTip(m_uCostCoin, this)){
        return;
    }
     
    vector<vmsg::CSWeaponLvUpConsume> stConsume;
    stConsume.clear();

    map<UINT64_t, vmsg::CSWeaponLvUpConsume>::const_iterator iter;
    map<UINT64_t, vmsg::CSWeaponLvUpConsume> stTemp;

    for(iter = stUpMaterials.begin(); iter != stUpMaterials.end(); iter++) {
        if(stTemp.find(iter->second.id()) == stTemp.end()){
            stTemp.insert(make_pair(iter->second.id(), iter->second));
        }else{
            unsigned int preCnt = stTemp.find(iter->second.id())->second.cnt();
            preCnt++;
            stTemp.find(iter->second.id())->second.set_cnt(preCnt);
        }
    }

    map<UINT64_t, vmsg::CSWeaponLvUpConsume>::iterator stIter;
    for(stIter = stTemp.begin(); stIter != stTemp.end(); stIter++) {
        stConsume.push_back(stIter->second);
    }

    m_pAutoSelBtn->setTouchEnabled(false); //设置为不可点击
    m_pDoLvUpBtn->setTouchEnabled(false);

    P_WEAPON_CTRL->getWeaponInfoPanel()->showLoading();
    P_WEAPON_CTRL->sendWeaponLvUpRqst(bagIdx, stWeapon.weaponguid(), stConsume);
}

bool WeaponLvUpLayout::isLvFull() {
    //检查是否满级
    const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(m_uWeaponGuid);
    const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeaponInfo.cfgid());
    if(NULL == weaponCfg) {
        return false;
    }
    const unsigned int maxLev = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(weaponCfg->initstar(), weaponCfg->initstep())->needlevel();
    if(stWeaponInfo.level() >= maxLev) {
        COMMON_DIALOG->show(TextStr(WEAPON_FULL_LEVEL), NULL, NounStr(OK));
        return true;
    }
    return false;
}

void WeaponLvUpLayout::doAutoSel() {
    if(isLvFull()) {
        return;
    }

    const vector<vmsg::CSBagCellInfo>& m_stMaterials = P_ARM_OBJ->getLvUpMaterials();
    vector<unsigned int> m_stDetailMaterials;		//将背包中材料拆分
    unsigned int size = 0;

    for(unsigned int i = 0; i < m_stMaterials.size(); i++){
        const vmsg::CSBagCellInfo& stBagInfo = m_stMaterials.at(i); 

        for(unsigned int j = 0; j < stBagInfo.iteminfo().itemcnt(); j++){
            m_stDetailMaterials.push_back(stBagInfo.iteminfo().itemcfgid());
        }

        size += stBagInfo.iteminfo().itemcnt();
    }

    unsigned int length = CFireArmsObj::LV_UP_MAT_MAX_SIZE;
    if(length > size){
        length = size;
    }
    const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(m_uWeaponGuid);
    int iFullNeedExp = P_ARM_OBJ->getFullLvNeedExp(stWeaponInfo.cfgid(), stWeaponInfo.level());
    iFullNeedExp = iFullNeedExp - stWeaponInfo.exp(); //还要减去当前等级已经有的经验
    if(iFullNeedExp <= 0) {
        CCLOG("[warn]WeaponLvUpLayout::doAutoSel, full need exp is 0!");
        return;
    }

    P_ARM_OBJ->resetUpMaterials();
    int uTotalGetExp = 0; //当前选择的材料总共增加的经验
	
    for (unsigned int i = 0; i < length; i++){
        int nowIdx = (i);
        unsigned int cfgID = m_stDetailMaterials.at(i);
        WeaponExpItemCfg* pExpCfg = WEAPON_EXP_CFG->GetWeaponExpItemCfgByID(cfgID);
        if(pExpCfg) {
            uTotalGetExp += pExpCfg->weaponexp();
        }
        P_ARM_OBJ->addUpMaterial(cfgID, vmsg::THING_TYPE_ITEM, nowIdx);
        if(uTotalGetExp >= iFullNeedExp) {
            break;
        }
        
    }

	int nCellCnt = P_ARM_OBJ->getUpMaterials().size();
	// auto select  weapon
	if(uTotalGetExp < iFullNeedExp && nCellCnt <= CFireArmsObj::LV_UP_MAT_MAX_SIZE - 2) {
		nCellCnt = nCellCnt % 2 == 0 ? nCellCnt : nCellCnt + 1;
		const vector<vmsg::CSWeapon> &stWeapons = P_ARM_OBJ->getAllWeaponBag();
		const vmsg::CSWeapon &stMajorWeapon =  P_USER_OBJ->getUserInfo()->weapon; // 主武器
		const vmsg::CSWeapon &stUpWeapon = P_ARM_OBJ->getUpWeapon(); // 升级枪械
		int idx = size;

		for(unsigned int i = 0; i < stWeapons.size(); i++) {
			const vmsg::CSWeapon &stWeapon = stWeapons.at(i);
			const WeaponCfg* pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());

			// 升级枪械
			if(stUpWeapon.weaponguid() == stWeapon.weaponguid()) continue;
			// 玩家枪械
			if(stMajorWeapon.weaponguid() == stWeapon.weaponguid()) continue;
			// 非空闲枪械
			if(stWeapon.state() != vmsg::WEAPON_IDLE) continue;
			
			if(pWeaponCfg->initstar() == 1) { // 只可以自动选择吞1星枪
				P_ARM_OBJ->addUpMaterial(stWeapon.weaponguid(), vmsg::THING_TYPE_WEAPON, idx);
				nCellCnt += 2;

				if(uTotalGetExp >= iFullNeedExp || nCellCnt > CFireArmsObj::LV_UP_MAT_MAX_SIZE - 2) {
					break;
				}
			}

			idx++;
		}

	}

    //
    if(P_ARM_OBJ->getUpMaterials().size() <= 0) {
        //POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_EQUIP_MAT_NULL));
		//P_TIP_CTRL->getItemLackPanel()->promptBuyItem(vmsg::RMB_STORE_ITEM_GUN_EXP_PKG);
		COMMON_DIALOG->show(TextStr(WEAPON_NO_ENOUGH_GOTOBUY), this, NounStr(NOUN_GOTO), "", dialogbtneventselector(WeaponLvUpLayout::onClickDialogGotoBtn));
        resetCells();
        return;
    }
    

    updateCells();
}

void WeaponLvUpLayout::onClickDialogGotoBtn()
{
	if(P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_BLACKMARKET) || P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_PANIC_BUYING_STORE))
	{
		P_ENTRY_CTRL->enter(ENTRY_BUSINESS);
	}else
	{
		POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_MODULE_NOOPEN));
	}
}

void WeaponLvUpLayout::updateCells() {
    //update cells
    vector<vmsg::CSWeaponLvUpConsume> stAllMaterials = P_ARM_OBJ->getSortUpMaterials();
    if(stAllMaterials.size() <= 0) {
        resetCells();
        return;
    }

	m_bHasWeaponMaterial = false;

    const vmsg::CSWeapon& stWeapon = P_ARM_OBJ->getUpWeapon();
    int m_uPreMaterialSize = 0;			//重置上次的材料数量
    IconCell* pCell = NULL;
    UIImageView* pImg;

    //stAllMaterials是排序好的，枪械在前面。显示时，枪械占2个格子
    bool bIsPreCellWeapon = false;
    unsigned int iItemDataIdx = 0;
    int uTotalGetExp = 0; //当前选择的材料总共增加的经验
    
    for(int i = 0; i < MAX_ITEM_NUM; i++){
        if(i >= MAX_ITEM_NUM){
            break;
        }
        pImg = m_pCellBgLst[i];
        if(bIsPreCellWeapon || (iItemDataIdx >= stAllMaterials.size() ) ) {
            pImg->setVisible(false);
            bIsPreCellWeapon = false;
            continue;
        }
        pImg->setVisible(true);

        pCell = m_pItemCellLst[i];
        const vmsg::CSWeaponLvUpConsume& stLvUpConsume = stAllMaterials.at(iItemDataIdx);
        if(stLvUpConsume.type() == vmsg::THING_TYPE_ITEM) {
            pImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
            pImg->setSize(CCSize(100, 100));
            pCell->setInfo(stLvUpConsume.type(), stLvUpConsume.cnt(), stLvUpConsume.id());
            bIsPreCellWeapon = false;
            WeaponExpItemCfg* pExpCfg = WEAPON_EXP_CFG->GetWeaponExpItemCfgByID(stLvUpConsume.id());
            if(pExpCfg) {
                uTotalGetExp += pExpCfg->weaponexp();
            }
        } else if(stLvUpConsume.type() == vmsg::THING_TYPE_WEAPON){
            const vmsg::CSWeapon& stOne = P_ARM_OBJ->getOneWeaponByGUID(stLvUpConsume.id());
            pImg->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
            pImg->setSize(CCSize(180, 100));
            pCell->setInfo(stLvUpConsume.type(), stLvUpConsume.cnt(), stOne.cfgid() );
            bIsPreCellWeapon = true;
            uTotalGetExp += stOne.totalexp()* GLOBAL_CFG_MASTER->GetCfg().weaponeatradio() / 10000.0
                + WEAPON_CFG_MASTER->GetWeaponCfgByID(stOne.cfgid())->baseexp();
			m_bHasWeaponMaterial = true;
        }

        iItemDataIdx++;
        m_uPreMaterialSize++;
    }
    
    m_pTotalItemExpTxt->setText(intToString(uTotalGetExp));

    m_uCostCoin = stAllMaterials.size() * GLOBAL_CFG_MASTER->GetCfg().weaponexpcoincost();
    m_pCoinTxt->setText(intToString(m_uCostCoin));

    //show final exp info
    P_WEAPON_CTRL->getWeaponInfoPanel()->showLvUpDoneAttr();
}

void WeaponLvUpLayout::doSelMatResult() {
    updateCells();
}

UIImageView* WeaponLvUpLayout::getMaterialCell() {
	UIImageView *pImg = m_pCellBgLst[0];

	return pImg;
}

UIButton* WeaponLvUpLayout::getLevelUpBtn() {
	if(m_pDoLvUpBtn != NULL && m_pDoLvUpBtn->isRunning() && m_pDoLvUpBtn->isVisible() && m_pDoLvUpBtn->isTouchEnabled()) {
		return m_pDoLvUpBtn;
	}

	return NULL;
}

UIButton* WeaponLvUpLayout::getAutoSelBtn() {
	if(m_pAutoSelBtn != NULL && m_pAutoSelBtn->isRunning() && m_pAutoSelBtn->isVisible() && m_pAutoSelBtn->isTouchEnabled()) {
		bool bTouchEnabled = m_pAutoSelBtn->isTouchEnabled();
		return m_pAutoSelBtn;
	}

	return NULL;
}

void WeaponLvUpLayout::resetCells() {
    IconCell* pCell = NULL;
    UIImageView* pImg;
    for(int i = 0; i < MAX_ITEM_NUM; ++i) {
        pCell = m_pItemCellLst[i];
        pCell->clearCell();
        pCell->getIconImg()->setVisible(true);
        pCell->getIconImg()->loadTexture("other/img_sel_mat_tip.png", UI_TEX_TYPE_PLIST);

        pImg = m_pCellBgLst[i];  
        pImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
        pImg->setSize(CCSize(100, 100));
        pImg->setScale9Enabled(false);
        pImg->setVisible(true);
        pImg->removeNodeByTag(LV_UP_EFF_NODE_TAG);


    }
    m_pTotalItemExpTxt->setText("0");
}

void WeaponLvUpLayout::resetUI(){
    resetCells();
    m_pAutoSelBtn->setTouchEnabled(true);
    m_pDoLvUpBtn->setTouchEnabled(true);
    m_pTotalItemExpTxt->setText("0");
}

void WeaponLvUpLayout::showLvUpDoneEff() {
    UIImageView* pBgImg;
    IconCell* pCell;
    m_iEffCellNum = 0;
    CToolUtils::loadArmatureRGBA4444("effect/ut00039/ut00039.ExportJson");
    for(int iCellIdx = 0; iCellIdx < MAX_ITEM_NUM; ++iCellIdx) {
        pCell = m_pItemCellLst[iCellIdx];
        pBgImg = m_pCellBgLst[iCellIdx];
        if( ! (pCell->getIconImg()->isVisible() ) ) {
            continue;
        }

        CCArmature* pEff = CCArmature::create("ut00039");
        pEff->setAnchorPoint(ccp(0.5f, 0.5f));
        pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(WeaponLvUpLayout::onEffDone));
        pBgImg->removeNodeByTag(LV_UP_EFF_NODE_TAG);
        pBgImg->addNode(pEff, 11, LV_UP_EFF_NODE_TAG);
        m_iEffCellNum++;
        if(vmsg::THING_TYPE_WEAPON == pCell->getItemType()) {
            pEff->getAnimation()->play("fly_d", -1, -1, 0); //not loop
        } else {
            pEff->getAnimation()->play("fly", -1, -1, 0); //not loop
        }
        
    }

    bool bCanClick = (m_iEffCellNum > 0);
    m_pAutoSelBtn->setTouchEnabled(bCanClick);
    m_pDoLvUpBtn->setTouchEnabled(bCanClick);

	GUIDE_SHOW(GUIDE_UI_WEAPON_INFO);
}


void WeaponLvUpLayout::onEffDone(CCArmature * armature, MovementEventType type, const char * name) {
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
            resetCells();
            m_iEffCellNum = 0;
            m_pAutoSelBtn->setTouchEnabled(true);
            m_pDoLvUpBtn->setTouchEnabled(true);
            CToolUtils::removeArmatureFile("effect/ut00039/ut00039.ExportJson");

			GUIDE_SHOW(GUIDE_UI_WEAPON_INFO); // 播完升级特效
        }
    } 
}
