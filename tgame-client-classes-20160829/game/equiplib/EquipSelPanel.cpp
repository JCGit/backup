#include "EquipSelPanel.h"
#include "EquipSelItem.h"
#include "UserObj.h"
#include "UserInfo.h"
#include "bag/BagControl.h"
#include "EquipmentCfgMaster.h"
#include "EquipLibControl.h"

#include <algorithm>

EquipSelPanel::EquipSelPanel()
:m_pCloseBtn(NULL)
,m_pTableView(NULL)
,m_pEquipDemo(NULL)
{
    m_pNowSelItem = NULL;
}

EquipSelPanel::~EquipSelPanel()
{
    CC_SAFE_RELEASE_NULL(m_pEquipDemo);
}

CCNode* EquipSelPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;
	
	if(name == "first_equip_item") {
		if(numberOfCellsInTableView(m_pTableView) > 0) {
			CCTableViewCell *pCell = m_pTableView->cellAtIndex(0);
			EquipSelItem *pItem = dynamic_cast<EquipSelItem*>(pCell->getChildByTag(EQUIP_SEL_ITEM_TAG));
			pRet = pItem;
		}
	}

	return pRet;
}

bool EquipSelPanel::init() 
{
	bool bRet = false;

	do 
    {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/equip_sel_panel.ExportJson");
		addChild(m_pRootLayout);
		
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(EquipSelPanel::touchEventHandler));

        UIButton* pOKBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("ok_btn"));
        pOKBtn->addTouchEventListener(this, toucheventselector(EquipSelPanel::touchEventHandler));

		EquipSelItem* pItem = EquipSelItem::create();
		m_pEquipDemo = pItem->getRootLayout();
		m_pEquipDemo->retain();

		m_pTableView = CCTableView::create(this, CCSizeMake(705, 448));
		m_pTableView->setDirection(kCCScrollViewDirectionVertical);
		m_pTableView->setAnchorPoint(CCPointZero);
		m_pTableView->setPosition(ccp(45, 97));
		m_pTableView->setDelegate(this);
		m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		m_pRootLayout->addNode(m_pTableView);

		bRet = true;
	} 
    while(0);

	return bRet;
}

CCSize EquipSelPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 112);
}

unsigned int EquipSelPanel::numberOfCellsInTableView(CCTableView *table){
	return m_stAllEquipInfos.size();
}

CCTableViewCell* EquipSelPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	EquipSelItem *pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = EquipSelItem::create(m_pEquipDemo);
		cell->addChild(pItem, 0, EQUIP_SEL_ITEM_TAG);
	}else{
		pItem = dynamic_cast<EquipSelItem*>(cell->getChildByTag(EQUIP_SEL_ITEM_TAG));
	}

	
    if(idx < m_stAllEquipInfos.size()) {

        pItem->updateEquipInfo(m_stAllEquipInfos[idx].equipInfo, 
            m_stAllEquipInfos[idx].bIsWear, 
            m_stAllEquipInfos[idx].iBagIdx);

        bool bIsSel = m_stAllEquipInfos[idx].equipInfo.equipguid() == P_EQUIP_LIB_CTRL->getObj()->getSelEquipGuid();
        if(bIsSel) {
            m_pNowSelItem = pItem;
        }
        pItem->setSelect(bIsSel);
    }

	return cell;
}

void EquipSelPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	EquipSelItem *pItem = dynamic_cast<EquipSelItem*>(cell->getChildByTag(EQUIP_SEL_ITEM_TAG));
    if(NULL == pItem) {
        return;
    }

	GUIDE_TOUCH(pItem);
    if(m_pNowSelItem) {
        m_pNowSelItem->setSelect(false);
    }
    m_pNowSelItem = pItem;
    m_pNowSelItem->setSelect(true);
    P_EQUIP_LIB_CTRL->getObj()->onSelEquip(pItem->getEquipInfo().equipguid() );
    close(); //选了就关闭
	GUIDE_SHOW(GUIDE_UI_EQUIP_LIB);
}

void EquipSelPanel::open()
{
    BasePanel::open();
    updateUI();
}


void EquipSelPanel::close()
{
    BasePanel::close();
    m_stAllEquipInfos.clear();
    m_pNowSelItem = NULL;
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->doUpdateUI(false); //关闭时更新一下
}

void EquipSelPanel::updateUI(){

	m_stAllEquipInfos.clear();
    //show role equip
    const std::vector<vmsg::CSEquipInfo>& stRoleEquipLst = P_USER_OBJ->getUserInfo()->stEquipLst;
    for (int i = 0; i < (int)stRoleEquipLst.size(); ++i) {
        addOneListItem(stRoleEquipLst[i], true, -1);
    }

    //备用防具
    const vmsg::CSEquipInfo& stSpareEquip = P_USER_OBJ->getUserInfo()->spareEquip;
    if(0 != stSpareEquip.equipguid()) {
		addOneListItem(stSpareEquip, false, -1);
    }
    
    //bag equip
    const vmsg::CSBagInfo& stBag = P_BAG_CTRL->getBagInfo();
    for (int i = 0; i < stBag.celllst_size(); ++i) {
        if (stBag.celllst(i).has_equipinfo()) {
			addOneListItem(stBag.celllst(i).equipinfo(), false, stBag.celllst(i).index() );
        }
    }
    
    //sort,当前装备排列前面，等级越高越靠前
    sort(m_stAllEquipInfos.begin(), m_stAllEquipInfos.end(), compareEquipLv );

	m_pTableView->reloadData();

	GUIDE_SHOW(GUIDE_UI_EQUIP_SEL);
}

void EquipSelPanel::addOneListItem(const vmsg::CSEquipInfo& stInfo, const bool bIsWear, const int iBagIdx) {
	/*
    if(m_iFromEquipPart != 0) {
		const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stInfo.equipid());
		if(pstCfg && pstCfg->part() != m_iFromEquipPart) {
			return; //仅显示相同部位
		}
        
		if(P_EQUIP_CTRL->getTranFromUID() == stInfo.equipguid()){
			return;
		}
	}*/
    m_stAllEquipInfos.push_back(EquipLibOneInfo(stInfo, bIsWear, iBagIdx) );
}



void EquipSelPanel::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch(type) {
    case TOUCH_EVENT_ENDED:
        if(std::strcmp(pszWidgetName, "close_btn") == 0) {
            close();
        } else if(std::strcmp(pszWidgetName, "ok_btn") == 0) {
            close(); //ok_btn
        }
        break;
    default:
        break;
    }
}


bool EquipSelPanel::compareEquipLv(const EquipLibOneInfo& stInfoA, const EquipLibOneInfo& stInfoB) {
    //返回A是否靠前排列
    //sort,当前装备排列前面，等级越高越靠前
    if(stInfoA.bIsWear == stInfoB.bIsWear) {
        return (stInfoA.equipInfo.level() > stInfoB.equipInfo.level() );
    } else if(stInfoA.bIsWear) {
        return true;
    } else {
        //B is wear
        return false;
    }
}

