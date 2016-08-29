
#include "CoverSprite.h"

CoverSprite::CoverSprite()
{
	m_pTargetTexture = NULL;
}

CoverSprite::~CoverSprite()
{
	if (m_pTargetTexture)
	{
		m_pTargetTexture->release();
	}
}

bool CoverSprite::initWithTexture(cocos2d::CCTexture2D *texture, CCTexture2D* targetTex, const cocos2d::CCRect &rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
	{
		this->initProgram();		
		m_pTargetTexture = targetTex;
		m_pTargetTexture->retain();
		return true;
	}
    
	return false;
}

void CoverSprite::initProgram()
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

void CoverSprite::draw()
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

void CoverSprite::initUniforms()
{
	cover = this->getShaderProgram()->getUniformLocationForName("GL_Texture1");
}

void CoverSprite::setUniforms()
{
	ccGLBindTexture2DN(1,m_pTargetTexture->getName());
    this->getShaderProgram()->setUniformLocationWith1i(cover, 1);	
}

char* CoverSprite::getShaderFile()
{
    static char retstr[32] = "shader/coversprite.sha";
	return retstr;
}
