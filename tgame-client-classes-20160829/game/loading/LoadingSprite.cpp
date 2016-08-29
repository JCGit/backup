#include "LoadingSprite.h"

LoadingSprite::LoadingSprite()
{
}

LoadingSprite::~LoadingSprite()
{
}

LoadingSprite* LoadingSprite::create() {
	LoadingSprite *pLoadingSprite = new LoadingSprite();

	if(pLoadingSprite != NULL && pLoadingSprite->initWithFile("loading/load.png")) {
		pLoadingSprite->autorelease();
		return pLoadingSprite;
	} 

	CC_SAFE_DELETE(pLoadingSprite);

	return NULL;
}

void LoadingSprite::onEnter() {
	CCSprite::onEnter();
	scheduleUpdate();
}

void LoadingSprite::onExit() {
	CCSprite::onExit();
	unscheduleUpdate();
}

void LoadingSprite::update(float delta) {
	setRotation(this->getRotation() + 6);
}