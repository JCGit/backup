#include "WeaponInfoPanel.h"
#include "control/PanelSoundType.h"
#include "WeaponCfgMaster.h"
#include "defines.h"
#include "WeaponStarStepCfgMaster.h"
#include "firearms/StarProgressBar.h"
#include "WeaponUtils.h"
#include "SkillCfgMaster.h"
#include "firearms/FireArmsObj.h"
#include "Resources.h"
#include "WeaponLevelCfgMaster.h"
#include "WeaponLibControl.h"
#include "firearms/FireArmsObj.h"
#include "WeaponExpItemCfgMaster.h"
#include "CCColorUtil.h"
#include "ClientSvrMsg.pb.h"
#include "Global.h"
#include "Resources.h"
#include "ToolUtils.h"
#include "control/AudioManager.h"
#include "game/UserObj.h"
#include "WeaponRefitCfgMaster.h"
#include "game/bag/BagControl.h"
#include "GlobalCfgMaster.h"

using namespace std;

WeaponInfoPanel::WeaponInfoPanel(){
    m_pOpenLayout = NULL;
    m_bIsForseClose = false;
}

WeaponInfoPanel::~WeaponInfoPanel(){
    CC_SAFE_RELEASE_NULL(m_pOpBtnLst);
}

bool WeaponInfoPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

        setOpenWithBgImg(true);
        setShowMoneyTopBar(true);

        initLayoutFromJsonFile("ui_v2/weapon_info_panel.ExportJson");
        addChild(m_pRootLayout);

		m_uSoundType = WEAPON_PANEL_TYPE;
        
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void WeaponInfoPanel::initUI(){
	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
    m_pCloseBtn->addTouchEventListener(this, toucheventselector(WeaponInfoPanel::touchEventHandler));

    //用临时变量就可以了
    UILayout* pBaseInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("base_info_layout"));

    m_pOpBtnLst = CCArray::create();
    m_pOpBtnLst->retain();
    
    m_pDoUpBtn = dynamic_cast<UIButton*>(pBaseInfoLayout->getChildByName("do_up_btn"));
    m_pDetailInfoBtn = dynamic_cast<UIButton*>(pBaseInfoLayout->getChildByName("detail_info_btn"));
    m_pAdvanceBtn = dynamic_cast<UIButton*>(pBaseInfoLayout->getChildByName("advance_btn"));
    m_pRefitBtn = dynamic_cast<UIButton*>(pBaseInfoLayout->getChildByName("refit_btn"));

    m_pOpBtnLst->addObject(m_pDoUpBtn);
    //m_pOpBtnLst->addObject(m_pDetailInfoBtn);
    m_pOpBtnLst->addObject(m_pAdvanceBtn);
    //m_pOpBtnLst->addObject(m_pRefitBtn);//暂时去掉了2015-1-6

    m_pDoUpBtn->addTouchEventListener(this, toucheventselector(WeaponInfoPanel::touchEventHandler));
    m_pDetailInfoBtn->addTouchEventListener(this, toucheventselector(WeaponInfoPanel::touchEventHandler));
    m_pAdvanceBtn->addTouchEventListener(this, toucheventselector(WeaponInfoPanel::touchEventHandler));
    
    m_pRefitBtn->setVisible(false); //暂时去掉了2015-1-6
    //m_pRefitBtn->addTouchEventListener(this, toucheventselector(WeaponInfoPanel::touchEventHandler));

    m_stInfoLayout.initWithLayout(pBaseInfoLayout);

    m_stDetailInfoLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("detail_info_layout")));
    m_stAdvanceLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("advance_layout")));
    m_stRefitLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("refit_layout")));
    m_stLvUpLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("lv_up_layout")));

    m_pWeaponImg = dynamic_cast<UIImageView*>(pBaseInfoLayout->getChildByName("bg_weapon"));
    m_pWeaponImg->setTouchEnabled(true);
    //m_pWeaponImg->addTouchEventListener(this, toucheventselector(WeaponInfoPanel::touchEventHandler));

    m_pSubPanelBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sub_panel_bg"));

}

void WeaponInfoPanel::refresh() {
    BasePanel::refresh();
    updateNowWeapon();
}

