#include "WeaponSelMatPanel.h"
#include "bag/BagObj.h"
#include "ClientSvrMsg.pb.h"
#include "TextHelper.h"
#include "CommonDialog.h"
#include "NounHelper.h"
#include "bag/BagControl.h"
#include "WeaponSelMatItem.h"
#include "firearms/FireArmsObj.h"
#include "WeaponLibControl.h"
#include "firearms/FireArmsObj.h"

using namespace CVLib;
#define MATERIAL_ITEM_TAG 101

WeaponSelMatPanel::WeaponSelMatPanel()
:m_pTableView(NULL)
,m_uCellCount(0)
,m_pMaterialDemo(NULL)
{
}

WeaponSelMatPanel::~WeaponSelMatPanel(){
	CC_SAFE_RELEASE_NULL(m_pMaterialDemo);
}

bool WeaponSelMatPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/weapon_sel_mat_panel.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

CCNode* WeaponSelMatPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "first_material_item") {
		if(numberOfCellsInTableView(m_pTableView) > 0) {
			CCTableViewCell *pCell = m_pTableView->cellAtIndex(0);
			WeaponSelMatItem *pItem = dynamic_cast<WeaponSelMatItem*>(pCell->getChildByTag(MATERIAL_ITEM_TAG));
			pRet = pItem;
		}
	} else if(name == "ok_btn") {
		if(m_pSureBtn != NULL && m_pSureBtn->isVisible() && m_pSureBtn->isTouchEnabled()) {
			pRet = m_pSureBtn;
		}
	}

	return pRet;
}

void WeaponSelMatPanel::initUI(){
	m_pSureBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("ok_btn"));
    m_pCloseBtn = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("close_btn"));
    m_pCloseBtn->setTouchEnabled(true);

	WeaponSelMatItem* pMaterial = WeaponSelMatItem::create();
	m_pMaterialDemo = pMaterial->getRootLayout();
	m_pMaterialDemo->retain();

    UIImageView* pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg")); 
	m_pTableView = CCTableView::create(this, CCSizeMake(690, 410));
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setAnchorPoint(CCPointZero);
    m_pTableView->setAnchorPoint(ccp(0, 0));
    CCPoint pt; // = pBgImg->getPosition();
    pt.x += 20;
    pt.y += 30;
	m_pTableView->setPosition(pt);
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	pBgImg->addNode(m_pTableView);

    m_pCloseBtn->addTouchEventListener(this, toucheventselector(WeaponSelMatPanel::touchEventHandler));
    m_pSureBtn->addTouchEventListener(this, toucheventselector(WeaponSelMatPanel::touchEventHandler));
}

CCSize WeaponSelMatPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 135);
}

unsigned int WeaponSelMatPanel::numberOfCellsInTableView(CCTableView *table){
	return m_uCellCount;
}

CCTableViewCell* WeaponSelMatPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	WeaponSelMatItem* pItemCell = NULL;

	bool isMaterial = true;
	if(idx < stIDX2CfgID.size()){
		isMaterial = true;
	}else{
		isMaterial = false;
	}

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();
		pItemCell = WeaponSelMatItem::create(m_pMaterialDemo);
		cell->addChild(pItemCell, 0, MATERIAL_ITEM_TAG);
	}else{
		pItemCell = dynamic_cast<WeaponSelMatItem*>(cell->getChildByTag(MATERIAL_ITEM_TAG));
	}

	UIN_t uID = 0;
	if(isMaterial){
		pItemCell->setType(vmsg::THING_TYPE_ITEM);
		uID = stIDX2CfgID.at(idx);
		pItemCell->setItemCfgID(uID);
		pItemCell->setIsEnable(true);
	}else{
		pItemCell->setType(vmsg::THING_TYPE_WEAPON);
		const vmsg::CSWeapon& stWeapon = stMaterialWeapon.at(idx - stIDX2CfgID.size());
		uID = stWeapon.weaponguid();
		pItemCell->setWeaponInfo(stWeapon);

		if(pItemCell->getWeaponInfo().weaponguid() == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid()){
			pItemCell->setIsEnable(false);
		}else{
			pItemCell->setIsEnable(true);
		}
	}

	if(P_ARM_OBJ->isInUpMaterial(idx)){
		pItemCell->setSelected(true);
	}else{
		pItemCell->setSelected(false);
	}

	if( !P_ARM_OBJ->canAddUpMat(pItemCell->getType()) && pItemCell->getSelected() == false && pItemCell->getIsEnable() == true){
		pItemCell->setIsEnable(false);
	}

	return cell;
}

void WeaponSelMatPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	unsigned int uIDX = cell->getIdx();
	unsigned int startIdx = uIDX < 3?0:uIDX - 3;
	unsigned int updateSize = (m_uCellCount - startIdx - 1) < 7?(m_uCellCount - startIdx):7;
	WeaponSelMatItem* pItem = dynamic_cast<WeaponSelMatItem*>(cell->getChildByTag(MATERIAL_ITEM_TAG));
	if(NULL == pItem){
		return;
	}
    
    if(P_GUIDE_CTRL->isGuideDuring() && !P_GUIDE_CTRL->isGuideNode(pItem)) {
        CCLOG("[warn]WeaponSelMatPanel,in guide,can't click other item,%d", P_GUIDE_CTRL->isGuideNode(pItem));
        return;
    }

    if(pItem->getSelected()){
        pItem->setSelected(false);

        UNIT64_t m_uID;
        if(pItem->getType() == vmsg::THING_TYPE_ITEM){
            m_uID = pItem->getItemCfgID();
        }else if(pItem->getType() == vmsg::THING_TYPE_WEAPON){
            m_uID = pItem->getWeaponInfo().weaponguid();
        }

        P_ARM_OBJ->removeUpMaterial(m_uID, uIDX);

        for(unsigned int i = startIdx; i < startIdx+ updateSize; i++){
            m_pTableView->updateCellAtIndex(i);
        }

    } else {
        GUIDE_TOUCH(pItem);
        const int iNowSelSize = getSelectedNum();
        if(iNowSelSize >= CFireArmsObj::LV_UP_MAT_MAX_SIZE || pItem->getIsEnable() == false){
            return;
        }
        //如果在新手指引，只可以点击第一个 vmsg::THING_TYPE_WEAPON
        CCTableViewCell *pFirstCell = m_pTableView->cellAtIndex(0);
        if(P_GUIDE_CTRL->isGuideDuring() && pFirstCell != cell ) {
            CCLOG("[warn]WeaponSelMatPanel::tableCellTouched,during guide,can't click other item!");
            return;
        }
        //判断是否有足够的位置
        if( !(P_ARM_OBJ->canAddUpMat(pItem->getType()) ) ) {
            CCLOG("[warn]WeaponSelMatPanel::tableCellTouched,not have enough space! now select size %d,type %d", iNowSelSize, pItem->getType() );
            return; 
        }

        pItem->setSelected(true);

        if(pItem->getType() == vmsg::THING_TYPE_ITEM){
            P_ARM_OBJ->addUpMaterial(pItem->getItemCfgID(), vmsg::THING_TYPE_ITEM, uIDX);
        }else if(pItem->getType() == vmsg::THING_TYPE_WEAPON){
            P_ARM_OBJ->addUpMaterial(pItem->getWeaponInfo().weaponguid(), vmsg::THING_TYPE_WEAPON, uIDX);
        }

        GUIDE_SHOW(GUIDE_UI_WEAPON_MATERIAL);

        //将其他的未选中置为不可选
        for(unsigned int i = startIdx; i < startIdx + updateSize; i++){
            m_pTableView->updateCellAtIndex(i);
        }
    }
}

void WeaponSelMatPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	//材料
	stAllItemType.clear();
	stIDX2CfgID.clear();
	stMaterialWeapon.clear();
	//m_stSelectArr.Clear();
	const vector<vmsg::CSBagCellInfo>& m_stMaterials = P_ARM_OBJ->getLvUpMaterials();

	unsigned int totalNum = 0;	//总原料数，拆分开的
	unsigned int bagItemNum = 0;		//背包中材料总数
	map<unsigned int, unsigned int>::iterator iter;
	for(unsigned int i = 0; i < m_stMaterials.size(); i++){
		const vmsg::CSBagCellInfo& stBagInfo = m_stMaterials[i]; 

		for(unsigned int j = 0; j < stBagInfo.iteminfo().itemcnt(); j++){
			iter = stIDX2CfgID.find(totalNum + j);
			if(iter == stIDX2CfgID.end()){
				stIDX2CfgID[totalNum + j] = stBagInfo.iteminfo().itemcfgid();
				stAllItemType.push_back(1);
			}
		}
		totalNum += stBagInfo.iteminfo().itemcnt();
	}

	bagItemNum = totalNum;

	//枪械做材料
	const vector<vmsg::CSWeapon>& stAllWeapon = P_ARM_OBJ->getAllWeaponBag();
	const vmsg::CSWeapon& stUpWeapon = P_ARM_OBJ->getUpWeapon();
	for(unsigned int i = 0; i < stAllWeapon.size(); i++){
		if(stUpWeapon.weaponguid() == stAllWeapon.at(i).weaponguid()){
			continue;
		}

		//自己身上的枪械
		if(stAllWeapon.at(i).weaponguid() == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid()){
			continue;
		}

		//武器不处于空闲状态
		if(stAllWeapon.at(i).state() != vmsg::WEAPON_IDLE){			
			continue;
		}

		stMaterialWeapon.push_back(stAllWeapon.at(i));
		stAllItemType.push_back(2);
	}
	totalNum += stMaterialWeapon.size();

	m_uCellCount = totalNum;
	m_pTableView->reloadData();

	GUIDE_SHOW(GUIDE_UI_WEAPON_MATERIAL);
}

void WeaponSelMatPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "ok_btn") == 0){
			GUIDE_TOUCH(pWidget);
			sureEvent();
		} else if(strcmp(pszWidgetName, "close_btn") == 0) {
            GUIDE_TOUCH(pWidget);
            sureEvent();
        }
		break;
	default:

		break;
	}
}

void WeaponSelMatPanel::backFun(){
	P_ARM_OBJ->resetUpMaterials();

	vmsg::CSWeaponLvUpConsume stLvUp;
	unsigned int uIdx;
	map<UINT64_t, vmsg::CSWeaponLvUpConsume>::iterator stIter;
	for(stIter = m_stUpMaterialCopy.begin(); stIter != m_stUpMaterialCopy.end(); stIter++){
		uIdx = stIter->first;
		stLvUp = stIter->second;
		P_ARM_OBJ->addUpMaterial(stLvUp.id(), stLvUp.type(), uIdx);
	}

	this->close();
}

unsigned int WeaponSelMatPanel::getSelectedNum(){
    return P_ARM_OBJ->getUpMatLogicSize();
}

bool WeaponSelMatPanel::checkIsFull(){
	if(getSelectedNum() >= CFireArmsObj::LV_UP_MAT_MAX_SIZE){
		return true;
	}

	return false;
}

void WeaponSelMatPanel::sureEvent(){
/*
	CCFiniteTimeAction *action2 = CCCallFunc::create(this, callfunc_selector(WeaponSelMatPanel::selectEventHandle));
	this->runAction(CCSequence::create(CCDelayTime::create(0.5f), action2, NULL));
    */
    selectEventHandle();
}

void WeaponSelMatPanel::selectEventHandle(){
	this->close();

    //选完后更新面板显示，neilwu
    if(P_WEAPON_CTRL->getWeaponInfoPanel()->isOpen()) {
        P_WEAPON_CTRL->getWeaponInfoPanel()->doSelMatResult();
    }
}
/*
void WeaponSelMatPanel::autoSelectEvent(){
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

	unsigned int length = 5;
	if(length > size){
		length = size;
	}

	P_ARM_OBJ->resetUpMaterials();
	for (unsigned int i = 0; i < length; i++){
		int nowIdx = (i);
		unsigned int cfgID = m_stDetailMaterials.at(i);
		P_ARM_OBJ->addUpMaterial(cfgID, vmsg::THING_TYPE_ITEM, nowIdx);
	}

	selectEventHandle();
}*/

void WeaponSelMatPanel::open(){
    P_BAG_CTRL->sendQueryBagInfoRqst();

	BasePanel::open();

	this->showLoading();

	m_stUpMaterialCopy = P_ARM_OBJ->getUpMaterials();
}

void WeaponSelMatPanel::close(){
	BasePanel::close();

	m_stUpMaterialCopy.clear();

    //neilwu mod
    stAllItemType.clear();
    stIDX2CfgID.clear();
    stMaterialWeapon.clear();
    //m_stSelectArr.Clear();
}