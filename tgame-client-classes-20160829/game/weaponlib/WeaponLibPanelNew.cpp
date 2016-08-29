#include "WeaponLibPanelNew.h"
#include "defines.h"
#include "firearms/WeaponTypeItem.h"
#include "firearms/FireArmsObj.h"
#include "WeaponUtils.h"
#include "control/PanelSoundType.h"

#include "WeaponRefitAdditionCfgMaster.h"
#include "WeaponTypeCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "WeaponStarStepCfgMaster.h"
#include "SkillCfgMaster.h"
#include "WeaponLibControl.h"
#include "WeaponLibListItem.h"
#include "bag/BagControl.h"

using namespace std;

#define WEAPON_LIB_ITEM_TAG 101

CWeaponLibPanelNew::CWeaponLibPanelNew(){
	m_pTabBtnLst = NULL;
	m_pTypeSelectItem = NULL;
	m_pTableView = NULL;
	m_pWeaponDemo = NULL;
	m_pSelectCell = NULL;
	m_iSelectTypeIdx = -1;
	m_uSelectWeapon = -1;
	m_uWeaponInBag = 0;
	m_uWeaponTotalSize = 0;
	m_uTypeSize = 0;
	m_uPlayerWeaponIdx = 0;
	m_uCellCount = 0;
	m_pBackBtn = NULL;
}

CWeaponLibPanelNew::~CWeaponLibPanelNew(){
	CC_SAFE_RELEASE_NULL(m_pTabBtnLst);
	CC_SAFE_RELEASE_NULL(m_pWeaponDemo);
}

bool CWeaponLibPanelNew::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);

        initLayoutFromJsonFile("ui_v2/weapon_lib_panel_new.ExportJson");
        addChild(m_pRootLayout);

		m_uSoundType = WEAPON_PANEL_TYPE;

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CWeaponLibPanelNew::initUI(){

	m_pBtnList = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("type_btn_list"));
	m_pBtnList->setItemsMargin(7);

    UIImageView* pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("list_bg2")); //list_bg2

	m_pTableView = CCTableView::create(this, CCSizeMake(665, 475));
	m_pTableView->setTouchPriority(96);
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
    m_pTableView->setContentOffset(ccp(0, 0));
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(ccp(33, 5));
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	pBgImg->addNode(m_pTableView);


    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_tp/firearm_panel_ui.plist");
    CWeaponLibListItem* pItem = CWeaponLibListItem::create();
    m_pWeaponDemo = pItem->getRootLayout();
    m_pWeaponDemo->retain();


	m_pTabBtnLst = CCArray::create();
	m_pTabBtnLst->retain();
	m_uTypeSize = WEAPON_TYPE_CFG->GetCfg().weapontypecfg_size();
	CWeaponTypeItem* pBtn;
    string btnName;
    for(int i = 0; i < m_uTypeSize; ++i) {
        const WeaponTypeCfg& oneCfg = WEAPON_TYPE_CFG->GetCfg().weapontypecfg(i);
        pBtn = CWeaponTypeItem::create();
        btnName = "btn_" + intToString(i);
        pBtn->setTag(oneCfg.type()); //类型设置为tag
        pBtn->setName(btnName.c_str());
        pBtn->setTxt( oneCfg.desc() );
        //pBtn->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pBtnList->pushBackCustomItem(pBtn);
        m_pTabBtnLst->addObject(pBtn);

    }
	m_pBtnList->addEventListenerListView(this, listvieweventselector(CWeaponLibPanelNew::typeListViewEventHandler));


    UIButton* pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
    pCloseBtn->addTouchEventListener(this, toucheventselector(CWeaponLibPanelNew::touchEventHandler));
	m_pBackBtn = pCloseBtn;
}

CCSize CWeaponLibPanelNew::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 158);
}

unsigned int CWeaponLibPanelNew::numberOfCellsInTableView(CCTableView *table){
	return m_uCellCount;
}

