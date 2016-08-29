#ifndef MaskSprite_h__
#define MaskSprite_h__

#include "cocos2d.h"

USING_NS_CC;

class MaskSprite: public CCSprite
{
public:
	MaskSprite();
	virtual ~MaskSprite();

	static MaskSprite* create(const char *pszTexFileName, const char *pszMaskFileName);

	static MaskSprite* createWithTexture(CCTexture2D *pTexture, CCTexture2D *pMask);

	bool initWithFile(const char *pszTexFileName, const char *pszMaskFileName);

	bool initWithTexture(CCTexture2D *pTexture, CCTexture2D *pMask);

	virtual void draw();

	void loadTexture(const char *pszFileName);

	void loadTexture(CCTexture2D *pTexture);

private:
	GLuint _textureLocaltion;
	GLuint _maskLocation;
	CCTexture2D *m_pMaskTexture;
	bool m_bInitialized;
};

#endif // MaskSprite_h__