#include "EquipLibPanel.h"
#include "control/PanelSoundType.h"
#include "defines.h"
#include "Resources.h"
#include "EquipLibControl.h"
#include "bag/BagControl.h"
#include "ToolUtils.h"
#include "PopTipLayer.h"
#include "Global.h"
#include "ToolUtils.h"
#include "EquipmentCfgMaster.h"
#include "EquipStarStepCfgMaster.h"
#include "EquipRefitCfgMaster.h"
#include "bag/BagControl.h"
#include "game/UserObj.h"

using namespace std;

EquipLibPanel::EquipLibPanel(){
    m_pOpenLayout = NULL;
}

EquipLibPanel::~EquipLibPanel(){
    CC_SAFE_RELEASE_NULL(m_pOpBtnLst);
}

bool EquipLibPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

        setOpenWithBgImg(true);
        setShowMoneyTopBar(true);

        initLayoutFromJsonFile("ui_v2/equip_lib_info_panel.ExportJson"); 
        addChild(m_pRootLayout);

		m_uSoundType = EQUIP_REFINE_PANEL_TYPE;

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void EquipLibPanel::initUI(){

    m_pOpBtnLst = CCArray::create();
    m_pOpBtnLst->retain();

    m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
    m_pCloseBtn->addTouchEventListener(this, toucheventselector(EquipLibPanel::touchEventHandler));

    m_pLvUpBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_lv_up"));
    m_pAdvanceBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_adv"));
    m_pMixBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_mix"));

    m_pOpBtnLst->addObject(m_pLvUpBtn);
    m_pOpBtnLst->addObject(m_pAdvanceBtn);
    m_pOpBtnLst->addObject(m_pMixBtn);

    m_pLvUpBtn->addTouchEventListener(this, toucheventselector(EquipLibPanel::touchEventHandler));
    m_pAdvanceBtn->addTouchEventListener(this, toucheventselector(EquipLibPanel::touchEventHandler));

    m_pMixBtn->setVisible(false); //暂时去掉了2015-1-6
    //m_pMixBtn->addTouchEventListener(this, toucheventselector(EquipLibPanel::touchEventHandler));

    m_stEquipInfoLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("info_layout")));

    m_stLvUpLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("lv_up_layout")));
    m_stAdvLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("adv_layout")));
    m_stMixLayout.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("mix_layout")));

}

void EquipLibPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string nameStr = string(pszWidgetName);
    
	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(nameStr == "tab_btn_lv_up") {
            showChildLayout(m_stLvUpLayout.getRefLayout(), m_pLvUpBtn);
        } else if(nameStr == "tab_btn_adv") {
            showChildLayout(m_stAdvLayout.getRefLayout(), m_pAdvanceBtn);
        } else if(nameStr == "tab_btn_mix") {
            showChildLayout(m_stMixLayout.getRefLayout(), m_pMixBtn);
        } else if(nameStr == "close_btn") {
			GUIDE_TOUCH(pWidget);
            close();
        } 
		break;
	default:

		break;
	}
}

void EquipLibPanel::selBtn(UIButton* pSelBtn) {
    if(NULL == pSelBtn) {
        return;
    }
    for(unsigned int i = 0; i < m_pOpBtnLst->count(); ++i ) {
        UIButton* pOneBtn = (UIButton*)(m_pOpBtnLst->objectAtIndex(i));
        if(pSelBtn == pOneBtn) {
            //按下
            pOneBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae) ); //按下
            pOneBtn->setTitleFontSize(24);
            pOneBtn->setBright(false);
            pOneBtn->setTouchEnabled(false);
        } else {
            pOneBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1) );
            pOneBtn->setTitleFontSize(22);
            pOneBtn->setBright(true);
            pOneBtn->setTouchEnabled(true);
        }
    }

}