CCTableViewCell* CWeaponLibPanelNew::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	CWeaponLibListItem *pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = CWeaponLibListItem::create(m_pWeaponDemo);
		cell->addChild(pItem, 0, WEAPON_LIB_ITEM_TAG);
	}else{
		pItem = dynamic_cast<CWeaponLibListItem*>(cell->getChildByTag(WEAPON_LIB_ITEM_TAG));
	}
    
	unsigned int type = 2; //m_iSelectTypeIdx + 1;
    CWeaponTypeItem* pTypeBtn = dynamic_cast<CWeaponTypeItem*>(m_pTabBtnLst->objectAtIndex(m_iSelectTypeIdx));
    if(pTypeBtn){
        type = pTypeBtn->getTag();
    }
	//获取去除背包中的配置中所有的枪械
	vector<int> stWeapons = WEAPON_CFG_MASTER->getWeaponsByType(type);
	if(stWeapons.size() <= 0){
		return cell;
	}

    //背包中该类型的所有枪械
    vector<vmsg::CSWeapon> stAllWeaponInBag;
    P_ARM_OBJ->getWeaponsInBagByType(type, stAllWeaponInBag); //我拥有的

	P_ARM_OBJ->getWeaponsInCfg(stWeapons, type, stAllWeaponInBag);

	m_uWeaponTotalSize = stAllWeaponInBag.size() + stWeapons.size();
	m_uWeaponInBag = stAllWeaponInBag.size();


    unsigned int iArrayIdx = idx * 2;

    if(iArrayIdx < m_uWeaponInBag) {
        //显示我背包里的
        pItem->setWeaponInfo(0, stAllWeaponInBag.at(iArrayIdx).cfgid(), 
            stAllWeaponInBag.at(iArrayIdx).weaponguid(), stAllWeaponInBag.at(iArrayIdx).level() );
        pItem->setShowNowOwnTip(false);

        iArrayIdx++;
        if(iArrayIdx < stAllWeaponInBag.size()) {
            pItem->setWeaponInfo(1, stAllWeaponInBag.at(iArrayIdx).cfgid(), 
                stAllWeaponInBag.at(iArrayIdx).weaponguid(), stAllWeaponInBag.at(iArrayIdx).level());
        } else {
            pItem->hideSecondItem();
        }
    } else {
        //显示配置里的
        const int iDeltaNum = m_uWeaponInBag % 2; //需要根据背包显示情况，做idx纠正
        unsigned int iTmpIdx = iArrayIdx - m_uWeaponInBag  - iDeltaNum;
        if(iTmpIdx < stWeapons.size()) {
            const bool bNeedShowNowOwnTip = (iTmpIdx == 0);
            pItem->setWeaponInfo(0, stWeapons.at(iTmpIdx), 0, 1); //配置里的枪械，默认ID为0，1级吧
            pItem->setShowNowOwnTip(bNeedShowNowOwnTip);
            
            iArrayIdx++;
            iTmpIdx = iArrayIdx - m_uWeaponInBag  - iDeltaNum;
            if(iTmpIdx < stWeapons.size()) {
                pItem->setWeaponInfo(1, stWeapons.at(iTmpIdx), 0, 1);
            } else {
                pItem->hideSecondItem();
            }
        }
        
    }

	return cell;
}

void CWeaponLibPanelNew::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {

    CCTableViewCell *pFirstCell = m_pTableView->cellAtIndex(0);
    if(P_GUIDE_CTRL->isGuideDuring() && pFirstCell != cell ) {
        CCLOG("[warn]CWeaponLibPanelNew::tableCellTouched,during guide,can't click other item!");
        return;
    }

    unsigned int preSelect = m_uSelectWeapon;
    CWeaponLibListItem* curItem = NULL;
    curItem = dynamic_cast<CWeaponLibListItem*>(cell->getChildByTag(WEAPON_LIB_ITEM_TAG));
    if(NULL == curItem) {
        return;
    }
    UILayout* pFirstRefLayout = dynamic_cast<UILayout*>(curItem->getFirstItemNode());
    if(P_GUIDE_CTRL->isGuideDuring() && pFirstCell == cell && pFirstRefLayout && !(pFirstRefLayout->hitTest(m_stGlobalTouchPt)) ) {
        CCLOG("[warn]CWeaponLibPanelNew::tableCellTouched 2,during guide,can't click other item!");
        return;
    }

    curItem->onItemTouched(m_stGlobalTouchPt);

}

