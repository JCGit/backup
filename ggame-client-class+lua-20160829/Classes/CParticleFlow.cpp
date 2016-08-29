#include "CParticleFlow.h"


static CCTexture2D* getDefaultTexture()
{
	CCTexture2D* pTexture = NULL;
	CCImage* pImage = NULL;
	do 
	{
		bool bRet = false;
		const char* key = "__firePngData";
		pTexture = CCTextureCache::sharedTextureCache()->textureForKey(key);
		CC_BREAK_IF(pTexture != NULL);

		pImage = new CCImage();
		CC_BREAK_IF(NULL == pImage);
		bRet = pImage->initWithImageData((void*)__firePngData, sizeof(__firePngData), CCImage::kFmtPng);
		CC_BREAK_IF(!bRet);

		pTexture = CCTextureCache::sharedTextureCache()->addUIImage(pImage, key);
	} while (0);

	CC_SAFE_RELEASE(pImage);

	return pTexture;
}

CCParticleFlow::CCParticleFlow(void)
{
}


CCParticleFlow::~CCParticleFlow(void)
{
}


CCParticleFlow* CCParticleFlow::createWithTotalParticles(unsigned int numberOfParticles,ccColor4F startColor)
{
	CCParticleFlow* pRet = new CCParticleFlow();
	if (pRet && pRet->initWithTotalParticles(numberOfParticles,startColor))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool CCParticleFlow::initWithTotalParticles(unsigned int numberOfParticles,ccColor4F startColor)
{
	if( CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
	{
		// additive
		this->setBlendAdditive(true);

		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		setEmitterMode(kCCParticleModeGravity);
		setPositionType(kCCPositionTypeRelative);
		// Gravity Mode: gravity
		setGravity(ccp(0,0));

		setAngleVar(0);
		setSpeed(0.0f);
		setAngle(0.0);

		m_fLife = 1.5f*0.6;
		m_fLifeVar = 0.0f;

		// size, in pixels
		m_fStartSize = 10.0f;
		m_fStartSizeVar = 2.0f;
		m_fEndSize = 3.0f;
		m_fEndSizeVar =0.0f		
			;
		// emits per second
		m_fEmissionRate = m_uTotalParticles/m_fLife;

		m_tStartColor.r = startColor.r;
		m_tStartColor.g = startColor.g;
		m_tStartColor.b = startColor.b;
		m_tStartColor.a = startColor.a;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D* pTexture = getDefaultTexture();
		if (pTexture != NULL)
		{
			setTexture(pTexture);
		}

		return true;
	}
	return false;
}



CCParticleBlink::CCParticleBlink(void)
{
}


CCParticleBlink::~CCParticleBlink(void)
{
}


CCParticleBlink* CCParticleBlink::createWithTotalParticles(unsigned int numberOfParticles)
{
	CCParticleBlink* pRet = new CCParticleBlink();
	if (pRet && pRet->initWithTotalParticles(numberOfParticles))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool CCParticleBlink::initWithTotalParticles(unsigned int numberOfParticles)
{
	if( CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
	{
		// additive
		this->setBlendAdditive(true);

		// duration
		m_fDuration = kCCParticleDurationInfinity;

		setEmitterMode(kCCParticleModeRadius);
		
		setStartRadius(20.0);
		setStartRadiusVar(0.0);
		setEndRadius(45.0);
		setEndRadiusVar(0.0);


		m_fLife = 4.0;
		m_fLifeVar = 2.0f;

		// size, in pixels
		m_fStartSize = 100.0f;
		m_fStartSizeVar = 20.0f;
		m_fEndSize = 20.0f;
		m_fEndSizeVar = 10.0f		
			;
		// emits per second
		m_fEmissionRate =1.0;

		m_fStartSpin = 0.0f;
		m_fStartSpinVar = 20.0f;
		m_fEndSpin = 50.0;
		m_fEndSpinVar = 100.0f;

		setAngleVar(360.0);
		
		setAngle(360.0);
		//setSpeed(0.0f);

		m_tStartColor.r = 1.0;
		m_tStartColor.g = 1.0;
		m_tStartColor.b = 1.0;
		m_tStartColor.a = 1.0;
		m_tStartColorVar.r = 1.0f;
		m_tStartColorVar.g = 1.0f;
		m_tStartColorVar.b = 1.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D* pTexture = getDefaultTexture();
		if (pTexture != NULL)
		{
			setTexture(pTexture);
		}

		return true;
	}
	return false;
}
