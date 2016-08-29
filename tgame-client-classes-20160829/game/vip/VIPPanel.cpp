#include "VIPPanel.h"

#include "UserObj.h"
#include "defines.h"
#include "VIPLvCfgMaster.h"
#include "ToolUtils.h"
#include "VIPDetailLayout.h"
#include "VIPControl.h"
#include "ImageNumber.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"

VIPPanel::VIPPanel()
:m_pVIPProgress(NULL)
,m_pVIPProgressTxt(NULL)
,m_pChargeTxt(NULL)
,m_pChargeValTxt(NULL)
,m_pIconGoldImg(NULL)
,m_pVIPLevelupTxt(NULL)
,m_pChargeBtn(NULL)
,m_pFlipLeftBtn(NULL)
,m_pFlipRightBtn(NULL)
,m_pDetailLayout(NULL)
,m_uMaxVIPLv(0)
,m_uShowVIPLv(0)
{
    
}

VIPPanel::~VIPPanel()
{
}

VIPPanel* VIPPanel::create() {
	VIPPanel *pRet = new VIPPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool VIPPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

        setOpenWithBgImg(true);
        setShowMoneyTopBar(true);

		initLayoutFromJsonFile("ui_v2/vip_panel_ui.ExportJson");
		addChild(m_pRootLayout);

        UIButton* pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        pCloseBtn->addTouchEventListener(this, toucheventselector(VIPPanel::touchEventHandler));

        m_pCurVIPLvImgNum = ImageNumber::create(25);
        m_pCurVIPLvImgNum->setAnchorPoint(ccp(0.0f, 0.0f));
        UILabel* pTmpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cur_vip_txt"));
        CCPoint pt = pTmpTxt->getPosition();
        pt.x = pt.x + pTmpTxt->getSize().width + 10;
        m_pCurVIPLvImgNum->setPosition(pt);
        m_pRootLayout->addNode(m_pCurVIPLvImgNum);
        m_pCurVIPLvImgNum->setNum("0", 2.0f, "vip");


		m_pVIPProgress = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("vip_progress"));
		m_pVIPProgressTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("vip_progress_txt"));
		m_pChargeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("charge_txt"));
		m_pChargeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("charge_val_txt"));
		m_pIconGoldImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_gold_img"));
		m_pVIPLevelupTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("levelup_txt"));

        //m_pNextVIPLvTxt = dynamic_cast<UILabelAtlas*>(m_pRootLayout->getChildByName("next_vip_lv_txt"));
        m_pNextVIPLvImgNum = ImageNumber::create(25);
        m_pNextVIPLvImgNum->setAnchorPoint(ccp(0.0f, 0.0f));
        pt = m_pVIPLevelupTxt->getPosition();
        pt.x = pt.x + m_pVIPLevelupTxt->getSize().width + 10;
        m_pNextVIPLvImgNum->setPosition(pt);
        m_pRootLayout->addNode(m_pNextVIPLvImgNum);
        m_pNextVIPLvImgNum->setNum("0", 2.0f, "vip");

		m_pChargeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("charge_btn"));
		m_pChargeBtn->addTouchEventListener(this, toucheventselector(VIPPanel::touchEventHandler));

		m_pFlipLeftBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("flip_left_btn"));
		m_pFlipLeftBtn->addTouchEventListener(this, toucheventselector(VIPPanel::touchEventHandler));

		m_pFlipRightBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("flip_right_btn"));
		m_pFlipRightBtn->addTouchEventListener(this, toucheventselector(VIPPanel::touchEventHandler));

		m_pDetailLayout = VIPDetailLayout::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("vip_detail_layout")));

		const VIPLvCfgSet &stVIPLvCfgSet = VIP_LV_CFG_MASTER->GetCfg();
		m_uMaxVIPLv = stVIPLvCfgSet.viplvcfg(stVIPLvCfgSet.viplvcfg_size() - 1).viplv();

		bRet = true;
	} while(0);

	return bRet;
}

