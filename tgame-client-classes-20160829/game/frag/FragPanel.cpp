#include "FragPanel.h"
#include "FragListItem.h"
#include "FragControl.h"
#include "DebrisCfgMaster.h"
#include "defines.h"
#include "ItemUtils.h"
#include "UserObj.h"
#include "PopTipLayer.h"
#include "NounHelper.h"
#include "CCColorUtil.h"
#include "TextHelper.h"
#include "weaponlib/WeaponLibControl.h"
#include "bag/BagControl.h"
#include "bag/EquipInfoPanel.h"
#include "tip/TipControl.h"


FragPanel::FragPanel()
:m_uMyCellCount(0)
,m_pTableView(NULL)
{
    m_pTabBtnWeapoon = NULL;
    m_pTabBtnEquip = NULL;
    m_iNowTabID = TAB_WEAPON;
    m_uCfgCellCount = 0;

}

FragPanel::~FragPanel()
{
}

FragPanel* FragPanel::create() {
	FragPanel *pRet = new FragPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool FragPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);
        setShowMoneyTopBar(true);

		initLayoutFromJsonFile("ui_v2/frag_panel_ui.ExportJson");
		addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));

        m_pSubPanelFrame = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("frame_2"));
        m_pSubCloseBtnImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("sub_close_btn_img"));
        m_pSubCloseBtnImg->setTouchEnabled(true);

        m_pTabBtnWeapoon = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_weapon"));
        m_pTabBtnEquip   = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_btn_equip"));

        m_pTabBtnWeapoon->addTouchEventListener(this, toucheventselector(FragPanel::touchEventHandler));
        m_pTabBtnEquip->addTouchEventListener(this, toucheventselector(FragPanel::touchEventHandler));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(FragPanel::touchEventHandler));
        m_pSubCloseBtnImg->addTouchEventListener(this, toucheventselector(FragPanel::touchEventHandler));

        m_pBgInfo = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_info"));

		m_pFromNameTxt = dynamic_cast<UILabel*>(m_pBgInfo->getChildByName("from_name_txt"));
        m_pFromCellBg = dynamic_cast<UIImageView*>(m_pBgInfo->getChildByName("from_cell"));
        m_pFromCell = IconCell::create();
        m_pFromCell->showNumTxt();
        m_pFromCellBg->addChild(m_pFromCell);
		m_pFromCellBg->setTouchEnabled(true);
		m_pFromCellBg->addTouchEventListener(this,toucheventselector(FragPanel::touchEventHandler));

        m_pFragImg = UIImageView::create();
        m_pFragImg->loadTexture("other/icon_frag.png", UI_TEX_TYPE_PLIST);
        m_pFragImg->setVisible(false);
        m_pFromCellBg->addChild(m_pFragImg, 1);
        
        m_pToNameTxt = dynamic_cast<UILabel*>(m_pBgInfo->getChildByName("to_name_txt"));
        m_pToCellBg = dynamic_cast<UIImageView*>(m_pBgInfo->getChildByName("to_cell"));
        m_pToCell = IconCell::create();
        m_pToCellBg->addChild(m_pToCell);
        m_pToCellBg->setTouchEnabled(true);
        m_pToCellBg->addTouchEventListener(this, toucheventselector(FragPanel::touchEventHandler));
        
        m_pCoinTxt = dynamic_cast<UILabel*>(m_pBgInfo->getChildByName("cost_num_txt"));
        m_pDoMixBtn = dynamic_cast<UIButton*>(m_pBgInfo->getChildByName("do_btn"));
        m_pDoMixBtn->addTouchEventListener(this, toucheventselector(FragPanel::touchEventHandler));

		m_pTableView = CCTableView::create(this, CCSizeMake(400, 450));
		m_pTableView->setDirection(kCCScrollViewDirectionVertical);
		m_pTableView->setPosition(ccp(17, 20));
		m_pTableView->setDelegate(this);
		m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

        UIImageView* pBgList = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_list"));
		pBgList->addNode(m_pTableView);

        resetUI();

		bRet = true;
	} while(0);

	return bRet;
}

