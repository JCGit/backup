#include "GuildMemberItem.h"

#include "defines.h"
#include "Resources.h"

#include "CfgMaster/HeroCfgMaster.h"

#include "basic/NounHelper.h"

#include "game/UserObj.h"

#include "GuildControl.h"

GuildMemberItem::GuildMemberItem()
:m_pPrivilegeTxt(NULL)
,m_pPlayerNameTxt(NULL)
,m_pPlayerHeadImg(NULL)
,m_pPlayerLevelTxt(NULL)
,m_pImpeachBtn(NULL)
//,m_pCheckBoxBtn(NULL)
,m_bSelected(false)
,m_uin(0)
{
}

GuildMemberItem::~GuildMemberItem()
{
}

GuildMemberItem* GuildMemberItem::create() {
	GuildMemberItem *pRet = new GuildMemberItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildMemberItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/GuildMemberItemUI.ExportJson");
		addChild(m_pRootLayout);

		m_pPrivilegeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("privilege_txt"));

		m_pPlayerNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("player_name_txt"));

		m_pPlayerHeadImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("head_img"));

		m_pPlayerLevelTxt = dynamic_cast<UILabelAtlas*>(m_pRootLayout->getChildByName("player_level_txt"));

		m_pImpeachBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("impeach_btn"));
		m_pImpeachBtn->addTouchEventListener(this, toucheventselector(GuildMemberItem::touchEventHandler));

		/*m_pCheckBoxBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("check_box_btn"));
		m_pCheckBoxBtn->setTouchEnabled(false);
		m_pCheckBoxBtn->setBright(true);*/

		bRet = true;
	} while(0);

	return bRet;
}

void GuildMemberItem::updateUI(const vmsg::CSGuildMember &stMember) {
	m_uin = stMember.brief().uin();
	m_pPlayerNameTxt->setText(stMember.brief().name());
	m_pPlayerLevelTxt->setStringValue(intToString(stMember.brief().level()));

	unsigned int uPriviType = stMember.brief().type();
	m_pImpeachBtn->setEnabled(uPriviType == vmsg::GUILD_PRIVI_CHAIRMAN && m_uin != P_USER_OBJ->getUserInfo()->uin);

	if(uPriviType == vmsg::GUILD_PRIVI_CHAIRMAN) {
		m_pPrivilegeTxt->setText(NounStr(NOUN_CHAIRMAN));
	} else if(uPriviType == vmsg::GUILD_PRIVI_DEPUTY) {
		m_pPrivilegeTxt->setText(NounStr(NOUN_DEPUTY));
	} else if(uPriviType == vmsg::GUILD_PRIVI_MEMBER) {
		m_pPrivilegeTxt->setText(NounStr(NOUN_MEMBER));
	}

	unsigned int uAvatarID = stMember.brief().showid();
	string url = HEAD_ICON_PATH + HERO_CFG_MASTER->GetHeroCfgByID(uAvatarID)->headicon() + ".png";
	m_pPlayerHeadImg->loadTexture(url.c_str());

	/*if(stMember.isonline() == 0) {
		m_pOnlineTxt->setText(NounStr(NOUN_OFFLINE));
	} else {
		m_pOnlineTxt->setText(NounStr(NOUN_ONLINE));
	}*/
}
//
//void GuildMemberItem::updateUI(const vmsg::CSGuildReq &stReq) {
//	m_uin = stReq.uin();
//	m_pPlayerNameTxt->setText(stReq.name());
//	m_pPlayerLevelTxt->setStringValue(intToString(stReq.level()));
//
//	unsigned int uPriviType = vmsg::GUILD_PRIVI_MEMBER;
//	m_pImpeachBtn->setEnabled(uPriviType == vmsg::GUILD_PRIVI_CHAIRMAN && m_uin != P_USER_OBJ->getUserInfo()->uin);
//
//	if(uPriviType == vmsg::GUILD_PRIVI_CHAIRMAN) {
//		m_pPrivilegeTxt->setText(NounStr(NOUN_CHAIRMAN));
//	} else if(uPriviType == vmsg::GUILD_PRIVI_DEPUTY) {
//		m_pPrivilegeTxt->setText(NounStr(NOUN_DEPUTY));
//	} else if(uPriviType == vmsg::GUILD_PRIVI_MEMBER) {
//		m_pPrivilegeTxt->setText(NounStr(NOUN_MEMBER));
//	}
//}
//
//bool GuildMemberItem::isSelected() {
//	return m_bSelected;
//}
//
//void GuildMemberItem::selected(bool bSelected) {
//	m_bSelected = bSelected;
//
//	/*if(m_bSelected) {
//		m_pCheckBoxBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
//	} else {
//		m_pCheckBoxBtn->setBrightStyle(BRIGHT_NORMAL);
//	}*/
//}

UIN_t GuildMemberItem::getMemberUIN() {
	return m_uin;
}

//void GuildMemberItem::enableCheckBox(bool enable) {
//	//m_pCheckBoxBtn->setEnabled(enable);
//}

void GuildMemberItem::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pImpeachBtn->getName()) == 0) {
				P_GUILD_CTRL->sendGuildImpeachRqst();
			}
			break;
		default:;
	}
}
