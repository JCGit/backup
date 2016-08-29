#include "LevelNotEnoughPanel.h"

#include "CfgMaster/EntryCfgMaster.h"

#include "basic/TextHelper.h"

#include "game/city/EntryControl.h"

LevelNotEnoughPanel::LevelNotEnoughPanel()
:m_pCloseBtn(NULL)
,m_pPromptTxt(NULL)
,m_pEntrysLayout(NULL)
{
}

LevelNotEnoughPanel::~LevelNotEnoughPanel()
{
}

LevelNotEnoughPanel* LevelNotEnoughPanel::create() {
	LevelNotEnoughPanel *pRet = new LevelNotEnoughPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LevelNotEnoughPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void LevelNotEnoughPanel::openWithLimitLevel(unsigned int uLimitLevel) {
	open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	char text[100];
	snprintf(text, 100, TextStr(TEXT_TASK_LEVEL_UNREACHED), uLimitLevel);
	m_pPromptTxt->setText(text);

	m_pEntrysLayout->removeAllChildren();

	unsigned int entrys[] = {
		ENTRY_WAR,
		ENTRY_DAILY_TASK,
		ENTRY_DAILY_WAR,
	};

	vector<unsigned int> stEntrys;

	for(unsigned int i = 0; i < sizeof(entrys) / sizeof(unsigned int); i++) {
		unsigned int uEntryID = entrys[i];

		if(P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			stEntrys.push_back(uEntryID);
		}
	}

	float originX = m_pEntrysLayout->getSize().width / 2 - (stEntrys.size() * (120 + 8) - 8) / 2 + 120 / 2;
	float originY = m_pEntrysLayout->getSize().height / 2;

	for(unsigned int i = 0; i < stEntrys.size(); i++) {
		unsigned int uEntryID = stEntrys[i];
		UIButton *pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
		pBtn->setPosition(ccp(originX + 128 * i, originY));
		m_pEntrysLayout->addChild(pBtn);
		pBtn->addTouchEventListener(this, toucheventselector(LevelNotEnoughPanel::touchEventHandler));
	}

	closeLoading();
}

void LevelNotEnoughPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/LevelNotEnoughPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(LevelNotEnoughPanel::touchEventHandler));

	m_pPromptTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("prompt_txt"));

	m_pEntrysLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("entrys_layout"));

	m_bUIInited = true;

}

void LevelNotEnoughPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			} else if(string(pszWidgetName).find(ENTRY_PREFIX_NAME) == 0) {
				close();
				string btnName = pszWidgetName;
				unsigned int uEntryID = atol(btnName.substr(string(ENTRY_PREFIX_NAME).size()).c_str());
				P_ENTRY_CTRL->enter(uEntryID);
			}
			break;
		default:;
	}
}