void WeaponInfoPanel::updateNowWeapon() {
    updateWeaponInfo(m_uWeaponCfgID, m_uWeaponGuid);
    //更新当前的显示,写在这，不影响回包里的调用
    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stLvUpLayout.updateWeaponInfo(m_uWeaponGuid);
    } else if( m_pOpenLayout == m_stAdvanceLayout.getRefLayout() ) {
        m_stAdvanceLayout.updateWeaponInfo(m_uWeaponGuid);
    } else if( m_pOpenLayout == m_stRefitLayout.getRefLayout() ) {
        m_stRefitLayout.updateWeaponInfo(m_uWeaponGuid);
    }
}
void WeaponInfoPanel::updateWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID) {
    m_uWeaponCfgID = cfgID;
    m_uWeaponGuid = weaponGUID;
    m_stInfoLayout.showWeaponInfo(cfgID, weaponGUID);
    const bool bIsShowOpBtn = (weaponGUID > 0);
    UIButton* pBtn = NULL;
    for(unsigned int i = 0; i < m_pOpBtnLst->count(); ++i) {
        pBtn = dynamic_cast<UIButton*>(m_pOpBtnLst->objectAtIndex(i));
        pBtn->setVisible(bIsShowOpBtn);
        pBtn->setEnabled(bIsShowOpBtn);
    }

    if(bIsShowOpBtn) {
        bool bIsInUse = (weaponGUID == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid());

        if(NULL == m_pOpenLayout) {
			m_stLvUpLayout.updateWeaponInfo(m_uWeaponGuid);
			showChildLayout(m_stLvUpLayout.getRefLayout(), m_pDoUpBtn);
        }
    }
    
    m_pSubPanelBg->setVisible(bIsShowOpBtn);
	m_pDetailInfoBtn->setEnabled(bIsShowOpBtn);

	// 标签提示
	const vmsg::CSWeapon& stWeapon = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);
	float offsetX = m_pAdvanceBtn->getSize().width * 0.36f;
	float offsetY = m_pAdvanceBtn->getSize().height * 0.18f;
	
	if(P_WEAPON_CTRL->isWeaponCanAdvance(stWeapon)) {
		CToolUtils::badgeWidgetWithPoint(m_pAdvanceBtn, 1, offsetX, offsetY);
	} else {
		CToolUtils::badgeWidgetWithPoint(m_pAdvanceBtn, 0, offsetX, offsetY);
	}
	
	
}


void WeaponInfoPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string nameStr = string(pszWidgetName);

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			close();			
        } else if(nameStr == "do_up_btn") {
			GUIDE_TOUCH(pWidget);
            if(m_uWeaponGuid <= 0) {
                break;
            }
            showChildLayout(m_stLvUpLayout.getRefLayout(), m_pDoUpBtn);
            m_stLvUpLayout.updateWeaponInfo(m_uWeaponGuid);
			GUIDE_SHOW(GUIDE_UI_WEAPON_INFO);
        } else if(nameStr == "detail_info_btn") {
            if(m_uWeaponGuid <= 0) {
                break;
            }
            showChildLayout(m_stDetailInfoLayout.getRefLayout(), m_pDetailInfoBtn);
            m_stDetailInfoLayout.updateWeaponDetailInfo(m_uWeaponCfgID, m_uWeaponGuid);
        } else if(nameStr == "advance_btn") {
			GUIDE_TOUCH(pWidget);

            if(m_uWeaponGuid <= 0) {
                break;
            }
            showChildLayout(m_stAdvanceLayout.getRefLayout(), m_pAdvanceBtn);
            m_stAdvanceLayout.updateWeaponInfo(m_uWeaponGuid);
			GUIDE_SHOW(GUIDE_UI_WEAPON_INFO);
        } else if(nameStr == "refit_btn") {
            if(m_uWeaponGuid <= 0) {
                break;
            }
            showChildLayout(m_stRefitLayout.getRefLayout(), m_pRefitBtn);
            m_stRefitLayout.updateWeaponInfo(m_uWeaponGuid);

        } else if(nameStr == "bg_weapon") {
            P_WEAPON_CTRL->getWeaponSelPanel()->open();
        }

        CCLog("===%s", nameStr.c_str());
		break;
	default:

		break;
	}
}

void WeaponInfoPanel::selBtn(UIButton* pSelBtn) {
    if(NULL == pSelBtn) {
        return;
    }
    for(unsigned int i = 0; i < m_pOpBtnLst->count(); ++i ) {
        UIButton* pOneBtn = (UIButton*)(m_pOpBtnLst->objectAtIndex(i));
        if(pSelBtn == pOneBtn) {
            pOneBtn->setBright(false);
            CCLabelTTF* pttf = (CCLabelTTF*)(pOneBtn->getTextVirtualRenderer());
            pttf->setColor(ccc3(0xff, 0xfa, 0xae) );
            pttf->setFontSize(24);
        } else {
            pOneBtn->setBright(true);
            CCLabelTTF* pttf = (CCLabelTTF*)(pOneBtn->getTextVirtualRenderer());
            pttf->setColor(ccc3(0x7b, 0xbc, 0xe1) );
            pttf->setFontSize(22);
        }
    }

}