void EquipLibPanel::showChildLayout(UILayout* pLayout, UIButton* pTabBtn) {
    
    if(m_pOpenLayout && m_pOpenLayout != pLayout) {
        m_pOpenLayout->removeFromParent();
    }
    
    if(NULL == pLayout) {
        CCLOG("[warn]EquipLibPanel::showChildLayout, layout is null.");
        return;
    }
    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stLvUpLayout.resetUI();
        m_stEquipInfoLayout.resetUI();
    } else if( m_pOpenLayout == m_stAdvLayout.getRefLayout() ) {
        m_stAdvLayout.resetUI();
    } else if( m_pOpenLayout == m_stMixLayout.getRefLayout() ) {
        m_stMixLayout.resetUI();
    }

    m_pOpenLayout = pLayout;
    m_pOpenLayout->setVisible(true);

    if(m_pOpenLayout->getParent()) {
        return;
    }
    m_pRootLayout->addChild(m_pOpenLayout);
    selBtn(pTabBtn);

    //update sub layout
    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stLvUpLayout.doUpdateUI(false);
    } else if( m_pOpenLayout == m_stAdvLayout.getRefLayout() ) {
        m_stAdvLayout.doUpdateUI();
    } else if( m_pOpenLayout == m_stMixLayout.getRefLayout() ) {
        m_stMixLayout.doUpdateUI();
    }
	GUIDE_SHOW(GUIDE_UI_EQUIP_LIB);
}

CCNode* EquipLibPanel::getGuideNodeByName(const std::string &name) {
	UIWidget *pBtn = NULL;

	if(name == "auto_sel_btn") {
		pBtn = m_stLvUpLayout.getAutoSelBtn();
	} else if(name == "level_up_btn") {
		pBtn = m_stLvUpLayout.getLvUpBtn();
	} else if(name == "back_btn") {
		if(m_pCloseBtn != NULL && m_pCloseBtn->isRunning() && m_pCloseBtn->isTouchEnabled()) {
			pBtn = m_pCloseBtn;
		}
	}

	return pBtn;
}

void EquipLibPanel::open(){
    BasePanel::open();
    showChildLayout(m_stLvUpLayout.getRefLayout(), m_pLvUpBtn);
    P_BAG_CTRL->sendQueryBagInfoRqst();
    CToolUtils::loadArmatureRGBA4444("effect/ut00039/ut00039.ExportJson");
    
    m_stEquipInfoLayout.doUpdateUI(false); //update
}

void EquipLibPanel::refresh() {
    BasePanel::refresh();
}


void EquipLibPanel::close(){
    BasePanel::close();
    if(m_pOpenLayout && m_pOpenLayout->getParent()) {
        m_pOpenLayout->removeFromParent();
    }

    P_EQUIP_LIB_CTRL->getObj()->clearLvUpMat();
    P_EQUIP_LIB_CTRL->getObj()->resetSelEquip();

    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stLvUpLayout.resetUI();
    } else if( m_pOpenLayout == m_stAdvLayout.getRefLayout() ) {
        m_stAdvLayout.resetUI();
    } else if( m_pOpenLayout == m_stMixLayout.getRefLayout() ) {
        m_stMixLayout.resetUI();
    }
    m_stEquipInfoLayout.resetUI();
    m_pOpenLayout = NULL;

    P_GLOBAL->clearPopEff(); //清除特效资源
    CToolUtils::removeArmatureFile("effect/ut00039/ut00039.ExportJson");

    POP_TIP_LAYER_MASTER->ClearAllTips();

    P_EQUIP_LIB_CTRL->getEquipLibListPanel()->open();
}

