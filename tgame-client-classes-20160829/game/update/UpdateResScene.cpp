#include "UpdateResScene.h"
#include "UpdateResScreen.h"
#include "SceneManager.h"

UpdateResScene::UpdateResScene()
{
    m_pScreen = NULL;
}

UpdateResScene::~UpdateResScene()
{

}


void UpdateResScene::loadRes() {
    BaseScene::loadRes();

    build();
    SceneManager::sharedSceneManager()->enterScene(this);
}

void UpdateResScene::onEnter() {
	BaseScene::onEnter();
    m_pScreen = UpdateResScreen::create();
	addChild(m_pScreen);
}

void UpdateResScene::onExit() {
	BaseScene::onExit();
	removeChild(m_pScreen);
}


bool UpdateResScene::init() {
    bool bRet = false;

    do {
        CC_BREAK_IF(!BaseScene::init());

        addLayer(LAYER_ID_UI);
        addLayer(LAYER_ID_TOP);
        addLayer(LAYER_ID_POP);

        bRet = true;
    } while(0);

    return bRet;
}

void UpdateResScene::setPercentage(float fPercentage) {
    if(m_pScreen) {
        m_pScreen->setPercentage(fPercentage);
    }
}

void UpdateResScene::setNoticeWords(const char *pszNoticeWords) {
    if(m_pScreen) {
        m_pScreen->setNoticeWords(pszNoticeWords);
    }
}