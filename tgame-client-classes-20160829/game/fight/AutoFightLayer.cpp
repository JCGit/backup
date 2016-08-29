#include "AutoFightLayer.h"

const string CAutoFightLayer::CANCEL_AUTO_EVENT = "CANCEL_AUTO_EVENT";

CAutoFightLayer::CAutoFightLayer()
:m_pArm(NULL)
,m_pLayer(NULL)
,m_pCancelBtn(NULL)
{

}

CAutoFightLayer::~CAutoFightLayer(){

}

bool CAutoFightLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(size.width + largeOffset * 2, size.height + largeOffset * 2);

		m_pLayer = UILayout::create();
		m_pLayer->setTouchEnabled(true);
		m_pLayer->setSize(largeSize);
		addChild(m_pLayer);

		setSize(size);
		setPosition(point);

		m_pArm = CCArmature::create("ut00078");
		m_pArm->setPosition(ccp(size.width/2 - 10, 87));
		m_pArm->getAnimation()->play("fly", -1, -1, 1);
		m_pLayer->addNode(m_pArm);

		m_pCancelBtn = Button::create();
		m_pCancelBtn->setName("cancel_btn");
		m_pCancelBtn->loadTextures("share/btn_yellow_up.png", "share/btn_yellow_down.png", "share/btn_yellow_disabled.png", UI_TEX_TYPE_PLIST);
		float disX = m_pCancelBtn->getContentSize().width/2 + m_pArm->getPositionX() + m_pArm->getContentSize().width/2 + 10;
		m_pCancelBtn->setPosition(ccp(disX, m_pArm->getPositionY()));
		m_pCancelBtn->addTouchEventListener(this, toucheventselector(CAutoFightLayer::touchEventHandler));
		m_pLayer->addChild(m_pCancelBtn);

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");
		ImageView* pImg = ImageView::create();
		pImg->loadTexture("fight/cancel_auto_img.png", UI_TEX_TYPE_PLIST);
		m_pCancelBtn->addChild(pImg);

		bRet = true;
	} while(0);

	return bRet;
}

void CAutoFightLayer::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, "cancel_btn") == 0) { 
				DispatchEvent(CANCEL_AUTO_EVENT, this);
			}
			break;
	}
}