void WeaponInfoPanel::showChildLayout(UILayout* pLayout, UIButton* pTabBtn) {
    if(m_pOpenLayout && m_pOpenLayout != pLayout) {
        m_pOpenLayout->removeFromParent();
    }
    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stInfoLayout.showDefaultAttr();
        m_stLvUpLayout.resetUI();
    } else if( m_pOpenLayout == m_stAdvanceLayout.getRefLayout() ) {
        m_stAdvanceLayout.resetUI();
    } else if( m_pOpenLayout == m_stRefitLayout.getRefLayout() ) {
        m_stRefitLayout.resetUI();
    }

    m_pOpenLayout = pLayout;
    m_pOpenLayout->setVisible(true);
    if(m_pOpenLayout->getParent()) {
        return;
    }

    m_pRootLayout->addChild(m_pOpenLayout);
    selBtn(pTabBtn);

	GUIDE_SHOW(GUIDE_UI_WEAPON_INFO);
}

void WeaponInfoPanel::open(){
    BasePanel::open();
    if(m_pOpenLayout) {
        resetUI();
    }
    m_stInfoLayout.startBgEff();

	
}
void WeaponInfoPanel::close(){
    BasePanel::close();
    resetUI();
    m_stInfoLayout.stopBgEff();

    if(!m_bIsForseClose) {
        //P_WEAPON_CTRL->getWeaponLibPanel()->open(); //关闭时打开枪械库面板
    }
    m_bIsForseClose = false; //默认为false，关闭时打开枪械库 
}

void WeaponInfoPanel::resetUI() {
    if(m_pOpenLayout && m_pOpenLayout->getParent()) {
        m_pOpenLayout->removeFromParent();
    }

    m_pOpenLayout = NULL;
    P_GLOBAL->clearPopEff(); //清除特效资源
   
    P_ARM_OBJ->resetUpMaterials();
    P_ARM_OBJ->resetUpWeapon();

    m_stLvUpLayout.resetUI();
    m_stAdvanceLayout.resetUI();
    m_stRefitLayout.resetUI();
    m_stInfoLayout.resetUI();
}

void WeaponInfoPanel::setForseClose() {
    m_bIsForseClose = true;
}

void WeaponInfoPanel::doRspError() {
    this->closeLoading();
    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stInfoLayout.showDefaultAttr();
        m_stLvUpLayout.resetUI();
    } else if( m_pOpenLayout == m_stAdvanceLayout.getRefLayout() ) {
        m_stAdvanceLayout.resetUI();
    } else if( m_pOpenLayout == m_stRefitLayout.getRefLayout() ) {
        m_stRefitLayout.resetUI();
    }
}

CCNode* WeaponInfoPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "tab_level_up_btn") { // 升级
		if(m_pDoUpBtn != NULL && m_pDoUpBtn->isVisible() && m_pDoUpBtn->isTouchEnabled()) {
			pRet = m_pDoUpBtn;
		}
	} else if(name == "material_cell") { // 升级
		pRet = m_stLvUpLayout.getMaterialCell();
	} else if(name == "auto_sel_btn") { // 一键选择
		pRet = m_stLvUpLayout.getAutoSelBtn();
	} else if(name == "level_up_btn") { // 升级
		pRet = m_stLvUpLayout.getLevelUpBtn();
	} else if(name == "tab_advance_btn") { // 进阶
		if(m_pAdvanceBtn != NULL && m_pAdvanceBtn->isVisible() && m_pAdvanceBtn->isTouchEnabled()) {
			pRet = m_pAdvanceBtn;
		}
	} else if(name == "advance_btn") { // 进阶
		pRet = m_stAdvanceLayout.getAdvanceBtn();
	} else if(name == "back_btn") {
		if(m_pCloseBtn != NULL && m_pCloseBtn->isRunning() && m_pCloseBtn->isTouchEnabled()) {
			pRet = m_pCloseBtn;
		}
	}else if (name == "advance_itemcell")//进阶材料
	{
		pRet = m_stAdvanceLayout.getAdvanceCell();
	}

	return pRet;
}

void WeaponInfoPanel::doSelMatResult() {
    if(m_stLvUpLayout.getRefLayout()->isVisible()) {
        m_stLvUpLayout.doSelMatResult();

		GUIDE_SHOW(GUIDE_UI_WEAPON_INFO);
    }
}

