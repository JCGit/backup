#include "FirstChargePanel.h"

#include "defines.h"

#include "ModuleID.pb.h"

#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/PackageCfgMaster.h"

#include "game/IconCell.h"
#include "game/UserObj.h"
#include "game/Global.h"
#include "game/tip/TipControl.h"
#include "game/city/AutoPopControl.h"
#include "game/city/CityControl.h"
#include "game/shop/ShopControl.h"
#include "game/shop/RmbShopPanel.h"
#include "game/notification/NotificationControl.h"

#define MAX_ITEM_NUM 4

FirstChargePanel::FirstChargePanel()
:m_pCloseBtn(NULL)
,m_pCellArray(NULL)
,m_pUICellArray(NULL)
,m_pGetBtn(NULL)
,m_pBtnImg(NULL)
,m_uPkgType(0)
,m_uState(0)
{
}

FirstChargePanel::~FirstChargePanel()
{
	if(m_pCellArray) {
		m_pCellArray->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_pCellArray);
	}

	if(m_pUICellArray) {
		m_pUICellArray->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_pUICellArray);
	}
}

FirstChargePanel* FirstChargePanel::create() {
	FirstChargePanel *pRet = new FirstChargePanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool FirstChargePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void FirstChargePanel::open() {
	BasePanel::open();

	if(!m_bUIInited) {
		showLoading();
		initUI();

		unsigned int uPkgID = GLOBAL_CFG_MASTER->GetCfg().firstchargegiftpkg();
		const PackageCfg* pPkgCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPkgID);
		if(pPkgCfg == NULL){
			return;
		}

		IconCell *pCell;
		UIImageView *pImg;

		for(int i = 0; i < MAX_ITEM_NUM; i++){
			pCell = dynamic_cast<IconCell*>(m_pCellArray->objectAtIndex(i));
			pImg = dynamic_cast<UIImageView*>(m_pUICellArray->objectAtIndex(i));
			pImg->setVisible(true);
			pImg->setTouchEnabled(true);

			if(i > pPkgCfg->elmtlst_size()){
				pCell->clearCell();
				pImg->setVisible(false);
				pImg->setTouchEnabled(false);
				continue;
			}

			PackageElmt stElmt = pPkgCfg->elmtlst(i);
			pCell->setInfo(stElmt.elementtype(), stElmt.elementcntmin(), stElmt.elementid());
		}

		closeLoading();
	}
}

void FirstChargePanel::close() {
	BasePanel::close();

	P_CITY_CTRL->getCityScreenView()->updateEntry();

	if(isMeetAutoOpen()) {
		PUSH_NOTIFICATION(ENTRY_FIRST_CHARGE_PKG, NOTIFY_TAG_FIRST_CHARGE_PKG, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_FIRST_CHARGE_PKG, NOTIFY_TAG_FIRST_CHARGE_PKG, 0);
	}

	P_AUTO_POP_CTRL->onClosePanel(this);
}

bool FirstChargePanel::isMeetAutoOpen() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_FIRST_CHARGE_PKG)); // 没开放

		// month card
		const vmsg::CSMonthCardInfo &stMonthCardInfo = P_USER_OBJ->getUserInfo()->stMonthCardInfo;
		unsigned int uSvrTime = P_GLOBAL->getSvrTime();
		unsigned int uEndTime = stMonthCardInfo.endtime();
		CC_BREAK_IF(uEndTime > uSvrTime); // 月卡没过期

		CC_BREAK_IF(P_USER_OBJ->getUserInfo()->totalGoldByMoney > 0); // 已充值过

		bRet = true;
	} while(0);

	return bRet;
}

void FirstChargePanel::initUI() {
	initLayoutFromJsonFile("ui_v2/FirstChargePanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(FirstChargePanel::touchEventHandler));

	m_pGetBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("get_btn"));
	m_pBtnImg = dynamic_cast<ImageView*>(m_pGetBtn->getChildByName("label_img"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(FirstChargePanel::touchEventHandler));

	m_pCellArray = CCArray::create();
	CC_SAFE_RETAIN(m_pCellArray);
	m_pUICellArray = CCArray::create();
	CC_SAFE_RETAIN(m_pUICellArray);

	UIImageView *pImg;
	IconCell *stCell;

	for(int i = 0; i < MAX_ITEM_NUM; ++i) {
		string strUIName = "cell_" + intToString(i);
		pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(strUIName.c_str()));
		stCell = IconCell::create();
		stCell->showNumTxt();
		pImg->addChild(stCell);
		pImg->setVisible(false);
		pImg->setTouchEnabled(true);
		m_pCellArray->addObject(stCell);
		m_pUICellArray->addObject(pImg);
		pImg->addTouchEventListener(this, toucheventselector(FirstChargePanel::touchEventHandler));
	}

	m_bUIInited = true;
}

void FirstChargePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			} else if(pSender == m_pGetBtn) {
				P_SHOP_CTRL->getRmbShopPanel()->open();
			} else if(string(pszWidgetName).find("cell_") == 0){
				string sub = string(pszWidgetName).substr(string(pszWidgetName).find_first_not_of("cell_"));
				unsigned int iIdx = atoi(sub.c_str());

				if(iIdx >= 0 && iIdx < m_pCellArray->count()){
					IconCell* pCell = dynamic_cast<IconCell*>(m_pCellArray->objectAtIndex(iIdx));
					P_TIP_CTRL->showItemTipPanel(pCell->getItemType(), pCell->getItemCfgid(), pCell->getItemNum(), false);
				}
			}
			break;
		default:;
	}
}
