#include "GuildFindPanel.h"

#include "CfgMaster/GlobalCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/TextHelper.h"
#include "basic/CommonDialog.h"

#include "game/UserObj.h"

#include "GuildControl.h"

GuildFindPanel::GuildFindPanel()
:m_pCloseBtn(NULL)
,m_pGuildNameInputTxt(NULL)
,m_pFindBtn(NULL)
{
}

GuildFindPanel::~GuildFindPanel()
{
}

GuildFindPanel* GuildFindPanel::create() {
	GuildFindPanel *pRet = new GuildFindPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildFindPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		

		bRet = true;
	} while(0);

	return bRet;
}

void GuildFindPanel::open() {
	BasePanel::open();

	if(!m_bUIInited) {
		showLoading();
		initUI();
	}
}

void GuildFindPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildFindPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(GuildFindPanel::touchEventHandler));

	m_pGuildNameInputTxt = dynamic_cast<UITextField*>(m_pRootLayout->getChildByName("guild_name_input_txt"));

	m_pFindBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("find_btn"));
	m_pFindBtn->addTouchEventListener(this, toucheventselector(GuildFindPanel::touchEventHandler));

	m_bUIInited = true;

	closeLoading();
}

void GuildFindPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(std::strcmp(pszWidgetName, m_pFindBtn->getName()) == 0) {
				close();
				// valid input 
				std::string name = m_pGuildNameInputTxt->getStringValue();

				if(name.empty()) {
					COMMON_DIALOG->show(TextStr(TEXT_GUILD_NULL_GUILD_NAME), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuildFindPanel::onDialog));
				} else if(name.substr(0,1) == " ") {
					COMMON_DIALOG->show(TextStr(TEXT_GUILD_INVALID_GUILD_NAME), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuildFindPanel::onDialog));
				} else {
					P_GUILD_CTRL->sendGuildSearchRqst(name);
				}

			}
			break;
		default:;
	}
}

void GuildFindPanel::onDialog() {

}
