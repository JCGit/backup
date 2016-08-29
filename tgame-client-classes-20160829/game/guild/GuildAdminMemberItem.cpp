#include "GuildAdminMemberItem.h"

#include "defines.h"

#include "basic/TextHelper.h"
#include "basic/NounHelper.h"

#include "GuildControl.h"

GuildAdminMemberItem::GuildAdminMemberItem()
:m_pNameTxt(NULL)
,m_pLevelTxt(NULL)
,m_pPrivilegeTxt(NULL)
,m_pLastLoginTxt(NULL)
{
}

GuildAdminMemberItem::~GuildAdminMemberItem()
{
}

GuildAdminMemberItem* GuildAdminMemberItem::create() {
	GuildAdminMemberItem *pRet = new GuildAdminMemberItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildAdminMemberItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

const vmsg::CSGuildMember& GuildAdminMemberItem::getInfo() {
	return m_stMemberInfo;
}

void GuildAdminMemberItem::setInfo(const vmsg::CSGuildMember &stMember) {
	m_stMemberInfo = stMember;
	m_pNameTxt->setText(stMember.brief().name());
	unsigned int uPriviType = stMember.brief().type();

	if(uPriviType == vmsg::GUILD_PRIVI_CHAIRMAN) {
		m_pPrivilegeTxt->setText(NounStr(NOUN_CHAIRMAN));
	} else if(uPriviType == vmsg::GUILD_PRIVI_DEPUTY) {
		m_pPrivilegeTxt->setText(NounStr(NOUN_DEPUTY));
	} else if(uPriviType == vmsg::GUILD_PRIVI_MEMBER) {
		m_pPrivilegeTxt->setText(NounStr(NOUN_MEMBER));
	}

	m_pLevelTxt->setText(intToString(stMember.brief().level()));
	m_pLastLoginTxt->setText(P_GUILD_CTRL->getLastLoginText(stMember.lastlogin()));
}

void GuildAdminMemberItem::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildAdminMemberItemUI.ExportJson");
	addChild(m_pRootLayout);

	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

	m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

	m_pPrivilegeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("privilege_txt"));

	m_pLastLoginTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("last_login_txt"));

}
