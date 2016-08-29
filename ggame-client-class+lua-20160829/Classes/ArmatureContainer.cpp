#include "ArmatureContainer.h"
#include "CocoStudio/Armature/utils/CCArmatureDataManager.h"
#include "CocoStudio/Armature/display/CCSkin.h"

USING_NS_CC;
USING_NS_CC_EXT;

ArmatureContainer::ArmatureContainer()
	: CCArmature()
	, m_pEventListener(NULL)
	, m_pResourcePath("")
	, m_iRemainLoopTimes(0)
{
}


ArmatureContainer::~ArmatureContainer()
{
}

ArmatureContainer* ArmatureContainer::create(const char* path, const char* name, CCNode* parent /* = NULL */)
{
	CC_RETURN_VAL_IF(!path || !name, NULL)
	ArmatureContainer* armatureContainer = new ArmatureContainer();
	if ( armatureContainer )
	{
		const char* filePath	= CCString::createWithFormat("%s/%s", path, name)->getCString();
		const char* imgFile		= CCString::createWithFormat("%s.png", filePath)->getCString(),
				* plistFile		= CCString::createWithFormat("%s.plist", filePath)->getCString(),
				* configFile	= CCString::createWithFormat("%s.xml", filePath)->getCString();
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(imgFile, plistFile, configFile);
		if ( armatureContainer->init(name) )
		{
			armatureContainer->setResourcePath(filePath);
			armatureContainer->autorelease();
			if ( parent )
			{
				parent->addChild(armatureContainer);
			}

			armatureContainer->addBonePlay(name);
			return armatureContainer;
		}
		delete armatureContainer;
	}
	return NULL;
}
void ArmatureContainer::addBonePlay(const char* boneName)
{
	CCBone *  bone = NULL;
	int cout = 1;
	std::string boneStr(boneName);
	boneStr= boneStr.substr(4,strlen(boneName));
	do 
	{
		char str[256] ={0,};
		sprintf(str,"%s_%d",boneStr.c_str(),cout);
		bone = this->getBone(str);
		if (bone == NULL)
		{
			break;
		}
		CCArmature * armature= bone->getChildArmature();
		CCArmatureAnimation  * animation = NULL;
		if(armature) animation = armature->getAnimation();
		if(animation) animation->play("play");
		bone = NULL;
		++cout;
		
	} while (1);

}
void ArmatureContainer::addBone(const char* boneName,const char* animationName)
{
	CCBone *  bone = this->getBone(boneName);
	if (bone != NULL)
	{
		CCArmature * armature= bone->getChildArmature();
		CCArmatureAnimation  * animation = NULL;
		if(armature) animation = armature->getAnimation();
		if(animation) animation->play(animationName);
	}
}
float ArmatureContainer::getAnimationTime(const char* name)
{
	auto  animation = this->getAnimation();
	CCAnimationData * aa = animation->getAnimationData();
	CCMovementData* mov = aa->getMovement(name);
	if (mov)
	{
		return mov->duration * CCDirector::sharedDirector()->getAnimationInterval();
	}
	return 0.5;
}
void ArmatureContainer::runAnimation(const char* name, unsigned int loopTimes)
{
	CCArmatureAnimation * ani = getAnimation();
	if ( ani )
	{
		m_iRemainLoopTimes = 0;
		if ( loopTimes > 1 )
		{
			m_iRemainLoopTimes = loopTimes - 1;
			if ( !m_pEventListener )
			{
				ani->setMovementEventCallFunc(this, movementEvent_selector(ArmatureContainer::onReceiveMovementEvent));
			}
		}
		ani->play(name);
	}
}

void ArmatureContainer::changeSkin(const char* boneName, const char* skinName, bool force /* = true */)
{
	CC_RETURN_IF(!boneName || !skinName)
	CCBone * bone = getBone(boneName);
	
	if ( bone )
	{
		if ( !bone->getDisplayByName(skinName) )
		{
			CCSkin* skin = CCSkin::createWithSpriteFrameName(skinName);
			CC_RETURN_IF(!skin)
			bone->addDisplay((CCNode*)skin, -1);
		}
		bone->changeDisplayWithName(skinName, force);
	}
}

void ArmatureContainer::changeSkin(const char* boneName, CCLabelTTF* label, bool force /* = true */)
{
	CC_RETURN_IF(!boneName || !label)
	CCBone * bone = getBone(boneName);
	if ( bone )
	{
		int index = bone->addDisplay(label, -1);
		bone->changeDisplayWithIndex(index, force);
	}
}

void ArmatureContainer::setListener(ArmatureEventListener* eventListener)
{
	m_pEventListener = eventListener;
	
	CCArmatureAnimation* animation = getAnimation();
	if ( animation )
	{
		if ( m_pEventListener )
		{
			animation->setMovementEventCallFunc(this, movementEvent_selector(ArmatureContainer::onReceiveMovementEvent));
			animation->setFrameEventCallFunc(this, frameEvent_selector(ArmatureContainer::onReceiveFrameEvent));
		}
		else
		{
			animation->setMovementEventCallFunc(this, NULL);
			animation->setFrameEventCallFunc(this, NULL);
		}
	}
}

void ArmatureContainer::setResourcePath(const char* resourcePath)
{
	m_pResourcePath = resourcePath;
}

void ArmatureContainer::clearResource(std::string resourcePath)
{
	if ( !resourcePath.empty() )
	{
		const char* configFile	= CCString::createWithFormat("%s.xml", resourcePath.c_str())->getCString();
		CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(configFile);
	}
}

void ArmatureContainer::onReceiveMovementEvent(CCArmature* armature, MovementEventType eventType, const char* animationName)
{
	CC_UNUSED_PARAM(armature);
	if ( eventType == COMPLETE || eventType == LOOP_COMPLETE )
	{
		if ( m_iRemainLoopTimes )
		{
			CCLog("animation[%s] remain %d times", animationName, m_iRemainLoopTimes);
			runAnimation(animationName, m_iRemainLoopTimes);
			return;
		}
		bool isLoop = eventType == LOOP_COMPLETE;
		CCLog("%s animation[name: %s] done", (isLoop ? "loop" : ""), animationName);
		if ( m_pEventListener )
		{
			m_pEventListener->onArmatureAnimationDone(animationName, isLoop);
		}
	}
}

void ArmatureContainer::onReceiveFrameEvent(CCBone* bone, const char* eventName, int originalIndex, int currentIndex)
{
	if ( m_pEventListener )
	{
		m_pEventListener->onFrameEvent(eventName);
	}
}