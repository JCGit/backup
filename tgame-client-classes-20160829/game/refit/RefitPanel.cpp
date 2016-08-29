#include "RefitPanel.h"

#include "defines.h"

#include "CfgMaster/DotaLvCfgMaster.h"
#include "CfgMaster/DotaItemCfgMaster.h"

#include "basic/ToolUtils.h"

#include "game/UserObj.h"
#include "game/bag/BagControl.h"

#include "RefitControl.h"
#include "RefitMaterialCell.h"

#define MAX_CELLS_NUM 6
#define ROW_CELLS_NUM 2
#define CELL_WIDTH 100
#define ROW_MARGIN 8
#define COL_MARGIN 12
#define CELL_PREFIX_NAME "cell_"

RefitPanel::RefitPanel()
:m_pBackBtn(NULL)
,m_pTabListView(NULL)
,m_pBigIconImg(NULL)
,m_pCellsLayout(NULL)
,m_pRefitLevelTxt(NULL)
,m_pAtkValTxt(NULL)
,m_pDefValTxt(NULL)
,m_pHpValTxt(NULL)
,m_pHitValTxt(NULL)
,m_pCostValTxt(NULL)
,m_pRefitBtn(NULL)
,m_nTabSelectedIdx(TAB_WEAPON)
,m_nRefitEffCnt(0)
{
}

RefitPanel::~RefitPanel()
{
}

RefitPanel* RefitPanel::create() {
	RefitPanel *pRet = new RefitPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RefitPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void RefitPanel::open() {
	BasePanel::open();
	
	P_REFIT_CTRL->sendRefitQueryRqst();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}
}

void RefitPanel::refresh() {
	BasePanel::refresh();
	P_REFIT_CTRL->sendRefitQueryRqst();
	showLoading();
}

CCNode* RefitPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "back_btn") {
		if(m_pBackBtn != NULL && m_pBackBtn->isRunning() && m_pBackBtn->isTouchEnabled()) {
			pRet = m_pBackBtn;
		}
	} else if(name == "tab_amulet_btn") {
		UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(part2Idx(vmsg::EQUIP_POS_RING)));

		if(pTabBtn != NULL && pTabBtn->isRunning() && pTabBtn->isTouchEnabled()) {
			pRet = pTabBtn;
		}
	} else if(name == "material_cell") {
		RefitMaterialCell *pCell = NULL;

		for(int i = 0; i < MAX_CELLS_NUM; i++) {
			string cellName = string(CELL_PREFIX_NAME) + intToString(i);
			pCell = dynamic_cast<RefitMaterialCell*>(m_pCellsLayout->getChildByName(cellName.c_str()));

			if(pCell != NULL && pCell->getStat() == RefitMaterialCell::CELL_STAT_ADD) {
				break;
			}
		}

		if(pCell != NULL && pCell->isRunning() && pCell->isTouchEnabled()) {
			pRet = pCell;
		}
	}

	return pRet;
}

void RefitPanel::switchTab(TabType type) {
	UIButton *pTabBtn = NULL;

	if(m_nTabSelectedIdx >= 0) {
		pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(m_nTabSelectedIdx));
		pTabBtn->setBrightStyle(BRIGHT_NORMAL);
		pTabBtn->setTouchEnabled(true);
		pTabBtn->getTextVirtualRenderer()->setScale(1.0f);
		pTabBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
	}

	pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(type));
	pTabBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
	pTabBtn->setTouchEnabled(false);
	pTabBtn->getTextVirtualRenderer()->setScale(1.2f);
	pTabBtn->setTitleColor(ccc3(0xff, 0xfa, 0xae));
	m_nTabSelectedIdx = type;
}

