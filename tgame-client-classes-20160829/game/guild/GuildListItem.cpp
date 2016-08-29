#include "GuildListItem.h"

#include "defines.h"

#include "basic/NounHelper.h"
#include "basic/TextHelper.h"
#include "CommonDialog.h"

#include "game/UserObj.h"

#include "GuildControl.h"

#define MAX_GUILD_MEMBER_CNT 30

GuildListItem::GuildListItem()
:m_pGuildLevelImg(NULL)
,m_pGuildNameTxt(NULL)
,m_pGuildNoticeTxt(NULL)
,m_pGuildSizeTxt(NULL)
,m_pJoinBtn(NULL)
,m_pGuildStatImg(NULL)
,m_uStat(GUILD_NORM)
{
}

GuildListItem::~GuildListItem()
{
}

GuildListItem* GuildListItem::create() {
	GuildListItem *pRet = new GuildListItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/GuildListItemUI.ExportJson");
		addChild(m_pRootLayout);

		m_pGuildLevelImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("guild_level_img"));

		m_pGuildNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("guild_name_txt"));

		m_pGuildNoticeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("guild_notice_txt"));

		m_pGuildSizeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("guild_size_txt"));

		m_pJoinBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("join_btn"));
		m_pJoinBtn->addTouchEventListener(this, toucheventselector(GuildListItem::touchEventHandler));

		m_pGuildStatImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("guild_stat_img"));

		bRet = true;
	} while(0);

	return bRet;
}

void GuildListItem::updateUI(const vmsg::CSGuildBrief &stGuildInfo, bool bIsApply) {
	m_stGuildInfo = stGuildInfo;
	
	std::string url = std::string("guild/guild_level_") + intToString(m_stGuildInfo.level() / 20) + ".png";
	
	m_pGuildLevelImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
	m_pGuildNameTxt->setText(m_stGuildInfo.guildname());
	m_pGuildSizeTxt->setText(intToString(m_stGuildInfo.membernum()) + "/" + intToString(MAX_GUILD_MEMBER_CNT));
	m_pGuildNoticeTxt->setText(m_stGuildInfo.notice());
	m_pJoinBtn->setTouchEnabled(true);

	if(P_USER_OBJ->getUserInfo()->guildID == m_stGuildInfo.guildid()) { // 我的部落
		setStat(GUILD_MINE);
	} else if(m_stGuildInfo.membernum() == MAX_GUILD_MEMBER_CNT) { // 部落已满
		setStat(GUILD_FULL);
	} else {
		setStat(GUILD_NORM);

		if(bIsApply) {
			m_pJoinBtn->setTitleText(NounStr(NOUN_UNDO));
		} else {
			m_pJoinBtn->setTitleText(NounStr(NOUN_APPLY));
		}
	}
}

const vmsg::CSGuildBrief& GuildListItem::getGuildInfo() {
	return m_stGuildInfo;
}

void GuildListItem::setStat(GuildStat stat) {
	m_uStat = stat;

	m_pGuildStatImg->setEnabled(m_uStat != GUILD_NORM);
	m_pJoinBtn->setEnabled(m_uStat == GUILD_NORM && P_USER_OBJ->getUserInfo()->guildID == 0);

	switch(m_uStat) {
		case GUILD_MINE:
			m_pGuildStatImg->loadTexture("guild/text_my_guild.png", UI_TEX_TYPE_PLIST);
			break;
		case GUILD_FULL:
			m_pGuildStatImg->loadTexture("guild/text_guild_full.png", UI_TEX_TYPE_PLIST);
			break;
		default:;
	}
}

unsigned int GuildListItem::getStat() {
	return m_uStat;
}

void GuildListItem::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pJoinBtn->getName()) == 0) {
				if(NounStr(NOUN_APPLY) == m_pJoinBtn->getTitleText()) { // 申请加入
					unsigned int uAppliedGuildCnt = P_GUILD_CTRL->getAppliedGuildCnt();

					if(uAppliedGuildCnt >= 5) {
						COMMON_DIALOG->show(TextStr(TEXT_GUILD_APPLIED_LIMITED), this, NounStr(OK), NounStr(CANCEL));
					} else {
						m_pJoinBtn->setTitleText(NounStr(NOUN_UNDO));
						P_GUILD_CTRL->sendGuildJoinRqst(m_stGuildInfo.guildid());
					}
				} else if(NounStr(NOUN_UNDO) == m_pJoinBtn->getTitleText()) { // 撤销申请
					m_pJoinBtn->setTitleText(NounStr(NOUN_APPLY));
					P_GUILD_CTRL->sendGuildDelMyJoinRqst(m_stGuildInfo.guildid());
				}

				m_pJoinBtn->setTouchEnabled(false);
			}
			break;
		default:;
	}
}
