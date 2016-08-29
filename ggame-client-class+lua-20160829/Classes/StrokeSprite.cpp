
#include "StrokeSprite.h"
#include "ResManager.h"

ColorSprite::ColorSprite(ccColor3B color, GLubyte opacity)
{
    m_color = color;        //默认是紫色描边
    m_opacity = opacity;
}

ColorSprite::~ColorSprite()
{
    
}

bool ColorSprite::initWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCRect &rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
	{
		this->initProgram();
		return true;
	}
    
	return false;
}

void ColorSprite::initProgram()
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

void ColorSprite::draw()
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
	ccGLBlendFunc(GL_ONE, GL_ONE);
    
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

void ColorSprite::initUniforms()
{
    strokeColor = getShaderProgram()->getUniformLocationForName("strokeColor");
}

void ColorSprite::setUniforms()
{
    getShaderProgram()->setUniformLocationWith4f(strokeColor,1.0f,0.0f,1.0f,1.0f);
}

char* ColorSprite::getShaderFile()
{
    static char retstr[32] = "shader/strokesprite.sha";
	return retstr;
}

StrokeChartlet::StrokeChartlet(ccColor3B color)
{
    m_color = color;
}

StrokeChartlet::~StrokeChartlet()
{
    
}

bool StrokeChartlet::initWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCRect &rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
	{
		this->initProgram();
		return true;
	}
    
	return false;
}

void StrokeChartlet::initProgram()
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

void StrokeChartlet::draw()
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

void StrokeChartlet::initUniforms()
{
    strokeColor = getShaderProgram()->getUniformLocationForName("strokeColor");
}

void StrokeChartlet::setUniforms()
{
    getShaderProgram()->setUniformLocationWith4f(strokeColor,m_color.r/255.0f,m_color.g/255.0f,m_color.b/255.0f,1.0f);
}

char* StrokeChartlet::getShaderFile()
{
    static char retstr[32] = "shader/strokechartlet.sha";
	return retstr;
}