//回包时更新当前的显示
//只在回包时调用哈 会导致播放特效
void WeaponInfoPanel::doUpdateUI(const vmsg::CSWeapon* info, const bool bIsPlayEff ) { 
    if(!isOpen()) {
        return;
    }
    if(this->getIsInLoading()) {
        this->closeLoading();
    }

    if(info) {
        m_uWeaponCfgID = info->cfgid();
        m_uWeaponGuid = info->weaponguid();
    } else {
        if(m_uWeaponGuid > 0) {
            const vmsg::CSWeapon* pstWeaponInfo = P_ARM_OBJ->getWeaponByGUID(m_uWeaponGuid);
            if(pstWeaponInfo) {
                m_uWeaponCfgID = pstWeaponInfo->cfgid(); //枪械进阶后可能改变其cfgid
            }
        }
    }

    //info == NULL 表示只更新当前显示（比如材料数量变化了）
    updateWeaponInfo(m_uWeaponCfgID, m_uWeaponGuid);

    //接着更新子面板显示
    if(m_uWeaponGuid <= 0) {
        return;
    }
    if(m_stLvUpLayout.getRefLayout()->getParent() ) {
        m_stLvUpLayout.updateWeaponInfo(m_uWeaponGuid, bIsPlayEff);
    } else if(m_stDetailInfoLayout.getRefLayout()->getParent() ) {
        m_stDetailInfoLayout.updateWeaponDetailInfo(m_uWeaponCfgID, m_uWeaponGuid);
    } else if(m_stAdvanceLayout.getRefLayout()->getParent() ) {
        m_stAdvanceLayout.updateWeaponInfo(m_uWeaponGuid, bIsPlayEff);
    } else if(m_stRefitLayout.getRefLayout()->getParent() ) {
        m_stRefitLayout.updateWeaponInfo(m_uWeaponGuid);
    }
	
}


void WeaponInfoPanel::showLvUpDoneAttr() {
    if(m_stLvUpLayout.getRefLayout()->getParent() == NULL ) {
        return;
    }
    m_stInfoLayout.showLvUpDoneAttr();
}

void WeaponInfoPanel::playRefitEffect() {
    /*
    string path(EFFECT_RES_PATH + "ut00047/ut00047" + JSON_TYPE_NAME);
    CToolUtils::loadArmatureRGBA4444(path);

    GPopEffInfo stEffInfo;
    stEffInfo.strJsonPath = path;
    stEffInfo.strSoundName = "ut00015.ogg";
    stEffInfo.strArmatureName = "ut00047";
    stEffInfo.strSoundName = "ut00047.ogg";
    stEffInfo.strAniName = "fly_gz";
    stEffInfo.bIsLoop = false;
    stEffInfo.target = this;
    stEffInfo.fYPosPercent = 0.5f;
    stEffInfo.callFunc = movementEvent_selector(WeaponInfoPanel::playEffCallback);
    P_GLOBAL->showPopEff(stEffInfo);
    */
}

void WeaponInfoPanel::playAdvEffect() {
    m_stInfoLayout.playAdvEff();
}

void WeaponInfoPanel::playEffCallback(CCArmature* armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        //armature->getAnimation()->play("fly_xh", -1, -1, 1); 
        armature->removeFromParentAndCleanup(true);
    }
}


void WeaponInfoPanel::playLvUpEff(const int iDeltaLv) {
    if(!isOpen()) {
        return;
    }
    if(m_uWeaponGuid <= 0) {
        return;
    }
    m_stInfoLayout.playLvUpEff(iDeltaLv);
}

