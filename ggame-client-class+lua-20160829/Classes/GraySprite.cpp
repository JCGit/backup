
#include "GraySprite.h"

GraySprite::~GraySprite()
{

}

bool GraySprite::initWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCRect &rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
	{
		this->initProgram();
		return true;
	}
    
	return false;
}

void GraySprite::initProgram()
{
	GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(getShaderFile()).c_str())->getCString();
	CCGLProgram* pProgram = new CCGLProgram();
	pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
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

void GraySprite::draw()
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

void GraySprite::initUniforms()
{
    grayColor = getShaderProgram()->getUniformLocationForName("grayColor");
}

void GraySprite::setUniforms()
{
    this->getShaderProgram()->setUniformLocationWith1f(grayColor,0.4f);
}

char* GraySprite::getShaderFile()
{
    static char retstr[32] = "shader/graysprite.sha";
	return retstr;
}


namespace GraySpriteMgr
{
	GraySprite* createGrayMask(CCNode* node,CCSize recSize)
	{
		CCRenderTexture* renderTex = CCRenderTexture::create(recSize.width,recSize.height);
		renderTex->clear(0.0f,0.0f,0.0f,0.0f);
		renderTex->begin();
		node->visit();
		renderTex->end();

		GraySprite* graySprite = new GraySprite();
		
		graySprite->initWithTexture(renderTex->getSprite()->getTexture(),CCRectMake(0.0f,0.0f,recSize.width,recSize.height));
		graySprite->setAnchorPoint(ccp(0.5,0.5));
		graySprite->setPosition(ccp(recSize.width*0.5,recSize.height*0.5));
		graySprite->setFlipY(true);
		graySprite->autorelease();

		return graySprite;
	}
}