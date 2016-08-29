#include "GuildOptionsPanel.h"

#include "GuildControl.h"
#include "GuildPanel.h"
#include "UserObj.h"
#include "NounHelper.h"
#include "TextHelper.h"
#include "CommonDialog.h"
#include "GlobalTime.h"
GuildOptionsPanel::GuildOptionsPanel()
:m_pCloseBtn(NULL)
,m_pOptionsListView(NULL)
{
}

GuildOptionsPanel::~GuildOptionsPanel()
{
}

GuildOptionsPanel* GuildOptionsPanel::create() {
	GuildOptionsPanel *pRet = new GuildOptionsPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildOptionsPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui/GuildOptionsPanelUI.json");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(GuildOptionsPanel::touchEventHandler));

		m_pOptionsListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("options_list"));
		m_pOptionsListView->addEventListenerListView(this, listvieweventselector(GuildOptionsPanel::listViewEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void GuildOptionsPanel::open() {
	BasePanel::open();

	updateUI();
}

void GuildOptionsPanel::updateUI() {
	bool isChairman = P_USER_OBJ->getUserInfo()->nick == P_GUILD_CTRL->getGuildInfo().guildbrief().chairmanname();

	UIButton *pBtn = NULL;

	if(isChairman) {
		pBtn = dynamic_cast<UIButton*>(m_pOptionsListView->getItem(0));
		pBtn->setVisible(true);
		pBtn->setTitleText(NounStr(NOUN_EXIT_GUILD));

		pBtn = dynamic_cast<UIButton*>(m_pOptionsListView->getItem(1));
		pBtn->setVisible(true);
		pBtn->setTitleText(NounStr(NOUN_DISMISS_GUILD));
	} else {
		pBtn = dynamic_cast<UIButton*>(m_pOptionsListView->getItem(0));
		pBtn->setVisible(true);
		pBtn->setTitleText(NounStr(NOUN_IMPEACH_CHAIRMAN));

		pBtn = dynamic_cast<UIButton*>(m_pOptionsListView->getItem(1));
		pBtn->setVisible(true);
		pBtn->setTitleText(NounStr(NOUN_EXIT_GUILD));
	}
}

void GuildOptionsPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			}
			break;
		default:
			break;
	}
}

void GuildOptionsPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszWidgetName, m_pOptionsListView->getName()) == 0) {
				int nSelectedIdx = m_pOptionsListView->getCurSelectedIndex();

				UIButton *pBtn = dynamic_cast<UIButton*>(m_pOptionsListView->getItem(nSelectedIdx));
				std::string title = pBtn->getTitleText();

				if(title == NounStr(NOUN_EXIT_GUILD)) { // 退出部落
					COMMON_DIALOG->show(TextStr(TEXT_GUILD_LEAVE_GUILD), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuildOptionsPanel::onLeaveDialog));
				} else if(title == NounStr(NOUN_IMPEACH_CHAIRMAN)) { // 弹劾大酋长
					const vmsg::CSGuildInfoRsp &stGuildInfo = P_GUILD_CTRL->getGuildInfo();

					if(stGuildInfo.impeachuin() > 0 && stGuildInfo.impeachtime() > 0) {
						COMMON_DIALOG->show(TextStr(TEXT_GUILD_HAS_IMPEACHED), this, NounStr(OK), NounStr(CANCEL));
					} else {
                        int iLastLoginTime = 0;
                        for(int i = 0; i < stGuildInfo.guildmemberlst_size(); ++i) {
                            const vmsg::CSGuildMember stOneMember = stGuildInfo.guildmemberlst(i);
                            if(vmsg::GUILD_PRIVI_CHAIRMAN == stOneMember.brief().type()) {
                                iLastLoginTime = stOneMember.lastlogin();
                                break;
                            }
                        }
                        if(iLastLoginTime > 0) {
                            //大酋长离线5天才可以被弹劾
                            if( (GlobalTime::getTime() - iLastLoginTime) >= 432000 ) {
                                COMMON_DIALOG->show(TextStr(TEXT_GUILD_IMPEACH_CHAIRMAN), this, 
                                    NounStr(OK), NounStr(CANCEL), 
                                    dialogbtneventselector(GuildOptionsPanel::onImpeachDialog));
                            } else {
                                COMMON_DIALOG->show(TextStr(TEXT_GUILD_IMPEACH_INVALID_RULE));
                                CCLOG("[trace]GuildOptionsPanel::listViewEventHandler,can not impeach now,invalid rule");
                            }
                        } else {
                            CCLOG("[error]GuildOptionsPanel::listViewEventHandler,fail to get guild owner's login time");
                        }

						
					}
					
				} else if(title == NounStr(NOUN_DISMISS_GUILD)) { // 解散部落
					COMMON_DIALOG->show(TextStr(TEXT_GUILD_DISMISS_GUILD), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuildOptionsPanel::onDismissDialog));
				}
			}
			break;
		default:
			break;
	}
}

void GuildOptionsPanel::onImpeachDialog() {
	P_GUILD_CTRL->sendGuildImpeachRqst();
	close();
}

void GuildOptionsPanel::onLeaveDialog() {
	P_GUILD_CTRL->sendGuildLeaveRqst();
	close();
	P_GUILD_CTRL->getGuildPanel()->close();
}

void GuildOptionsPanel::onDismissDialog() {
	P_GUILD_CTRL->sendGuildDismissRqst();
	close();
	P_GUILD_CTRL->getGuildPanel()->close();
}
