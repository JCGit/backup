#pragma once

#include "CocoStudio/Armature/CCArmature.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ArmatureEventListener
{
public:
	virtual void onArmatureAnimationDone(const char* animationName, bool isLoop){};
	virtual void onFrameEvent(const char* eventName) {};
};

class ArmatureContainer
	: public CCArmature
{
public:
	ArmatureContainer();
	~ArmatureContainer();
	
	static ArmatureContainer* create(const char* path, const char* name, CCNode* parent = NULL);

	void runAnimation(const char* name, unsigned int loopTimes = 1);
	void changeSkin(const char* boneName, const char* skinName, bool force = true);
	void changeSkin(const char* boneName, CCLabelTTF* label, bool force = true);
	float getAnimationTime(const char* name);
	void addBonePlay(const char* boneName);
	void addBone(const char* boneName,const char* animationName);
	void setListener(ArmatureEventListener* eventListener);
	void setResourcePath(const char* resourcePath);
	std::string getResourcePath() { return m_pResourcePath; };
	static void clearResource(std::string resourcePath);

private:
	void onReceiveMovementEvent(CCArmature* armature, MovementEventType eventType, const char* animationName);
	void onReceiveFrameEvent(CCBone* bone, const char* eventName, int originalIndex, int currentIndex);

	ArmatureEventListener* m_pEventListener;
	std::string m_pResourcePath;
	unsigned int m_iRemainLoopTimes;
};

