//
//  BlurSprite.cpp
//  
//
//  Created by Hui Zhen on 13-12-4.
//
//

#include "RadialBlurSprite.h"
RadialBlurSprite::RadialBlurSprite()
{
	m_fSelectX = 0.5f;
	m_fSelectY = 0.5f;
}

RadialBlurSprite::~RadialBlurSprite()
{
    
}

bool RadialBlurSprite::initWithFile(const char *pszFilename){
	if( CCSprite::initWithFile(pszFilename) )
	{
		this->initProgram();
		return true;
	}

	return false;

}

bool RadialBlurSprite::initWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCRect &rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
	{
		this->initProgram();
		return true;
	}
    
	return false;
}

void RadialBlurSprite::initProgram()
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

void RadialBlurSprite::draw()
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
	
    ccBlendFunc blend = getBlendFunc();
    ccGLBlendFunc(blend.src, blend.dst);
    
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

void RadialBlurSprite::initUniforms()
{
    selectPos = glGetUniformLocation( getShaderProgram()->getProgram(), "selectPos");
}

void RadialBlurSprite::setTouchTextPos(float x,float y){
	m_fSelectX = x;
	m_fSelectY =y;
}

void RadialBlurSprite::setUniforms()
{
    getShaderProgram()->setUniformLocationWith2f(selectPos, m_fSelectX,m_fSelectY);
}

char* RadialBlurSprite::getShaderFile()
{
    static char retstr[32] = "shader/radialBlur.sha";
	return retstr;
}

namespace RadialBlurEffect
{

    CCSprite* generateRadialSpriteByTex(CCTexture2D* tex){
        RadialBlurSprite* blurSprite = new RadialBlurSprite();
        blurSprite->initWithTexture(tex,CCRect(0.0,0.0,tex->getContentSize().width,tex->getContentSize().height));
        
        
        CCRenderTexture* rt = CCRenderTexture::create(tex->getContentSize().width,tex->getContentSize().height);
        
        clock_t blur_s,blur_e;
        blur_s = clock();
        rt->clear(0,0,0,0);
        rt->begin();
        blurSprite->setPosition(ccp(0,0));
        blurSprite->setAnchorPoint(ccp(0,0));
        blurSprite->visit();
        
        rt->end();
        
        blur_e = clock();
        CCLog("generate texture use time:%d",(int)(blur_e-blur_s));
        
        blurSprite->release();
        
        CCSprite* blurResult = CCSprite::create();
        CCTexture2D * rtTexture = rt->getSprite()->getTexture();
        blurResult->initWithTexture(rtTexture);
        //since the render texture 
        blurResult->setFlipY(true);
        
        return blurResult;
        
    }


	RadialBlurSprite* generateRadialEffectByNode(CCNode* BlurNode,CCNode* centerNode){
		
		CCPoint worldPos = centerNode->getParent()->convertToWorldSpaceAR(centerNode->getPosition());
		float posX = worldPos.x / CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width;
		float posY = worldPos.y / CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height;


		CCRenderTexture* rt = CCRenderTexture::create(BlurNode->getContentSize().width,BlurNode->getContentSize().height);
		rt->clear(0,0,0,0);
		rt->begin();
		BlurNode->visit();

		rt->end();


		CCTexture2D * rtTexture = rt->getSprite()->getTexture();

		RadialBlurSprite* blurSprite = RadialBlurSprite::create();
		blurSprite->initWithTexture(rtTexture,CCRect(0.0,0.0,rtTexture->getContentSize().width,rtTexture->getContentSize().height));
		blurSprite->setTouchTextPos(posX,posY);
		//since the render texture 
		blurSprite->setFlipY(true);

		return blurSprite;

	}
}