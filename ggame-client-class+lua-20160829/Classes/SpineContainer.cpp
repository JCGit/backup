#include "SpineContainer.h"

USING_NS_CC;
#define TIMES_SPINE_LOOP -1

SpineContainer* SpineContainer::create(const char* path, const char* name, float scale /* = 0.0f */)
{
	const char* filePath	= CCString::createWithFormat("%s/%s", path, name)->getCString();
	const char* configFile	= CCString::createWithFormat("%s.json", filePath)->getCString(),
		* atlasFile		= CCString::createWithFormat("%s.atlas", filePath)->getCString();
	SpineContainer* spineContainer = new SpineContainer(configFile, atlasFile, scale);
	if ( !spineContainer )
	{
		return NULL;
	}
	spineContainer->autorelease();
	return spineContainer;
}

void SpineContainer::runAnimation(int trackIndex,const char* name, int loopTimes,float delay)
{
	CC_RETURN_IF(loopTimes == 0 || loopTimes < TIMES_SPINE_LOOP)
	--loopTimes;

	AnimationTrackMap::iterator it = m_mapTrack.find(name);
	if ( it == m_mapTrack.end() )
	{
		m_mapTrack.insert(std::make_pair(name, SAnimationInfo(name, trackIndex, loopTimes)));
	}
	else
	{
		trackIndex = it->second.trackIndex;
		it->second.loopTimes = loopTimes;
	}

	spTrackEntry* aniEntry=NULL;
	if(delay==0.0f)
	{
		aniEntry=addAnimation(trackIndex, name, loopTimes != 0,delay);
	}
	else
	{
		aniEntry=setAnimation(trackIndex, name, loopTimes != 0);
	}

	/*
	if ( aniEntry )
	{
		if (m_pEventListener )
		{
			m_pSkeletonNode->setStartListener(this,aniEntry,SpineStartEvent_selector(SpineContainer::onReceiveStartEventListener));
			m_pSkeletonNode->setEndListener(this,aniEntry,SpineEndEvent_selector(SpineContainer::onReceiveEndEventListener));
			m_pSkeletonNode->setCompleteListener(this,aniEntry,SpineCompleteEvent_selector(SpineContainer::onReceiveCompleteEventListener));
			m_pSkeletonNode->setEventListener(this,aniEntry,SpineEvent_selector(SpineContainer::onReceiveEventListener));
		}
	}
	*/
}


void SpineContainer::setListener(SpineEventListener* eventListener)
{
	m_pEventListener = eventListener;
}

void SpineContainer::stopAllAnimations() 
{ 
	clearTracks();
}

void SpineContainer::stopAnimationByIndex(int trackIndex)
{
	clearTrack(trackIndex);
}


void SpineContainer::onAnimationStateEvent (int trackIndex, const char* animationName,spEventType type, spEvent* _event, int loopCount)
{
	CC_RETURN_IF(!m_pEventListener)

	switch ( type )
	{
	case SP_ANIMATION_COMPLETE:
		{
			SAnimationInfo* ctrlInfo = getAnimationInfo(trackIndex);
			if ( ctrlInfo->loopTimes > 0 )
			{
				runAnimation(trackIndex,ctrlInfo->aniName, ctrlInfo->loopTimes);
			}
			else if ( ctrlInfo->loopTimes == 0 )
			{
				if ( m_pEventListener )
				{
					m_pEventListener->onSpineAnimationComplete(trackIndex,animationName,loopCount);
				}
			}
		}
		break;
	case SP_ANIMATION_START:
		if ( m_pEventListener )
		{
			m_pEventListener->onSpineAnimationStart(trackIndex,animationName);
		}
		break;
	case SP_ANIMATION_END:
		if ( m_pEventListener )
		{
			m_pEventListener->onSpineAnimationEnd(trackIndex,animationName);
		}
		break;
	case SP_ANIMATION_EVENT:
		if ( m_pEventListener )
		{
			m_pEventListener->onSpineAnimationEvent(trackIndex,animationName, _event);
		}
		break;
	default:
		break;
	}
}

/*
void SpineContainer::onReceiveStartEventListener(int trackIndex,const char* animationName)
{
	if ( m_pEventListener )
	{
		m_pEventListener->onSpineAnimationStart(trackIndex,animationName);
	}
}

void SpineContainer::onReceiveEndEventListener(int trackIndex,const char* animationName)
{
	if ( m_pEventListener )
	{
		m_pEventListener->onSpineAnimationEnd(trackIndex,animationName);
	}
}

void SpineContainer::onReceiveCompleteEventListener(int trackIndex, const char* animationName,int loopCount)
{
	if ( m_pEventListener )
	{
		m_pEventListener->onSpineAnimationComplete(trackIndex,animationName,loopCount);
	}
}

void SpineContainer::onReceiveEventListener(int trackIndex,const char* animationName,spEvent* event)
{
	if ( m_pEventListener )
	{
		m_pEventListener->onSpineAnimationEvent(trackIndex,animationName, event);
	}
}
*/