void RefitPanel::updateUI() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	const vmsg::CSDotaPartInfo &stPartInfo = P_REFIT_CTRL->getPartInfo();
	unsigned int uPart = idx2Part(m_nTabSelectedIdx);

	switch(uPart) {
		case vmsg::EQUIP_POS_RING:
			m_pBigIconImg->loadTexture("refit/icon_amulet_x.png", UI_TEX_TYPE_PLIST);
			break;
		case vmsg::EQUIP_POS_ARMOR:
			m_pBigIconImg->loadTexture("refit/icon_armor_x.png", UI_TEX_TYPE_PLIST);
			break;
		case vmsg::EQUIP_POS_CLOTH:
			m_pBigIconImg->loadTexture("refit/icon_cloak_x.png", UI_TEX_TYPE_PLIST);
			break;
		case vmsg::EQUIP_POS_SHOE:
			m_pBigIconImg->loadTexture("refit/icon_shoes_x.png", UI_TEX_TYPE_PLIST);
			break;
		default:
			m_pBigIconImg->loadTexture("refit/icon_weapon_x.png", UI_TEX_TYPE_PLIST);
	}

	const vmsg::CSDotaPart &stPart = stPartInfo.parts(uPart);

	m_pRefitLevelTxt->setText(string("Lv.") + intToString(stPart.lv()));
	m_pAtkValTxt->setText(intToString(stPart.atk()));
	m_pDefValTxt->setText(intToString(stPart.def()));
	m_pHpValTxt->setText(intToString(stPart.hp()));
	m_pHitValTxt->setText(intToString((unsigned int)(floor(stPart.hit() * 2.3f + 0.5f))));

	const DotaLvCfg *pDotaLvCfg = DOTA_LV_CFG_MASTER->getDotaLvCfg(stPart.lv(), uPart);

	int nMosaicCnt = 0;

	for(int i = 0; i < MAX_CELLS_NUM; i++) {
		string cellName = string(CELL_PREFIX_NAME) + intToString(i);
		RefitMaterialCell *pCell = dynamic_cast<RefitMaterialCell*>(m_pCellsLayout->getChildByName(cellName.c_str()));

		if(pCell == NULL) {
			pCell = RefitMaterialCell::create();
			pCell->setName(cellName.c_str());
			float posX = i % ROW_CELLS_NUM * (CELL_WIDTH + COL_MARGIN) + CELL_WIDTH / 2;
			float posY = m_pCellsLayout->getSize().height - i / ROW_CELLS_NUM * (CELL_WIDTH + ROW_MARGIN) - CELL_WIDTH / 2;
			pCell->setPosition(ccp(posX, posY));
			m_pCellsLayout->addChild(pCell);
			pCell->addTouchEventListener(this, toucheventselector(RefitPanel::touchEventHandler));
		}

		pCell->setTouchEnabled(false);

		if(pDotaLvCfg != NULL && i < pDotaLvCfg->dotalvupneeds_size()) {
			const DotaLvUpNeed &stDotaLvUpNeed = pDotaLvCfg->dotalvupneeds(i);
			unsigned int uNeedItemID = stDotaLvUpNeed.dotaitemid();
			unsigned int uNeedItemCnt = stDotaLvUpNeed.dotaitemcnt();
			pCell->setItemID(uNeedItemID);

			bool bAdded = false;

			for(int j = 0; j < stPart.items_size(); j++) {
				if(stPart.items(j) == uNeedItemID) {
					bAdded = true;
					break;
				}
			}

			if(bAdded) {
				pCell->setStat(RefitMaterialCell::CELL_STAT_ADDED);
				nMosaicCnt++;
			} else {
				if(P_REFIT_CTRL->getItemCnt(uNeedItemID) >= uNeedItemCnt) {
					pCell->setStat(RefitMaterialCell::CELL_STAT_ADD);
				} else {
					// 是否可以合成
					/*const DotaItemCfg *pNeedItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(uNeedItemID);
					bool bCanCompose = pNeedItemCfg->dotaitemneeds_size() > 0;

					for(int i = 0; i < pNeedItemCfg->dotaitemneeds_size(); i++) {
						const DotaItemComposeNeed &stNeedDotaItem = pNeedItemCfg->dotaitemneeds(i);
						unsigned int uNeedChildItemID = stNeedDotaItem.dotaitemid();

						if(P_REFIT_CTRL->getItemCnt(uNeedChildItemID) < stNeedDotaItem.dotaitemcnt()) {
							bCanCompose = false;
						}
					}*/

					if(P_REFIT_CTRL->isItemCanComposeCountCompose(uNeedItemID)) {
						pCell->setStat(RefitMaterialCell::CELL_STAT_ADD);
					} else {
						pCell->setStat(RefitMaterialCell::CELL_STAT_EMPTY);
					}
				}

			}

			pCell->setTouchEnabled(true);
		} else {
			pCell->setStat(RefitMaterialCell::CELL_STAT_UNKNOWN);
		}

	}

	if(pDotaLvCfg != NULL) {
		/*m_pAtkValTxt->setText(string("+") + intToString(pDotaLvCfg->atk()));
		m_pDefValTxt->setText(string("+") + intToString(pDotaLvCfg->def()));
		m_pHpValTxt->setText(string("+") + intToString(pDotaLvCfg->hp()));
		m_pHitValTxt->setText(string("+") + intToString(pDotaLvCfg->hit()));*/
		m_pCostValTxt->setText(intToString(pDotaLvCfg->costcoin()));
	}

	bool isCanRefit = pDotaLvCfg != NULL && nMosaicCnt == pDotaLvCfg->dotalvupneeds_size() && P_USER_OBJ->getUserInfo()->coin >= pDotaLvCfg->costcoin();
	m_pRefitBtn->setTouchEnabled(isCanRefit);
	m_pRefitBtn->setBright(isCanRefit);

	// 提醒
	for(int i = 0; i < stPartInfo.parts_size(); i++) {
		if(P_REFIT_CTRL->isPartCanRefit(i) || P_REFIT_CTRL->isPartCanAdd(i)) {
			badgeTabBtn(i, 1);
		} else {
			badgeTabBtn(i, 0);
		}
	}

	GUIDE_SHOW(GUIDE_UI_REFIT);

	closeLoading();
}

