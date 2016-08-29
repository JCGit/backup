#include "WeaponSelPanel.h"
#include "bag/BagControl.h"
#include "bag/BagProxy.h"
#include "basic/NounHelper.h"
#include "firearms/FireArmsObj.h"
#include "WeaponSelItem.h"
#include "WeaponLibControl.h"
#include "UserObj.h"
#include "fight/FightObj.h"
#include "WeaponCfgMaster.h"

#define WEAPON_CHANGE_ITEM_TAG 101

WeaponSelPanel::WeaponSelPanel()
:m_pWeaponDemo(NULL)
,m_pTableView(NULL)
,m_uCellCount(0)
,m_uSelType(MAIN_WEAPON_TYPE)
{

}

WeaponSelPanel::~WeaponSelPanel(){
    CC_SAFE_RELEASE_NULL(m_pWeaponDemo);
}

bool WeaponSelPanel::init(){
    bool bRet = false;

    do 
    {
        CC_BREAK_IF(!BasePanel::init());

        initLayoutFromJsonFile("ui_v2/weapon_arr_choose_ui.ExportJson");
        addChild(m_pRootLayout);

		setClickOutToClose(true);

        initUI();

        bRet = true;
    } while (0);

    return bRet;
}

CCNode* WeaponSelPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "first_weapon_item") {
		if(numberOfCellsInTableView(m_pTableView) > 0) {
			CCTableViewCell *pCell = m_pTableView->cellAtIndex(0);
			WeaponSelItem *pItem = dynamic_cast<WeaponSelItem*>(pCell->getChildByTag(WEAPON_CHANGE_ITEM_TAG));
			pRet = pItem;
		}
	}

	return pRet;
}

void WeaponSelPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "close_btn") == 0) {
            close();			
        }
        break;
    default:

        break;
    }
}


void WeaponSelPanel::initUI(){
    WeaponSelItem* pWeapon = WeaponSelItem::create();
    m_pWeaponDemo = pWeapon->getRootLayout();
    m_pWeaponDemo->retain();

    m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
    m_pCloseBtn->setTouchEnabled(true);
    m_pCloseBtn->addTouchEventListener(this, toucheventselector(WeaponSelPanel::touchEventHandler));

    m_pTableView = CCTableView::create(this, CCSizeMake(497, 270));
    m_pTableView->setDirection(kCCScrollViewDirectionVertical);
    m_pTableView->setAnchorPoint(CCPointZero);
    m_pTableView->setPosition(ccp(33, 65));
    m_pTableView->setDelegate(this);
    m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pRootLayout->addNode(m_pTableView);

}

CCSize WeaponSelPanel::cellSizeForTable(CCTableView *table){
    return CCSizeMake(table->getContentSize().width, 114);
}

unsigned int WeaponSelPanel::numberOfCellsInTableView(CCTableView *table){
    return m_uCellCount;
}

CCTableViewCell* WeaponSelPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();
    WeaponSelItem *pCell = NULL;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        pCell = WeaponSelItem::create(m_pWeaponDemo);
        cell->addChild(pCell, 0, WEAPON_CHANGE_ITEM_TAG);
    }else{
        pCell = dynamic_cast<WeaponSelItem*>(cell->getChildByTag(WEAPON_CHANGE_ITEM_TAG));
    }

    pCell->setInfo(m_stSelWeaponLst.at(idx));

    return cell;
}

void WeaponSelPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    WeaponSelItem* curItem = NULL;
    curItem = dynamic_cast<WeaponSelItem*>(cell->getChildByTag(WEAPON_CHANGE_ITEM_TAG));
    if(NULL == curItem) {
        return;
    }

	GUIDE_TOUCH(curItem);
	int bagIdx = P_ARM_OBJ->getWeaponIdx(curItem->getInfo().weaponguid());
	//点击后直接换枪
	if(m_uSelType == MAIN_WEAPON_TYPE){  //换主武器
		P_WEAPON_CTRL->sendWeaponChangeRqst(bagIdx, curItem->getInfo().weaponguid());
	}else{		//换副武器
		const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon = P_ARM_OBJ->getOldWeapon();

		vmsg::CSInbattleWeaponElmtBrief stNewWeapon;
		stNewWeapon.set_weaponguid(curItem->getInfo().weaponguid());
		stNewWeapon.set_bagidx(bagIdx);
		stNewWeapon.set_arrayidx(stOldWeapon.arrayidx());

		P_WEAPON_CTRL->sendChangeWeaponInBattleRqst(stOldWeapon, stNewWeapon);
	}
    
    //然后直接关闭
    close();
}

void WeaponSelPanel::updateUI(){
    if (!isOpen()) {
        return;
    }

    if(this->getIsInLoading()){
        this->closeLoading();
    }

	const vmsg::CSWeaponInBattleRsp& stWeaponArrInfo = P_ARM_OBJ->getWeaponInBattleInfo();
    vector<vmsg::CSWeapon> m_stAllWeapon = P_ARM_OBJ->getChangeWeaponLst();
	m_stSelWeaponLst.clear();

	vmsg::CSWeapon stWeapon;
	if(m_uSelType == MAIN_WEAPON_TYPE){
		stWeapon = stWeaponArrInfo.equippedweapon();
	}else{
		stWeapon = P_ARM_OBJ->getOneWeaponByGUID(P_ARM_OBJ->getOldWeapon().weaponguid());
	}

	//当前选择的枪械类型
	unsigned int uSelWeaponType = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid())->weapontype();

	unsigned int uCfgID;
	unsigned int uType;
	vector<unsigned int> stSelTypes;
	for(int i = 0; i < stWeaponArrInfo.inbattleweapons_size(); i++){
		uCfgID = stWeaponArrInfo.inbattleweapons(i).weapon().cfgid();
		if(uCfgID == 0){
			continue;
		}

		uType = WEAPON_CFG_MASTER->GetWeaponCfgByID(uCfgID)->weapontype();
		if(uType != uSelWeaponType){   //过滤掉已经装备的枪械类型，当前选择的除外
			stSelTypes.push_back(uType);
		}
	}

	if(m_uSelType != MAIN_WEAPON_TYPE){   //如果不是选择主武器，需要加入主武器类型过滤
		uType = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeaponArrInfo.equippedweapon().cfgid())->weapontype();
		stSelTypes.push_back(uType);
	}

	bool bIsAdd = true;
	for(unsigned int i = 0; i < m_stAllWeapon.size(); i++){
		uType = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_stAllWeapon.at(i).cfgid())->weapontype();
		
		for(unsigned int idx = 0; idx < stSelTypes.size(); idx++){
			if(uType == stSelTypes.at(idx)){
				bIsAdd = false;
				break;
			}

			bIsAdd = true;
		}

		if(bIsAdd){
			m_stSelWeaponLst.push_back(m_stAllWeapon.at(i));
		}
	}

    m_uCellCount = m_stSelWeaponLst.size();

    m_pTableView->reloadData();

	GUIDE_SHOW(GUIDE_UI_WEAPON_SEL);
}

void WeaponSelPanel::open(){
    BasePanel::open();

    P_WEAPON_CTRL->sendWeaponBagRqst();

    this->showLoading();
}

void WeaponSelPanel::close(){
    BasePanel::close();

	m_uSelType = MAIN_WEAPON_TYPE;	//默认为主武器切换时的列表
}