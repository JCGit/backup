#include "FailPanel.h"

#include "defines.h"

#include "CfgMaster/VIPLvCfgMaster.h"
#include "CfgMaster/VIPCfgMaster.h"
#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/PackageCfgMaster.h"

#include "basic/SceneManager.h"
#include "basic/TextHelper.h"
#include "basic/ImageNumber.h"

#include "game/UserObj.h"
#include "game/IconFrameCell.h"
#include "game/tip/TipControl.h"
#include "game/shop/ShopControl.h"
#include "game/shop/RmbShopPanel.h"
#include "game/city/EntryControl.h"
#include "game/fight/FightControl.h"
#include "game/city/AutoPopControl.h"
#include "game/vip/VIPControl.h"
#include "game/city/CityControl.h"

#define MAX_CELL_CNT 4

CFailPanel::CFailPanel()
:m_pExitBtn(NULL)
,m_pEntrysListView(NULL)
,m_pChargeBtn(NULL)
,m_pBecomeTxt(NULL)
,m_pVIPImgTxt(NULL)
,m_pOwnTxt(NULL)
,m_pPrivilegeTxt(NULL)
,m_pBgPackageTitleImg(NULL)
,m_pPackageTitleTxt(NULL)
,m_pElmtsListView(NULL)
{

}

CFailPanel::~CFailPanel(){

}

CFailPanel* CFailPanel::create() {
	CFailPanel *pRet = new CFailPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CFailPanel::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while (0);

	return bRet;
}

void CFailPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	updateUI();

	closeLoading();
}

void CFailPanel::updateUI() {
	m_pEntrysListView->removeAllItems();

	unsigned int entrys[] = {
		ENTRY_GENE_STRENGTHEN,
		ENTRY_WEAPON_LIB,
		ENTRY_EQUIP_CULTURE,
		ENTRY_REFIT,
		ENTRY_EVOLUTION,
	};

	for(unsigned int i = 0; i < sizeof(entrys) / sizeof(unsigned int); i++) {
		unsigned int uEntryID = entrys[i];

		if(P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			UIButton *pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
			m_pEntrysListView->pushBackCustomItem(pBtn);
		}
	}

	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
	unsigned int vipLv = pUserInfo->vipLv;

	VIPLvCfg *pCurVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv);

	const VIPLvCfgSet &stVIPLvCfgSet = VIP_LV_CFG_MASTER->GetCfg();
	unsigned int uMaxVIPLv = stVIPLvCfgSet.viplvcfg(stVIPLvCfgSet.viplvcfg_size() - 1).viplv();
	bool isTopVIP = vipLv == uMaxVIPLv;

	if(!isTopVIP) {
		m_pVIPImgTxt->setNum(intToString(vipLv + 1), 0.0f, "vip");

		char viptext[100];
		snprintf(viptext, 100, TextStr(TEXT_VIP_GIFT), vipLv + 1);
		m_pPackageTitleTxt->setText(vipLv == 0 ? TextStr(TEXT_FIRST_CHARGE_PKG) : viptext);

		m_pElmtsListView->removeAllItems();

		VIPLvCfg *pNextVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv + 1);
		unsigned int uPkgID = P_USER_OBJ->getUserInfo()->totalGoldByMoney == 0 && vipLv == 0 ? GLOBAL_CFG_MASTER->GetCfg().firstchargegiftpkg() : pNextVIPLvCfg->packageid();
		const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPkgID);

		if(pPackageCfg != NULL) {
			int nShowCellCnt = pPackageCfg->elmtlst_size() > MAX_CELL_CNT ? MAX_CELL_CNT : pPackageCfg->elmtlst_size();

			for(int i = 0; i < nShowCellCnt; i++) {
				IconFrameCell *pCell = IconFrameCell::create();
				pCell->setTouchEnabled(true);
				const PackageElmt &stElmt = pPackageCfg->elmtlst(i);
				pCell->setInfo(stElmt.elementtype(), stElmt.elementid(), stElmt.elementcntmin());
				m_pElmtsListView->pushBackCustomItem(pCell);
			}
		}

	}

	m_pChargeBtn->setEnabled(!isTopVIP);
	m_pBecomeTxt->setEnabled(!isTopVIP);
	m_pVIPImgTxt->setVisible(!isTopVIP);
	m_pOwnTxt->setEnabled(!isTopVIP);
	m_pPrivilegeTxt->setEnabled(!isTopVIP);
	m_pBgPackageTitleImg->setEnabled(!isTopVIP);
	m_pPackageTitleTxt->setEnabled(!isTopVIP);
	m_pElmtsListView->setEnabled(!isTopVIP);
}

void CFailPanel::initUI(){
	initLayoutFromJsonFile("ui_v2/fail_panel_ui.ExportJson");
	addChild(m_pRootLayout);

	m_pExitBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("exit_btn"));
	m_pExitBtn->addTouchEventListener(this, toucheventselector(CFailPanel::touchEventHandler));

	m_pEntrysListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("entrys_list"));
	m_pEntrysListView->addEventListenerListView(this, listvieweventselector(CFailPanel::listViewEventhandler));

	m_pChargeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("charge_btn"));
	m_pChargeBtn->addTouchEventListener(this, toucheventselector(CFailPanel::touchEventHandler));

	m_pBecomeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("become_txt"));

	m_pVIPImgTxt = ImageNumber::create(25);
	m_pVIPImgTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pVIPImgTxt->setPosition(m_pBecomeTxt->getPosition() + ccp(m_pBecomeTxt->getSize().width + 8, 0));
	m_pRootLayout->addNode(m_pVIPImgTxt);
	m_pVIPImgTxt->setNum("0", 0.0f, "vip");

	m_pOwnTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("own_txt"));

	m_pPrivilegeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("privilege_txt"));

	m_pBgPackageTitleImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_package_title_img"));

	m_pPackageTitleTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("package_title_txt"));

	m_pElmtsListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("elmts_list"));
	m_pElmtsListView->addEventListenerListView(this, listvieweventselector(CFailPanel::listViewEventhandler));

	m_bUIInited = true;
}

void CFailPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pExitBtn) {
				exit();
			} else if(pSender == m_pChargeBtn) {
				exit();
				P_CITY_CTRL->setDelayedEntryID(ENTRY_SHOP);
			}
			break;
		default:;
	}
}

void CFailPanel::listViewEventhandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(pSender == m_pEntrysListView) {
				exit();
				int nSelectedIdx = m_pEntrysListView->getCurSelectedIndex();
				UIButton *pBtn = dynamic_cast<UIButton*>(m_pEntrysListView->getItem(nSelectedIdx));

				if(pBtn != NULL) {
					string btnName = pBtn->getName();
					unsigned int uEntryID = atol(btnName.substr(string(ENTRY_PREFIX_NAME).size()).c_str());

					P_CITY_CTRL->setDelayedEntryID(uEntryID);
				}
			} else if(pSender == m_pElmtsListView) {
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

void CFailPanel::exit(){
	close();
	SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
}