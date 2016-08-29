//
//  LightSweepSprite.cpp
//  DevPro
//
//  Created by long on 13-12-18.
//
//

#include "LightSweepSprite.h"
#include "StringConverter.h"
#include "DataTableManager.h"
#include <ctime>

LightSweepSprite* LightSweepSprite::create(const char *pszFileName)
{
    LightSweepSprite *pobSprite = new LightSweepSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

LightSweepSprite::LightSweepSprite()
{
    m_posSpeed = START_SPEED;
	m_posInitSpeed = START_SPEED;
	m_posAcceleratedSpeed = ACCELERATED_SPEED;
	this->initData();
}

LightSweepSprite::LightSweepSprite(float speed, float accelerate)
{
	m_posSpeed = speed;
	m_posInitSpeed = speed;
	m_posAcceleratedSpeed = accelerate;
	this->initData();
}

void LightSweepSprite::initData()
{
	m_frequence = -90;
	m_frequenceMax = FREQUENCE;
	m_pTarget = NULL;
	m_pStar = NULL;
	m_scaleToMaxTime = 0.4f;
	m_scaleToMinTime = 0.25f;
	m_pTargetTexture = NULL;
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this,0,false);
}

LightSweepSprite::~LightSweepSprite()
{

	if (m_pStar)
	{
		m_pStar->release();
	}
	if (m_pTarget)
	{
		m_pTarget->release();
	}

	if (m_pTargetTexture)
	{
		m_pTargetTexture->release();
	}

	CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

bool LightSweepSprite::initWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCRect &rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
	{
		this->initProgram();
		return true;
	}
    
	return false;
}

bool LightSweepSprite::initWithFile(const char *pszFilename)
{
    CCAssert(pszFilename != NULL, "Invalid filename for sprite");
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
    if (pTexture)
    {
        CCRect rect = CCRectZero;
        rect.size = pTexture->getContentSize();
        return initWithTexture(pTexture, rect);
    }
    
    return false;
}

void LightSweepSprite::initProgram()
{
    //GLchar * vertSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(getVertShaderFile()).c_str())->getCString();
	GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(getShaderFile()).c_str())->getCString();
	CCGLProgram* pProgram = new CCGLProgram();
	pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
    //pProgram->initWithVertexShaderByteArray(vertSource, fragSource);
	setShaderProgram(pProgram);
	pProgram->release();
    
	CHECK_GL_ERROR_DEBUG();
    
	getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
	CHECK_GL_ERROR_DEBUG();
    
	getShaderProgram()->link();
    
	CHECK_GL_ERROR_DEBUG();
    
	getShaderProgram()->updateUniforms();
    
	CHECK_GL_ERROR_DEBUG();
    
	initUniforms();
    
	CHECK_GL_ERROR_DEBUG();
}

void LightSweepSprite::draw()
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
//    ccGLBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ccGLBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	getShaderProgram()->use();
	getShaderProgram()->setUniformsForBuiltins();
	
	setUniforms();
	
	ccGLBindTexture2D(  getTexture()->getName() );
    
	//
	// Attributes
	//
#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;
    
	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	CC_INCREMENT_GL_DRAWS(1);    
}

void LightSweepSprite::update(float dt)
{
	++m_frequence;
	m_posSpeed += m_posAcceleratedSpeed;
	CCPoint pos = this->getPosition();
	this->setPosition(ccp(pos.x+m_posSpeed,pos.y));
	pos = this->getPosition();
	if (m_pStar && pos.x >= m_srcPosition.x + m_targetSize.width && this->isVisible())
	{
		//扫光结束，自身设置不可见，出现闪光的星星
		this->setVisible(false);
		CCPoint pos =  this->getStarRandPos();
		m_pTarget->addChild(m_pStar);
		m_pStar->setPosition(pos);
		m_pStar->setScale(0.0f);
		CCScaleTo* scaleMax = CCScaleTo::create(m_scaleToMaxTime,1.0f,1.0f);
		CCScaleTo* scaleMin = CCScaleTo::create(m_scaleToMinTime,0.0f,0.0f);
		CCSequence* seq = CCSequence::create(scaleMax,scaleMin,CCCallFuncN::create(this,callfuncN_selector(LightSweepSprite::onStarFlickerEnd)),NULL);
		m_pStar->runAction(seq);
	}

	if (pos.x >= m_srcPosition.x + m_targetSize.width && m_frequence >= m_frequenceMax)
	{
		this->setPosition(m_srcPosition);
		m_posSpeed = m_posInitSpeed;
		m_frequence = 0;
		this->setVisible(true);
		m_frequenceMax = rand() % 90 + FREQUENCE;
	}
}

void LightSweepSprite::onStarFlickerEnd(CCNode* node)
{
	if (m_pStar)
	{
		m_pStar->removeFromParentAndCleanup(false);
	}
}

void LightSweepSprite::initUniforms()
{
    sampler = this->getShaderProgram()->getUniformLocationForName("GL_Texture1");
    pos = this->getShaderProgram()->getUniformLocationForName("pos");
    texSize = this->getShaderProgram()->getUniformLocationForName("texSize");
    targetSize = this->getShaderProgram()->getUniformLocationForName("targetSize");
}

void LightSweepSprite::setUniforms()
{
	ccGLBindTexture2DN(1,m_pTargetTexture->getName());
    this->getShaderProgram()->setUniformLocationWith1i(sampler, 1);
	float x = this->getPosition().x-m_srcPosition.x-this->getContentSize().width*this->getAnchorPoint().x;
    this->getShaderProgram()->setUniformLocationWith1f(pos, x);
    this->getShaderProgram()->setUniformLocationWith2f(texSize, this->getContentSize().width, this->getContentSize().height);
    this->getShaderProgram()->setUniformLocationWith2f(targetSize, m_targetSize.width, m_targetSize.height);
}

