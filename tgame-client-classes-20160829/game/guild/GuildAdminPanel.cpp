#include "GuildAdminPanel.h"

#include "basic/NounHelper.h"
#include "basic/TextHelper.h"
#include "basic/CommonDialog.h"

#include "GuildControl.h"
#include "GuildAdminMemberItem.h"
#include "GuildPanel.h"
#include "game/UserObj.h"

GuildAdminPanel::GuildAdminPanel()
:m_pBackbtn(NULL)
,m_pMemberListView(NULL)
,m_pOperationListView(NULL)
,m_pReplyBtn(NULL)
,m_pDismissBtn(NULL)
,m_pLeaveBtn(NULL)
{
}

GuildAdminPanel::~GuildAdminPanel()
{
}

GuildAdminPanel* GuildAdminPanel::create() {
	GuildAdminPanel *pRet = new GuildAdminPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildAdminPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void GuildAdminPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	updateUI();
}

void GuildAdminPanel::updateUI() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	vmsg::CSGuildInfoRsp stGuildInfo = P_GUILD_CTRL->getGuildInfo();

	m_pMemberListView->removeAllItems();

	sort(stGuildInfo.mutable_guildmemberlst()->begin(), stGuildInfo.mutable_guildmemberlst()->end(), compareLastLogin);

	for(int i = 0; i < stGuildInfo.guildmemberlst_size(); i++) {
		const vmsg::CSGuildMember &stMember = stGuildInfo.guildmemberlst(i);
		GuildAdminMemberItem *pItem = GuildAdminMemberItem::create();
		pItem->setInfo(stGuildInfo.guildmemberlst(i));
		m_pMemberListView->pushBackCustomItem(pItem);
	}

	m_pOperationListView->removeAllItems();
	m_pReplyBtn = NULL;
	m_pLeaveBtn = NULL;
	m_pDismissBtn = NULL;
	unsigned int uMyPrivilege = P_GUILD_CTRL->getMemberPrivilege(P_USER_OBJ->getUserInfo()->uin);
	UIButton *pBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("operate_btn"));

	if(uMyPrivilege != vmsg::GUILD_PRIVI_MEMBER) {
		UIButton *pReplyBtn = dynamic_cast<UIButton*>(pBtn->clone());
		pReplyBtn->setVisible(true);
		pReplyBtn->setName("reply_btn");
		pReplyBtn->setTitleText(NounStr(NOUN_APPLY_LIST));
		m_pOperationListView->pushBackCustomItem(pReplyBtn);
		m_pReplyBtn = pReplyBtn;
	}

	if(uMyPrivilege == vmsg::GUILD_PRIVI_CHAIRMAN) {
		UIButton *pDismissBtn = dynamic_cast<UIButton*>(pBtn->clone());
		pDismissBtn->setVisible(true);
		pDismissBtn->setName("dismiss_btn");
		pDismissBtn->setTitleText(NounStr(NOUN_DISMISS_GUILD));
		m_pOperationListView->pushBackCustomItem(pDismissBtn);
		m_pDismissBtn = pDismissBtn;
	} else {
		UIButton *pLeaveBtn = dynamic_cast<UIButton*>(pBtn->clone());
		pLeaveBtn->setVisible(true);
		pLeaveBtn->setName("leave_btn");
		pLeaveBtn->setTitleText(NounStr(NOUN_EXIT_GUILD));
		m_pOperationListView->pushBackCustomItem(pLeaveBtn);
		m_pLeaveBtn = pLeaveBtn;
	}

	closeLoading();
}

void GuildAdminPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildAdminPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pBackbtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pBackbtn->addTouchEventListener(this, toucheventselector(GuildAdminPanel::touchEventHandler));

	m_pMemberListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("member_list"));
	m_pMemberListView->addEventListenerListView(this, listvieweventselector(GuildAdminPanel::listViewEventHandler));

	m_pOperationListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("operation_list"));
	m_pOperationListView->addEventListenerListView(this, listvieweventselector(GuildAdminPanel::listViewEventHandler));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}

}

void GuildAdminPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pBackbtn->getName()) == 0) {
				close();
				P_GUILD_CTRL->getGuildPanel()->open();
			}
			break;
		default:;
	}
}

void GuildAdminPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszListViewName, m_pMemberListView->getName()) == 0) {
				int nSelectedIdx = m_pMemberListView->getCurSelectedIndex();
				GuildAdminMemberItem *pItem = dynamic_cast<GuildAdminMemberItem*>(m_pMemberListView->getItem(nSelectedIdx));
				const vmsg::CSGuildMember &stMemberInfo = pItem->getInfo();
				unsigned int uMyPrivilege = P_GUILD_CTRL->getMemberPrivilege(P_USER_OBJ->getUserInfo()->uin);
				bool isGuildChairman = stMemberInfo.brief().type() == vmsg::GUILD_PRIVI_CHAIRMAN;

				if(!isGuildChairman && stMemberInfo.brief().type() != uMyPrivilege && uMyPrivilege != vmsg::GUILD_PRIVI_MEMBER) {
					P_GUILD_CTRL->getGuildMemberAdminPanel()->openWithMemberInfo(pItem->getInfo());
				}
			} else if(std::strcmp(pszListViewName, m_pOperationListView->getName()) == 0) {
				int nSelectedIdx = m_pOperationListView->getCurSelectedIndex();
				UIButton *pBtn = dynamic_cast<UIButton*>(m_pOperationListView->getItem(nSelectedIdx));
				const char *pszBtnName = pBtn->getName();

				if(m_pReplyBtn != NULL && strcmp(pszBtnName, m_pReplyBtn->getName()) == 0) {
					P_GUILD_CTRL->getGuildJoinReplyPanel()->open();
				} else if(m_pDismissBtn != NULL && strcmp(pszBtnName, m_pDismissBtn->getName()) == 0) {
					COMMON_DIALOG->show(TextStr(TEXT_GUILD_DISMISS_GUILD), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuildAdminPanel::onDismissGuildConfirm));
				} else if(m_pLeaveBtn != NULL && strcmp(pszBtnName, m_pLeaveBtn->getName()) == 0) {
					close();
					P_GUILD_CTRL->sendGuildLeaveRqst();
				}
			}
			break;
		default:;
	}
}

void GuildAdminPanel::onDismissGuildConfirm() {
	close();
	P_GUILD_CTRL->sendGuildDismissRqst();
}

bool GuildAdminPanel::compareLastLogin(const vmsg::CSGuildMember &stMemberA, const vmsg::CSGuildMember &stMemberB) {
	return stMemberA.lastlogin() >= stMemberB.lastlogin();
}
