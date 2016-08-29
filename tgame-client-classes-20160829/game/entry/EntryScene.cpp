#include "EntryScene.h"

#include "basic/SceneManager.h"
#include "basic/UIBoxLayer.h"

#include "game/city/CityControl.h"

#include "EntryScreenView.h"
#include "ToolUtils.h"
EntryScene::EntryScene()
:m_pScreenView(NULL)
{
}

EntryScene::~EntryScene()
{
    CCLOG("[trace]=====destroy scene:EntryScene=====");
}

EntryScene* EntryScene::create() {
	EntryScene *pRet = new EntryScene();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EntryScene::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseScene::init());

		addLayer(LAYER_ID_WIDGET);
		addLayer(LAYER_ID_UI);
		addLayer(LAYER_ID_POP);
		addLayer(LAYER_ID_TOP);

		bRet = true;
	} while(0);

	return bRet;
}

void EntryScene::onEnter() {
	BaseScene::onEnter();

}

void EntryScene::onExit() {
	BaseScene::onExit();

}

void EntryScene::loadRes() {
	BaseScene::loadRes();

	m_stRes.clear();
	m_stRes.insert("ui_bg/bg_entry_scene.jpg");
	//m_stRes.insert("ui_bg/bg_panel.jpg");
    //m_stRes.insert("ui_bg/bg_panel.plist");

	CCTextureCache::sharedTextureCache()->addImage("ui_bg/bg_entry_scene.jpg");
    //CCTextureCache::sharedTextureCache()->addImage("ui_bg/bg_panel.jpg");
    //CToolUtils::loadSpriteFrameRGBA4444("ui_bg/bg_panel.plist");

	build();
	SceneManager::sharedSceneManager()->prepareNextSceneRes();
}

void EntryScene::build() {
	BaseScene::build();

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();

	m_pScreenView = EntryScreenView::create();
	
	/*HeadLayout *pHeadLayout = P_CITY_CTRL->getHeadLayout();
	
	UILayer *pWidgetLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_WIDGET));

	if(pWidgetLayer != NULL) {
		pWidgetLayer->addWidget(m_pScreenView);
		m_pScreenView->setPosition(screenOrigin);

		if(pHeadLayout->getParent() != NULL) {
			pHeadLayout->removeFromParent();
		}

		pWidgetLayer->addWidget(pHeadLayout);
		pHeadLayout->setPosition(ccp(screenOrigin.x, screenOrigin.y + screenSize.height - pHeadLayout->getSize().height));
	}*/

}
