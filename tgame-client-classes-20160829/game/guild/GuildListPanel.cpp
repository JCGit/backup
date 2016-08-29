#include "GuildListPanel.h"

#include "CfgMaster/NounCfgMaster.h"

#include "basic/NounHelper.h"

#include "game/UserObj.h"

#include "GuildListItem.h"
#include "GuildControl.h"

GuildListPanel::GuildListPanel()
:m_pBackBtn(NULL)
,m_pGuildListView(NULL)
,m_pFindBtn(NULL)
,m_pGuildBtn(NULL)
,m_pEmptyListTipTxt(NULL)
{
}

GuildListPanel::~GuildListPanel()
{
}

GuildListPanel* GuildListPanel::create() {
	GuildListPanel *pRet = new GuildListPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildListPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void GuildListPanel::open() {
	BasePanel::open();

	showLoading();

	P_GUILD_CTRL->sendQueryGuildListRqst();

	if(!m_bUIInited) {
		initUI();
	}
}

void GuildListPanel::refresh() {
	BasePanel::refresh();
	showLoading();
	P_GUILD_CTRL->sendQueryGuildListRqst();
}

void GuildListPanel::updateGuildList() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	m_pGuildListView->removeAllItems();

	unsigned int uAppliedGuildCnt = 0;
	const vmsg::CSQueryGuildLstRsp &stGuildListInfo = P_GUILD_CTRL->getGuildListInfo();
	int nGuildLstSize = stGuildListInfo.guildlst_size();

	for(int i = 0; i < nGuildLstSize; i++) {
		GuildListItem *pItem = GuildListItem::create();
		bool bApplied = isGuildApplied(stGuildListInfo.guildlst(i).guildid());
		pItem->updateUI(stGuildListInfo.guildlst(i), bApplied);
		m_pGuildListView->pushBackCustomItem(pItem);

		if(bApplied) {
			uAppliedGuildCnt++;
		}
	}

	m_pEmptyListTipTxt->setVisible(nGuildLstSize <= 0);

	m_pFindBtn->setTitleText(NounStr(NOUN_SEARCH));
	m_pFindBtn->setBright(nGuildLstSize > 0);
	m_pFindBtn->setTouchEnabled(nGuildLstSize > 0);

	m_pGuildBtn->setVisible(P_USER_OBJ->getUserInfo()->guildID == 0);
	P_GUILD_CTRL->setAppliedGuildCnt(uAppliedGuildCnt);

	closeLoading();
}

void GuildListPanel::updateFindGuild() {
	m_pGuildListView->removeAllItems();

	const vmsg::CSGuildBrief &stGuildSearchInfo = P_GUILD_CTRL->getGuildSearchInfo();

	GuildListItem *pItem = GuildListItem::create();
	pItem->updateUI(stGuildSearchInfo, isGuildApplied(stGuildSearchInfo.guildid()));
	m_pGuildListView->pushBackCustomItem(pItem);

	m_pFindBtn->setTitleText(NounStr(NOUN_ALL_GUILD));
}

void GuildListPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildListPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(GuildListPanel::touchEventHandler));

	m_pGuildListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("guild_list"));
	m_pGuildListView->addEventListenerListView(this, listvieweventselector(GuildListPanel::listViewEventHandler));

	m_pFindBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("find_btn"));
	m_pFindBtn->setTitleText(NOUN_CFG_MASTER->GetStr(NOUN_SEARCH));
	m_pFindBtn->addTouchEventListener(this, toucheventselector(GuildListPanel::touchEventHandler));

	m_pGuildBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("guild_btn"));
	m_pGuildBtn->addTouchEventListener(this, toucheventselector(GuildListPanel::touchEventHandler));

	m_pEmptyListTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("empty_list_tip_txt"));
	m_pEmptyListTipTxt->setVisible(false);

	schedule(schedule_selector(GuildListPanel::updateApplied), 5 * 60.0f);

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateGuildList();
	}
}

void GuildListPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pFindBtn->getName()) == 0) {
				if(NounStr(NOUN_ALL_GUILD) == m_pFindBtn->getTitleText()) {
					updateGuildList();
				} else if(NounStr(NOUN_SEARCH) == m_pFindBtn->getTitleText()) {
					P_GUILD_CTRL->getGuildFindPanel()->open();
				}
			} else if(strcmp(pszWidgetName, m_pGuildBtn->getName()) == 0) {
				P_GUILD_CTRL->getGuildCreatePanel()->open();
			}
			break;
		default:;
	}
}

void GuildListPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszListViewName, m_pGuildListView->getName()) == 0) {
				int nSelectedIdx = m_pGuildListView->getCurSelectedIndex();
				GuildListItem *pItem = dynamic_cast<GuildListItem*>(m_pGuildListView->getItem(nSelectedIdx));

				/*const vmsg::CSGuildBrief &stGuildInfo = pItem->getGuildInfo();
				P_GUILD_CTRL->openGuildPanel();
				P_GUILD_CTRL->sendQueryOtherGuildRqst(stGuildInfo.guildid());*/
			}
			break;
		default:;
	}
}

bool GuildListPanel::isGuildApplied(UINT64_t uGuildID) {
	bool bRet = false;

	const vmsg::CSQueryGuildLstRsp &stGuildListInfo = P_GUILD_CTRL->getGuildListInfo();

	for(int i = 0; i < stGuildListInfo.guildreqlst_size(); i++) {
		if(uGuildID == stGuildListInfo.guildreqlst(i)) {
			bRet = true;
			break;
		}
	}

	return bRet;
}

void GuildListPanel::updateApplied(float dt) {
	P_GUILD_CTRL->sendQueryGuildListRqst();
}