void RefitPanel::refitDone() {
	// 其他地方有用，所以用时载入，用完清除
	CToolUtils::loadArmatureRGBA4444("effect/ut00039/ut00039.ExportJson");
	CToolUtils::loadArmatureRGBA4444("effect/ut00080/ut00080.ExportJson");

	CCArmature* pRefitEff = CCArmature::create("ut00080");
	pRefitEff->setAnchorPoint(ccp(0.5f, 0.5f));
	pRefitEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(RefitPanel::onEffDone));
	pRefitEff->setPosition(m_pBigIconImg->getPosition());
	m_pRootLayout->addNode(pRefitEff);
	pRefitEff->getAnimation()->play("fly", -1, -1, 0); //not loop

	for(int i = 0; i < MAX_CELLS_NUM; i++) {
		string cellName = string(CELL_PREFIX_NAME) + intToString(i);
		RefitMaterialCell *pCell = dynamic_cast<RefitMaterialCell*>(m_pCellsLayout->getChildByName(cellName.c_str()));

		if(pCell != NULL && pCell->getItemID() > 0) {
			float posX = i % ROW_CELLS_NUM * (CELL_WIDTH + COL_MARGIN) + CELL_WIDTH / 2;
			float posY = m_pCellsLayout->getSize().height - i / ROW_CELLS_NUM * (CELL_WIDTH + ROW_MARGIN) - CELL_WIDTH / 2;

			m_nRefitEffCnt++;
			CCArmature* pArmature = CCArmature::create("ut00039");
			pArmature->setAnchorPoint(ccp(0.5f, 0.5f));
			pArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(RefitPanel::onEffDone));
			pArmature->setPosition(ccp(posX, posY));
			m_pCellsLayout->addNode(pArmature);
			pArmature->getAnimation()->play("fly", -1, -1, 0); //not loop
		}

	}
}

void RefitPanel::badgeTabBtn(unsigned int uPart, unsigned int uBadgeNum) {
	UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(part2Idx(uPart)));

	if(pTabBtn != NULL) {
		float offsetX = pTabBtn->getSize().width * 0.36f;
		float offsetY = pTabBtn->getSize().height * 0.18f;
		CToolUtils::badgeWidgetWithPoint(pTabBtn, uBadgeNum, offsetX, offsetY);
	}
}

void RefitPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/RefitPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(RefitPanel::touchEventHandler));

	m_pTabListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("tab_list"));
	m_pTabListView->addEventListenerListView(this, listvieweventselector(RefitPanel::listViewEventHandler));

	m_pBigIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("big_icon_img"));

	m_pCellsLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("cells_layout"));

	m_pRefitLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("refit_level_txt"));

	m_pAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_val_txt"));

	m_pDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("def_val_txt"));

	m_pHpValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hp_val_txt"));

	m_pHitValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hit_val_txt"));

	m_pCostValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cost_val_txt"));

	m_pRefitBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("refit_btn"));
	m_pRefitBtn->addTouchEventListener(this, toucheventselector(RefitPanel::touchEventHandler));

	switchTab(idx2TabType(m_nTabSelectedIdx));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void RefitPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				GUIDE_TOUCH(pWidget);
				close();
			} else if(strcmp(pszWidgetName, m_pRefitBtn->getName()) == 0) {
				m_pRefitBtn->setTouchEnabled(false);
				int nSelectedIdx = m_pTabListView->getCurSelectedIndex();
				unsigned int uPart = idx2Part(nSelectedIdx);
				P_REFIT_CTRL->getRefitDonePanel()->refitPart(uPart);
				P_REFIT_CTRL->sendRefitLvUpRqst(uPart);
			} else if(string(pszWidgetName).substr(0, string(CELL_PREFIX_NAME).size()) == CELL_PREFIX_NAME) {
				RefitMaterialCell *pCell = dynamic_cast<RefitMaterialCell*>(m_pCellsLayout->getChildByName(pszWidgetName));
				GUIDE_TOUCH(pWidget);

				if(pCell != NULL) {
					unsigned int uItemID = pCell->getItemID();

					if(pCell->getStat() == RefitMaterialCell::CELL_STAT_ADDED) {
						P_BAG_CTRL->getItemInfoPanel()->open();
						P_BAG_CTRL->getItemInfoPanel()->updateInfo(uItemID, vmsg::THING_TYPE_DOTAITEM, false);
					} else {
						int nSelectedIdx = m_pTabListView->getCurSelectedIndex();
						unsigned int uPart = idx2Part(nSelectedIdx);
						P_REFIT_CTRL->getRefitMaterialPanel()->openWithItemID(uPart, uItemID);
					}
				}
			}
			break;
		default:;
	}
}

void RefitPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_START:
			CCLOG("ios test list select start");
			break;
		case LISTVIEW_ONSELECTEDITEM_END:
			CCLOG("ios test list select end");
			if(strcmp(pszListViewName, m_pTabListView->getName()) == 0) {
				int nSelectedIdx = m_pTabListView->getCurSelectedIndex();
				switchTab(idx2TabType(nSelectedIdx));

				if(idx2Part(nSelectedIdx) == vmsg::EQUIP_POS_RING) {
					UIButton *pTabBtn = dynamic_cast<UIButton*>(m_pTabListView->getItem(nSelectedIdx));
					GUIDE_TOUCH(pTabBtn);
				}

				updateUI();
			}
			break;
		default:;
	}
}

void RefitPanel::onEffDone(CCArmature *pArmature, MovementEventType type, const char *pszActionName) {
	if(type == COMPLETE) {
		if(pArmature->getName() == "ut00080") {
			CToolUtils::removeArmatureFile("effect/ut00080/ut00080.ExportJson");
			updateUI();
			P_REFIT_CTRL->getRefitDonePanel()->open();
		}

		pArmature->removeFromParent();
		m_nRefitEffCnt--;

		if(m_nRefitEffCnt == 0) {
			CToolUtils::removeArmatureFile("effect/ut00039/ut00039.ExportJson");
			/*updateUI();
			P_REFIT_CTRL->getRefitDonePanel()->open();*/
		}
	}
}

RefitPanel::TabType RefitPanel::idx2TabType(int idx) {
	TabType type = TAB_WEAPON;

	switch(idx) {
		case 0:
			type = TAB_WEAPON;
			break;
		case 1:
			type = TAB_ARMOR;
			break;
		case 2:
			type = TAB_SHOES;
			break;
		case 3:
			type = TAB_CLOAK;
			break;
		case 4:
			type = TAB_AMULET;
			break;
		default:;
	}

	return type;
}

unsigned int RefitPanel::idx2Part(int idx) {
	unsigned int uPart = 0;

	switch(idx) {
		case 0:
			uPart = 0;
			break;
		case 1:
			uPart = vmsg::EQUIP_POS_ARMOR;
			break;
		case 2:
			uPart = vmsg::EQUIP_POS_SHOE;
			break;
		case 3:
			uPart = vmsg::EQUIP_POS_CLOTH;
			break;
		case 4:
			uPart = vmsg::EQUIP_POS_RING;
			break;
		default:;
	}

	return uPart;
}

int RefitPanel::part2Idx(unsigned int uPart) {
	int idx = 0;

	switch(uPart) {
		case 0:
			idx = 0;
			break;
		case vmsg::EQUIP_POS_ARMOR:
			idx = 1;
			break;
		case vmsg::EQUIP_POS_SHOE:
			idx = 2;
			break;
		case vmsg::EQUIP_POS_CLOTH:
			idx = 3;
			break;
		case vmsg::EQUIP_POS_RING:
			idx = 4;
			break;
		default:;
	}

	return idx;
}