void FragPanel::showTab(UIButton* m_pBtn) {
    int iShowThingType = 0;
    int iMaxNum;
    if(m_pTabBtnWeapoon == m_pBtn) {
        m_pTabBtnWeapoon->setTitleColor(ccc3(0xff, 0xfa, 0xae) ); //按下
        m_pTabBtnWeapoon->setTitleFontSize(24);
        m_pTabBtnWeapoon->setBright(false);

        m_pTabBtnEquip->setTitleColor(ccc3(0x7b, 0xbc, 0xe1) );
        m_pTabBtnEquip->setTitleFontSize(22);
        m_pTabBtnEquip->setBright(true);

        m_iNowTabID = TAB_WEAPON;
        iShowThingType = vmsg::THING_TYPE_WEAPON;
        iMaxNum = FragListItem::WEAPON_CELL_TYPE_NUM;
    } else if(m_pTabBtnEquip == m_pBtn) {
        m_pTabBtnWeapoon->setTitleColor(ccc3(0x7b, 0xbc, 0xe1)  );
        m_pTabBtnWeapoon->setTitleFontSize(22);
        m_pTabBtnWeapoon->setBright(true);

        m_pTabBtnEquip->setTitleColor(ccc3(0xff, 0xfa, 0xae));  //按下
        m_pTabBtnEquip->setTitleFontSize(24);
        m_pTabBtnEquip->setBright(false);

        m_iNowTabID = TAB_EQUIP;
        iShowThingType = vmsg::THING_TYPE_EQUIP;
        iMaxNum = FragListItem::EQUIP_CELL_TYPE_NUM;
    } else {
        return;
    }
    //我拥有的
    const vector<int>* pstTypeIdxList = P_FRAG_CTRL->getTypeIndexList(iShowThingType);
    if(pstTypeIdxList) {
        m_uMyCellCount = ceil(float(pstTypeIdxList->size()) / (1.0f * iMaxNum) ) ;
    } else {
        m_uMyCellCount = 0;
    }

    //配置中的
    const vector<int>* pstCfgTypeList = P_FRAG_CTRL->getTypeCfgList(iShowThingType);
    if(pstCfgTypeList) {
        m_uCfgCellCount = ceil(float(pstCfgTypeList->size()) / (1.0f * iMaxNum) ) ;
    } else {
        m_uCfgCellCount = 0;
    }


    m_pTableView->reloadData();

    if(m_iNowSelCfgID <= 0) {
        //没有选择时，默认选第一个吧
        if(pstTypeIdxList && pstTypeIdxList->size() > 0) {
            onSelBagIdx( (*pstTypeIdxList)[0] );
        }
    }
}


void FragPanel::updateUI() {
    if(! this->isOpen()) {
        return;
    }
    this->closeLoading();

    switch(m_iNowTabID) {
    case TAB_WEAPON:
        showTab(m_pTabBtnWeapoon);
        break;
    case TAB_EQUIP:
        showTab(m_pTabBtnEquip);
        break;
    case TAB_AUTO_SEL:
        const vector<int>* pstWeaponTypeList = P_FRAG_CTRL->getTypeIndexList(vmsg::THING_TYPE_WEAPON);
        const vector<int>* pstEquipTypeList = P_FRAG_CTRL->getTypeIndexList(vmsg::THING_TYPE_EQUIP);
        if(pstWeaponTypeList && pstWeaponTypeList->size() > 0){
            showTab(m_pTabBtnWeapoon);
        } else if(pstEquipTypeList && pstEquipTypeList->size() > 0) {
            showTab(m_pTabBtnEquip);
        } else {
            showTab(m_pTabBtnWeapoon);
        }

        break;
    }

    //update sel info
    if(m_iNowSelCfgID <= 0) {
        
        return;
    }
    int idx = P_FRAG_CTRL->getBagIdxByCfgID(m_iNowSelCfgID);
	idx = (idx >= 0) ? idx : 0;
    onSelBagIdx(idx);
}

CCSize FragPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 110);
}

unsigned int FragPanel::numberOfCellsInTableView(CCTableView *table){
	return (m_uMyCellCount + m_uCfgCellCount);
}

