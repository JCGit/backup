#include "EquipLibListPanel.h"
#include "EquipLibListItem.h"
#include "UserObj.h"
#include "UserInfo.h"
#include "bag/BagControl.h"
#include "EquipmentCfgMaster.h"
#include "EquipLibControl.h"

#include <algorithm>

EquipLibListPanel::EquipLibListPanel()
:m_pCloseBtn(NULL)
,m_pTableView(NULL)
{

}

EquipLibListPanel::~EquipLibListPanel()
{

}

CCNode* EquipLibListPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "first_equip_item") {
		if(numberOfCellsInTableView(m_pTableView) > 0) {
			CCTableViewCell *pCell = m_pTableView->cellAtIndex(0);
			EquipLibListItem *pItem = dynamic_cast<EquipLibListItem*>(pCell->getChildByTag(EQUIP_LIST_ITEM_TAG));
			pRet = pItem->getFirstItemNode();
		}
	}else if (name == "select_equip_item_0")
	{
		if(numberOfCellsInTableView(m_pTableView) > 0) {
			for (int i = 0; i< numberOfCellsInTableView(m_pTableView); i ++)
			{
				CCTableViewCell *pCell = m_pTableView->cellAtIndex(i);
				EquipLibListItem *pItem = dynamic_cast<EquipLibListItem*>(pCell->getChildByTag(EQUIP_LIST_ITEM_TAG));
				if (pItem->getEquipInfo().equipid()== 101)
				{
					pRet = pItem->getFirstItemNode();
					break;
				}				
			}
			
		}
	}

	return pRet;
}

bool EquipLibListPanel::init() 
{
	bool bRet = false;

	do 
    {
		CC_BREAK_IF(!BasePanel::init());

        setOpenWithBgImg(true);

		initLayoutFromJsonFile("ui_v2/equip_lib_list_panel.ExportJson");
		addChild(m_pRootLayout);
		
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(EquipLibListPanel::touchEventHandler));

		m_pTableView = CCTableView::create(this, CCSizeMake(675, 460));
		m_pTableView->setTouchPriority(96);
		m_pTableView->setDirection(kCCScrollViewDirectionVertical);
		m_pTableView->setAnchorPoint(CCPointZero);
		m_pTableView->setPosition(ccp(25, 10));
		m_pTableView->setDelegate(this);
		m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

        UIImageView* pBgInner = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_inner"));
		pBgInner->addNode(m_pTableView);

        m_pBtnTabAll = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_tab_all"));
        m_pBtnTabRing = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_tab_ring"));
        m_pBtnTabArmor = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_tab_armor"));
        m_pBtnTabCloth = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_tab_cloth"));
        m_pBtnTabShoe = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_tab_shoe"));

        m_stTabBtnList[0] = m_pBtnTabAll;
        m_stTabBtnList[1] = m_pBtnTabRing;
        m_stTabBtnList[2] = m_pBtnTabArmor;
        m_stTabBtnList[3] = m_pBtnTabCloth;
        m_stTabBtnList[4] = m_pBtnTabShoe;
        for (int i = 0; i < EQUIP_LIB_MAX_TAB; ++i) {
            m_stTabBtnList[i]->addTouchEventListener(this, toucheventselector(EquipLibListPanel::touchEventHandler));
        }
		bRet = true;
	} 
    while(0);

	return bRet;
}

CCSize EquipLibListPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 154);
}

unsigned int EquipLibListPanel::numberOfCellsInTableView(CCTableView *table){
	return ceil(m_stAllEquipInfos.size() / 2.0f);
}

CCTableViewCell* EquipLibListPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	EquipLibListItem *pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = EquipLibListItem::create();
		cell->addChild(pItem, 0, EQUIP_LIST_ITEM_TAG);
	}else{
		pItem = dynamic_cast<EquipLibListItem*>(cell->getChildByTag(EQUIP_LIST_ITEM_TAG));
	}

    unsigned int iArrayIdx = idx * 2;
	
    if(iArrayIdx < m_stAllEquipInfos.size()) {
        pItem->setEquipInfo(0, m_stAllEquipInfos[iArrayIdx]);

        iArrayIdx++;
        if(iArrayIdx < m_stAllEquipInfos.size() ) {
            pItem->setEquipInfo(1, m_stAllEquipInfos[iArrayIdx]);
        } else {
            pItem->hideSecondItem();
        }

    }

	return cell;
}

void EquipLibListPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	EquipLibListItem *pItem = dynamic_cast<EquipLibListItem*>(cell->getChildByTag(EQUIP_LIST_ITEM_TAG));
    if(NULL == pItem) {
        return;
    }

	GUIDE_TOUCH(pItem);
    if(pItem->onItemTouched(m_stGlobalTouchPt)) {
        close(); //选了就关闭
        P_EQUIP_LIB_CTRL->getEquipLibPanel()->open();
    }
    
	GUIDE_SHOW(GUIDE_UI_EQUIP_LIB);
}