void VIPPanel::open() {
	BasePanel::open();

	P_VIP_CTRL->sendQueryVIPInfoRqst();

	this->showLoading();
}

void VIPPanel::updateUI() {
	if(this->getIsInLoading()) {
		this->closeLoading();
	}

	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();

	unsigned int vipLv = pUserInfo->vipLv;

    m_pCurVIPLvImgNum->setNum(intToString(vipLv), 2.0f, "vip");

	VIPLvCfg *pCurVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv);

	const VIPLvCfgSet &stVIPLvCfgSet = VIP_LV_CFG_MASTER->GetCfg();
	unsigned int uMaxVIPLv = stVIPLvCfgSet.viplvcfg(stVIPLvCfgSet.viplvcfg_size() - 1).viplv();

	unsigned int uTotalGold = P_USER_OBJ->getUserInfo()->totalGoldByMoney;
	bool isTopVIP = vipLv == uMaxVIPLv;

	if(!isTopVIP) {
		VIPLvCfg *pNextVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv + 1);
		unsigned int uTotalNeedGold = pNextVIPLvCfg->needgold();
		unsigned int uNowNeedGold = pNextVIPLvCfg->needgold() - P_USER_OBJ->getUserInfo()->totalGoldByMoney;
		m_pChargeValTxt->setText(intToString(uNowNeedGold));
		m_pVIPProgress->setPercent(100 * uTotalGold / uTotalNeedGold);
		m_pVIPProgressTxt->setText(intToString(uTotalGold) + "/" + intToString(uTotalNeedGold));
		m_pIconGoldImg->setPositionX(m_pChargeValTxt->getPositionX() + m_pChargeValTxt->getSize().width + 32);
        m_pNextVIPLvImgNum->setNum(intToString(vipLv + 1), 2.0f, "vip");
		showVIPLv(vipLv);
	} else {

        m_pNextVIPLvImgNum->setNum(intToString(uMaxVIPLv), 2.0f, "vip");
		m_pVIPProgress->setPercent(100);
		m_pVIPProgressTxt->setText(intToString(uTotalGold));
		showVIPLv(uMaxVIPLv);
	}

	m_pDetailLayout->updateAwardCell();
	
	m_pChargeTxt->setVisible(!isTopVIP);
	m_pChargeValTxt->setVisible(!isTopVIP);
	m_pIconGoldImg->setVisible(!isTopVIP);
	m_pVIPLevelupTxt->setVisible(!isTopVIP);
	m_pNextVIPLvImgNum->setVisible(!isTopVIP);

}

void VIPPanel::showVIPLv(const unsigned int uVIPLv) {
	if(uVIPLv >= 0 && uVIPLv <= m_uMaxVIPLv) {
		m_uShowVIPLv = uVIPLv;
		m_pDetailLayout->updateUI(uVIPLv);

		m_pFlipLeftBtn->setBright(uVIPLv > 0);
		m_pFlipLeftBtn->setBrightStyle(BRIGHT_NORMAL);
		m_pFlipLeftBtn->setTouchEnabled(uVIPLv > 0);

		m_pFlipRightBtn->setBright(uVIPLv < m_uMaxVIPLv);
		m_pFlipRightBtn->setBrightStyle(BRIGHT_NORMAL);
		m_pFlipRightBtn->setTouchEnabled(uVIPLv < m_uMaxVIPLv);
	}
}

void VIPPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pChargeBtn->getName()) == 0) {
				// ³äÖµÈë¿Ú
                close();
                P_SHOP_CTRL->getRmbShopPanel()->open();
			} else if(std::strcmp(pszWidgetName, m_pFlipLeftBtn->getName()) == 0) {
                showVIPLv(m_uShowVIPLv - 1);
				
			} else if(std::strcmp(pszWidgetName, m_pFlipRightBtn->getName()) == 0) {
				showVIPLv(m_uShowVIPLv + 1);
			} else if(std::strcmp(pszWidgetName, "close_btn") == 0) {
                close();
            }
			break;
		default:;
	}
}
