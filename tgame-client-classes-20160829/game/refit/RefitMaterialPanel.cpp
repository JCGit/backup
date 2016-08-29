#include "RefitMaterialPanel.h"

#include "defines.h"

#include "CfgMaster/DotaItemCfgMaster.h"
#include "CfgMaster/ItemDropTipCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/TextHelper.h"
#include "basic/PopTipLayer.h"

#include "game/UserObj.h"
#include "game/fight/LevelControl.h"
#include "game/fight/LevelObj.h"

#include "RefitControl.h"
#include "RefitMaterialCell.h"
#include "DropItem.h"

#define CHILD_CELL_TAG 101
#define CELL_WIDTH 100
#define NODE_ZOOM_FACTOR 0.6f
#define NODE_DISTANCE 72
#define NODE_PREFIX_NAME "node_"

RefitMaterialPanel::RefitMaterialPanel()
:m_pCloseBtn(NULL)
,m_pItemInfoLayout(NULL)
,m_pItemNameTxt(NULL)
,m_pItemImg(NULL)
,m_pItemDescTxt(NULL)
,m_pAttr1Txt(NULL)
,m_pAttr1ValTxt(NULL)
,m_pAttr2Txt(NULL)
,m_pAttr2ValTxt(NULL)
,m_pComposeOrMosaicBtn(NULL)
,m_pComposeLayout(NULL)
,m_pTreeListView(NULL)
,m_pTreeInfoLayout(NULL)
,m_pTreeItemImg(NULL)
,m_pTreeLineImg(NULL)
,m_pNodesLayout(NULL)
,m_pItemCntTxt(NULL)
,m_pComposeBtn(NULL)
,m_pDropInfoLayout(NULL)
,m_pDropListView(NULL)
,m_pBackBtn(NULL)
,m_uPart(0)
,m_uItemID(0)
{
}

RefitMaterialPanel::~RefitMaterialPanel()
{
}

RefitMaterialPanel* RefitMaterialPanel::create() {
	RefitMaterialPanel *pRet = new RefitMaterialPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RefitMaterialPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void RefitMaterialPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}
}

CCNode* RefitMaterialPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "mosaic_btn") {
		if(m_pComposeOrMosaicBtn != NULL && m_pComposeOrMosaicBtn->isRunning() && m_pComposeOrMosaicBtn->isTouchEnabled()) {
			pRet = m_pComposeOrMosaicBtn;
		}
	}

	return pRet;
}

void RefitMaterialPanel::openWithItemID(unsigned int uPart, unsigned int uItemID) {
	open();

	m_uPart = uPart;
	m_uItemID = uItemID;
	updateUI();
}

void RefitMaterialPanel::updateUI() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	const DotaItemCfg *pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(m_uItemID);

	if(pDotaItemCfg != NULL) {
		RefitMaterialCell *pCurCell = dynamic_cast<RefitMaterialCell*>(m_pItemImg->getChildByTag(CHILD_CELL_TAG));
		pCurCell->setItemID(m_uItemID);
		pCurCell->setStat(RefitMaterialCell::CELL_STAT_SHOW);

		m_pItemNameTxt->setText(pDotaItemCfg->dotaitemname());
		m_pItemDescTxt->setText(pDotaItemCfg->dotaitemdesc());

		string attr1Name = "";
		string attr2Name = "";
		unsigned int attr1Val = 0;
		unsigned int attr2Val = 0;

		if(pDotaItemCfg->atk() > 0) {
			if(attr1Val == 0) {
				attr1Name = NounStr(NOUN_ATTR_DPS);
				attr1Val = pDotaItemCfg->atk();
			} else if(attr2Val == 0) {
				attr2Name = NounStr(NOUN_ATTR_DPS);
				attr2Val = pDotaItemCfg->atk();
			}
		}

		if(pDotaItemCfg->def() > 0) {
			if(attr1Val == 0) {
				attr1Name = NounStr(NOUN_DEFENSE);
				attr1Val = pDotaItemCfg->def();
			} else if(attr2Val == 0) {
				attr2Name = NounStr(NOUN_DEFENSE);
				attr2Val = pDotaItemCfg->def();
			}
		}

		if(pDotaItemCfg->hp() > 0) {
			if(attr1Val == 0) {
				attr1Name = NounStr(NOUN_HP);
				attr1Val = pDotaItemCfg->hp();
			} else if(attr2Val == 0) {
				attr2Name = NounStr(NOUN_HP);
				attr2Val = pDotaItemCfg->hp();
			}
		} 

		if(pDotaItemCfg->hit() > 0) {
			if(attr1Val == 0) {
				attr1Name = NounStr(NOUN_ATTR_HIT);
				attr1Val = floor(pDotaItemCfg->hit() * 2.3f + 0.5f);
			} else if(attr2Val == 0) {
				attr2Name = NounStr(NOUN_ATTR_HIT);
				attr2Val = floor(pDotaItemCfg->hit() * 2.3f + 0.5f);
			}
		}

		m_pAttr1Txt->setText(attr1Name);
		m_pAttr1ValTxt->setText(intToString(attr1Val));
		m_pAttr1Txt->setVisible(attr1Val > 0);
		m_pAttr1ValTxt->setVisible(attr1Val > 0);
		m_pAttr2Txt->setText(attr2Name);
		m_pAttr2ValTxt->setText(intToString(attr2Val));
		m_pAttr2Txt->setVisible(attr2Val > 0);
		m_pAttr2ValTxt->setVisible(attr2Val > 0);

		m_pComposeOrMosaicBtn->setTouchEnabled(P_REFIT_CTRL->getItemCnt(m_uItemID) > 0);
		m_pComposeOrMosaicBtn->setBright(m_pComposeOrMosaicBtn->isTouchEnabled());

		m_pTreeListView->removeAllItems();
		RefitMaterialCell *pCell = RefitMaterialCell::create();
		pCell->setSize(pCell->getSize() * NODE_ZOOM_FACTOR);
		pCell->setItemID(m_uItemID);
		pCell->setStat(RefitMaterialCell::CELL_STAT_SHOW);
		m_pTreeListView->pushBackCustomItem(pCell);
		pCell->setTouchEnabled(true);
		showItem(m_uItemID);
	}
	
	GUIDE_SHOW(GUIDE_UI_REFIT_MATERIAL);
	closeLoading();
}

