#ifndef LoadingSprite_h__
#define LoadingSprite_h__

#include "cocos2d.h"

class LoadingSprite: public cocos2d::CCSprite
{
public:
	LoadingSprite();
	virtual ~LoadingSprite();

	static LoadingSprite* create();

	virtual void onEnter();

	virtual void onExit();
	
	virtual void update(float delta);
	
private:

};

#endif // LoadingSprite_h__
