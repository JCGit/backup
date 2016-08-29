#include "RefitDonePanel.h"

#include "defines.h"

#include "RefitControl.h"

RefitDonePanel::RefitDonePanel()
:m_pCloseBtn(NULL)
,m_pItemImg(NULL)
,m_pAtkValTxt(NULL)
,m_pDefValTxt(NULL)
,m_pHpValTxt(NULL)
,m_pHitValTxt(NULL)
,m_pAtkCurValTxt(NULL)
,m_pDefCurValTxt(NULL)
,m_pHpCurValTxt(NULL)
,m_pHitCurValTxt(NULL)
,m_uPart(0)
,m_uAtkVal(0)
,m_uDefVal(0)
,m_uHpVal(0)
,m_uHitVal(0)
{
}

RefitDonePanel::~RefitDonePanel()
{
}

RefitDonePanel* RefitDonePanel::create() {
	RefitDonePanel *pRet = new RefitDonePanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RefitDonePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void RefitDonePanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	updateUI();
}

void RefitDonePanel::refitPart(unsigned int uPart) {
	m_uPart = uPart;
	const vmsg::CSDotaPartInfo &stPartInfo = P_REFIT_CTRL->getPartInfo();
	
	if(stPartInfo.ByteSize() > 0) {
		const vmsg::CSDotaPart &stPart = stPartInfo.parts(uPart);
		m_uAtkVal = stPart.atk();
		m_uDefVal = stPart.def();
		m_uHpVal = stPart.hp();
		m_uHitVal = stPart.hit();
	}
}

void RefitDonePanel::updateUI() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	switch(m_uPart) {
		case vmsg::EQUIP_POS_RING:
			m_pItemImg->loadTexture("refit/icon_amulet_x.png", UI_TEX_TYPE_PLIST);
			break;
		case vmsg::EQUIP_POS_ARMOR:
			m_pItemImg->loadTexture("refit/icon_armor_x.png", UI_TEX_TYPE_PLIST);
			break;
		case vmsg::EQUIP_POS_CLOTH:
			m_pItemImg->loadTexture("refit/icon_cloak_x.png", UI_TEX_TYPE_PLIST);
			break;
		case vmsg::EQUIP_POS_SHOE:
			m_pItemImg->loadTexture("refit/icon_shoes_x.png", UI_TEX_TYPE_PLIST);
			break;
		default:
			m_pItemImg->loadTexture("refit/icon_weapon_x.png", UI_TEX_TYPE_PLIST);
	}

	m_pAtkValTxt->setText(intToString(m_uAtkVal));
	m_pDefValTxt->setText(intToString(m_uDefVal));
	m_pHpValTxt->setText(intToString(m_uHpVal));
	m_pHitValTxt->setText(intToString((unsigned int)(floor(m_uHitVal * 2.3f + 0.5f))));

	const vmsg::CSDotaPartInfo &stPartInfo = P_REFIT_CTRL->getPartInfo();

	if(stPartInfo.ByteSize() > 0) {
		const vmsg::CSDotaPart &stPart = stPartInfo.parts(m_uPart);

		m_pAtkCurValTxt->setText(intToString(stPart.atk()));
		m_pDefCurValTxt->setText(intToString(stPart.def()));
		m_pHpCurValTxt->setText(intToString(stPart.hp()));
		m_pHitCurValTxt->setText(intToString((unsigned int)(floor(stPart.hit() * 2.3f + 0.5f))));
	}
	
	closeLoading();
}

void RefitDonePanel::initUI() {
	initLayoutFromJsonFile("ui_v2/RefitDonePanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(RefitDonePanel::touchEventHandler));

	m_pItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_img"));
	m_pItemImg->ignoreContentAdaptWithSize(false);
	m_pItemImg->setSize(CCSizeMake(100, 100));

	m_pAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_val_txt"));

	m_pDefValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("def_val_txt"));

	m_pHpValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hp_val_txt"));

	m_pHitValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hit_val_txt"));

	m_pAtkCurValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_cur_val_txt"));

	m_pDefCurValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("def_cur_val_txt"));

	m_pHpCurValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hp_cur_val_txt"));

	m_pHitCurValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("hit_cur_val_txt"));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void RefitDonePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			}
			break;
		default:;
	}
}