char* LightSweepSprite::getShaderFile()
{
    static char retstr[32] = "shader/lightsweep.sha";
	return retstr;
}

char* LightSweepSprite::getVertShaderFile()
{
    static char retstr[32] = "shader/lightsweep.vsha";
	return retstr;
}

void LightSweepSprite::setTarget(cocos2d::CCSprite *sprite)
{
    if (!sprite)
    {
        return;
    }
    m_targetSize = sprite->getContentSize();
	m_srcPosition = ccp(-(this->getContentSize().width-this->getContentSize().width*this->getAnchorPoint().x),sprite->getContentSize().height/2.0f);
	CCSize size = sprite->getTexture()->getContentSize();
	m_pTarget = sprite;
	//m_pTarget->retain();//不能retain，导致互相引用，释放不了

	CCPoint srcPoint = sprite->getPosition();
	int zorder = sprite->getZOrder();
	CCSize spriteSize = sprite->getContentSize();
	CCRenderTexture* renderT = CCRenderTexture::create(spriteSize.width,spriteSize.height);
	renderT->clear(0.0f,0.0f,0.0f,0.0f);
	renderT->begin();
	sprite->setPosition(ccp(spriteSize.width*sprite->getAnchorPoint().x,spriteSize.height*sprite->getAnchorPoint().y));
	sprite->visit();
	renderT->end();

	//CCSprite* renderS = CCSprite::createWithTexture(renderT->getSprite()->getTexture());
	//CCRenderTexture* renderTT = CCRenderTexture::create(spriteSize.width,spriteSize.height);
	//renderTT->clear(0.0f,0.0f,0.0f,0.0f);
	//renderTT->begin();
	//renderS->setPosition(ccp(spriteSize.width*renderS->getAnchorPoint().x,spriteSize.height*renderS->getAnchorPoint().y));
	//renderS->visit();
	//renderTT->end();

	///*m_pTargetTexture = renderTT->getSprite()->getTexture();
	//m_pTargetTexture->retain();*/
	//renderT->saveToFile("sweep.png");

	CCImage* image = renderT->newCCImage();
	m_pTargetTexture = new CCTexture2D();
	m_pTargetTexture->initWithImage(image);

	sprite->setPosition(srcPoint);
	sprite->setZOrder(zorder);
}

void LightSweepSprite::setStarSprite(const char* starFile)
{
	m_pStar = CCSprite::create(starFile);
	m_pStar->retain();
}

void LightSweepSprite::addStarPos(CCPoint pos)
{
	m_starPosVec.push_back(pos);
}

CCPoint LightSweepSprite::getStarRandPos()
{
	if (m_starPosVec.size() == 0)
	{
		return ccp(m_pTarget->getContentSize().width/2.0,m_pTarget->getContentSize().height);
	}

	srand((unsigned)time(0));
	int idx = rand() % m_starPosVec.size();
	return m_starPosVec[idx];
}

namespace LightSweep
{
    LightSweepSprite* createLightSweep(CCSprite* sprite)
    {
		bool sweepFlag = StringConverter::parseBool(VaribleManager::Get()->getSetting("Money_LightSweep_Flag"),false);
		if (!sweepFlag)
		{
			return NULL;
		}
        CCSize spriteSize = sprite->getContentSize();
		CCSprite* lightS = CCSprite::create("lightsweep_5.png");
        LightSweepSprite* light = new LightSweepSprite();
        light->setTarget(sprite);
		light->setStarSprite("star.png");
		light->initWithTexture(lightS->getTexture(),CCRectMake(0.0f,0.0f,lightS->getContentSize().width,lightS->getContentSize().height));
        float scaleY = spriteSize.height/light->getContentSize().height;
        CCLog("scaleY:%f",scaleY);
        light->setScaleY(scaleY);
		light->autorelease();
        return light;
    }


	LightSweepSprite* createLightSweepForMoney(CCSprite* sprite)
	{
		CCSize spriteSize = sprite->getContentSize();
		CCSprite* lightS = CCSprite::create("effect/lightsweep_money.png");
		LightSweepSprite* light = new LightSweepSprite(0.8f,0.20f);
		light->setTarget(sprite);
		light->setScaleToMaxTime(0.4f);
		light->setScaleToMinTime(0.3f);
		light->initWithTexture(lightS->getTexture(),CCRectMake(0.0f,0.0f,lightS->getContentSize().width,lightS->getContentSize().height));
		light->setStarSprite("effect/star_money.png");
		float scaleY = spriteSize.height/light->getContentSize().height;
		light->setScaleY(scaleY);
		light->release();
		return light;
	}

	void createLSforJewel(CCSprite* banner)
	{
		bool sweepFlag = StringConverter::parseBool(VaribleManager::Get()->getSetting("Money_LightSweep_Flag"),false);
		if (!sweepFlag)
		{
			return;
		}
		LightSweepSprite* light = LightSweep::createLightSweepForMoney(banner);
		light->setPosition(ccp(-(light->getContentSize().width-light->getContentSize().width*light->getAnchorPoint().x),banner->getContentSize().height/2.0f));
		light->addStarPos(ccp(3.0f,20.0f));
		light->addStarPos(ccp(29.0f,24.0f));
		light->addStarPos(ccp(17.0f,7.0f));
		light->addStarPos(ccp(15.0f,20.0f));
		light->addStarPos(ccp(26.0f,14.0f));
		light->setTag(LIGHT_SWEEP_TAG);
		banner->addChild(light);
	}
}





