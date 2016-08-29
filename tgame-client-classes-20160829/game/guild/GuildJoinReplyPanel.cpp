#include "GuildJoinReplyPanel.h"

#include "GuildControl.h"
#include "GuildJoinReplyItem.h"

GuildJoinReplyPanel::GuildJoinReplyPanel()
:m_pCloseBtn(NULL)
,m_pApplyListView(NULL)
,m_pAgreeBtn(NULL)
,m_pDenyBtn(NULL)
,m_pSelectAllLayout(NULL)
,m_pCheckboxBtn(NULL)
{
}

GuildJoinReplyPanel::~GuildJoinReplyPanel()
{
}

GuildJoinReplyPanel* GuildJoinReplyPanel::create() {
	GuildJoinReplyPanel *pRet = new GuildJoinReplyPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildJoinReplyPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void GuildJoinReplyPanel::open() {
	BasePanel::open();

	showLoading();

	P_GUILD_CTRL->sendGuildQueryJoinListRqst();

	if(!m_bUIInited) {
		initUI();
	}

}

void GuildJoinReplyPanel::updateUI() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	const vmsg::CSAllGuildReqRsp &stAllGuildRqst = P_GUILD_CTRL->getAllGuildRqst();

	m_pApplyListView->removeAllItems();

	for(int i = 0; i < stAllGuildRqst.guildreqlst_size(); i++) {
		GuildJoinReplyItem *pItem = GuildJoinReplyItem::create();
		pItem->setInfo(stAllGuildRqst.guildreqlst(i));
		pItem->selected(false);
		m_pApplyListView->pushBackCustomItem(pItem);
	}

	m_pAgreeBtn->setTouchEnabled(false);
	m_pAgreeBtn->setBright(m_pAgreeBtn->isTouchEnabled());
	m_pDenyBtn->setTouchEnabled(false);
	m_pDenyBtn->setBright(m_pDenyBtn->isTouchEnabled());

	closeLoading();
}

void GuildJoinReplyPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildJoinReplyPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(GuildJoinReplyPanel::touchEventHandler));

	m_pApplyListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("apply_list"));
	m_pApplyListView->addEventListenerListView(this, listvieweventselector(GuildJoinReplyPanel::listViewEventHandler));

	m_pAgreeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("agree_btn"));
	m_pAgreeBtn->addTouchEventListener(this, toucheventselector(GuildJoinReplyPanel::touchEventHandler));

	m_pDenyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("deny_btn"));
	m_pDenyBtn->addTouchEventListener(this, toucheventselector(GuildJoinReplyPanel::touchEventHandler));

	m_pSelectAllLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("checkbox_layout"));
	m_pSelectAllLayout->addTouchEventListener(this, toucheventselector(GuildJoinReplyPanel::touchEventHandler));

	m_pCheckboxBtn = dynamic_cast<UICheckBox*>(m_pSelectAllLayout->getChildByName("checkbox_btn"));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateUI();
	}
}

void GuildJoinReplyPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pAgreeBtn->getName()) == 0) {
				const vmsg::CSAllGuildReqRsp &stAllGuildRqst = P_GUILD_CTRL->getAllGuildRqst();
				GuildJoinReplyItem *pItem = NULL;
				vector<UIN_t> stPlayerLst;

				for(unsigned int i = 0; i < m_pApplyListView->getItems()->count(); i++) {
					pItem = dynamic_cast<GuildJoinReplyItem*>(m_pApplyListView->getItem(i));

					if(pItem->isSelected()) {
						stPlayerLst.push_back(stAllGuildRqst.guildreqlst(i).uin());
					}
				}

				if(stPlayerLst.size() > 0) {
					close();
					P_GUILD_CTRL->sendGuildJoinReplyRqst(stPlayerLst, true);
				}
			} else if(strcmp(pszWidgetName, m_pDenyBtn->getName()) == 0) {
				const vmsg::CSAllGuildReqRsp &stAllGuildRqst = P_GUILD_CTRL->getAllGuildRqst();
				GuildJoinReplyItem *pItem = NULL;
				vector<UIN_t> stPlayerLst;

				for(unsigned int i = 0; i < m_pApplyListView->getItems()->count(); i++) {
					pItem = dynamic_cast<GuildJoinReplyItem*>(m_pApplyListView->getItem(i));

					if(pItem->isSelected()) {
						stPlayerLst.push_back(stAllGuildRqst.guildreqlst(i).uin());
					}
				}

				if(stPlayerLst.size() > 0) {
					close();
					P_GUILD_CTRL->sendGuildJoinReplyRqst(stPlayerLst, false);
				}
			} else if(strcmp(pszWidgetName, m_pSelectAllLayout->getName()) == 0) {
				m_pCheckboxBtn->setSelectedState(!m_pCheckboxBtn->getSelectedState());

				for(unsigned int i = 0; i < m_pApplyListView->getItems()->count(); i++) {
					GuildJoinReplyItem *pItem = dynamic_cast<GuildJoinReplyItem*>(m_pApplyListView->getItem(i));
					pItem->selected(m_pCheckboxBtn->getSelectedState());
				}

				unsigned int uTotalSelectedCnt = m_pCheckboxBtn->getSelectedState() ? m_pApplyListView->getItems()->count() : 0;
				m_pAgreeBtn->setTouchEnabled(uTotalSelectedCnt > 0);
				m_pAgreeBtn->setBright(m_pAgreeBtn->isTouchEnabled());
				m_pDenyBtn->setTouchEnabled(uTotalSelectedCnt > 0);
				m_pDenyBtn->setBright(m_pDenyBtn->isTouchEnabled());
			}
			break;
		default:;
	}
}

void GuildJoinReplyPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(strcmp(pszListViewName, m_pApplyListView->getName()) == 0) {
				int nSelectedIdx = m_pApplyListView->getCurSelectedIndex();
				GuildJoinReplyItem *pItem = dynamic_cast<GuildJoinReplyItem*>(m_pApplyListView->getItem(nSelectedIdx));
				pItem->selected(!pItem->isSelected());
				unsigned int uTotalSelectedCnt = 0;

				for(unsigned int i = 0; i < m_pApplyListView->getItems()->count(); i++) {
					GuildJoinReplyItem *pItem = dynamic_cast<GuildJoinReplyItem*>(m_pApplyListView->getItem(i));
					
					if(pItem->isSelected()) {
						uTotalSelectedCnt++;
					}
				}

				m_pAgreeBtn->setTouchEnabled(uTotalSelectedCnt > 0);
				m_pAgreeBtn->setBright(m_pAgreeBtn->isTouchEnabled());
				m_pDenyBtn->setTouchEnabled(uTotalSelectedCnt > 0);
				m_pDenyBtn->setBright(m_pDenyBtn->isTouchEnabled());
			}
			break;
		default:;
	}
}
