//
//  StrokeSample.h
//  StrokeSprite
//
//  Created by long on 13-11-30.
//
//

#ifndef __StrokeSprite__StrokeSample__
#define __StrokeSprite__StrokeSample__

#include <string>
#include "cocos2d.h"

USING_NS_CC;

class StrokeSample : public CCSprite
{
protected:
    virtual char* getShaderFile();
    virtual void initUniforms();
    virtual void setUniforms();
    
public:
    ~StrokeSample();
    bool initWithTexture(CCTexture2D* texture, const CCRect& rect);
    void draw();
    void initProgram();
    
private:
    GLuint      strokeColor;
    GLuint      sampler;
//    CCTexture2D*        m_pColorTexture;
};

#endif /* defined(__StrokeSprite__StrokeSample__) */