namespace StrokeSprite
{
    CCSprite* createStroke(CCSprite* sprite, int quality)
    {
        int size = STROKE_SIZE;
        GLubyte opacity = 100;
        ccColor3B color;
        switch (quality+DISCIPLE_TYPE*1000)
        {
			case S_DISCIPLE:
			{
				color = S_STROKE_COLOR;
			}
				break;
            case A_DISCIPLE:
            {
                color = A_STROKE_COLOR;
            }
                break;
            case B_DISCIPLE:
            {
                color = B_STROKE_COLOR;
            }
                break;
            case C_DISCIPLE:
            {
                color = C_STROKE_COLOR;
            }
                break;
            case D_DISCIPLE:
            {
                color = D_STROKE_COLOR;
            }
                break;
            default:
            {
                color = A_STROKE_COLOR;
            }
                break;
        }
		CCSprite* tmpSprite = CCSprite::createWithTexture(sprite->getTexture());
		CCSize spriteSize = tmpSprite->getTexture()->getContentSize();
		CCRenderTexture* renderT = CCRenderTexture::create(spriteSize.width,spriteSize.height);
		renderT->clear(0.0f,0.0f,0.0f,0.0f);
		renderT->begin();
		tmpSprite->setPosition(ccp(spriteSize.width*sprite->getAnchorPoint().x,spriteSize.height*sprite->getAnchorPoint().y));
		tmpSprite->visit();
		renderT->end();

		CCSprite* renderS = CCSprite::createWithTexture(renderT->getSprite()->getTexture());
		CCRenderTexture* renderTT = CCRenderTexture::create(spriteSize.width,spriteSize.height);
		renderTT->clear(0.0f,0.0f,0.0f,0.0f);
		renderTT->begin();
		renderS->setPosition(ccp(spriteSize.width*renderS->getAnchorPoint().x,spriteSize.height*renderS->getAnchorPoint().y));
		renderS->visit();
		renderTT->end();

        ColorSprite* label = new ColorSprite(color,opacity);
        label->initWithTexture(renderTT->getSprite()->getTexture(),CCRect(0.0,0.0,sprite->getTexture()->getContentSize().width,sprite->getTexture()->getContentSize().height));
        label->setAnchorPoint(ccp(0.5,0.5));
        ccBlendFunc bf = {GL_ONE, GL_ONE};
        label->setBlendFunc(bf);
        clock_t start,ends;
        start = clock();
        CCLog("start time:%d",(int)start);
        //生成描边纹理
        CCRenderTexture* rt = CCRenderTexture::create(label->getTexture()->getContentSize().width + size * 2,label->getTexture()->getContentSize().height+size * 2);
        CCPoint bottomLeft = ccp(label->getTexture()->getContentSize().width * label->getAnchorPoint().x + size,label->getTexture()->getContentSize().height * label->getAnchorPoint().y + size);
        
        clock_t stroke_s,stroke_e;
        stroke_s = clock();
        rt->clear(0,0,0,0);
        rt->begin();
        for (int i = 0; i < 360; i += 15) // you should optimize that for your needs
        {
            label->setPosition(ccp(bottomLeft.x + sin(CC_DEGREES_TO_RADIANS(i))*size, bottomLeft.y +cos(CC_DEGREES_TO_RADIANS(i))*size));
            label->visit();
        }
        rt->end();
        
        stroke_e = clock();
        CCLog("stroke use time:%d",(int)(stroke_e-stroke_s));
        
        label->release();
        
        CCSprite* tmp = CCSprite::create();
        tmp->initWithTexture(rt->getSprite()->getTexture());
        
        CCSize ss = rt->getSprite()->getTexture()->getContentSize();
        rt->setAnchorPoint(ccp(0.5f,0.5f));
        CCRenderTexture* strokeRT = CCRenderTexture::create(ss.width,ss.height);
        strokeRT->clear(0.0f, 0.0f, 0.0f, 0.0f);
        strokeRT->begin();
        tmp->setPosition(ccp(ss.width*rt->getAnchorPoint().x,ss.height*rt->getAnchorPoint().y));
        tmp->visit();
        strokeRT->end();
        
        //使用StrokeChartlet，渲染描边纹理，需要特殊处理，详情见strokechartlet.sha
        StrokeChartlet* chartlet = new StrokeChartlet(color);
        chartlet->autorelease();
        chartlet->initWithTexture(strokeRT->getSprite()->getTexture(), CCRect(0.0f,0.0f,ss.width,ss.height));
        
        ends = clock();
        CCLog("total use time:%d",(int)(ends-start));
        chartlet->getTexture()->setAntiAliasTexParameters();

        return chartlet;
    }
    
    
    CCSprite* createInnerStroke(CCSprite* sprite, int quality)
    {
        float scale = sprite->getScale();
        //获取框内纹理
        CCSprite* spriteInner = CCSprite::createWithTexture(sprite->getTexture());
        CCSize spriteSize = spriteInner->getTexture()->getContentSize();
        CCRenderTexture* renderInnerTex = CCRenderTexture::create(INNER_FRAME_SIZE/scale,INNER_FRAME_SIZE/scale);
        renderInnerTex->clear(0.0f, 0.0f, 0.0f, 0.0f);
        renderInnerTex->begin();
        spriteInner->setPosition(ccp(spriteSize.width*spriteInner->getAnchorPoint().x-(spriteSize.width-INNER_FRAME_SIZE/scale)/2.0,spriteSize.height*spriteInner->getAnchorPoint().y-(spriteSize.height-INNER_FRAME_SIZE/scale)/2.0));
        spriteInner->visit();
        renderInnerTex->end();
        renderInnerTex->setScale(scale);
        
        //纹理翻转
        CCSprite* renderS = CCSprite::createWithTexture(renderInnerTex->getSprite()->getTexture());
        CCRenderTexture* renderT = CCRenderTexture::create(INNER_FRAME_SIZE/scale, INNER_FRAME_SIZE/scale);
        renderT->clear(0.0f, 0.0f, 0.0f, 0.0f);
        renderT->begin();
        renderS->setPosition(ccp(renderS->getContentSize().width*renderS->getAnchorPoint().x,renderS->getContentSize().height*renderS->getAnchorPoint().y));
        renderS->visit();
        renderT->end();
        
        CCSprite* renderSS = CCSprite::createWithTexture(renderT->getSprite()->getTexture());
		
        //描边
        CCSprite* res = createStroke(renderSS, quality);
        res->setScale(scale);
        return res;
    }
}






















