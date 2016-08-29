#include "GuildApplyPanel.h"

#include "PlayerListItem.h"
#include "GuildControl.h"

GuildApplyPanel::GuildApplyPanel()
:m_pTitleTxt(NULL)
,m_pNameTxt(NULL)
,m_pLevelTxt(NULL)
,m_pApplyListView(NULL)
,m_pOKBtn(NULL)
,m_pCancelBtn(NULL)
//,m_nSelectedIdx(-1)
{
}

GuildApplyPanel::~GuildApplyPanel()
{
}

GuildApplyPanel* GuildApplyPanel::create() {
	GuildApplyPanel *pRet = new GuildApplyPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildApplyPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui/GuildApplyPanelUI.json");
		addChild(m_pRootLayout);

		m_pTitleTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("title_txt"));

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

		m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

		m_pApplyListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("apply_list"));
		m_pApplyListView->addEventListenerListView(this, listvieweventselector(GuildApplyPanel::listViewEventHandler));

		m_pOKBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("ok_btn"));
		m_pOKBtn->addTouchEventListener(this, toucheventselector(GuildApplyPanel::touchEventHandler));

		m_pCancelBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("cancel_btn"));
		m_pCancelBtn->addTouchEventListener(this, toucheventselector(GuildApplyPanel::touchEventHandler));

		updateUI();

		bRet = true;
	} while(0);

	return bRet;
}

void GuildApplyPanel::updateUI() {
	const vmsg::CSAllGuildReqRsp &stAllGuildRqst = P_GUILD_CTRL->getAllGuildRqst();

	m_pApplyListView->removeAllItems();

	for(int i = 0; i < stAllGuildRqst.guildreqlst_size(); i++) {
		PlayerListItem *pItem = PlayerListItem::create();
		pItem->updateUI(stAllGuildRqst.guildreqlst(i));
		m_pApplyListView->pushBackCustomItem(pItem);
	}
}

void GuildApplyPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pOKBtn->getName()) == 0) {
				const vmsg::CSAllGuildReqRsp &stAllGuildRqst = P_GUILD_CTRL->getAllGuildRqst();
				PlayerListItem *pItem = NULL;
				vector<UIN_t> stPlayerLst;

				for(unsigned int i = 0; i < m_pApplyListView->getItems()->count(); i++) {
					pItem = dynamic_cast<PlayerListItem*>(m_pApplyListView->getItem(i));

					if(pItem->isSelected()) {
						stPlayerLst.push_back(stAllGuildRqst.guildreqlst(i).uin());
					}
				}

				P_GUILD_CTRL->sendGuildJoinReplyRqst(stPlayerLst, true);

				close();
			} else if(std::strcmp(pszWidgetName, m_pCancelBtn->getName()) == 0) {
				close();
			}
			break;
		default:
			break;
	}
}

void GuildApplyPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	int nSelectedIdx = m_pApplyListView->getCurSelectedIndex();
	PlayerListItem *pSelectedItem = NULL;

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(nSelectedIdx >= 0) {
				/*if(m_nSelectedIdx >= 0) {
					pSelectedItem = dynamic_cast<PlayerListItem*>(m_pApplyListView->getItem(m_nSelectedIdx));
					pSelectedItem->selected(false);
				}

				pSelectedItem = dynamic_cast<PlayerListItem*>(m_pApplyListView->getItem(nSelectedIdx));
				pSelectedItem->selected(true);
				m_nSelectedIdx = nSelectedIdx;*/

				pSelectedItem = dynamic_cast<PlayerListItem*>(m_pApplyListView->getItem(nSelectedIdx));
				pSelectedItem->selected(!pSelectedItem->isSelected());
			}
			break;
		default:
			break;
	}
}