void CWeaponLibPanelNew::updateUI(){
    if(!isOpen()) {
        return;
    }
	if(this->getIsInLoading()){
		this->closeLoading();
	}

    //拥有的枪械tab btn排列在前面
    const vector<vmsg::CSWeapon>& stAllWeaponBag = P_ARM_OBJ->getAllWeaponBag();
    unsigned int cfgID;
    int iType = 0;
    map<int, int> stNumMap; //<Type, Num>
    for(unsigned int i = 0; i < stAllWeaponBag.size(); i++){
        cfgID = stAllWeaponBag.at(i).cfgid();
        iType = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID)->weapontype();
        if(iType <= 0){
            CCLOG("[warn]CWeaponLibPanelNew::updateUI,invalid weapon type %d,cfgid %d", iType, cfgID);
            continue;
        }
        if(stNumMap.count(iType) <= 0 ) {
            stNumMap[iType] = 0;
        }
        stNumMap[iType] = stNumMap[iType] + 1;
    }


    CWeaponTypeItem* pBtn;
    pBtn = dynamic_cast<CWeaponTypeItem*>(m_pTabBtnLst->objectAtIndex(m_iSelectTypeIdx));
    int iPreSelType = -1;
    int iPreSelIdx = -1;
    if( pBtn ) {
        iPreSelType = pBtn->getTag(); //记录下当前选择的tab类型
    }


    unsigned int num = 0;
    int iBtnIdx = 0;
    for(auto iter = stNumMap.begin(); iter != stNumMap.end(); ++iter ) {
        pBtn = dynamic_cast<CWeaponTypeItem*>(m_pTabBtnLst->objectAtIndex(iBtnIdx));
        iType = iter->first;
        num = iter->second;
        if(iPreSelType == iType) {
            iPreSelIdx = iBtnIdx;
        }
        pBtn->setTag(iType);
        const WeaponTypeCfg* pOneCfg = WEAPON_TYPE_CFG->GetWeaponTypeCfgByID(iType);
        pBtn->setTxt(pOneCfg->desc().c_str() );
        pBtn->setNum(num);
        //CCLOG("--type=%d,num=%d,name=%s", iType, num, pOneCfg->desc().c_str());
        iBtnIdx++;
    }
    for(int i = 0; i < m_uTypeSize; ++i) {
        const WeaponTypeCfg& oneCfg = WEAPON_TYPE_CFG->GetCfg().weapontypecfg(i);
        iType = oneCfg.type();
        if(stNumMap.count(iType) <= 0) {
            pBtn = dynamic_cast<CWeaponTypeItem*>(m_pTabBtnLst->objectAtIndex(iBtnIdx));
            pBtn->setTag(iType);
            pBtn->setTxt(oneCfg.desc().c_str() );
            pBtn->setNum(0);
            //CCLOG("----type=%d,num=%d,name=%s", iType, 0, oneCfg.desc().c_str());
            if(iPreSelType == iType) {
                iPreSelIdx = iBtnIdx;
            }
            iBtnIdx++;
        }
    }
    if(iPreSelIdx >= 0) {
        setTypeIdx(iPreSelIdx);
    } else {
        setTypeIdx(0);
    }
    
	GUIDE_SHOW(GUIDE_UI_WEAPON_LIB);
}

void CWeaponLibPanelNew::setTypeIdx(unsigned int idx){

	m_iSelectTypeIdx = idx;

	CWeaponTypeItem* curItem = (CWeaponTypeItem*)m_pBtnList->getItem(m_iSelectTypeIdx);
	if(curItem != NULL){
		if(m_pTypeSelectItem != NULL){
			m_pTypeSelectItem->setSelected(false);
		}
		m_pTypeSelectItem = curItem;
		m_pTypeSelectItem->setSelected(true);
        
	}

	updateWeaponList();
}

unsigned int CWeaponLibPanelNew::getTypeIdx(){
	return m_iSelectTypeIdx;
}

unsigned int CWeaponLibPanelNew::getWeaponIdx(){
	return m_uSelectWeapon;
}

void CWeaponLibPanelNew::updateWeaponList(){
    CWeaponTypeItem* pTypeBtn = dynamic_cast<CWeaponTypeItem*>(m_pTabBtnLst->objectAtIndex(m_iSelectTypeIdx));
    if(NULL == pTypeBtn){
        return;
    }
	unsigned int type = pTypeBtn->getTag();
	//获取去除背包中的配置中所有的枪械
	vector<int> stWeapons = WEAPON_CFG_MASTER->getWeaponsByType(type);

	if(stWeapons.size() <= 0){
		return;
	}

    //背包中该类型的所有枪械
    vector<vmsg::CSWeapon> stAllWeaponInBag;
    P_ARM_OBJ->getWeaponsInBagByType(type, stAllWeaponInBag);

	P_ARM_OBJ->getWeaponsInCfg(stWeapons, type, stAllWeaponInBag);


	m_uWeaponTotalSize = stAllWeaponInBag.size() + stWeapons.size();
	m_uWeaponInBag = stAllWeaponInBag.size();

	for(unsigned int i = 0; i < m_uWeaponTotalSize; i++){
		if(i < m_uWeaponInBag){  //背包中
			//记录玩家身上枪械的索引
			if(stAllWeaponInBag.at(i).weaponguid() == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid()){
				m_uPlayerWeaponIdx = i;
			}
		}
	}
    int iListCellCnt = stAllWeaponInBag.size() / 2 + stAllWeaponInBag.size() % 2;
    iListCellCnt += stWeapons.size() / 2 + stWeapons.size() % 2;

	//m_uCellCount = m_uWeaponTotalSize;
    m_uCellCount = iListCellCnt; //neilwu mod
	m_pSelectCell = NULL;
	m_uSelectWeapon = 0;

	m_pTableView->reloadData();


}


