#ifndef HeartBeat_h__
#define HeartBeat_h__

#include "cocos2d.h"

#include "vlib/base/CVSingleton.h"

USING_NS_CC;

class HeartBeat: public CCObject
{
public:
	HeartBeat();
	~HeartBeat();

	void startHeart();
private:
	
};

#define HEART_BEAT_INTERVAL 60.0 // seconds

#define P_HEART_BEAT CVLib::CVSingleton<HeartBeat>::Instance()

#endif // HeartBeat_h__
