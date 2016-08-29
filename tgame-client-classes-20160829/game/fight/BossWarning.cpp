#include "BossWarning.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"
#include "control/AudioManager.h"

const string CBossWarning::BOSS_WARNING_COMPLETE = "boss_warning_complete";

CBossWarning::CBossWarning(){
	m_uEffectID = 0;
}

CBossWarning::~CBossWarning(){

}

bool CBossWarning::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BaseLayout::init());

		this->setAnchorPoint(ccp(0.5, 0.5));

		m_pWarningImg = ImageView::create();
		m_pWarningImg->loadTexture("ui_bg/warning.png");
		addChild(m_pWarningImg);
		m_pWarningImg->setOpacity(0x00);

		setSize(m_pWarningImg->getSize());

		string fullPath = SOUND_RES_PATH + "boss_warning.ogg";
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(fullPath.c_str());
		AudioManager::preloadEffect(fullPath.c_str());

		bRet = true;
	}while(0);

	return bRet;
}

void CBossWarning::showWarning(){
	float duration = 0.3f;
	m_pWarningImg->setOpacity(0x00);

	CCActionInterval *fadeIn = CCFadeTo::create(duration, 0xFF);
	CCActionInterval *fadeOut = CCFadeTo::create(duration, 0x00);
	CCFiniteTimeAction* soundFun = CCCallFunc::create(this, callfunc_selector(CBossWarning::playSound));
	CCFiniteTimeAction* spawn = CCSpawn::create(CCSequence::create(soundFun, fadeIn, fadeOut, NULL), NULL);
	CCFiniteTimeAction* endFun = CCCallFunc::create(this, callfunc_selector(CBossWarning::endFun));
	CCFiniteTimeAction* spawn2 = CCSpawn::create(CCDelayTime::create(duration), soundFun, NULL);

	m_pWarningImg->runAction(CCSequence::create(CCRepeat::create(spawn, 3), spawn2, endFun, NULL));
}

void CBossWarning::stopWarning(){
	m_pWarningImg->stopAllActions();
	AudioManager::stopEffect(m_uEffectID);
}

void CBossWarning::playSound(){
	m_uEffectID = AudioManager::playEffect("boss_warning.ogg");
}

void CBossWarning::endFun(){
	AudioManager::stopEffect(m_uEffectID);
	//this->removeFromParentAndCleanup(true);

	this->DispatchEvent(BOSS_WARNING_COMPLETE, this);
}