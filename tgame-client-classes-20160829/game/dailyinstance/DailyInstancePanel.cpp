#include "DailyInstancePanel.h"
#include "DailyInstanceControl.h"
#include "DailyInstanceListItem.h"
#include "TimeEx.h"
#include "DailyInstanceCfgMaster.h"
#include "VIPCfgMaster.h"
#include "UserObj.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "ImageNumber.h"
#include "bag/BagControl.h"
#include "PackageCfgMaster.h"
#include "ToolUtils.h"
#include "DebrisCfgMaster.h"
#include "DailyInstanceCfgMaster.h"
#include "CommonDialog.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "tip/TipControl.h"
#include "manual/ManualControl.h"
#include "fight/LevelObj.h"
#include "GlobalCfgMaster.h"

DailyInstancePanel::DailyInstancePanel()
{
    m_pNowSelItem = NULL;
    m_iSelLvIdx = -1;
}

DailyInstancePanel::~DailyInstancePanel()
{
}

DailyInstancePanel* DailyInstancePanel::create() {
	DailyInstancePanel *pRet = new DailyInstancePanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DailyInstancePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());
        
		initLayoutFromJsonFile("ui_v2/DailyInstancePanel.ExportJson");
		addChild(m_pRootLayout);


        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(DailyInstancePanel::touchEventHandler));
        m_pWarBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("war_bg"));
        m_pWarBgMask = dynamic_cast<UIImageView*>(m_pWarBg->getChildByName("img_mask"));    
        m_pWarBgMask->setScale9Enabled(true);

        char buff[30];
        for(int i = 0; i < MAX_LV_NUM; ++i) {
            snprintf(buff, 30, "tab_btn_%d", i);
            m_pLvTabBtn[i] = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName(buff));
            m_pLvTabBtn[i]->setTag(i);
            m_pLvTabBtn[i]->addTouchEventListener(this, toucheventselector(DailyInstancePanel::touchEventHandler));
        }

        m_pBtnFight = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_fight"));
        m_pBtnCharge = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_charge"));
        m_pBtnFight->addTouchEventListener(this, toucheventselector(DailyInstancePanel::touchEventHandler));
        m_pBtnCharge->addTouchEventListener(this, toucheventselector(DailyInstancePanel::touchEventHandler));

        m_pTodayNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("today_num_txt"));

        m_pNowVipLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("now_vip_lv_txt"));
        m_pNextTipTxt0 = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("next_tip_txt_0"));
        m_pNextVipLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("next_vip_lv_txt"));
        m_pNextTipTxt1 = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("next_tip_txt_1"));

        //init list

        //list table view
        m_pTableView = CCTableView::create(this, CCSizeMake(330, 360));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);

        m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(13, 65));
        m_pTableView->setDelegate(this);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        UIImageView* pBgLeft = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_left"));
        pBgLeft->addNode(m_pTableView);

        m_pCoinTipImgNum = ImageNumber::create(20);
        m_pCoinTipImgNum->setAnchorPoint(ccp(0.5, 0.5f));
        m_pCoinTipImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("img_coin_tip"));
        m_pCoinTipImgNum->setPosition(m_pCoinTipImg->getPosition() + ccp(-10, 30));
        m_pCoinTipImgNum->setRotation(-15.0f);
        m_pCoinTipImgNum->setScale(1.3f);
        m_pRootLayout->addNode(m_pCoinTipImgNum);
        
        m_pFpTipImgNum = ImageNumber::create(20);
        m_pFpTipImgNum->setAnchorPoint(ccp(0.5, 0.5f));
        UIImageView* pBgFpTip = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("img_fp_tip"));
        m_pFpTipImgNum->setPosition(pBgFpTip->getPosition() + ccp(200, 30));
        m_pFpTipImgNum->setRotation(-15.0f);
        m_pFpTipImgNum->setScale(1.3f);
        m_pRootLayout->addNode(m_pFpTipImgNum); 

        m_pDebrisStarNum = ImageNumber::create(20);
        m_pDebrisStarNum->setAnchorPoint(ccp(0.5, 0.5f));
        m_pDebrisStarNum->setPosition(m_pCoinTipImg->getPosition() + ccp(30, 30));
        m_pDebrisStarNum->setRotation(0.0f);
        m_pDebrisStarNum->setScale(1.3f);
        m_pRootLayout->addNode(m_pDebrisStarNum);
        m_pDebrisStarNum->setVisible(false);

		bRet = true;
	} while(0);

	return bRet;
}

void DailyInstancePanel::open() {
	BasePanel::open();
    CToolUtils::loadArmatureRGBA4444("effect/ut00090/ut00090.ExportJson");;

    DAILY_INSTANCE_CTRL->getProxy()->sendQueryRqst();
	this->showLoading();
    //updateUI(); //test
}

