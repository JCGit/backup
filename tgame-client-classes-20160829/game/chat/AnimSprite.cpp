#include "AnimSprite.h"

AnimSprite::AnimSprite()
:m_uAnimFrames(0)
,m_fAnimDelay(0.0f)
,m_stAnimFrameSize(CCSizeZero)
,m_pAnimate(NULL)
{
}

AnimSprite::~AnimSprite()
{
	CC_SAFE_RELEASE_NULL(m_pAnimate);
}

AnimSprite* AnimSprite::create(const char *pszFileName, const CCSize &size, unsigned int uFrames, float delay/* = 0.1f*/) {
	AnimSprite *pRet = new AnimSprite();

	if(pRet != NULL && pRet->init(pszFileName, size, uFrames, delay)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool AnimSprite::init(const char *pszFileName, const CCSize &size, unsigned int uFrames, float delay) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCSprite::init());

		m_stAnimFrameSize= size;
		CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFileName);
		CCAnimation *pAnimation = CCAnimation::create();

		for(unsigned int i = 0; i < uFrames; i++) {
			CCRect rect = CCRect(size.width * i, 0, size.width, size.height);
			pAnimation->addSpriteFrame(CCSpriteFrame::createWithTexture(pTexture, rect));
		}

		pAnimation->setDelayPerUnit(delay);
		m_pAnimate = CCAnimate::create(pAnimation);
		m_pAnimate->retain();

		bRet = true;
	} while(0);

	return bRet;
}

void AnimSprite::onEnterTransitionDidFinish() {
	CCSprite::onEnterTransitionDidFinish();
	play();
}

void AnimSprite::onExit() {
	CCSprite::onExit();
	stop();
}

void AnimSprite::play() {
	runAction(CCRepeatForever::create(m_pAnimate));
}

void AnimSprite::stop() {
	stopAllActions();
}
