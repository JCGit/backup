
#pragma once

#include <string>
#include "cocos2d.h"

USING_NS_CC;

class GraySprite : public CCSprite
{
protected:
    virtual char* getShaderFile();
    virtual void initUniforms();
    virtual void setUniforms();
    
public:
    ~GraySprite();
    bool initWithTexture(CCTexture2D* texture, const CCRect& rect);
    void draw();
    void initProgram();    
private:
    GLuint      grayColor;
};


namespace GraySpriteMgr
{
	GraySprite* createGrayMask(CCNode* parent,CCSize recSize);
}