#include "ActChargeLayout.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "ActControl.h"
#include "Macros.pb.h"
#include "UserObj.h"
#include "IconCell.h"
#include "defines.h"
#include "tip/TipControl.h"

#include "GlobalCfgMaster.h"
#include "ActCfgMaster.h"
#include "PackageCfgMaster.h"

CActChargeLayout::CActChargeLayout(){
	m_pCellArray = NULL;
	m_pUICellArray = NULL;
	m_pRefLayout = NULL;
	m_uPkgType = 0;
	m_uState = 0;
}

CActChargeLayout::~CActChargeLayout(){
	if(m_pCellArray) {
		m_pCellArray->removeAllObjects();
		m_pCellArray->release();
	}

	if(m_pUICellArray) {
		m_pUICellArray->removeAllObjects();
		m_pUICellArray->release();
	}
}

bool CActChargeLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CActChargeLayout::initLayout(UILayout* pRefLayout){
	if(NULL == pRefLayout) {
		return;
	}
	m_pRefLayout = pRefLayout;


	m_pGetBtn = dynamic_cast<Button*>(m_pRefLayout->getChildByName("get_btn"));
	m_pBtnImg = dynamic_cast<ImageView*>(m_pGetBtn->getChildByName("label_img"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(CActChargeLayout::touchEventHandler));

	m_pCellArray = CCArray::create();
	m_pCellArray->retain();
	m_pUICellArray = CCArray::create();
	m_pUICellArray->retain();

	UIImageView *pImg;
	IconCell *stCell;
	for(int i = 0; i < MAX_ITEM_NUM; ++i) {
		string strUIName = "cell_" + intToString(i);
		pImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName(strUIName.c_str()));
		stCell = IconCell::create();
		stCell->showNumTxt();
		pImg->addChild(stCell);
		pImg->setVisible(false);
		pImg->setTouchEnabled(true);
		m_pCellArray->addObject(stCell);
		m_pUICellArray->addObject(pImg);
		pImg->addTouchEventListener(this, toucheventselector(CActChargeLayout::touchEventHandler));
	}
}

void CActChargeLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {
	setInfo(info);
}

void CActChargeLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strName = pszWidgetName;

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, m_pGetBtn->getName()) == 0) {
			doGet();
		}else if(strName.find("cell_") != string::npos){
			string sub = strName.substr(strName.find_first_not_of("cell_"));
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

void CActChargeLayout::setInfo(const CSCouponActStatusItem& stMsg){
	m_uPkgType = stMsg.type();
	m_uState = stMsg.status();

	const ActCfg* stCfg = ACT_CFG_MASTER->GetActCfgByType(m_uPkgType);
	if(stCfg == NULL){
		return;
	}

	string str = "act_ui/";
	if(stCfg->type() == FIRST_BUY_PKG){   //平台特权礼包
		if(m_uState == 0){  //未领取
			if(P_USER_OBJ->getUserInfo()->totalGoldByMoney <= 0){  //未充值
				str += "charge_btn_label.png";
			}else{
				str += "claim_btn_label.png";
			}
		}else{
			str += "claimed_btn_label.png";
			m_pGetBtn->setTouchEnabled(false);
		}
	}

	m_pBtnImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);

	unsigned int uPkgID = GLOBAL_CFG_MASTER->GetCfg().firstchargegiftpkg();
	const PackageCfg* stPkgCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPkgID);
	if(stPkgCfg == NULL){
		return;
	}

	IconCell *stCell;
	UIImageView *pImg;
	for(int i = 0; i < MAX_ITEM_NUM; i++){
		stCell = dynamic_cast<IconCell*>(m_pCellArray->objectAtIndex(i));
		pImg = dynamic_cast<UIImageView*>(m_pUICellArray->objectAtIndex(i));
		pImg->setVisible(true);
		pImg->setTouchEnabled(true);
		if(i > stPkgCfg->elmtlst_size()){
			stCell->clearCell();
			pImg->setVisible(false);
			pImg->setTouchEnabled(false);
			continue;
		}

		PackageElmt stElmt = stPkgCfg->elmtlst(i);
		stCell->setInfo(stElmt.elementtype(), stElmt.elementcntmin(), stElmt.elementid());
	}
}

void CActChargeLayout::doGet(){
	if(m_uState != 0){
		return;
	}

	if(P_USER_OBJ->getUserInfo()->totalGoldByMoney <= 0){  //未充值
		P_ACT_CTRL->getActPanel()->close();
		P_SHOP_CTRL->getRmbShopPanel()->open();
	}else{
		P_ACT_CTRL->sendClaimFirstChargePkgRqst();
	}
}

void CActChargeLayout::setLayoutEnabel(bool bIsEnabel){
	m_pRefLayout->setVisible(bIsEnabel);
	m_pRefLayout->setEnabled(bIsEnabel);
	m_pGetBtn->setTouchEnabled(bIsEnabel);

	//UIImageView *pImg;
	//for(int i = 0; i < MAX_ITEM_NUM; i++){
	//	pImg = dynamic_cast<UIImageView*>(m_pUICellArray->objectAtIndex(i));
	//	if(pImg->isVisible()){
	//		pImg->setTouchEnabled(bIsEnabel);
	//	}
	//}
}

void CActChargeLayout::initUI() {
	initLayoutFromJsonFile("ui_v2/ActChargeLayout.ExportJson");
	addChild(m_pRootLayout);

	m_pGetBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("get_btn"));
	m_pBtnImg = dynamic_cast<ImageView*>(m_pGetBtn->getChildByName("label_img"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(CActChargeLayout::touchEventHandler));

	m_pCellArray = CCArray::create();
	m_pCellArray->retain();
	m_pUICellArray = CCArray::create();
	m_pUICellArray->retain();

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
		pImg->addTouchEventListener(this, toucheventselector(CActChargeLayout::touchEventHandler));
	}

	m_bUIInited = true;
}
