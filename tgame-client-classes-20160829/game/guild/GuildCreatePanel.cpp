#include "GuildCreatePanel.h"

#include "CfgMaster/GlobalCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/TextHelper.h"
#include "basic/CommonDialog.h"

#include "game/UserObj.h"
#include "tip/TipControl.h"

#include "GuildControl.h"

GuildCreatePanel::GuildCreatePanel()
:m_pCloseBtn(NULL)
,m_pWordsTxt(NULL)
,m_pGuildNameInputTxt(NULL)
,m_pCreateBtn(NULL)
{
}

GuildCreatePanel::~GuildCreatePanel()
{
}

GuildCreatePanel* GuildCreatePanel::create() {
	GuildCreatePanel *pRet = new GuildCreatePanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildCreatePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void GuildCreatePanel::open() {
	BasePanel::open();

	if(!m_bUIInited) {
		showLoading();
		initUI();
	}
}

void GuildCreatePanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildCreatePanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(GuildCreatePanel::touchEventHandler));

	m_pWordsTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("words_txt"));
	char text[100];
	snprintf(text, 100, TextStr(TEXT_GUILD_CREATE_COST), GLOBAL_CFG_MASTER->GetCfg().createguildcostcoin() / 10000);
	m_pWordsTxt->setText(text);

	m_pGuildNameInputTxt = dynamic_cast<UITextField*>(m_pRootLayout->getChildByName("guild_name_input_txt"));

	m_pCreateBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("create_btn"));
	m_pCreateBtn->addTouchEventListener(this, toucheventselector(GuildCreatePanel::touchEventHandler));

	m_bUIInited = true;

	closeLoading();
}

void GuildCreatePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(std::strcmp(pszWidgetName, m_pCreateBtn->getName()) == 0) {
				close();
				// valid input 
				std::string name = m_pGuildNameInputTxt->getStringValue();

				if(name.empty()) {
					COMMON_DIALOG->show(TextStr(TEXT_GUILD_NULL_GUILD_NAME), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuildCreatePanel::onDialog));
				} else if(name.substr(0,1) == " ") {
					COMMON_DIALOG->show(TextStr(TEXT_GUILD_INVALID_GUILD_NAME), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuildCreatePanel::onDialog));
				} else {
					// check coin
					UINT64_t needCoin = GLOBAL_CFG_MASTER->GetCfg().createguildcostcoin();

					if(P_TIP_CTRL->CoinCostTip(needCoin, this)) {
						P_GUILD_CTRL->sendGuildCreateRqst(name);
                    }
				}
				
			}
			break;
		default:;
	}
}

void GuildCreatePanel::onDialog() {
	
}