void DailyInstancePanel::close() {
	if(this->getIsInLoading()){
		this->closeLoading();
	}

    BasePanel::close();
    m_pNowSelItem = NULL;
}

void DailyInstancePanel::updateUI() {
	if(this->getIsInLoading()) {
		this->closeLoading();
	}
    
    //m_stInstList
    const int iToday = TimeEx::getTime()->tm_wday; //0 标示星期天，1-6标示星期1到星期6
    DAILY_INST_CFG_MASTER->sortTodayInstance(iToday, m_stInstCfgKeyList);

    m_pTableView->reloadData();
    updateVipTipTxt();
    updateSelInsance();
}


void DailyInstancePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const std::string strWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strWidgetName == "close_btn") {
                close();
            } else if(strWidgetName == "btn_fight") {
                doStartFight(); 
            } else if(strWidgetName == "btn_charge") {
                P_SHOP_CTRL->getRmbShopPanel()->open();
            } else if(strWidgetName.find("tab_btn_") != string::npos ) {
                showLvTabByIdx(pWidget->getTag());
            }
			break;
		default:;
	}
}



CCSize DailyInstancePanel::cellSizeForTable(CCTableView *table){
    return CCSizeMake(table->getContentSize().width, 120);
}

unsigned int DailyInstancePanel::numberOfCellsInTableView(CCTableView *table){
    return m_stInstCfgKeyList.size();
}

CCTableViewCell* DailyInstancePanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();
    
    DailyInstanceListItem *pItem = NULL;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        pItem = DailyInstanceListItem::create();
        cell->addChild(pItem, 0, INSTANCE_LIST_ITEM_TAG);
    }else{
        pItem = dynamic_cast<DailyInstanceListItem*>(cell->getChildByTag(INSTANCE_LIST_ITEM_TAG));
    }
    pItem->setCfgKey(m_stInstCfgKeyList[idx]);
    if(NULL == m_pNowSelItem && idx == 0) {
        m_pNowSelItem = pItem; //默认选择第一个
        m_pNowSelItem->setSelect(true);
    }
    return cell;
}

void DailyInstancePanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    if(!this->isPanelOnTop()) {
        return;
    }
    
    DailyInstanceListItem* curItem = NULL;
    curItem = dynamic_cast<DailyInstanceListItem*>(cell->getChildByTag(INSTANCE_LIST_ITEM_TAG));
    if(NULL == curItem) {
        return;
    }
    if(curItem == m_pNowSelItem) {
        return;
    }
    if(m_pNowSelItem) {
        m_pNowSelItem->setSelect(false);
    }
    
    m_pNowSelItem = curItem;
    m_pNowSelItem->setSelect(true);
    
    updateSelInsance();
    
}

void DailyInstancePanel::updateSelInsance() {
    if(m_pNowSelItem == NULL) {
        CCLOG("[warn]DailyInstancePanel::updateSelInsance, now sel item is null!");
        return;
    }
    const int iSelCfgKey = m_pNowSelItem->getCfgKey();

    //根据当前已经通过的难度，自动选择显示对应的难度按钮
    const int iRemainChallengeCnt = DAILY_INSTANCE_CTRL->getRemainChallengeCnt(iSelCfgKey); //今天的剩余次数
    char buff[30] = {0};
    snprintf(buff, 30, "%d", iRemainChallengeCnt);
    m_pTodayNumTxt->setText(buff);
    m_pBtnFight->setEnabled( (iRemainChallengeCnt > 0) );
    m_pBtnFight->setVisible( (iRemainChallengeCnt > 0) );


    const DailyInstanceCfg* pDailyInstCfg = DAILY_INST_CFG_MASTER->getOneCfg(iSelCfgKey);
    if(NULL == pDailyInstCfg) {
        return;
    }
    //DailyInstanceControl::finalizePanel 里清除该资源
    CToolUtils::loadSpriteFrameRGBA4444("ui_bg/dailyinstancebg/" + pDailyInstCfg->bgname() + ".plist");
    m_pWarBg->loadTexture(string("dailyinstancebg/" + pDailyInstCfg->bgname()).c_str(), UI_TEX_TYPE_PLIST);
    
    int iNowVipLvNum = getNowVipFightNum(); //当前vip等级可以挑战的次数
    //根据剩余次数自动选择显示对应的难度
    int iDelta = iNowVipLvNum - iRemainChallengeCnt;
    if(iDelta < 0 || iDelta >= iNowVipLvNum) {
        iDelta = 0;
    }
    showLvTabByIdx(iDelta);
}