void RefitMaterialPanel::showItem(unsigned int uItemID) {
	const DotaItemCfg *pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(uItemID);

	if(pDotaItemCfg != NULL) {
		int nComposeNeeds = pDotaItemCfg->dotaitemneeds_size();

		m_pTreeInfoLayout->setEnabled(nComposeNeeds > 0);
		m_pDropInfoLayout->setEnabled(nComposeNeeds == 0);
		
		if(nComposeNeeds > 0) {
			RefitMaterialCell *pTreeCell = dynamic_cast<RefitMaterialCell*>(m_pTreeItemImg->getChildByTag(CHILD_CELL_TAG));
			pTreeCell->setItemID(uItemID);
			pTreeCell->setStat(RefitMaterialCell::CELL_STAT_SHOW);

			m_pNodesLayout->removeAllChildren();
			string iconURL = string("share/tree_line_") + intToString(nComposeNeeds) + ".png";
			m_pTreeLineImg->loadTexture(iconURL.c_str(), UI_TEX_TYPE_PLIST);

			float originX = m_pNodesLayout->getSize().width / 2 - (nComposeNeeds * (NODE_DISTANCE) - (NODE_DISTANCE - CELL_WIDTH * NODE_ZOOM_FACTOR)) / 2 + CELL_WIDTH * NODE_ZOOM_FACTOR / 2;
			float originY = m_pNodesLayout->getSize().height / 2;

			bool bCanCompose = true;

			for(int i = 0; i < nComposeNeeds; i++) {
				const DotaItemComposeNeed &stNeedDotaItem = pDotaItemCfg->dotaitemneeds(i);
				unsigned int uNeedItemID = stNeedDotaItem.dotaitemid();
				unsigned int uNeedItemCnt = stNeedDotaItem.dotaitemcnt();
				string name = string(NODE_PREFIX_NAME) + intToString(i);
				RefitMaterialCell *pCell = RefitMaterialCell::create();
				pCell->setName(name.c_str());
				pCell->setTouchEnabled(true);
				pCell->setSize(pCell->getSize() * NODE_ZOOM_FACTOR);
				pCell->setItemID(uNeedItemID);
				
				pCell->setPosition(ccp(originX + NODE_DISTANCE * i, originY));
				m_pNodesLayout->addChild(pCell);
				pCell->addTouchEventListener(this, toucheventselector(RefitMaterialPanel::touchEventHandler));

				unsigned int uItemCnt = P_REFIT_CTRL->getItemCnt(uNeedItemID);
				UILabel *pCntTxt = dynamic_cast<UILabel*>(m_pItemCntTxt->clone());
				pCntTxt->setText(intToString(uItemCnt) + "/" + intToString(uNeedItemCnt));
				pCntTxt->setPosition(pCell->getPosition() + ccp(0, -CELL_WIDTH * NODE_ZOOM_FACTOR / 2 - pCntTxt->getSize().height / 2 - 4));
				m_pNodesLayout->addChild(pCntTxt);

				if(uItemCnt < uNeedItemCnt) {
					bCanCompose = false;
					if(P_REFIT_CTRL->isItemCanComposeCountCompose(uNeedItemID)) {
						pCell->setStat(RefitMaterialCell::CELL_STAT_ADD);
					}else
					{
						pCell->setStat(RefitMaterialCell::CELL_STAT_SHOW);
					}
					pCntTxt->setColor(ccc3(0xff, 0x00, 0x00));
				} else {
					pCell->setStat(RefitMaterialCell::CELL_STAT_ADDED);
					pCntTxt->setColor(ccc3(0x00, 0xff, 0x00));
				}
			}

			m_pComposeBtn->setTouchEnabled(bCanCompose);
			m_pComposeBtn->setBright(m_pComposeBtn->isTouchEnabled());
		} else {
			m_pDropListView->removeAllItems();
			const ItemDropTipCfg *pDropCfg = ITEM_DROP_TIP_MASTER->GetOneCfg(uItemID, vmsg::THING_TYPE_DOTAITEM);

			for(int i = 0; i < pDropCfg->warlist_size(); i++) {
				const ItemDropTipOne &stWarTip = pDropCfg->warlist(i);
				DropItem *pItem = DropItem::create();
				pItem->setInfo(vmsg::THING_TYPE_DOTAITEM, uItemID, stWarTip.wartype(), stWarTip.warid());
				m_pDropListView->pushBackCustomItem(pItem);
			}

			m_pBackBtn->setTouchEnabled(m_pTreeListView->getItems()->count() > 1);
			m_pBackBtn->setBright(m_pBackBtn->isTouchEnabled());
		}
	}
}

void RefitMaterialPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/RefitMaterialPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(RefitMaterialPanel::touchEventHandler));

	m_pItemInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("item_info_layout"));

	m_pItemNameTxt = dynamic_cast<UILabel*>(m_pItemInfoLayout->getChildByName("item_name_txt"));

	m_pItemImg = dynamic_cast<UIImageView*>(m_pItemInfoLayout->getChildByName("item_img"));
	RefitMaterialCell *pCell = RefitMaterialCell::create();
	pCell->setStat(RefitMaterialCell::CELL_STAT_UNKNOWN);
	m_pItemImg->addChild(pCell, 0, CHILD_CELL_TAG);

	m_pItemDescTxt = dynamic_cast<UILabel*>(m_pItemInfoLayout->getChildByName("item_desc_txt"));

	m_pAttr1Txt = dynamic_cast<UILabel*>(m_pItemInfoLayout->getChildByName("attr1_txt"));

	m_pAttr1ValTxt = dynamic_cast<UILabel*>(m_pItemInfoLayout->getChildByName("attr1_val_txt"));

	m_pAttr2Txt = dynamic_cast<UILabel*>(m_pItemInfoLayout->getChildByName("attr2_txt"));

	m_pAttr2ValTxt = dynamic_cast<UILabel*>(m_pItemInfoLayout->getChildByName("attr2_val_txt"));

	m_pComposeOrMosaicBtn = dynamic_cast<UIButton*>(m_pItemInfoLayout->getChildByName("compose_or_mosaic_btn"));
	m_pComposeOrMosaicBtn->addTouchEventListener(this, toucheventselector(RefitMaterialPanel::touchEventHandler));

	m_pComposeLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("compose_layout"));

	m_pTreeListView = dynamic_cast<UIListView*>(m_pComposeLayout->getChildByName("tree_list"));
	m_pTreeListView->addEventListenerListView(this, listvieweventselector(RefitMaterialPanel::listViewEventHandler));

	m_pTreeInfoLayout = dynamic_cast<UILayout*>(m_pComposeLayout->getChildByName("tree_info_layout"));

	m_pTreeItemImg = dynamic_cast<UIImageView*>(m_pTreeInfoLayout->getChildByName("item_img"));
	RefitMaterialCell *pTreeCell = RefitMaterialCell::create();
	pTreeCell->setStat(RefitMaterialCell::CELL_STAT_UNKNOWN);
	m_pTreeItemImg->addChild(pTreeCell, 0, CHILD_CELL_TAG);

	m_pTreeLineImg = dynamic_cast<UIImageView*>(m_pTreeInfoLayout->getChildByName("tree_line_img"));

	m_pNodesLayout = dynamic_cast<UILayout*>(m_pTreeInfoLayout->getChildByName("nodes_layout"));

	m_pItemCntTxt = dynamic_cast<UILabel*>(m_pTreeInfoLayout->getChildByName("item_cnt_txt"));

	m_pComposeBtn = dynamic_cast<UIButton*>(m_pTreeInfoLayout->getChildByName("compose_btn"));
	m_pComposeBtn->addTouchEventListener(this, toucheventselector(RefitMaterialPanel::touchEventHandler));

	m_pDropInfoLayout = dynamic_cast<UILayout*>(m_pComposeLayout->getChildByName("drop_info_layout"));

	m_pDropListView = dynamic_cast<UIListView*>(m_pDropInfoLayout->getChildByName("drop_list"));
	m_pDropListView->addEventListenerListView(this, listvieweventselector(RefitMaterialPanel::listViewEventHandler));

	m_pBackBtn = dynamic_cast<UIButton*>(m_pDropInfoLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(RefitMaterialPanel::touchEventHandler));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void RefitMaterialPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pComposeOrMosaicBtn->getName()) == 0) {
				GUIDE_TOUCH(pWidget);
				P_REFIT_CTRL->sendRefitMosaicRqst(m_uPart, m_uItemID);
				close();
			} else if(strcmp(pszWidgetName, m_pComposeBtn->getName()) == 0) {
				RefitMaterialCell *pTreeCell = dynamic_cast<RefitMaterialCell*>(m_pTreeItemImg->getChildByTag(CHILD_CELL_TAG));
				unsigned int uItemID = pTreeCell->getItemID();
				showLoading();
				P_REFIT_CTRL->sendRefitComposeRqst(uItemID);
			} else if(strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				int nItemCnt = m_pTreeListView->getItems()->count();
				int nSelectedIdx = nItemCnt - 2;
				RefitMaterialCell *pCell = dynamic_cast<RefitMaterialCell*>(m_pTreeListView->getItem(nSelectedIdx));
				showItem(pCell->getItemID());
				m_pTreeListView->removeItem(nItemCnt - 1);
			} else if(string(pszWidgetName).substr(0, string(NODE_PREFIX_NAME).size()) == NODE_PREFIX_NAME) {
				RefitMaterialCell *pCell = dynamic_cast<RefitMaterialCell*>(m_pNodesLayout->getChildByName(pszWidgetName));

				if(pCell != NULL) {
					unsigned int uItemID = pCell->getItemID();
					RefitMaterialCell *pCell = RefitMaterialCell::create();
					pCell->setSize(pCell->getSize() * NODE_ZOOM_FACTOR);
					pCell->setItemID(uItemID);
					pCell->setStat(RefitMaterialCell::CELL_STAT_SHOW);
					pCell->setTouchEnabled(true);
					m_pTreeListView->pushBackCustomItem(pCell);
					showItem(uItemID);
				}
			}
			break;
		default:;
	}
}

void RefitMaterialPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(strcmp(pszListViewName, m_pTreeListView->getName()) == 0) {
				int nSelectedIdx = m_pTreeListView->getCurSelectedIndex();
				RefitMaterialCell *pCell = dynamic_cast<RefitMaterialCell*>(m_pTreeListView->getItem(nSelectedIdx));
				int nItemCnt = m_pTreeListView->getItems()->count();

				for(int i = 0; i < nItemCnt; i++) {
					if(i > nSelectedIdx) {
						RefitMaterialCell *pTmpCell = dynamic_cast<RefitMaterialCell*>(m_pTreeListView->getItem(i));
						pTmpCell->removeFromParent();
						//m_pTreeListView->removeItem(i);
					}
				}

				showItem(pCell->getItemID());
			} else if(strcmp(pszListViewName, m_pDropListView->getName()) == 0) {
				//close();
				int nSelectedIdx = m_pDropListView->getCurSelectedIndex();
				DropItem *pItem = dynamic_cast<DropItem*>(m_pDropListView->getItem(nSelectedIdx));
				unsigned int uWarType = pItem->getWarType();
				unsigned int uWarID = pItem->getWarID();

				//检查是否已经通关了
				if(ItemDropTipCfgMaster::WAR_TYPE_NORMAL == uWarType) {
					if(uWarID <= P_LEVEL_OBJ->getMapInfo().newestwarmapid()) {
						P_LEVEL_CTL->getEnterPanel()->open();
						P_LEVEL_CTL->getEnterPanel()->setLevelID(uWarID);
					} else {
						//还没通关
						POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_WAR_NOT_CLEAR_TIP));
					}

				} else if(ItemDropTipCfgMaster::WAR_TYPE_NORMAL == uWarType) {
					P_LEVEL_CTL->getEliteWarEntryPanel()->openWithQuery(uWarID);
				}
			}
			break;
		default:;
	}
}
