#include "GuildMemberAdminPanel.h"

#include "defines.h"

#include "basic/NounHelper.h"

#include "GuildControl.h"

GuildMemberAdminPanel::GuildMemberAdminPanel()
:m_pCloseBtn(NULL)
,m_pNameTxt(NULL)
,m_pPrivilegeValTxt(NULL)
,m_pLevelValTxt(NULL)
,m_pLastLoginValTxt(NULL)
,m_pChangePrivilegeBtn(NULL)
,m_pKickOutBtn(NULL)
{
}

GuildMemberAdminPanel::~GuildMemberAdminPanel()
{
}

GuildMemberAdminPanel* GuildMemberAdminPanel::create() {
	GuildMemberAdminPanel *pRet = new GuildMemberAdminPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildMemberAdminPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void GuildMemberAdminPanel::openWithMemberInfo(const vmsg::CSGuildMember &stMemberInfo) {
	BasePanel::open();

	m_stMemberInfo = stMemberInfo;
	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	m_pNameTxt->setText(m_stMemberInfo.brief().name());
	m_pLevelValTxt->setText(intToString(m_stMemberInfo.brief().level()));
	
	unsigned int uPriviType = m_stMemberInfo.brief().type();

	if(uPriviType == vmsg::GUILD_PRIVI_CHAIRMAN) {
		m_pPrivilegeValTxt->setText(NounStr(NOUN_CHAIRMAN));
		m_pChangePrivilegeBtn->setTitleText(NounStr(NOUN_TO_DEPUTY));
	} else if(uPriviType == vmsg::GUILD_PRIVI_DEPUTY) {
		m_pPrivilegeValTxt->setText(NounStr(NOUN_DEPUTY));
		m_pChangePrivilegeBtn->setTitleText(NounStr(NOUN_TO_MEMBER));
	} else if(uPriviType == vmsg::GUILD_PRIVI_MEMBER) {
		m_pPrivilegeValTxt->setText(NounStr(NOUN_MEMBER));
		m_pChangePrivilegeBtn->setTitleText(NounStr(NOUN_TO_DEPUTY));
	}

	m_pLastLoginValTxt->setText(P_GUILD_CTRL->getLastLoginText(m_stMemberInfo.lastlogin()));

	closeLoading();
}

void GuildMemberAdminPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildMemberAdminPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(GuildMemberAdminPanel::touchEventHandler));

	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

	m_pPrivilegeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("privilege_val_txt"));

	m_pLevelValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_val_txt"));

	m_pLastLoginValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("last_login_val_txt"));

	m_pChangePrivilegeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("change_privilege_btn"));
	m_pChangePrivilegeBtn->addTouchEventListener(this, toucheventselector(GuildMemberAdminPanel::touchEventHandler));

	m_pKickOutBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("kick_out_btn"));
	m_pKickOutBtn->addTouchEventListener(this, toucheventselector(GuildMemberAdminPanel::touchEventHandler));

	m_bUIInited = true;
}

void GuildMemberAdminPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pChangePrivilegeBtn->getName()) == 0) {
				if(m_stMemberInfo.ByteSize() > 0) {
					close();
					unsigned int uPriviType = m_stMemberInfo.brief().type();

					if(uPriviType == vmsg::GUILD_PRIVI_DEPUTY) { // ½µÖ°
						P_GUILD_CTRL->sendGuildChgPriviRqst(m_stMemberInfo.brief().uin(), vmsg::GUILD_PRIVI_MEMBER);
					} else if(uPriviType == vmsg::GUILD_PRIVI_MEMBER) { // ÉýÖ°
						P_GUILD_CTRL->sendGuildChgPriviRqst(m_stMemberInfo.brief().uin(), vmsg::GUILD_PRIVI_DEPUTY);
					}
				}
			} else if(strcmp(pszWidgetName, m_pKickOutBtn->getName()) == 0) {
				if(m_stMemberInfo.ByteSize() > 0) {
					close();
					P_GUILD_CTRL->sendGuildKickRqst(m_stMemberInfo.brief().uin());
				}
			}
			break;
		default:;
	}
}
