#include "BagLayout.h"

#include "defines.h"

#include "CfgMaster/BagenlargeCfgMaster.h"
#include "CfgMaster/ItemCfgMaster.h"
#include "CfgMaster/EquipmentCfgMaster.h"
#include "CfgMaster/HeroCfgMaster.h"
#include "CfgMaster/PackageCfgMaster.h"

#include "basic/NounHelper.h"

#include "BagControl.h"
#include "BagCell.h"
#include "ItemInfoPanel.h"
#include "EquipInfoPanel.h"

#include "game/UserObj.h"
#include "game/role/MainHero.h"
#include "game/login/LoginControl.h"
#include "game/login/LoginProxy.h"
#include "game/tip/TipControl.h"

#define MAX_ROW_CELLS 5
#define MAX_COL_CELLS 4

BagLayout::BagLayout()
:m_pBagScrollView(NULL)
,m_pCapacityTxt(NULL)
,m_pCurSelectedCell(NULL)
,m_uBagCapacity(0)
,m_pTabListView(NULL)
,m_nSelectedTabIdx(-1)
,m_uTabType(0)
,m_pBagLayout(NULL)
{
}

BagLayout::~BagLayout()
{
	for(unsigned int i = 0; i < m_stBagCells.size(); i++) {
		CC_SAFE_RELEASE_NULL(m_stBagCells[i]);
	}

	m_stBagCells.clear();
}

BagLayout* BagLayout::create(UILayout *pRefLayout) {
	BagLayout *pRet = new BagLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BagLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		m_pBagScrollView = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("bag_scroll"));
		m_pBagScrollView->addEventListenerScrollView(this, scrollvieweventselector(BagLayout::scrollViewEventHandler));
		m_pBagLayout = UILayout::create();
		m_pBagScrollView->addChild(m_pBagLayout);

		m_pCapacityTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("capacity_txt"));

		/*RelativeLayoutParameter *pParameter = new RelativeLayoutParameter();
		Margin m(342.0f, 72.0f, 0.0f, 0.0f);
		pParameter->setMargin(m);
		pParameter->setAlign(RELATIVE_ALIGN_PARENT_TOP_LEFT);*/

		m_pTabListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("tab_list"));
		m_pTabListView->addEventListenerListView(this, listvieweventselector(BagLayout::listViewEventHandler));

		UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_uTabType));
		pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
		pTabBtn->setTouchEnabled(false);
		pTabBtn->getTextVirtualRenderer()->setScale(1.2f);
		pTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));
		m_nSelectedTabIdx = m_uTabType;

		bRet = true;
	} while(0);

	return bRet;
}

void BagLayout::updateUI() {
	const vmsg::CSBagInfo &stBagInfo = P_BAG_CTRL->getBagInfo();
   
	m_pCapacityTxt->setText(intToString(stBagInfo.celllst_size()) + "/" + intToString(stBagInfo.capacity()));

	updateCells();
	m_pBagScrollView->scrollToTop(0.8f, true);

	GUIDE_SHOW(GUIDE_UI_ROLE);
}

void BagLayout::updateCells() {
	const vmsg::CSBagInfo &stBagInfo = P_BAG_CTRL->getBagInfo();

	const CCSize &viewSize = m_pBagScrollView->getSize();
	float scrollH = viewSize.height;
	bool isScrollHDirty = false;
	unsigned int curCapacity = m_stBagCells.size();
	float marginY = 0.0f;
	float cellH = 0.0f;
	//for(unsigned int i = curCapacity; i < stBagInfo.capacity(); i++) { // create cell
	for(int i = curCapacity; i < stBagInfo.celllst_size(); i++) { // create cell
		BagCell *pCell = BagCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("bag_cell_layout")));
		CC_SAFE_RETAIN(pCell);
		m_stBagCells.push_back(pCell);
		const CCSize &cellSize = pCell->getSize();
		const CCPoint &cellAnchor = pCell->getAnchorPoint();
		float dx = (viewSize.width - cellSize.width * MAX_COL_CELLS) / (MAX_COL_CELLS + 1);
		float dy = (viewSize.height - cellSize.height * MAX_ROW_CELLS) / (MAX_ROW_CELLS + 1);
		float posX = i % MAX_COL_CELLS * (cellSize.width + 4.0f) + cellSize.width * cellAnchor.x;
		float posY = viewSize.height - (i / MAX_COL_CELLS + 1) * (cellSize.height + 4.0f) + 4.0f + cellSize.height * cellAnchor.y;
		pCell->setPosition(ccp(posX, posY));
		//m_pBagLayout->addChild(pCell);
		pCell->addTouchEventListener(this, toucheventselector(BagLayout::touchEventHandler));
		marginY = dy;
		cellH = cellSize.height;
	}

	std::vector<vmsg::CSBagCellInfo> stFilterCellInfos;

	for(int i = 0; i < stBagInfo.celllst_size(); i++) {
		const vmsg::CSBagCellInfo &stBagCellInfo = stBagInfo.celllst(i);

		if(stBagCellInfo.type() == vmsg::THING_TYPE_EQUIP) { // equip
			const vmsg::CSEquipInfo &stEquipInfo = stBagCellInfo.equipinfo();

			if(m_uTabType == BAG_TAB_EQUIP) {
				stFilterCellInfos.push_back(stBagCellInfo);
			} else if(m_uTabType == BAG_TAB_ALL) {
				stFilterCellInfos.push_back(stBagCellInfo);
			}
		} else if(stBagCellInfo.type() == vmsg::THING_TYPE_ITEM) { // item
			const vmsg::CSItemInfo &stItemInfo = stBagCellInfo.iteminfo();
			const ItemCfg *pItemCfg = ITEM_CFG_MASTER->GetItemCfgByID(stItemInfo.itemcfgid());

			if(m_uTabType == BAG_TAB_MATERIAL && pItemCfg->itemtype() == 1) { // ²ÄÁÏ
				stFilterCellInfos.push_back(stBagCellInfo);
			} else if(m_uTabType == BAG_TAB_ALL) {
				stFilterCellInfos.push_back(stBagCellInfo);
			}
			
		}
	}

	unsigned int cellLstSize = stFilterCellInfos.size();
	unsigned int rows = cellLstSize % MAX_COL_CELLS == 0 ? cellLstSize / MAX_COL_CELLS : cellLstSize / MAX_COL_CELLS + 1;
	float newScrollH = rows * (100 + 4.0f) - 4.0f;
	scrollH = newScrollH > viewSize.height ? newScrollH : viewSize.height;
	isScrollHDirty = true;

	if(isScrollHDirty) {
		isScrollHDirty = false;
		m_pBagScrollView->setInnerContainerSize(CCSizeMake(viewSize.width, scrollH));
		m_pBagLayout->setPosition(ccp(0, scrollH - viewSize.height));
	}

	m_pBagLayout->removeAllChildren();

	for(unsigned int i = 0; i < stFilterCellInfos.size(); i++) {
		BagCell *pCell = m_stBagCells[i];
		m_pBagLayout->addChild(pCell);
		pCell->setInfo(stFilterCellInfos[i]);
	}

	/*for(unsigned int i = 0; i < stBagInfo.capacity(); i++) {
		BagCell *pCell = m_stBagCells[i];

		if(i < stFilterCellInfos.size()) {
			pCell->setInfo(stFilterCellInfos[i]);
		} else {
			pCell->clearInfo();
		}
	}*/

	
}