int DailyInstancePanel::getNowVipFightNum() {
    //获取当前VIP等级可以挑战的次数
    static int iCacheMyVipLv = 0;
    static int iCacheNum = 0;

    const int iMyVipLv = P_USER_OBJ->getUserInfo()->vipLv;
    if(iMyVipLv == iCacheMyVipLv && iCacheNum > 0) {
        return iCacheNum;
    }

    const VIPCfg* pVipCfg = VIP_CFG_MASTER->getVIPCfg(vmsg::VIP_MODULE_DAILY_INSTANCE);
    if(!pVipCfg) {
        return 0;
    }
    for(int i = 0; i < pVipCfg->vipparam_size(); ++i) {
        if(iMyVipLv == i) {
            iCacheMyVipLv = iMyVipLv;
            iCacheNum = pVipCfg->vipparam(i);
            return iCacheNum;
        }
    }
    return 0;
}

void DailyInstancePanel::showLvTabByIdx(const int idx) {
    m_iSelLvIdx = idx;
    if(idx < 0 || idx >= MAX_LV_NUM) {
        CCLOG("[warn]DailyInstancePanel::showLvTabByIdx, invalid tab idx %d!", idx);
        return;
    }
    
    for(int i = 0; i < MAX_LV_NUM; ++i) {
        m_pLvTabBtn[i]->setBright(true);
        if((idx == i)) {
            //选中
            m_pLvTabBtn[i]->setBright(false);
        }
    }
    

    const int iSelCfgKey = m_pNowSelItem->getCfgKey();
    const DailyInstanceCfg* pCfg = DAILY_INST_CFG_MASTER->getOneCfg(iSelCfgKey);
    if(!pCfg) {
        CCLOG("[warn]DailyInstancePanel::showLvTabByIdx, invalid iSelCfgKey %d!", iSelCfgKey);
        return;
    }

    char buff[50] = {0};
    //显示遮罩
    snprintf(buff, 50, "ui_bg/dailyinstancebg/mask_lv%d.png", idx);
    m_pWarBgMask->loadTexture(buff); 
    m_pWarBgMask->setSize(m_pWarBg->getSize());

    //已经打过的给灰调
    const int iRemainChallengeCnt = DAILY_INSTANCE_CTRL->getRemainChallengeCnt(iSelCfgKey);
    const int iNowVipLvNum = getNowVipFightNum(); //当前vip等级可以挑战的次数
    int iDelta = iNowVipLvNum - iRemainChallengeCnt;
    //if(iRemainChallengeCnt > 0 && (idx >= iDelta ) && (idx < iNowVipLvNum) ) {
	if(iRemainChallengeCnt > 0) {
        m_pBtnFight->setEnabled( true );
        m_pBtnFight->setVisible( true );
    } else {
        m_pBtnFight->setEnabled( false );
        m_pBtnFight->setVisible( false );
    }

    //显示推荐战力
    snprintf(buff, 50, "%d", pCfg->todayinstances(idx).fp());
    m_pFpTipImgNum->setNum(buff);

    //显示奖励信息
    const int iPkgID = pCfg->todayinstances(idx).awardpkgid();
    const PackageCfg* pPkgCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(iPkgID);
    if(NULL == pPkgCfg) {
        CCLOG("[warn]DailyInstancePanel::showLvTabByIdx, invalid iPkgID %d!", iPkgID);
        return;
    }
    if(pPkgCfg->elmtlst_size() != 1) {
        CCLOG("[warn]DailyInstancePanel::showLvTabByIdx, invalid pkg elmt size: %d!", pPkgCfg->elmtlst_size());
        return;
    }

    if(vmsg::THING_TYPE_COIN == pPkgCfg->elmtlst(0).elementtype()  ) {
        m_pCoinTipImg->loadTexture("dailyinstance/coin_tip_img.png", UI_TEX_TYPE_PLIST);
        snprintf(buff, 50, "%d", pPkgCfg->elmtlst(0).elementcntmin() );
        m_pCoinTipImgNum->setNum(buff);

        m_pDebrisStarNum->setVisible(false);
        m_pCoinTipImgNum->setPositionX(m_pCoinTipImg->getPositionX() - 10 );
        m_pCoinTipImgNum->setRotation(-15.0f);
        
    } else if(vmsg::THING_TYPE_EXP == pPkgCfg->elmtlst(0).elementtype()) {
        m_pCoinTipImg->loadTexture("dailyinstance/exp_tip_img.png", UI_TEX_TYPE_PLIST);
        snprintf(buff, 50, "%d", pPkgCfg->elmtlst(0).elementcntmin() );
        m_pCoinTipImgNum->setNum(buff);

        m_pDebrisStarNum->setVisible(false);
        m_pCoinTipImgNum->setPositionX(m_pCoinTipImg->getPositionX() - 10);
        m_pCoinTipImgNum->setRotation(-15.0f);
    } else if(vmsg::THING_TYPE_ITEM == pPkgCfg->elmtlst(0).elementtype()) {
        m_pCoinTipImg->loadTexture("dailyinstance/energy_tip_img.png", UI_TEX_TYPE_PLIST);
        snprintf(buff, 50, "%d", pPkgCfg->elmtlst(0).elementcntmin() );
        m_pCoinTipImgNum->setNum(buff);

        snprintf(buff, 50, "%d",  idx + 1 ); 
        m_pDebrisStarNum->setNum(buff);

        m_pDebrisStarNum->setVisible(true);
        m_pCoinTipImgNum->setPositionX(m_pCoinTipImg->getPositionX() - 80);
        m_pCoinTipImgNum->setRotation(0.0f);

    }
}

