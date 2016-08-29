
#include "BlurSprite.h"
#include "DeviceCfgMgr.h"

BlurSprite::~BlurSprite()
{

}

bool BlurSprite::initWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCRect &rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
	{
		this->initProgram();
		return true;
	}
    
	return false;
}

void BlurSprite::initProgram()
{
	GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(getShaderFile()).c_str())->getCString();
	GLchar * vertSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(getVertShaderFile()).c_str())->getCString();
	CCGLProgram* pProgram = new CCGLProgram();
	pProgram->initWithVertexShaderByteArray(vertSource, fragSource);
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

void BlurSprite::draw()
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
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

void BlurSprite::initUniforms()
{
	radius = getShaderProgram()->getUniformLocationForName("radius");
	dir	= getShaderProgram()->getUniformLocationForName("dir");
}

void BlurSprite::setUniforms()
{
	float rad = 2.5f;
	if (mDir.x > 0.0f)
	{
		this->getShaderProgram()->setUniformLocationWith1f(radius,rad/this->getTexture()->getContentSize().width);
	}
	else
	{
		this->getShaderProgram()->setUniformLocationWith1f(radius,rad/this->getTexture()->getContentSize().height);
	}
	this->getShaderProgram()->setUniformLocationWith2f(dir,mDir.x,mDir.y);
}

char* BlurSprite::getShaderFile()
{
    static char retstr[32] = "shader/blursprite_l.fsha";
	int type = DeviceCfgMgr::Get()->getDeviceBlurType();
	switch (type)
	{
	case DeviceBlurType_Low:
		{
			std::string str = "shader/blursprite_l.fsha";			
			strcpy(retstr,str.c_str());
		}
		break;
	case DeviceBlurType_Middle:
		{
			std::string str = "shader/blursprite_m.fsha";
			strcpy(retstr,str.c_str());
		}
		break;
	case DeviceBlurType_High:
		{
			std::string str = "shader/blursprite_h.fsha";
			strcpy(retstr,str.c_str());
		}
		break;
	default:
		break;
	}
	return retstr;
}

char* BlurSprite::getVertShaderFile()
{
	static char retstr[32] = "shader/blursprite.vsha";
	return retstr;
}

void BlurSprite::setDir(CCPoint dir)
{
	mDir = dir;
}