void BagLayout::gotoTabPage(unsigned int uTabType) {
	UIButton *pTabBtn = NULL;

	switch(uTabType) {
		case BAG_TAB_ALL:
		case BAG_TAB_EQUIP:
		case BAG_TAB_MATERIAL:
			m_uTabType = uTabType;

			if(m_nSelectedTabIdx >= 0) {
				pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_nSelectedTabIdx));
				pTabBtn->setBrightStyle(BRIGHT_NORMAL);
				pTabBtn->setTouchEnabled(true);
				pTabBtn->getTextVirtualRenderer()->setScale(1.0f);
				pTabBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
			}

			pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_uTabType));
			pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
			pTabBtn->setTouchEnabled(false);
			pTabBtn->getTextVirtualRenderer()->setScale(1.2f);
			pTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));
			m_nSelectedTabIdx = m_uTabType;

			updateCells();
			m_pBagScrollView->scrollToTop(0.8f, true);
			break;
		default:;
	}
}

CCNode* BagLayout::getGuideBagCell(unsigned int equipId) {
	CCNode *pRet = NULL;
	BagCell *pCell = NULL;
	CCArray *pChildren = m_pBagLayout->getChildren();
	CCObject *pChild = NULL;

	CCARRAY_FOREACH(pChildren, pChild) {
		pCell = dynamic_cast<BagCell*>(pChild);
		if(pCell != NULL && pCell->getInfo().has_equipinfo()) {
			const vmsg::CSEquipInfo peq = pCell->getInfo().equipinfo();
			
				if (equipId == peq.equipid())
				{
					pRet = pCell;
					break;
				}
			
		}
	}

	return pRet;
}

void BagLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	
	switch (type) {
		case TOUCH_EVENT_ENDED:
			if(pWidget->getDescription() == "BagCell") {
				GUIDE_TOUCH(pWidget);

				BagCell *pCell = dynamic_cast<BagCell*>(pWidget);

				if(m_pCurSelectedCell != NULL) {
					m_pCurSelectedCell->selected(false);
				}

				pCell->selected(true);
				m_pCurSelectedCell = pCell;

				const vmsg::CSBagCellInfo &stBagCellInfo = pCell->getInfo();
				
				if(stBagCellInfo.type() == vmsg::THING_TYPE_EQUIP) {
					P_BAG_CTRL->getEquipInfoPanel()->open();
					P_BAG_CTRL->getEquipInfoPanel()->updateInfo(stBagCellInfo.equipinfo(), EquipInfoPanel::EQUIP_IN_BAG);
				} else if(stBagCellInfo.type() == vmsg::THING_TYPE_ITEM) {
					const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(stBagCellInfo.iteminfo().itemcfgid());
					
					if(pPackageCfg != NULL) {
						P_BAG_CTRL->getPackageInfoPanel()->showPackage(stBagCellInfo.iteminfo().itemcfgid());
					} else {
						P_BAG_CTRL->getItemInfoPanel()->open();
						P_BAG_CTRL->getItemInfoPanel()->updateInfo(stBagCellInfo.iteminfo().itemcfgid(), vmsg::THING_TYPE_ITEM, true, stBagCellInfo.iteminfo().itemcnt(), stBagCellInfo.index());
					}
				}

			}
		break;
		default:;
	}
}

void BagLayout::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszWidgetName, m_pTabListView->getName()) == 0) {
				
				int nSelectedIdx = m_pTabListView->getCurSelectedIndex();
				GUIDE_TOUCH(m_pTabListView->getItem(nSelectedIdx));
				gotoTabPage(nSelectedIdx);
				GUIDE_SHOW(GUIDE_UI_ROLE);
			}
			break;
		default:;
	}
}

void BagLayout::scrollViewEventHandler(CCObject *pSender, ScrollviewEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

}

CCNode* BagLayout::getEquipTab()
{
	CCNode* pNode = NULL;
	if (m_pTabListView&& m_pTabListView->getItems()->count()>=2)
	{
		pNode = m_pTabListView->getItem(1);
	}
	return pNode;
}