////////////////////////////////////////////////////////////////////////
void WeaponInfoOne::initWithLayout(UILayout *pRefLayout) {
    m_pRefLayout = pRefLayout;
    if(NULL == m_pRefLayout) {
        return;
    }
    m_pChangeWeaponTipImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("change_tip_img")); //change_tip_img
    m_pEffBg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("bg"));
    m_pInUseTipImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("in_use_tip_img"));
    m_pNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("name_txt"));
    m_pLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("lv_txt"));
	m_pChangeWeaponTipImg->setVisible(false);
    
    m_pAttrLvTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("lv_val_txt"));
    m_pAtkValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_val_txt"));
    m_pAtkSpeedTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("atk_sp_txt"));
	m_pBulletValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("bullet_val_txt"));

    m_pExpPerTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("exp_per_txt"));
    m_pExpBar = dynamic_cast<UILoadingBar*>(m_pRefLayout->getChildByName("exp_bar"));
    //m_pExpBar->setPercent(70);

    //level star
    m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", 
        "share/little_star_light.png", "ui_v2/share.png");
    m_pStarBar->setAnchorPoint(ccp(0.5f, 0.5f));
    UIImageView* pTmpBg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("bg_star"));
    m_pStarBar->setPosition(pTmpBg->getPosition());
    m_pStarBar->setStarMargin(1);
    m_pStarBar->setStarNum(7);
    pRefLayout->addChild(m_pStarBar);
    
    m_pWeaponIconBg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("bg_weapon")); //枪械库底图框
    m_pWeaponColorImg = dynamic_cast<UIImageView*>(m_pWeaponIconBg->getChildByName("weapon_color_bg")); 
    m_pWeaponImg = dynamic_cast<UIImageView*>(m_pWeaponIconBg->getChildByName("weapon_img"));

    m_pHitValTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("hit_val_txt"));

}
void WeaponInfoOne::showWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID) {
    m_uWeaponCfgID = cfgID;
    m_uWeaponGuid = weaponGUID;
    const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);
    if(NULL == weaponCfg) {
        return;
    }

    showDefaultAttr(); //等级，攻击力

    int iWeaponLv = 1;
    int iWeaponColor = 1;
    bool bIsInUse = false;
    if(weaponGUID > 0) {
        const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);
        iWeaponLv = stWeaponInfo.level();
        iWeaponColor = stWeaponInfo.quality();

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/other.plist");
		if(weaponGUID == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid()){   //身上主武器
			bIsInUse = true;
			m_pInUseTipImg->loadTexture("other/main_weapon_use_img.png", UI_TEX_TYPE_PLIST);
		}else{
			if(stWeaponInfo.state() == vmsg::WEAPON_IN_BATTLE){  //武器处于上阵状态
				bIsInUse = true;
				m_pInUseTipImg->loadTexture("other/deputy_weapon_use_img.png", UI_TEX_TYPE_PLIST);
			}else{
				bIsInUse = false;
			}
		}

        //升级还需经验
        unsigned int upExp = 0;
        WeaponLevelCfg* pLvCfg = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(stWeaponInfo.level());
        if(pLvCfg) {
            //m_pExpPerTxt m_pExpBar
            const int iPer = stWeaponInfo.exp() * 100 / (pLvCfg->exp() * 1.0f * weaponCfg->expmulti()/ 10000 );
            m_pExpPerTxt->setText(intToString(iPer) + "%");
            m_pExpBar->setPercent(iPer);
        } else {
            CCLOG("[warn]WeaponInfoOne::showWeaponInfo, fail to get WeaponLvCfg,lv=%d", stWeaponInfo.level());
            m_pExpPerTxt->setText("");
            m_pExpBar->setPercent(0);
        }

        const unsigned int maxLev = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(weaponCfg->initstar(), weaponCfg->initstep())->needlevel();
        if(stWeaponInfo.level() >= maxLev) {
            m_pExpPerTxt->setText("100%");
            m_pExpBar->setPercent(100);
        }


    } else {
        m_pExpPerTxt->setText("");
        m_pExpBar->setPercent(0);
    }
    m_iRealExpPercent = m_pExpBar->getPercent();

    m_pInUseTipImg->setVisible(bIsInUse);

    string strTmp(weaponCfg->weaponname());
    /*
    if(weaponCfg->initstep() > 0){
        strTmp += ("+" + intToString(weaponCfg->initstep()));
    }*/
    m_pNameTxt->setText(strTmp.c_str());
    m_pLvTxt->setText("Lv." + intToString(iWeaponLv));
    
    m_pAtkSpeedTxt->setText(intToString(weaponCfg->shotspeed()));
    //m_pSkillTxt->setText(SKILL_CFG_MASTER->GetSkillCfgByID(weaponCfg->weaponsuperskill())->name());
	m_pBulletValTxt->setText(intToString(weaponCfg->bulletshowval()));

    m_pStarBar->setStarNum(weaponCfg->initstar());
    
    m_pWeaponColorImg->loadTexture(WeaponUtils::getWeaponBigIconColorBg(weaponCfg->weaponlv()), UI_TEX_TYPE_PLIST );

    string iconStr = ICON_BIG_WEAPON_PATH + weaponCfg->weaponicon() + PNG_TYPE_NAME;
    m_pWeaponImg->loadTexture(iconStr.c_str());

    m_pHitValTxt->setText(intToString(weaponCfg->hit()));

}