void EquipLibPanel::doUpdateUI(const bool bIsRsp){
    if(!isOpen()) {
        return;
    }
    if(this->getIsInLoading()) {
        closeLoading();
    }

    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stLvUpLayout.doUpdateUI(bIsRsp);
    } else if( m_pOpenLayout == m_stAdvLayout.getRefLayout() ) {
        m_stAdvLayout.doUpdateUI();
    } else if( m_pOpenLayout == m_stMixLayout.getRefLayout() ) {
        m_stMixLayout.doUpdateUI();
    }
    m_stEquipInfoLayout.doUpdateUI(bIsRsp);

	const UINT64_t& uEquipGuid = P_EQUIP_LIB_CTRL->getObj()->getSelEquipGuid();
	if(uEquipGuid <= 0) {
		//暂时没有选择装备,不提示
		CCLOG("[warn]EquipInfoLayout::doUpdateUI, equip guid is 0.");
		return;
	}
	const EquipLibOneInfo stEquip = P_EQUIP_LIB_CTRL->getObj()->getEquipInfo(uEquipGuid);
	vmsg::CSEquipInfo stEquipInfo = stEquip.equipInfo;
	float offsetX = m_pAdvanceBtn->getSize().width * 0.36f;
	float offsetY = m_pAdvanceBtn->getSize().height * 0.18f;

	const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
	const EquipStarStepCfg *pEquipAdvanceCfg = EQUIPSTARSTEP_CFG_MASTER->GetCfg(pEquipCfg->initstar(), pEquipCfg->initstep());
	unsigned int uMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(stEquipInfo.equipid());

	if(stEquipInfo.level() >= uMaxLv
		&& pEquipAdvanceCfg != NULL 
		&& (pEquipAdvanceCfg->star() != pEquipAdvanceCfg->nextstar() || pEquipAdvanceCfg->step() != pEquipAdvanceCfg->nextstep())) {
			bool isMaterialEnough = true;

			for(int i = 0; i < pEquipAdvanceCfg->needs_size(); i++) {
				const EquipNeeds &stEquipNeeds = pEquipAdvanceCfg->needs(i);

				if(P_BAG_CTRL->getItemCnt(stEquipNeeds.cfgid()) < stEquipNeeds.cfgcnt()) {
					isMaterialEnough = false;
					break;
				}
			}

			if(isMaterialEnough && pEquipAdvanceCfg->needequipcnt() > 0) {
				if(P_BAG_CTRL->getEquipCnt(stEquipInfo.equipid(), stEquipInfo.equipguid()) < (int)pEquipAdvanceCfg->needequipcnt()) {
					isMaterialEnough = false;
				}
			}

			if(isMaterialEnough && P_USER_OBJ->checkCoin(pEquipAdvanceCfg->costcoin()) == 0) {
				CToolUtils::badgeWidgetWithPoint(m_pAdvanceBtn, 1, offsetX, offsetY);
			} else {
				CToolUtils::badgeWidgetWithPoint(m_pAdvanceBtn, 0, offsetX, offsetY);
			}
	} else {
        CToolUtils::badgeWidgetWithPoint(m_pAdvanceBtn, 0, offsetX, offsetY);
    }
    /*
	const EquipRefitCfg *pEquipRefitCfg = EQUIPREFIT_CFG_MASTER->GetCfg(pEquipCfg->part(), stEquipInfo.quality() + 1);

	if(pEquipRefitCfg != NULL) {
		bool isMaterialEnough = true;

		for(int i = 0; i < pEquipRefitCfg->elmtlist_size(); i++) {
			const EquipRefitConsumeElement &stElmt = pEquipRefitCfg->elmtlist(i);

			if(P_BAG_CTRL->getItemCnt(stElmt.elementid()) < stElmt.elementcnt()) {
				isMaterialEnough = false;
				break;
			}
		}

		if(isMaterialEnough && P_USER_OBJ->checkCoin(pEquipRefitCfg->costcoin()) == 0) {
			CToolUtils::badgeWidgetWithPoint(m_pMixBtn, 1, offsetX, offsetY);
		} else {
			CToolUtils::badgeWidgetWithPoint(m_pMixBtn, 0, offsetX, offsetY);
		}
	}*/
}

void EquipLibPanel::updateSelMat() {
    if(!isOpen()) {
        return;
    }
    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stLvUpLayout.updateSelMat();
    } 
}



void EquipLibPanel::playMixEffect() {
    /*
    string path(EFFECT_RES_PATH + "ut00047/ut00047" + JSON_TYPE_NAME);
    CToolUtils::loadArmatureRGBA4444(path);

    GPopEffInfo stEffInfo;
    stEffInfo.strJsonPath = path;
    stEffInfo.strSoundName = "ut00015.ogg";
    stEffInfo.strArmatureName = "ut00047";
    stEffInfo.strSoundName = "ut00047.ogg";
    stEffInfo.strAniName = "fly_rh";
    stEffInfo.bIsLoop = false;
    stEffInfo.target = this;
    stEffInfo.fYPosPercent = 0.5f;
    stEffInfo.callFunc = movementEvent_selector(EquipLibPanel::playEffCallback);
    P_GLOBAL->showPopEff(stEffInfo);
    */
}

void EquipLibPanel::playAdvEffect() {
    m_stEquipInfoLayout.playAdvEff();
}


void EquipLibPanel::playEffCallback(CCArmature* armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        armature->removeFromParentAndCleanup(true);
    }
}


void EquipLibPanel::previewLvUp() {
    if(!isOpen()) {
        return;
    }
    if(m_pOpenLayout == m_stLvUpLayout.getRefLayout()) {
        m_stEquipInfoLayout.previewLvUpExp(); //仅当选择升级tab时才预览
    } 
}


