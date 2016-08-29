#include "TencentLoginPanel.h"
#include "ChannelInf.h"

TencentLoginPanel::TencentLoginPanel()
:m_pBgImg(NULL)
,m_pWeiXinLoginBtn(NULL)
,m_pQQLoginBtn(NULL)
{
    m_bIsSelQQ = false;
    m_bIsSelWX = false;
}

TencentLoginPanel::~TencentLoginPanel()
{
}

TencentLoginPanel* TencentLoginPanel::create() {
	TencentLoginPanel *pRet = new TencentLoginPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool TencentLoginPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void TencentLoginPanel::open() {
	BasePanel::open();

	if(!m_bUIInited) {
		initUI();
	}
}

void TencentLoginPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/TencentLoginPanelUI.ExportJson");
	addChild(m_pRootLayout);

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();

	m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));
	m_pBgImg->loadTexture("bg_login.png", UI_TEX_TYPE_PLIST);
	float scaleX = screenSize.width / m_pBgImg->getSize().width;
	float scaleY = screenSize.height / m_pBgImg->getSize().height;
	float scale = scaleX > scaleY ? scaleX : scaleY;
	m_pBgImg->setScale(scale);

	m_pWeiXinLoginBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("weixin_login_btn"));
	m_pWeiXinLoginBtn->addTouchEventListener(this, toucheventselector(TencentLoginPanel::touchEventHandler));

	m_pQQLoginBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("qq_login_btn"));
	m_pQQLoginBtn->addTouchEventListener(this, toucheventselector(TencentLoginPanel::touchEventHandler));

	m_bUIInited = true;
}

void TencentLoginPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pWeiXinLoginBtn->getName()) == 0) {
				// weixin login
                m_bIsSelWX = true;
                ChannelInf::doRunProtocol(0, "qqmsdk_setplat", "wx");
			} else if(strcmp(pszWidgetName, m_pQQLoginBtn->getName()) == 0) {
				// qq login
                m_bIsSelQQ = true;
                ChannelInf::doRunProtocol(0, "qqmsdk_setplat", "qq");
			} else {
                break;
            }
            this->showLoading();
            ChannelInf::Login();

			break;

	}
}
