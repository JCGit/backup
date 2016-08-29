
#ifndef _BASE_PAGE_
#define _BASE_PAGE_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "AppDelegate.h"
#include "CCBContainer.h"

class BasePage : public CCBContainer
{
public:
	BasePage()
	: CCBContainer()
	{
		mIsFullScreen=false;
	}
	static int calcAdjustResolutionOffY();
	virtual void load(void) = 0;
	virtual void preload(void){}
	virtual bool getPreloadDone(void){return true;}
	virtual void unload(void){CCBContainer::unload();}
	virtual void autoAdjustResolution();
	virtual void autoAdjustResizeScrollview(cocos2d::extension::CCScrollView *pView);
	virtual void autoAdjustResizeScale9Sprite(cocos2d::extension::CCScale9Sprite * pscale9Sprite);


	bool init(){mIsFullScreen = false; return true;}

	std::string		mPageName;
	bool			mIsFullScreen;

public:
	static void recycleNode(CCNode* pNode, int sizeLimit = 0, bool purgeCached = false)
	{
		std::map<cocos2d::CCTexture2D*, cocos2d::CCTexture2D*> mTex;
		getNodeSpriteTex(pNode, mTex);

		if (mTex.size())
		{
			std::map<cocos2d::CCTexture2D*, cocos2d::CCTexture2D*>::iterator it = mTex.begin();
			for (;it != mTex.end();it++)
			{
				cocos2d::CCSize sz = it->second->getContentSize();
				if (!sizeLimit || (sz.width >= sizeLimit && sz.height >= sizeLimit))
				{
					//remove可能引起资源泄漏，从管理器除名的资源，可能引用计数得不到正确释放，重用时管理器又重新load，就发生泄漏了
					//cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromTexture(it->second);
					//cocos2d::CCTextureCache::sharedTextureCache()->removeTexture(it->second);
				}
			}
		}

		if (purgeCached)
			g_AppDelegate->notifyPurgeCached();
	}

protected:
	static void getNodeSpriteTex(CCNode* pNode, std::map<cocos2d::CCTexture2D*, cocos2d::CCTexture2D*>& mTex)
	{		
		cocos2d::CCArray* pChildren = pNode->getChildren();
		for (unsigned int i=0;pChildren && i<pChildren->count();i++)
		{
			cocos2d::CCNode* pChild = dynamic_cast<cocos2d::CCNode*>(pChildren->objectAtIndex(i));
			if (pChild)
			{
				cocos2d::CCSprite* pSprite = dynamic_cast<cocos2d::CCSprite*>(pChild);
				if (pSprite && pSprite->getTexture())
					mTex[pSprite->getTexture()] = pSprite->getTexture();

				getNodeSpriteTex(pChild, mTex);
			}			
		}		
	}

protected:
	class inner_CCMenu : public cocos2d::CCMenu
	{
	public:
		void resetTouchState()
		{
			if (m_eState == cocos2d::kCCMenuStateTrackingTouch)
			{
				if (m_pSelectedItem)
				{
					if (m_pSelectedItem->isEnabled())
						m_pSelectedItem->unselected();

					m_pSelectedItem = NULL;
				}
				m_eState = cocos2d::kCCMenuStateWaiting;
			}
		}
	};

	class inner_CCScrollView : public cocos2d::extension::CCScrollView
	{
	public:
		void resetTouchState()
		{
			m_bDragging   = false;
			m_bTouchMoved = false;
			m_pTouches->removeAllObjects();
		}
	};

public:
	void resetTouchState(cocos2d::CCNode* pNode)
	{
		if (pNode)
		{
			cocos2d::CCMenu* pMenu = dynamic_cast<cocos2d::CCMenu*>(pNode);
			if(pMenu)					
				((inner_CCMenu*)pMenu)->resetTouchState();

			cocos2d::extension::CCScrollView* pScroll = dynamic_cast<cocos2d::extension::CCScrollView*>(pNode);
			if(pScroll)					
				((inner_CCScrollView*)pScroll)->resetTouchState();

			cocos2d::CCArray* pChildren = pNode->getChildren();
			for (unsigned int i=0; pChildren && i<pChildren->count(); i++)
			{
				cocos2d::CCNode* pChild = dynamic_cast<cocos2d::CCNode*>(pChildren->objectAtIndex(i));
				resetTouchState(pChild);
			}
		}
	}

	void onBringToBackend()
	{
		cocos2d::CCArray* pChildren = getChildren();
		for (unsigned int i=0; pChildren && i<pChildren->count(); i++)
		{
			cocos2d::CCNode* pNode = dynamic_cast<cocos2d::CCNode*>(pChildren->objectAtIndex(i));
			resetTouchState(pNode);
		}
	}

	void onBringToFronted()
	{

	}
};

#endif//_BASE_PAGE_