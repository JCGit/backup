#include "LoadingScene.h"
#include "LoadingScreen.h"

static LoadingScene *s_pLoadingScene = NULL;

LoadingScene *LoadingScene::sharedLoadingScene() {
	if(s_pLoadingScene == NULL) {
		s_pLoadingScene = new LoadingScene();
	}

	return s_pLoadingScene;
}

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::onEnter() {
	BaseScene::onEnter();
	addChild(LoadingScreen::getInstance());
}

void LoadingScene::onExit() {
	BaseScene::onExit();
	removeChild(LoadingScreen::getInstance());
}


void LoadingScene::setPercentage(float fPercentage) {
	LoadingScreen::getInstance()->setPercentage(fPercentage);
}

void LoadingScene::setNoticeWords(const char *pszNoticeWords) {
	LoadingScreen::getInstance()->setNoticeWords(pszNoticeWords);
}