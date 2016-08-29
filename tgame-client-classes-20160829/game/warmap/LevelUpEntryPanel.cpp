#include "LevelUpEntryPanel.h"

#include "game/city/EntryControl.h"

#include "WarMapControl.h"

LevelUpEntryPanel::LevelUpEntryPanel()
:m_pCloseBtn(NULL)
,m_pEntrysListView(NULL)
{
}

LevelUpEntryPanel::~LevelUpEntryPanel()
{
}

LevelUpEntryPanel* LevelUpEntryPanel::create() {
	LevelUpEntryPanel *pRet = new LevelUpEntryPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LevelUpEntryPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void LevelUpEntryPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	m_pEntrysListView->removeAllItems();

	unsigned int entrys[] = {
		ENTRY_WAR,
		ENTRY_DAILY_WAR,
		ENTRY_BOSS_WAR,
	};

	for(unsigned int i = 0; i < sizeof(entrys) / sizeof(unsigned int); i++) {
		unsigned int uEntryID = entrys[i];
		
		if(P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			UIButton *pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
			m_pEntrysListView->pushBackCustomItem(pBtn);
		}
	}

	closeLoading();
}

void LevelUpEntryPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/LevelUpEntryPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(LevelUpEntryPanel::touchEventHandler));

	m_pEntrysListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("entrys_list"));
	m_pEntrysListView->addEventListenerListView(this, listvieweventselector(LevelUpEntryPanel::listViewEventHandler));

	m_bUIInited = true;
}

void LevelUpEntryPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			}/* else if(string(pszWidgetName).find(ENTRY_PREFIX_NAME) == 0) {
				
			}*/
			break;
		default:;
	}
}

void LevelUpEntryPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(pSender == m_pEntrysListView) {
				int nSelectedIdx = m_pEntrysListView->getCurSelectedIndex();
				UIButton *pBtn = dynamic_cast<UIButton*>(m_pEntrysListView->getItem(nSelectedIdx));

				if(pBtn != NULL) {
					string btnName = pBtn->getName();
					unsigned int uEntryID = atol(btnName.substr(string(ENTRY_PREFIX_NAME).size()).c_str());
					
					if(uEntryID != ENTRY_UNKNOWN) {
						close();
						P_WAR_MAP_CTRL->getBeStrongerPanel()->close();
						P_ENTRY_CTRL->enter(uEntryID);
					}
				}
			}
			break;
		default:;
	}
}
