#pragma once
#include <string>
#include "cocos2d.h"

class SpriteShader : public cocos2d::CCSprite
{
protected:
	virtual char* getShaderfile() = 0;
	//used to get uniforms from name into handle
	virtual void initUniforms() = 0;
	//used to set uniforms value 
	virtual void setUniforms() = 0;
public:
	~SpriteShader();
	bool initWithTexture(cocos2d::CCTexture2D* texture, const cocos2d::CCRect&  rect);
	void draw();
	void initProgram();
	void listenBackToForeground(cocos2d::CCObject *obj);
};
