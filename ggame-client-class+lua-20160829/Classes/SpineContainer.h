#pragma once



#include "cocos2d.h"
#include "cocos-ext.h"
#include <spine/spine-cocos2dx.h>
#include <string>
#include <map>
using namespace spine;
USING_NS_CC;
USING_NS_CC_EXT;

class SpineEventListener
{
public:
	virtual void onSpineAnimationStart(int trackIndex,const char* animationName){};
	virtual void onSpineAnimationEnd(int trackIndex,const char* animationName){};
	virtual void onSpineAnimationComplete(int trackIndex,const char* animationName,int loopCount){};
	virtual void onSpineAnimationEvent(int trackIndex,const char* animationName,spEvent* event) {};
};

class SpineContainer: public SkeletonAnimation
{
public:
	SpineContainer(const char* skeletonDataFile, const char* atlasFile, float scale = 1.0f)
		: SkeletonAnimation(skeletonDataFile, atlasFile, scale)
		,m_pEventListener(NULL)
	{
		m_mapTrack.clear();
	};
	~SpineContainer()
	{
		m_mapTrack.clear();
	};

	static SpineContainer* create(const char* path, const char* name, float scale = 1.0f);
	void runAnimation(int trackIndex,const char* name, int loopTimes=1,float delay=0);

	void setListener(SpineEventListener* eventListener);

	void stopAllAnimations();

	void stopAnimationByIndex(int trackIndex);

private:
	struct SAnimationInfo
	{
		const char*			aniName;
		unsigned int	trackIndex;
		int				loopTimes;

		SAnimationInfo(const char* name, unsigned int index, int times)
			: aniName(name)
			, trackIndex(index)
			, loopTimes(times)
		{};
	};

	/*
	void onReceiveStartEventListener(int trackIndex,const char* animationName);
	void onReceiveEndEventListener(int trackIndex,const char* animationName);
	void onReceiveCompleteEventListener(int trackIndex,const char* animationName, int loopCount);
	void onReceiveEventListener(int trackIndex,const char* animationName,spEvent* event);
	*/

	typedef std::map<const char*, SAnimationInfo> AnimationTrackMap;
	void onAnimationStateEvent (int trackIndex, const char* animationName,spEventType type, spEvent* event, int loopCount);
	SAnimationInfo* getAnimationInfo(unsigned int trackIndex)
	{
		AnimationTrackMap::iterator it = m_mapTrack.begin(), itEnd = m_mapTrack.end();
		while ( it != itEnd )
		{
			if ( it->second.trackIndex == trackIndex )
			{
				return &(it->second);
			}
			++it;
		}
		return NULL;
	};
	SpineEventListener* m_pEventListener;

	AnimationTrackMap	m_mapTrack; 
};