CCTableViewCell* FragPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
    
	FragListItem* pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = FragListItem::create();

		cell->addChild(pItem, 0, FRAG_LIST_ITEM_TAG);
	}else{
		pItem = dynamic_cast<FragListItem*>(cell->getChildByTag(FRAG_LIST_ITEM_TAG));
	}

    //因为每行有多个
    unsigned int iArrayIdx = 0;
    int iShowThingType = 0;
    int iMaxNum = 0;
    if(TAB_WEAPON == m_iNowTabID) {
        iMaxNum = FragListItem::WEAPON_CELL_TYPE_NUM;
        iShowThingType = vmsg::THING_TYPE_WEAPON;
    } else {
        iMaxNum = FragListItem::EQUIP_CELL_TYPE_NUM;
        iShowThingType = vmsg::THING_TYPE_EQUIP;
    }

    iArrayIdx = idx * iMaxNum;
    pItem->setCellTypeNum(iMaxNum);

    
    //我拥有的
    const vector<int>* pstTypeIdxList = P_FRAG_CTRL->getTypeIndexList(iShowThingType);
    if(pstTypeIdxList && iArrayIdx < pstTypeIdxList->size()) {
        //显示我拥有的
        unsigned int iDataIdx = -1;
        bool bIsLastListCell = false;
        for(int i = 0; i < iMaxNum; ++i) {
            iDataIdx = iArrayIdx + i;
            if(iDataIdx >= pstTypeIdxList->size()) {
                pItem->resetCellByIdx(i);
                bIsLastListCell = true;
                continue;
            }
            pItem->setInfoByIdx(i, (*pstTypeIdxList)[iDataIdx]);
        }
        pItem->setNotOwnTip(false);
    } else {

        //我没有的
        pItem->setNotOwnTip(false);
        const vector<int>* pstTypeCfgList = P_FRAG_CTRL->getTypeCfgList(iShowThingType);
        if(NULL == pstTypeCfgList) {
            return cell;
        }
        int iTmpIdx = (idx - m_uMyCellCount) * iMaxNum;
        pItem->setNotOwnTip((iTmpIdx == 0));
        unsigned int iDataIdx = -1;
        for(int i = 0; i < iMaxNum; ++i) {
            iDataIdx = iTmpIdx + i;
            if(iDataIdx >= pstTypeCfgList->size()) {
                pItem->resetCellByIdx(i);
                continue;
            }
            pItem->setInfoByCfgID(i, (*pstTypeCfgList)[iDataIdx]); //cfg ID
        }
    }
    
	return cell;
}

void FragPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    FragListItem* curItem = dynamic_cast<FragListItem*>(cell->getChildByTag(FRAG_LIST_ITEM_TAG));
    if(curItem) {
        curItem->onTouched(m_stGlobalTouchPt);

        setSubPanelVisible(true);
    }
    
}

void FragPanel::open(){
    BasePanel::open();
    m_iNowTabID = TAB_AUTO_SEL;
    P_FRAG_CTRL->getProxy()->sendQueryDebrisBagRqst();
    showLoading();
}

void FragPanel::close(){
    BasePanel::close();
    m_iNowSelCfgID = 0;
    m_iNowCost = 0;
    resetUI();
}

void FragPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    string nameStr = string(pszWidgetName);

    if(TOUCH_EVENT_ENDED != type) {
        return;
    }
    if("tab_btn_weapon" == nameStr) {
        showTab(m_pTabBtnWeapoon);
    } else if("tab_btn_equip" == nameStr) {
        showTab(m_pTabBtnEquip);
    } else if("do_btn" == nameStr) {
        if(m_iNowSelCfgID <= 0) {
            return;
        }
        if(!m_pSubPanelFrame->isVisible()) {
            return;
        }

        if(!m_bIsEnough) {
            POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(WEAPON_NO_ENOUGH));
            return;
        }

        //联邦币不足
        if(!P_TIP_CTRL->CoinCostTip(m_iNowCost, this)){
            return;
        }
		m_pDoMixBtn->setTouchEnabled(false);
        P_FRAG_CTRL->sendDebrisCompose(m_iNowSelCfgID);// getProxy()->sendDebrisComposeRqst(m_iNowSelCfgID);
    } else if("to_cell" == nameStr) {
        if(m_iNowSelCfgID <= 0) {
            return;
        }
        if(!m_pSubPanelFrame->isVisible()) {
            return;
        }
        if(vmsg::THING_TYPE_WEAPON == m_pToCell->getItemType()) {
            P_WEAPON_CTRL->getWeaponInfoPanel()->open();
            P_WEAPON_CTRL->getWeaponInfoPanel()->setForseClose();
            P_WEAPON_CTRL->getWeaponInfoPanel()->updateWeaponInfo(m_pToCell->getItemCfgid(), 0);
        } else if(vmsg::THING_TYPE_EQUIP == m_pToCell->getItemType()) {
            P_BAG_CTRL->getEquipInfoPanel()->open();
            P_BAG_CTRL->getEquipInfoPanel()->updateSimpleEquipInfo(m_pToCell->getItemCfgid());
        }
        
    } else if("close_btn" == nameStr) {
        close();
    } else if("sub_close_btn_img" == nameStr) {
        close();
    }else if ("from_cell" == nameStr)
    {
		int pThingType = m_iNowTabID==TAB_WEAPON?THING_TYPE_WEAPON:THING_TYPE_EQUIP;
		P_TIP_CTRL->showItemTipPanel(vmsg::THING_TYPE_DEBRIS,m_iNowSelCfgID,0);
    }

}


