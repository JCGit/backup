#include "BeStrongerPanel.h"

#include "defines.h"

#include "CfgMaster/VIPLvCfgMaster.h"
#include "CfgMaster/VIPCfgMaster.h"
#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/PackageCfgMaster.h"

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

#define MAX_CELL_CNT 4
#define CELL_PREFIX_NAME "cell_"

BeStrongerPanel::BeStrongerPanel()
:m_pCloseBtn(NULL)
,m_pVIPIntroLayout(NULL)
,m_pBecomeTxt(NULL)
,m_pNextVIPImgTxt(NULL)
,m_pPrivilegeBtn(NULL)
,m_pChargeBtn(NULL)
,m_pVIPPackageTitleTxt(NULL)
,m_pAwardsLayout(NULL)
,m_pTopVIPLayout(NULL)
,m_pTopChargeBtn(NULL)
,m_pEntrysListView(NULL)
{
}

BeStrongerPanel::~BeStrongerPanel()
{
}

BeStrongerPanel* BeStrongerPanel::create() {
	BeStrongerPanel *pRet = new BeStrongerPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BeStrongerPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void BeStrongerPanel::open() {
	BasePanel::open();

	P_FIGHT_CTRL->setLastFightFailed(false);

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	updateUI();

	closeLoading();
}

void BeStrongerPanel::close() {
	BasePanel::close();

	P_AUTO_POP_CTRL->onClosePanel(this);
}

bool BeStrongerPanel::isMeetAutoOpen() {
	return P_FIGHT_CTRL->isLastFightFailed();
}

void BeStrongerPanel::updateUI() {
	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
	unsigned int vipLv = pUserInfo->vipLv;

	VIPLvCfg *pCurVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv);

	const VIPLvCfgSet &stVIPLvCfgSet = VIP_LV_CFG_MASTER->GetCfg();
	unsigned int uMaxVIPLv = stVIPLvCfgSet.viplvcfg(stVIPLvCfgSet.viplvcfg_size() - 1).viplv();
	bool isTopVIP = vipLv == uMaxVIPLv;

	if(!isTopVIP) {
		m_pVIPIntroLayout->setEnabled(true);
		m_pTopVIPLayout->setEnabled(false);
		m_pNextVIPImgTxt->setNum(intToString(vipLv + 1), 0.0f, "vip");

		char viptext[100];
		snprintf(viptext, 100, TextStr(TEXT_VIP_GIFT), vipLv + 1);
		m_pVIPPackageTitleTxt->setText(vipLv == 0 ? TextStr(TEXT_FIRST_CHARGE_PKG) : viptext);

		m_pAwardsLayout->removeAllChildren();

		VIPLvCfg *pNextVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv + 1);
		unsigned int uPkgID = vipLv == 0 ? GLOBAL_CFG_MASTER->GetCfg().firstchargegiftpkg() : pNextVIPLvCfg->packageid();
		const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPkgID);
		int nShowCellCnt = pPackageCfg->elmtlst_size() > MAX_CELL_CNT ? MAX_CELL_CNT : pPackageCfg->elmtlst_size();
		LinearLayoutParameter *pLayoutParameter = LinearLayoutParameter::create();
		pLayoutParameter->setGravity(LINEAR_GRAVITY_CENTER_VERTICAL);
		pLayoutParameter->setMargin(Margin(0, 0, 20, 0));

		if(pPackageCfg != NULL) {
			for(int i = 0; i < nShowCellCnt; i++) {
				IconFrameCell *pCell = IconFrameCell::create();
				pCell->setTouchEnabled(true);
				string cellName = string(CELL_PREFIX_NAME) + intToString(i);
				pCell->setName(cellName.c_str());
				pCell->setLayoutParameter(pLayoutParameter);

				if(i < pPackageCfg->elmtlst_size()) {
					const PackageElmt &stElmt = pPackageCfg->elmtlst(i);
					pCell->setInfo(stElmt.elementtype(), stElmt.elementid(), stElmt.elementcntmin());
					m_pAwardsLayout->addChild(pCell);
					pCell->addTouchEventListener(this, toucheventselector(BeStrongerPanel::touchEventHandler));
				}
			}
		}

	} else {
		m_pVIPIntroLayout->setEnabled(false);
		m_pTopVIPLayout->setEnabled(true);
	}

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
}

void BeStrongerPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/BeStrongerPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(BeStrongerPanel::touchEventHandler));

	m_pVIPIntroLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("vip_intro_layout"));

	m_pBecomeTxt = dynamic_cast<UILabel*>(m_pVIPIntroLayout->getChildByName("become_txt"));

	m_pNextVIPImgTxt = ImageNumber::create(25);
	m_pNextVIPImgTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pNextVIPImgTxt->setPosition(m_pBecomeTxt->getPosition() + ccp(m_pBecomeTxt->getSize().width + 8, 0));
	m_pVIPIntroLayout->addNode(m_pNextVIPImgTxt);
	m_pNextVIPImgTxt->setNum("0", 0.0f, "vip");

	m_pPrivilegeBtn = dynamic_cast<UIButton*>(m_pVIPIntroLayout->getChildByName("privilege_btn"));
	m_pPrivilegeBtn->addTouchEventListener(this, toucheventselector(BeStrongerPanel::touchEventHandler));

	m_pChargeBtn = dynamic_cast<UIButton*>(m_pVIPIntroLayout->getChildByName("charge_btn"));
	m_pChargeBtn->addTouchEventListener(this, toucheventselector(BeStrongerPanel::touchEventHandler));

	m_pVIPPackageTitleTxt = dynamic_cast<UILabel*>(m_pVIPIntroLayout->getChildByName("vip_package_title_txt"));

	m_pAwardsLayout = dynamic_cast<UILayout*>(m_pVIPIntroLayout->getChildByName("awards_layout"));

	m_pTopVIPLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("top_vip_layout"));
	m_pTopVIPLayout->setVisible(true);

	m_pTopChargeBtn = dynamic_cast<UIButton*>(m_pTopVIPLayout->getChildByName("charge_btn"));
	m_pTopChargeBtn->addTouchEventListener(this, toucheventselector(BeStrongerPanel::touchEventHandler));

	m_pEntrysListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("entrys_list"));
	m_pEntrysListView->addEventListenerListView(this, listvieweventselector(BeStrongerPanel::listViewEventHandler));

	m_bUIInited = true;

}

void BeStrongerPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			} else if(pSender == m_pPrivilegeBtn) {
				close();
				P_VIP_CTRL->getVIPPanel()->open();
			} else if(pSender == m_pChargeBtn) {
				close();
				P_SHOP_CTRL->getRmbShopPanel()->open();
			} else if(string(pszWidgetName).find(CELL_PREFIX_NAME) == 0) {
				IconFrameCell *pCell = dynamic_cast<IconFrameCell*>(pWidget);

				if(pCell != NULL) {
					P_TIP_CTRL->showItemTipPanel(pCell->getType(), pCell->getCfgID(), pCell->getCnt(), false);
				}
			} else if(pSender == m_pTopChargeBtn) {
				close();
				P_SHOP_CTRL->getRmbShopPanel()->open();
			}
			break;
		default:;
	}
}

void BeStrongerPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(pSender == m_pEntrysListView) {
				int nSelectedIdx = m_pEntrysListView->getCurSelectedIndex();
				UIButton *pBtn = dynamic_cast<UIButton*>(m_pEntrysListView->getItem(nSelectedIdx));

				if(pBtn != NULL) {
					string btnName = pBtn->getName();
					unsigned int uEntryID = atol(btnName.substr(string(ENTRY_PREFIX_NAME).size()).c_str());
					
					if(uEntryID != ENTRY_UNKNOWN) {
                        if(uEntryID != ENTRY_LEVEL_UP) {
                            close();
                        }
						P_ENTRY_CTRL->enter(uEntryID);
					}
				}
			}
			break;
		default:;
	}
}