void WeaponInfoOne::showLvUpDoneAttr() {
    const vmsg::CSWeapon* pstWeaponInfo = P_ARM_OBJ->getWeaponByGUID(m_uWeaponGuid);
    if(NULL == pstWeaponInfo) {
        return;
    }
    const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(pstWeaponInfo->cfgid());
    if(NULL == weaponCfg) {
        return;
    }

    const map<UINT64_t, vmsg::CSWeaponLvUpConsume>& stAllMaterials = P_ARM_OBJ->getUpMaterials();
    if(stAllMaterials.size() <= 0){
        return;
    }
    //
    int uTotalGetExp = 0; //当前选择的材料总共增加的经验
    for(auto iter = stAllMaterials.begin(); iter != stAllMaterials.end(); ++iter) {
        const vmsg::CSWeaponLvUpConsume& stOneItem = iter->second;
        if(stOneItem.type() == vmsg::THING_TYPE_ITEM){
            WeaponExpItemCfg* pExpCfg = WEAPON_EXP_CFG->GetWeaponExpItemCfgByID(stOneItem.id());
            if(pExpCfg) {
                uTotalGetExp += pExpCfg->weaponexp();
            }
        }else if(stOneItem.type() == vmsg::THING_TYPE_WEAPON){
            const vmsg::CSWeapon& stOne = P_ARM_OBJ->getOneWeaponByGUID(stOneItem.id());
            uTotalGetExp += stOne.totalexp()* GLOBAL_CFG_MASTER->GetCfg().weaponeatradio() / 10000.0
                + WEAPON_CFG_MASTER->GetWeaponCfgByID(stOne.cfgid())->baseexp();
        }
    }
    const int iDeltaLv = P_ARM_OBJ->getLevelsByExp(*pstWeaponInfo, uTotalGetExp);
    const int iFinalTotalExp = pstWeaponInfo->exp() + uTotalGetExp;
    int iFinalLv = pstWeaponInfo->level() + iDeltaLv;
    const int maxLev = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(weaponCfg->initstar(), weaponCfg->initstep())->needlevel();
    if(iFinalLv > maxLev) {
        iFinalLv = maxLev;
    }

    m_strPreviewAttrLvTxt = "Lv." + intToString(iFinalLv) + "/" + intToString(maxLev);
    m_pAttrLvTxt->setText(m_strPreviewAttrLvTxt);

    int iAddCoe = WeaponUtils::getWeaponAddCoe(pstWeaponInfo->cfgid(), iFinalLv, pstWeaponInfo->quality());
    iAddCoe += WeaponUtils::getWeaponAtk(m_uWeaponCfgID, iFinalLv, pstWeaponInfo->quality());
    m_strPreviewAtkValTxt = intToString(iAddCoe);
    m_pAtkValTxt->setText(m_strPreviewAtkValTxt);

    CCLOG("[trace]WeaponInfoOne::showLvUpDoneAttr,now lv %d, addexp %d, now exp %llu,delta lv %d,max lv %d", 
        pstWeaponInfo->level(), uTotalGetExp, pstWeaponInfo->exp(), iDeltaLv, maxLev);

    bool bIsShowTxtEff = true;
    if(iDeltaLv <= 0) {
        bIsShowTxtEff = false;
    }

    //exp bar
    int iNowPercent = m_iRealExpPercent;
    iNowPercent = iNowPercent > 100 ? 100 : iNowPercent;

    int iToPercent = 100;
    if((int)pstWeaponInfo->level() <= iFinalLv) {
        WeaponLevelCfg* pLvCfg = WEAPON_LEVEL_CFG->GetWeaponLevelCfgByID(pstWeaponInfo->level());
        if(pLvCfg) {
            iToPercent = iFinalTotalExp * 100 / pLvCfg->exp();
            iToPercent = iToPercent > 100 ? 100 : iToPercent;
        }
    }

    m_pAttrLvTxt->stopAllActions();
    m_pAtkValTxt->stopAllActions();
    m_pExpBar->stopAllActions();
    m_pAttrLvTxt->setOpacity(255);
    m_pAtkValTxt->setOpacity(255);
    m_pExpBar->setOpacity(255);
    /*
    //CCColorUtil::setLabelColor(m_pAttrLvTxt, ccGREEN);
    //CCColorUtil::setLabelColor(m_pAtkValTxt, ccGREEN);
    CCSequence* seq = CCSequence::create( CCFadeTo::create(1.0f, 80), CCFadeTo::create(1.0f, 255), NULL );
    m_pAttrLvTxt->runAction( CCRepeatForever::create(seq) );
    seq = CCSequence::create( CCFadeTo::create(1.0f, 80), CCFadeTo::create(1.0f, 255), NULL ); //不能共用
    m_pAtkValTxt->runAction( CCRepeatForever::create(seq) );
    */

    CCCallFuncND *fun1 = CCCallFuncND::create(this, callfuncND_selector(WeaponInfoOne::txtPreviewCallback), (void *)iToPercent );
    CCCallFuncND *fun2 = CCCallFuncND::create(this, callfuncND_selector(WeaponInfoOne::txtRealCallback),  (void *)iNowPercent );
    CCSequence* seq = NULL;
    if(bIsShowTxtEff) {
        seq  = CCSequence::create( CCFadeTo::create(1.0f, 30), fun1, CCFadeTo::create(1.0f, 255),  
            CCFadeTo::create(1.0f, 30), fun2, CCFadeTo::create(1.0f, 255),  NULL );
        m_pAttrLvTxt->runAction( CCRepeatForever::create(seq) );

        seq  = CCSequence::create( CCFadeTo::create(1.0f, 30), fun1, CCFadeTo::create(1.0f, 255),  
            CCFadeTo::create(1.0f, 30), fun2, CCFadeTo::create(1.0f, 255),  NULL ); //不能共用
        m_pAtkValTxt->runAction( CCRepeatForever::create(seq) );
    }
    seq  = CCSequence::create( CCFadeTo::create(1.0f, 30), fun1, CCFadeTo::create(1.0f, 255),  
        CCFadeTo::create(1.0f, 30), fun2, CCFadeTo::create(1.0f, 255),  NULL ); //不能共用
    m_pExpBar->runAction( CCRepeatForever::create(seq) );
}

