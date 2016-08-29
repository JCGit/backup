#include "AwardPromptPanel.h"

AwardPromptPanel::AwardPromptPanel()
:m_pCloseBtn(NULL)
,m_pItemFrameImg(NULL)
,m_pItemNameTxt(NULL)
{
}

AwardPromptPanel::~AwardPromptPanel()
{
}

AwardPromptPanel* AwardPromptPanel::create() {
	AwardPromptPanel *pRet = new AwardPromptPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool AwardPromptPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void AwardPromptPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	closeLoading();
}

void AwardPromptPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/AwardPromptPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(AwardPromptPanel::touchEventHandler));

	m_pItemFrameImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_frame_img"));

	m_pItemNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("item_name_txt"));

	m_bUIInited = true;

}

void AwardPromptPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			}
			break;
		default:;
	}
}
