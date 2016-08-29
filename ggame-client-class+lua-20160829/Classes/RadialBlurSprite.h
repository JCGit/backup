//
//  BlurSprite.h
//  
//
//  Created by Hui Zhen on 13-12-4.
//
//



#include <string>
#include "cocos2d.h"

USING_NS_CC;
class RadialBlurSprite : public CCSprite
{
protected:
    virtual char* getShaderFile();
    virtual void initUniforms();
    virtual void setUniforms();
    
public:
    RadialBlurSprite();
    ~RadialBlurSprite();
    CREATE_FUNC(RadialBlurSprite);
    bool initWithTexture(CCTexture2D* texture, const CCRect& rect);
	bool initWithFile(const char *pszFilename);
    void draw();
    void initProgram();
    
	void setTouchTextPos(float x,float y);

private:
    GLuint    selectPos;
	float		m_fSelectX;//texture coord based, aka 0-1.0
	float		m_fSelectY;
};
namespace RadialBlurEffect
{
    CCSprite* generateRadialSpriteByTex(CCTexture2D* tex);
	RadialBlurSprite* generateRadialEffectByNode(CCNode* BlurNode,CCNode* centerNode);
}