#include "PackageInfoPanel.h"

#include "defines.h"

#include "ClientSvrMsg.pb.h"

#include "CfgMaster/PackageCfgMaster.h"

#include "basic/TextHelper.h"
#include "basic/ItemUtils.h"

#include "game/IconFrameCell.h"
#include "game/tip/TipControl.h"

#include "BagControl.h"

#define MAX_ELMT_CNT 5
#define CELL_PREFIX_NAME "cell_"

PackageInfoPanel::PackageInfoPanel()
:m_pCloseBtn(NULL)
,m_pCongratulationsTxt(NULL)
,m_pElmtsListView(NULL)
,m_pOpenBtn(NULL)
,m_uPackageID(0)
{
}

PackageInfoPanel::~PackageInfoPanel()
{
}

PackageInfoPanel* PackageInfoPanel::create() {
	PackageInfoPanel *pRet = new PackageInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool PackageInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void PackageInfoPanel::showPackage(unsigned int uPackageID) {
	open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	m_pElmtsListView->removeAllItems();
	const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPackageID);

	if(pPackageCfg != NULL) {
		m_uPackageID = uPackageID;
		string pkgname = ItemUtils::getItemName(vmsg::THING_TYPE_ITEM, uPackageID);
		char text[100];
		snprintf(text, 100, TextStr(TEXT_CONGRATULATIONS_TO_GET), pkgname.c_str());
		m_pCongratulationsTxt->setText(text);

		int nShowCellCnt = pPackageCfg->elmtlst_size() > MAX_ELMT_CNT ? MAX_ELMT_CNT : pPackageCfg->elmtlst_size();

		for(int i = 0; i < nShowCellCnt; i++) {
			IconFrameCell *pCell = IconFrameCell::create();
			pCell->setTouchEnabled(true);
			string cellName = string(CELL_PREFIX_NAME) + intToString(i);
			pCell->setName(cellName.c_str());

			const PackageElmt &stElmt = pPackageCfg->elmtlst(i);
			pCell->setInfo(stElmt.elementtype(), stElmt.elementid(), stElmt.elementcntmin());
			m_pElmtsListView->pushBackCustomItem(pCell);
		}
	}

	closeLoading();
}

void PackageInfoPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/PackageInfoPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(PackageInfoPanel::touchEventHandler));

	m_pCongratulationsTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("congratulations_txt"));

	m_pElmtsListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("elmts_list"));
	m_pElmtsListView->addEventListenerListView(this, listvieweventselector(PackageInfoPanel::listViewEventHandler));

	m_pOpenBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("open_btn"));
	m_pOpenBtn->addTouchEventListener(this, toucheventselector(PackageInfoPanel::touchEventHandler));

	m_bUIInited = true;
}

void PackageInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			} else if(pSender == m_pOpenBtn) {
				close();
				
				if(m_uPackageID != 0) {
					P_BAG_CTRL->sendOpenPackageRqst(P_BAG_CTRL->getIdxByItemCfgID(m_uPackageID), m_uPackageID);
				}
			}
			break;
		default:;
	}
}

void PackageInfoPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(pSender == m_pElmtsListView) {
				int nSelectedIdx = m_pElmtsListView->getCurSelectedIndex();
				IconFrameCell *pCell = dynamic_cast<IconFrameCell*>(m_pElmtsListView->getItem(nSelectedIdx));

				if(pCell != NULL) {
					P_TIP_CTRL->showItemTipPanel(pCell->getType(), pCell->getCfgID(), pCell->getCnt(), false);
				}
			}
			break;
		default:;
	}
}