int CWeaponLibPanelNew::getWeaponInBagInfoIdxByGUID(UINT64_t weaponGUID){
	unsigned int type = m_iSelectTypeIdx + 1;
	//背包中该类型的所有枪械
	vector<vmsg::CSWeapon> stAllWeaponInBag;
    P_ARM_OBJ->getWeaponsInBagByType(type, stAllWeaponInBag);

	int idx = -1;
	vmsg::CSWeapon stWeapon;
	for(int i = 0; i < (int)stAllWeaponInBag.size(); i++){
		stWeapon = stAllWeaponInBag.at(i);
		if(stWeapon.weaponguid() == weaponGUID){
			idx = i;
			break;
		}
	}

	return idx;
}


void CWeaponLibPanelNew::updateWeaponByChange(){
	if(m_pSelectCell == NULL){
		return;
	}

	unsigned int uIDX = m_pSelectCell->getIdx();
	unsigned int startIdx = uIDX < 3?0:uIDX - 3;
	unsigned int updateSize = (m_uCellCount - startIdx - 1) < 7?(m_uCellCount - startIdx):7;

	//更新换枪后的可见item信息
	for(unsigned int i = startIdx; i < startIdx + updateSize; i++){
		m_pTableView->updateCellAtIndex(i);
	}

	unsigned int preSelect = m_uSelectWeapon;
	m_uSelectWeapon = m_uPlayerWeaponIdx;
	m_pTableView->updateCellAtIndex(preSelect);
	m_pTableView->updateCellAtIndex(m_uSelectWeapon);

	//更新选中的枪械信息
    /*
	CWeaponLibItem* pItem = dynamic_cast<CWeaponLibItem*>(m_pSelectCell->getChildByTag(WEAPON_LIB_ITEM_TAG));
	pItem->updateUI();

	if(pItem->getWeaponGUID() == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid()){
		m_pEquipBtn->setVisible(false);
	}else{
		m_pEquipBtn->setVisible(true);
	}

	m_pEquipBtn->setTouchEnabled(true);*/
}

void CWeaponLibPanelNew::typeListViewEventHandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
	case LISTVIEW_ONSELECTEDITEM_END:
		UIListView *pListView = dynamic_cast<UIListView*>(pSender);
		int selectedIdx = pListView->getCurSelectedIndex();
		GUIDE_TOUCH(m_pBtnList->getItem(selectedIdx));
		setTypeIdx(selectedIdx);
		break;
	}
}


void CWeaponLibPanelNew::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string nameStr = string(pszWidgetName);

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			close();			
		}
		break;
	default:

		break;
	}
}

void CWeaponLibPanelNew::open(){
    BasePanel::open();

	P_WEAPON_CTRL->sendWeaponBagRqst();
    P_BAG_CTRL->sendQueryBagInfoRqst();

    this->showLoading();
}


void CWeaponLibPanelNew::close(){
    BasePanel::close();
    
	m_uSelectWeapon = -1;
	m_uWeaponInBag = 0;
	m_uWeaponTotalSize = 0;

	if(m_pTypeSelectItem){
		m_pTypeSelectItem->setSelected(false);
	}
	m_pTypeSelectItem = NULL;
	m_pSelectCell = NULL;
}

CCNode* CWeaponLibPanelNew::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "weapon_type_btn") {

	} else if(name == "weapon_cell") {
        //选中实体步枪tab，指向第一把枪,实体武器type=3,idx=2
        setTypeIdx(0);
        if(m_uWeaponInBag <= 0){
            return NULL;
        }
        CCTableViewCell* pCell = m_pTableView->cellAtIndex(0);
        if(NULL == pCell) {
            return NULL;
        }
        CWeaponLibListItem* pItem = dynamic_cast<CWeaponLibListItem*>(pCell->getChildByTag(WEAPON_LIB_ITEM_TAG));
        if(NULL == pItem) {
            return NULL;
        }
        return pItem->getFirstItemNode();

	} else if(name == "back_btn") {
		if(m_pBackBtn != NULL && m_pBackBtn->isRunning() && m_pBackBtn->isTouchEnabled()) {
			pRet = m_pBackBtn;
		}
	}else if (name == "weapon_tab_btn")
	{
		for (int i = 1; i<m_pTabBtnLst->count(); i++)
		{
			CWeaponTypeItem* pWeaponItem = dynamic_cast<CWeaponTypeItem*>(m_pTabBtnLst->objectAtIndex(i));
			if (pWeaponItem->isNumShow())
			{
				return m_pBtnList->getItem(i);
			}
		}
		
	}

	return pRet;
}
