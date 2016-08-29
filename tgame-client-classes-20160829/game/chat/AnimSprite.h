#ifndef AnimSprite_h__
#define AnimSprite_h__

#include "cocos2d.h"

USING_NS_CC;

class AnimSprite: public CCSprite
{
public:
	AnimSprite();
	virtual ~AnimSprite();
	
	static AnimSprite* create(const char *pszFileName, const CCSize &size, unsigned int uFrames, float delay = 0.1f);

	// override start

	virtual bool init(const char *pszFileName, const CCSize &size, unsigned int uFrames, float delay);

	virtual void onEnterTransitionDidFinish();

	virtual void onExit();

	// override end

	void play();

	void stop();

	const CCSize getAniSize(){
		return m_stAnimFrameSize;
	}
private:
	unsigned int m_uAnimFrames; // ��֡��

	CCSize m_stAnimFrameSize; // ֡���

	float m_fAnimDelay; // ���ż��

	CCAnimate *m_pAnimate;
};

#endif // AnimSprite_h__