void WeaponInfoOne::txtPreviewCallback(CCNode* pSender, void* pData) {
    UILoadingBar* pbar = dynamic_cast<UILoadingBar*>(pSender);
    if(pbar) {
        int val = static_cast<int>((long)pData);
        m_pExpBar->setPercent(val);
        m_pExpPerTxt->setText( intToString(val) + "%" );
        return;
    }

    UILabel* ptxt = dynamic_cast<UILabel*>(pSender);
    if(NULL == ptxt){
        return;
    }
    if(m_pAttrLvTxt == ptxt) {
        m_pAttrLvTxt->setText(m_strPreviewAttrLvTxt);
    } else if(m_pAtkValTxt == ptxt) {
        m_pAtkValTxt->setText(m_strPreviewAtkValTxt);
    } 
}

void WeaponInfoOne::txtRealCallback(CCNode* pSender, void* pData) {
    UILoadingBar* pbar = dynamic_cast<UILoadingBar*>(pSender);
    if(pbar) {
        int val = static_cast<int>((long)pData);
        m_pExpBar->setPercent(val);
        m_pExpPerTxt->setText( intToString(val) + "%" );
        return;
    }

    UILabel* ptxt = dynamic_cast<UILabel*>(pSender);
    if(NULL == ptxt){
        return;
    }
    if(m_pAttrLvTxt == ptxt) {
        m_pAttrLvTxt->setText(m_strRealAttrLvTxt);
    } else if(m_pAtkValTxt == ptxt) {
        m_pAtkValTxt->setText(m_strRealAtkValTxt);
    } 
} 

void WeaponInfoOne::showDefaultAttr() {
    //m_pWeaponImg->removeNodeByTag(LV_UP_EFF_NODE_TAG); //时序问题，不用这个
    const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_uWeaponCfgID);
    if(NULL == weaponCfg) {
        return;
    }
    int iWeaponLv = 1;
    int iWeaponColor = 1;
    if(m_uWeaponGuid > 0) {
        const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(m_uWeaponGuid);
        iWeaponLv = stWeaponInfo.level();
        iWeaponColor = stWeaponInfo.quality();
    } 
    
    m_pAttrLvTxt->stopAllActions();
    m_pAtkValTxt->stopAllActions();
    m_pExpBar->stopAllActions();
    m_pAttrLvTxt->setOpacity(255);
    m_pAtkValTxt->setOpacity(255);
    m_pExpBar->setOpacity(255);

    const int maxLev = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(weaponCfg->initstar(), weaponCfg->initstep())->needlevel();
    iWeaponLv = iWeaponLv > maxLev ? maxLev : iWeaponLv;
    m_pAttrLvTxt->setText("Lv." + intToString(iWeaponLv) + "/" + intToString(maxLev));

	unsigned int weaponAddCoe = WeaponUtils::getWeaponAddCoe(m_uWeaponCfgID, iWeaponLv, iWeaponColor);
    m_pAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(m_uWeaponCfgID, iWeaponLv, iWeaponColor) + weaponAddCoe));

    m_pExpBar->setPercent(m_iRealExpPercent);
    m_pExpPerTxt->setText( intToString(m_iRealExpPercent) + "%");

    m_strRealAttrLvTxt = m_pAttrLvTxt->getStringValue(); //记录下属性
    m_strRealAtkValTxt = m_pAtkValTxt->getStringValue();
}