void FragPanel::onSelBagIdx(const int iBagIdx) {
    if(!isOpen()) {
        return;
    }
    const CSDebrisInfo* pstInfo = P_FRAG_CTRL->getOneFragByIdx(iBagIdx);
    if(NULL == pstInfo) {
        return;
    }
    showFragInfo(pstInfo->debriscfgid(), pstInfo->debriscnt(), true);
}


void FragPanel::showFragInfo(const int iCfgID, const int iMyNum, const bool bShowMixBtn) {
    const DebrisCfg* pstCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(iCfgID);
    if(NULL == pstCfg) {
        return;
    }

    //set from info
    if(vmsg::THING_TYPE_WEAPON == pstCfg->composetype()) {
        m_pFromCellBg->setSize(CCSize(180, 100));
        m_pFromCellBg->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
        m_pToCellBg->setSize(CCSize(180, 100));
        m_pToCellBg->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
        m_pFragImg->setPosition(ccp(-65, -25));
        m_pFromCell->getNumTxt()->setAnchorPoint(ccp(1.0f, 0.5f));
    } else {
        m_pFromCellBg->setSize(CCSize(100, 100));
        m_pFromCellBg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
        m_pToCellBg->setSize(CCSize(100, 100));
        m_pToCellBg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
        m_pFragImg->setPosition(ccp(-25, -25));
        m_pFromCell->getNumTxt()->setAnchorPoint(ccp(0.5f, 0.5f));
    }

    m_pFragImg->setVisible(true);

    m_pFromCell->setInfo(vmsg::THING_TYPE_DEBRIS//pstCfg->composetype()
		, 1, pstCfg->debrisid() );
    m_pFromNameTxt->setText(pstCfg->debrisname());

    char txt[30] = {0};
    snprintf(txt, 30, "%d/%d", iMyNum, pstCfg->composeneedcnt());
    m_pFromCell->getNumTxt()->setText(txt);
    m_bIsEnough = (iMyNum >= (int)pstCfg->composeneedcnt());
    if(m_bIsEnough) {
        CCColorUtil::setLabelColor(m_pFromCell->getNumTxt(), ccGREEN);
    } else {
        CCColorUtil::setLabelColor(m_pFromCell->getNumTxt(), ccRED);
    }

    //set to info 
    m_pToCell->setInfo(pstCfg->composetype(), 0, pstCfg->composecfgid() );
    const string strName = ItemUtils::getItemName(pstCfg->composetype(), pstCfg->composecfgid());
    m_pToNameTxt->setText(strName);

    //set cost
    if(P_USER_OBJ->checkCoin(pstCfg->costcoin()) == 0 ) {
        //enough
        CCColorUtil::setLabelColor(m_pCoinTxt, ccGREEN);
    } else {
        CCColorUtil::setLabelColor(m_pCoinTxt, ccRED);
    }

    m_pCoinTxt->setText(intToString( pstCfg->costcoin() ));

    m_iNowSelCfgID = iCfgID;
    m_iNowCost = pstCfg->costcoin();


    m_pDoMixBtn->setTouchEnabled(bShowMixBtn);
    m_pDoMixBtn->setVisible(bShowMixBtn);
}

void FragPanel::resetUI() {
    m_pFromNameTxt->setText("");
    m_pFromCell->clearCell();
    m_pFragImg->setVisible(false);

    m_pToNameTxt->setText("");
    m_pToCell->clearCell();
    m_pCoinTxt->setText("0");
    m_bIsEnough = false;

    setSubPanelVisible(false);
}


void FragPanel::setSubPanelVisible(bool bIsVisible) {

    m_pBgInfo->setVisible(bIsVisible);
    m_pSubPanelFrame->setVisible(bIsVisible);
    m_pSubCloseBtnImg->setVisible(bIsVisible);
}



void FragPanel::onSelCfgID(const int iCfgID) {
    if(!isOpen()) {
        return;
    }
    showFragInfo(iCfgID, 0, false);
}