void DailyInstancePanel::updateVipTipTxt() {
    const VIPCfg* pVipCfg = VIP_CFG_MASTER->getVIPCfg(vmsg::VIP_MODULE_DAILY_INSTANCE);
    if(!pVipCfg) {
        CCLOG("[warn]DailyInstancePanel::updateVipTipTxt, fail to get vip cfg ");
        return;
    }
    const int iMyVipLv = P_USER_OBJ->getUserInfo()->vipLv;
    int iNowVipLvNum = 0;
    int iNextVipLv = 0;
    for(int i = 0; i < pVipCfg->vipparam_size(); ++i) {
        if(iMyVipLv == i) {
            iNowVipLvNum = pVipCfg->vipparam(i);
        }
        if(iNowVipLvNum != 0 && ((int)pVipCfg->vipparam(i) > iNowVipLvNum) ) {
            iNextVipLv = i;
            break;
        }
    }
    char buff[50] = {0};

    //now vip lv
    snprintf(buff, 50, "VIP %d", iMyVipLv);
    m_pNowVipLvTxt->setText(buff);

    if(iNextVipLv > iMyVipLv) {
        snprintf(buff, 50, "VIP %d", iNextVipLv);
        m_pNextVipLvTxt->setText(buff);

        m_pNextTipTxt0->setVisible(true);
        m_pNextVipLvTxt->setVisible(true);
        m_pNextTipTxt1->setVisible(true);
    } else {
        m_pNextTipTxt0->setVisible(false);
        m_pNextVipLvTxt->setVisible(false);
        m_pNextTipTxt1->setVisible(false);
    }
    
}

void DailyInstancePanel::doStartFight() {
    if(m_pNowSelItem == NULL) {
        CCLOG("[warn]DailyInstancePanel::doStartFight, now sel item is null!");
        return;
    }
    //当前显示的配置CfgKey
    const int iSelCfgKey = m_pNowSelItem->getCfgKey();
    //当前选择的难度index
    if(m_iSelLvIdx < 0 || m_iSelLvIdx >= MAX_LV_NUM) {
        CCLOG("[warn]DailyInstancePanel::doStartFight, invalid lv idx %d!", m_iSelLvIdx);
        return;
    }
    CCLOG("[warn]DailyInstancePanel::doStartFight, cfg key %d, lv idx %d!", iSelCfgKey, m_iSelLvIdx);
    DAILY_INSTANCE_CTRL->setNowFightInfo(iSelCfgKey, m_iSelLvIdx);
    //TODO:
    //保存iSelCfgKey和m_iSelLvIdx
    //副本通关后，发送协议时需要，DailyInstanceProxy::sendPassRqst(iCfgKey, iPassLvIdx),注意iPassLvIdx是下标

    //test
    //DAILY_INSTANCE_CTRL->getProxy()->sendPassRqst(iSelCfgKey, m_iSelLvIdx); //test

	//体力不足
	if(P_USER_OBJ->checkManual(GLOBAL_CFG_MASTER->GetCfg().dailyinstancemanualcost()) != 0){
		COMMON_DIALOG->show(TextStr(TEXT_MANUAL_BUY_TIP), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(DailyInstancePanel::buyManualTip));
		//COMMON_DIALOG->show(NounStr(LOW_MANUAL_TXT), NULL, NounStr(OK));
		return;
	}

	//背包已满
	if(P_BAG_CTRL->isFull()){
		COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP), NULL, NounStr(OK));
		return;
	}

	P_FIGHT_OBJ->setFightMapID(DAILY_INSTANCE_CTRL->getWarMapID());
	P_LEVEL_OBJ->setIsHasFriend(false);
	P_FIGHT_OBJ->setFightType(DAILY_FIGHT_TYPE);
	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();

	showLoading();
}

void DailyInstancePanel::buyManualTip(){
	P_TIP_CTRL->LowManualBuyTip(this, dialogbtneventselector(DailyInstancePanel::buyManual));
}

void DailyInstancePanel::buyManual() {
	P_MANUAL_CTRL->sendBuyManualRqst();
}

