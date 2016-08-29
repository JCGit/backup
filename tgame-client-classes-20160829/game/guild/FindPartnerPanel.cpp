#include "FindPartnerPanel.h"

#include "PartnerListItem.h"

FindPartnerPanel::FindPartnerPanel()
:m_pCloseBtn(NULL)
,m_pPartnerListView(NULL)
,m_pInviteBtn(NULL)
,m_pCancelBtn(NULL)
,m_nSelectedIdx(-1)
{
}

FindPartnerPanel::~FindPartnerPanel()
{
}

FindPartnerPanel* FindPartnerPanel::create() {
	FindPartnerPanel *pRet = new FindPartnerPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool FindPartnerPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui/FindPartnerPanelUI.json");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(FindPartnerPanel::touchEventHandler));

		m_pPartnerListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("partner_list"));
		m_pPartnerListView->addEventListenerListView(this, listvieweventselector(FindPartnerPanel::listViewEventHandler));

		m_pInviteBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("invite_btn"));
		m_pInviteBtn->addTouchEventListener(this, toucheventselector(FindPartnerPanel::touchEventHandler));

		m_pCancelBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("cancel_btn"));
		m_pCancelBtn->addTouchEventListener(this, toucheventselector(FindPartnerPanel::touchEventHandler));

		updateUI();

		bRet = true;
	} while(0);

	return bRet;
}

void FindPartnerPanel::updateUI() {
	m_pPartnerListView->removeAllItems();

	// test
	for(int i = 0; i < 16; i++) {
		PartnerListItem *pItem = PartnerListItem::create();
		pItem->updateUI();
		m_pPartnerListView->pushBackCustomItem(pItem);
	}
}

void FindPartnerPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
			close();
		} else if(std::strcmp(pszWidgetName, "invite_btn") == 0) {

		} else if(std::strcmp(pszWidgetName, "cancel_btn") == 0) {
			close();
		}
		break;
	default:
		break;
	}
}

void FindPartnerPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	int nSelectedIdx = m_pPartnerListView->getCurSelectedIndex();
	PartnerListItem *pItem = NULL;

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(nSelectedIdx >= 0) {
				if(m_nSelectedIdx >= 0) {
					pItem = dynamic_cast<PartnerListItem*>(m_pPartnerListView->getItem(m_nSelectedIdx));
					pItem->selected(false);
				}

				pItem = dynamic_cast<PartnerListItem*>(m_pPartnerListView->getItem(nSelectedIdx));
				pItem->selected(true);
				m_nSelectedIdx = nSelectedIdx;
			}
			break;
		default:
			break;
	}
}