void WeaponInfoOne::playLvUpEff(const int iDeltaLv) {
    //升多少级就播放对应次数
    int iPlayNum = iDeltaLv;
    if(iDeltaLv > 20) {
        iPlayNum = 20;
    }
    m_iLvUpEffPlayNum = iPlayNum;
    m_pExpBar->showRunEff(iPlayNum, 5, m_pExpBar->getPercent() );

    CToolUtils::loadArmatureRGBA4444("effect/ut00066/ut00066.ExportJson");
    m_pRefLayout->removeNodeByTag(LV_UP_EFF_TAG);

    AudioManager::playEffect( "ut00066.ogg" );

    CCArmature* pEff = CCArmature::create("ut00066");
    pEff->setAnchorPoint(ccp(0.5f, 1.0f));

    int xPos = m_pWeaponIconBg->getPositionX() - 28;
    int yPos = m_pWeaponIconBg->getPositionY() + 287;
    pEff->setPosition(xPos, yPos);
    pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(WeaponInfoOne::playLvUpEffCallback));
    m_pRefLayout->addNode(pEff, 8, LV_UP_EFF_TAG);
    pEff->getAnimation()->play("fly", -1, -1, 1); //not loop

}

void WeaponInfoOne::playLvUpEffCallback(CCArmature* armature, MovementEventType type, const char * name) {
    if (LOOP_COMPLETE == type){
        m_iLvUpEffPlayNum--;
        if(m_iLvUpEffPlayNum <= 0) {
            armature->getAnimation()->stop();
            armature->removeFromParentAndCleanup(true);
        }
    }
}

void WeaponInfoOne::resetUI() {
    m_pAttrLvTxt->stopAllActions();
    m_pAtkValTxt->stopAllActions();
    m_pExpBar->stopAllActions();
    m_pAttrLvTxt->setOpacity(255);
    m_pAtkValTxt->setOpacity(255);
    m_pExpBar->setOpacity(255);
    m_pExpBar->stopRunEff();
    m_pRefLayout->removeNodeByTag(LV_UP_EFF_TAG);
}


void WeaponInfoOne::startBgEff() {
    //Global::releaseUnusePanel 里清除
    CToolUtils::loadArmatureRGBA4444("effect/ut00064/ut00064.ExportJson");
   
    m_pEffBg->removeNodeByTag(BG_EFF_TAG);

    CCArmature* pEff = CCArmature::create("ut00064");
    pEff->setAnchorPoint(ccp(0.5f, 0.5f));
    pEff->setPosition(m_pEffBg->getSize().width/2 - 20, m_pEffBg->getSize().height/2 + 30);
    m_pEffBg->addNode(pEff, 0, BG_EFF_TAG);
    pEff->getAnimation()->play("fly", -1, -1, 1); //loop
    
    //reset visible
    m_pWeaponImg->setVisible(true); //visible
    m_pWeaponColorImg->setVisible(true);
    //m_pChangeWeaponTipImg->setVisible(true);

}

void WeaponInfoOne::stopBgEff() {
    
    m_pEffBg->removeNodeByTag(BG_EFF_TAG);
    m_pRefLayout->removeNodeByTag(ADV_EFF_TAG);
    m_pRefLayout->removeNodeByTag(LV_UP_EFF_TAG);
    m_pExpBar->stopRunEff();
}

void WeaponInfoOne::playAdvEff() {

    CToolUtils::loadArmatureRGBA4444("effect/ut00065/ut00065.ExportJson");

    m_pRefLayout->removeNodeByTag(ADV_EFF_TAG);

    m_pWeaponImg->setVisible(false); //invisible
    m_pWeaponColorImg->setVisible(false);
    //m_pChangeWeaponTipImg->setVisible(false);

    AudioManager::playEffect( "ut00066.ogg" );

    CCArmature* pEff = CCArmature::create("ut00065");
    pEff->setAnchorPoint(ccp(0.5f, 1.0f));

    int xPos = m_pWeaponIconBg->getPositionX() - 28;
    int yPos = m_pWeaponIconBg->getPositionY() + 287;
    pEff->setPosition(xPos, yPos);

    pEff->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(WeaponInfoOne::onPlayAdvEffFrameEvt));
    pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(WeaponInfoOne::playAdvEffCallback));

    m_pRefLayout->addNode(pEff, 8, ADV_EFF_TAG);
    pEff->getAnimation()->play("fly", -1, -1, 0); //not loop
}

void WeaponInfoOne::onPlayAdvEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx) {
    string evtName(stEvent);
    if(evtName == "chuxian") {
        m_pWeaponImg->setVisible(true); //visible
        m_pWeaponColorImg->setVisible(true);
        //m_pChangeWeaponTipImg->setVisible(true);
    }
}

void WeaponInfoOne::playAdvEffCallback(CCArmature* armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        armature->removeFromParentAndCleanup(true);
        CToolUtils::removeArmatureFile("effect/ut00065/ut00065.ExportJson");
    }
}
