
#pragma once

#include <string>
#include "cocos2d.h"

USING_NS_CC;

#define COVER_SPRITE_TAG 12345

class CoverSprite : public CCSprite
{
protected:
    virtual char* getShaderFile();
    virtual void initUniforms();
    virtual void setUniforms();
    
public:
	CoverSprite();
    ~CoverSprite();
    bool initWithTexture(CCTexture2D* texture,CCTexture2D* targetTex, const CCRect& rect);
    void draw();
    void initProgram();    
private:
	 GLuint		cover;
	 CCTexture2D*	m_pTargetTexture;
};
