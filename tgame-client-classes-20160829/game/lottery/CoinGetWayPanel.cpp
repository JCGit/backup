#include "CoinGetWayPanel.h"

#include "defines.h"

#include "game/city/EntryControl.h"

#include "LotteryControl.h"

#define BTN_PREFIX_NAME "entry_btn_"

CoinGetWayPanel::CoinGetWayPanel()
:m_pCloseBtn(NULL)
,m_pEntryLayout(NULL)
{
}

CoinGetWayPanel::~CoinGetWayPanel()
{
}

CoinGetWayPanel* CoinGetWayPanel::create() {
	CoinGetWayPanel *pRet = new CoinGetWayPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CoinGetWayPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CoinGetWayPanel::open() {
	BasePanel::open();

	if(!m_bUIInited) {
		showLoading();
		initUI();
		closeLoading();
	}
}

void CoinGetWayPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/CoinGetWayPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CoinGetWayPanel::touchEventHandler));

	m_pEntryLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("entry_layout"));

	UIButton *pBtn = NULL;
	string btnName = "";
	LinearLayoutParameter *pLayoutParameter = LinearLayoutParameter::create();
	pLayoutParameter->setGravity(LINEAR_GRAVITY_CENTER_VERTICAL);
	pLayoutParameter->setMargin(Margin(0, 0, 20, 0));

	pBtn = UIButton::create();
	btnName = string(BTN_PREFIX_NAME) + intToString(ENTRY_BUY_COIN);
	pBtn->setName(btnName.c_str());
	pBtn->loadTextureNormal("entry/btn_buy_coin_entry_up.png", UI_TEX_TYPE_PLIST);
	pBtn->setLayoutParameter(pLayoutParameter);
	m_pEntryLayout->addChild(pBtn);
	pBtn->addTouchEventListener(this, toucheventselector(CoinGetWayPanel::touchEventHandler));

	pBtn = UIButton::create();
	btnName = string(BTN_PREFIX_NAME) + intToString(ENTRY_NOTHING_LAND);
	pBtn->setName(btnName.c_str());
	pBtn->loadTextureNormal("entry/btn_nothing_land_entry_up.png", UI_TEX_TYPE_PLIST);
	pBtn->setLayoutParameter(pLayoutParameter);
	m_pEntryLayout->addChild(pBtn);
	pBtn->addTouchEventListener(this, toucheventselector(CoinGetWayPanel::touchEventHandler));

	pBtn = UIButton::create();
	btnName = string(BTN_PREFIX_NAME) + intToString(ENTRY_ARENA);
	pBtn->setName(btnName.c_str());
	pBtn->loadTextureNormal("entry/btn_arena_entry_up.png", UI_TEX_TYPE_PLIST);
	pBtn->setLayoutParameter(pLayoutParameter);
	m_pEntryLayout->addChild(pBtn);
	pBtn->addTouchEventListener(this, toucheventselector(CoinGetWayPanel::touchEventHandler));

	pBtn = UIButton::create();
	btnName = string(BTN_PREFIX_NAME) + intToString(ENTRY_WEAPON_BOOK);
	pBtn->setName(btnName.c_str());
	pBtn->loadTextureNormal("entry/btn_qianghun_entry_up.png", UI_TEX_TYPE_PLIST);
	pBtn->setLayoutParameter(pLayoutParameter);
	m_pEntryLayout->addChild(pBtn);
	pBtn->addTouchEventListener(this, toucheventselector(CoinGetWayPanel::touchEventHandler));

	m_bUIInited = true;
}

void CoinGetWayPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(string(pszWidgetName).substr(0, string(BTN_PREFIX_NAME).size()) == BTN_PREFIX_NAME) {
				close();
				P_LOTTERY_CTRL->getLotteryPanel()->close();
				unsigned int uEntryID = atol(string(pszWidgetName).substr(string(BTN_PREFIX_NAME).size(), string(pszWidgetName).size()).c_str());
				P_ENTRY_CTRL->enter(uEntryID);
			}
			break;
		default:;
	}
}