namespace BlurSpriteMgr
{
	CCSprite* createLowBlur(CCNode* node,CCSize recSize)
	{
		CCPoint anchor = node->getAnchorPoint();
		CCPoint pos = node->getPosition();
		float rotation = node->getRotation();
		CCRenderTexture* rt = CCRenderTexture::create(recSize.width,recSize.height);
		rt->clear(0.0f,0.0f,0.0f,0.0f);
		rt->begin();		
		node->setPosition(ccp(0.0f,0.0f));
		node->setAnchorPoint(ccp(0.0f,0.0f));
		node->setRotation(0.0f);
		node->visit();
		rt->end();

		node->setAnchorPoint(anchor);
		node->setRotation(rotation);
		node->setPosition(pos);

		//横向 
		BlurSprite* blurX = new BlurSprite();
		blurX->setDir(ccp(1,1));
		blurX->initWithTexture(rt->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		blurX->setFlipY(true);
		blurX->autorelease();

		CCRenderTexture* rt4 = CCRenderTexture::create(recSize.width,recSize.height);
		rt4->clear(0.0f,0.0f,0.0f,0.0f);
		rt4->begin();
		blurX->setPosition(ccp(0.0f,0.0f));
		blurX->setAnchorPoint(ccp(0.0f,0.0f));
		blurX->setRotation(0.0f);
		blurX->visit();
		rt4->end();

		CCSprite* res = CCSprite::createWithTexture(rt4->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		res->setFlipY(true);

		return res;
	}

	CCSprite* createMiddleBlur(CCNode* node,CCSize recSize)
	{
		CCPoint anchor = node->getAnchorPoint();
		CCPoint pos = node->getPosition();
		float rotation = node->getRotation();
		CCRenderTexture* rt = CCRenderTexture::create(recSize.width,recSize.height);
		rt->clear(0.0f,0.0f,0.0f,0.0f);
		rt->begin();		
		node->setPosition(ccp(0.0f,0.0f));
		node->setAnchorPoint(ccp(0.0f,0.0f));
		node->setRotation(0.0f);
		node->visit();
		rt->end();

		node->setAnchorPoint(anchor);
		node->setRotation(rotation);
		node->setPosition(pos);

		//横向 
		BlurSprite* blurX = new BlurSprite();
		blurX->setDir(ccp(1,0));
		blurX->initWithTexture(rt->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		blurX->setFlipY(true);
		blurX->autorelease();

		CCRenderTexture* rt2 = CCRenderTexture::create(recSize.width,recSize.height);
		rt2->clear(0.0f,0.0f,0.0f,0.0f);
		rt2->begin();
		blurX->setPosition(ccp(0.0f,0.0f));
		blurX->setAnchorPoint(ccp(0.0f,0.0f));
		blurX->setRotation(0.0f);
		blurX->visit();
		rt2->end();

		//纵向
		BlurSprite* blurY = new BlurSprite();
		blurY->setDir(ccp(0,1));
		blurY->initWithTexture(rt2->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		blurY->setFlipY(true);
		blurY->autorelease();

		
		CCRenderTexture* rt3 = CCRenderTexture::create(recSize.width,recSize.height);
		rt3->clear(0.0f,0.0f,0.0f,0.0f);
		rt3->begin();
		blurY->setPosition(ccp(0.0f,0.0f));
		blurY->setAnchorPoint(ccp(0.0f,0.0f));
		blurY->setRotation(0.0f);
		blurY->visit();
		rt3->end();

		CCSprite* spr = CCSprite::createWithTexture(rt3->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		spr->setFlipY(true);
		return spr;
	}

	CCSprite* createHighBlur(CCNode* node,CCSize recSize)
	{
		CCPoint anchor = node->getAnchorPoint();
		CCPoint pos = node->getPosition();
		float rotation = node->getRotation();
		CCRenderTexture* rt = CCRenderTexture::create(recSize.width,recSize.height);
		rt->clear(0.0f,0.0f,0.0f,0.0f);
		rt->begin();		
		node->setPosition(ccp(0.0f,0.0f));
		node->setAnchorPoint(ccp(0.0f,0.0f));
		node->setRotation(0.0f);
		node->visit();
		rt->end();

		node->setAnchorPoint(anchor);
		node->setRotation(rotation);
		node->setPosition(pos);

		//横向 
		BlurSprite* blurX = new BlurSprite();
		blurX->setDir(ccp(1,0));
		blurX->initWithTexture(rt->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		blurX->setFlipY(true);
		blurX->autorelease();

		CCRenderTexture* rt2 = CCRenderTexture::create(recSize.width,recSize.height);
		rt2->clear(0.0f,0.0f,0.0f,0.0f);
		rt2->begin();
		blurX->setPosition(ccp(0.0f,0.0f));
		blurX->setAnchorPoint(ccp(0.0f,0.0f));
		blurX->setRotation(0.0f);
		blurX->visit();
		rt2->end();

		//纵向
		BlurSprite* blurY = new BlurSprite();
		blurY->setDir(ccp(0,1));
		blurY->initWithTexture(rt2->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		blurY->setFlipY(true);
		blurY->autorelease();

		
		CCRenderTexture* rt3 = CCRenderTexture::create(recSize.width,recSize.height);
		rt3->clear(0.0f,0.0f,0.0f,0.0f);
		rt3->begin();
		blurY->setPosition(ccp(0.0f,0.0f));
		blurY->setAnchorPoint(ccp(0.0f,0.0f));
		blurY->setRotation(0.0f);
		blurY->visit();
		rt3->end();

		CCSprite* spr = CCSprite::createWithTexture(rt3->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		spr->setFlipY(true);
		return spr;
	}

	CCSprite* createBlurSprite(CCNode* node,CCSize recSize)
	{
		int type = DeviceCfgMgr::Get()->getDeviceBlurType();
		switch (type)
		{
		case DeviceBlurType_Low:
			{
				return createLowBlur(node,recSize);
			}
			break;
		case DeviceBlurType_Middle:
			{
				return createMiddleBlur(node,recSize);
			}
			break;
		case DeviceBlurType_High:
			{
				return createHighBlur(node,recSize);
			}
			break;
		default:
			break;
		}
		
		return NULL;
	}
}