void EquipLibListPanel::open()
{
    BasePanel::open();
    showTab(m_pBtnTabAll, EQUIP_PART_ALL);
}


void EquipLibListPanel::close()
{
    BasePanel::close();
    m_stAllEquipInfos.clear();
    //m_pNowSelItem = NULL;
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->doUpdateUI(false); //关闭时更新一下
}


void EquipLibListPanel::filterEquipByPart(const int iEquipPart) {

    m_stAllEquipInfos.clear();

    //show role equip
    const std::vector<vmsg::CSEquipInfo>& stRoleEquipLst = P_USER_OBJ->getUserInfo()->stEquipLst;
    for (int i = 0; i < (int)stRoleEquipLst.size(); ++i) {
        addOneListItem(stRoleEquipLst[i], true, -1, iEquipPart);
    }

    //备用防具
    const vmsg::CSEquipInfo& stSpareEquip = P_USER_OBJ->getUserInfo()->spareEquip;
    if(0 != stSpareEquip.equipguid()) {
        addOneListItem(stSpareEquip, false, -1, iEquipPart);
    }

    //bag equip
    const vmsg::CSBagInfo& stBag = P_BAG_CTRL->getBagInfo();
    for (int i = 0; i < stBag.celllst_size(); ++i) {
        if (stBag.celllst(i).has_equipinfo()) {
            addOneListItem(stBag.celllst(i).equipinfo(), false, stBag.celllst(i).index(), iEquipPart);
        }
    }

    //sort,当前装备排列前面，等级越高越靠前
    sort(m_stAllEquipInfos.begin(), m_stAllEquipInfos.end(), compareEquipLv );

    m_pTableView->reloadData();
}



void EquipLibListPanel::addOneListItem(const vmsg::CSEquipInfo& stInfo, const bool bIsWear, const int iBagIdx, const int iEquipPart) {

    if(EQUIP_PART_ALL == iEquipPart) {
        m_stAllEquipInfos.push_back(EquipLibOneInfo(stInfo, bIsWear, iBagIdx) );
    } else {
        const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stInfo.equipid());
        if(pstCfg && iEquipPart == pstCfg->part() ) {
            m_stAllEquipInfos.push_back(EquipLibOneInfo(stInfo, bIsWear, iBagIdx) );
        }
    }
    
}



void EquipLibListPanel::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch(type) {
    case TOUCH_EVENT_ENDED:
        if(std::strcmp(pszWidgetName, "close_btn") == 0) {
            close();
        } else if(std::strcmp(pszWidgetName, "btn_tab_all") == 0) {
            showTab(m_pBtnTabAll, EQUIP_PART_ALL);
        } else if(std::strcmp(pszWidgetName, "btn_tab_ring") == 0) {
            showTab(m_pBtnTabRing, vmsg::EQUIP_POS_RING);
        } else if(std::strcmp(pszWidgetName, "btn_tab_armor") == 0) {
            showTab(m_pBtnTabArmor, vmsg::EQUIP_POS_ARMOR);
        } else if(std::strcmp(pszWidgetName, "btn_tab_cloth") == 0) {
            showTab(m_pBtnTabCloth, vmsg::EQUIP_POS_CLOTH);
        } else if(std::strcmp(pszWidgetName, "btn_tab_shoe") == 0) {
            showTab(m_pBtnTabShoe, vmsg::EQUIP_POS_SHOE);
        }
        break;
    default:
        break;
    }
}


bool EquipLibListPanel::compareEquipLv(const EquipLibOneInfo& stInfoA, const EquipLibOneInfo& stInfoB) {
    //返回A是否靠前排列
    //sort,当前装备排列前面，等级越高越靠前
    if(stInfoA.bIsWear == stInfoB.bIsWear) {
        //星级高的在前面
        const int aColor = EQUIPMENT_CFG_MASTER->GetEquipColor(stInfoA.equipInfo.equipid());
        const int bColor = EQUIPMENT_CFG_MASTER->GetEquipColor(stInfoB.equipInfo.equipid());
        if(aColor == bColor) {
            return (stInfoA.equipInfo.level() > stInfoB.equipInfo.level() );
        } else {
            return (aColor > bColor );
        }
    } else if(stInfoA.bIsWear) {
        return true;
    } else {
        //B is wear
        return false;
    }
}


void EquipLibListPanel::showTab(UIButton* m_pTabBtn, const int iEquipPart) {
    if(NULL == m_pTabBtn) {
        return;
    }
    UIButton* pBtn;
    for(int i = 0; i < EQUIP_LIB_MAX_TAB; ++i) {
        pBtn = m_stTabBtnList[i];
        if(pBtn == m_pTabBtn) {
            //选中
            pBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae) ); //按下
            pBtn->setTitleFontSize(24);
            pBtn->setBright(false);
        } else {
            //不选
            pBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1) );
            pBtn->setTitleFontSize(22);
            pBtn->setBright(true);
        }
    }
    filterEquipByPart(iEquipPart);

	GUIDE_SHOW(GUIDE_UI_EQUIP_LIST);
}

