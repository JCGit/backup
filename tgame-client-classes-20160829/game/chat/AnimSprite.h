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
	unsigned int m_uAnimFrames; // 总帧数

	CCSize m_stAnimFrameSize; // 帧宽高

	float m_fAnimDelay; // 播放间隔

	CCAnimate *m_pAnimate;
};

#endif // AnimSprite_h__