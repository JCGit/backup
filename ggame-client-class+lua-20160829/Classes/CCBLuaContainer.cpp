
#include "stdafx.h"

#include "CCBLuaContainer.h"
#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "Gamelua.h"

#define LUA_LINK_THIS mLualink=this;

CCBLuaContainer* CCBLuaContainer::create( const std::string& ccbfile , int tag)
{ 
	CCBLuaContainer *pRet = new CCBLuaContainer(); 
	pRet->setTag(tag);
	pRet->mCcbfilename = ccbfile;
	if (pRet && pRet->init()) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

bool CCBLuaContainer::init()
{
	if(mCcbfilename=="")return false;
	loadCcbiFile(mCcbfilename);
	std::string luaName = mCcbfilename;
	size_t lastdot = mCcbfilename.find_last_of('.');
	if(lastdot!=std::string::npos)
		luaName = mCcbfilename.substr(0,lastdot);
	std::string luafile = luaName.append(".lua");

	// register lua engine
	cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
	std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(luafile.c_str());
	LUA_LINK_THIS;
	pEngine->executeScriptFile(path.c_str());
	//
	return true;
}

CCBLuaContainer* CCBLuaContainer::lualink()
{
	return mLualink;
}


void CCBLuaContainer::registerScriptHandler( FuncType type, int nHandler )
{
	mScriptHandlers[type]=nHandler;
}

void CCBLuaContainer::unregisterScriptTapHandler( FuncType type )
{
	if(mScriptHandlers.find(type)!=mScriptHandlers.end())
		mScriptHandlers.erase(type);
}

void CCBLuaContainer::registerFunctionHandler( char* func, int nHandler )
{
	std::map<std::string, int>::iterator it = mFuncHandlers.find(func);
	if(it==mFuncHandlers.end())
		mFuncHandlers.insert(std::make_pair(func,nHandler));
	else
	{
		CCLog("replace %s handler from %d to %d",func,it->second,nHandler);
		it->second = nHandler;
	}
}

void CCBLuaContainer::unregisterFunctionHandler( char* func )
{
	std::map<std::string, int>::iterator it = mFuncHandlers.find(func);
	if(it!=mFuncHandlers.end())
		mFuncHandlers.erase(it);
}

void CCBLuaContainer::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{
	std::map<std::string,int>::iterator it = mFuncHandlers.find(itemName);
	if(it!=mFuncHandlers.end())
	{
		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		LUA_LINK_THIS;
		pEngine->executeEvent(it->second,it->first.c_str(),this,"CCBLuaContainer");
	}
}

void CCBLuaContainer::run()
{
	if(mCcbfilename=="")return;
	std::string luaName = mCcbfilename;
	size_t lastdot = mCcbfilename.find_last_of('.');
	if(lastdot!=std::string::npos)
		luaName = mCcbfilename.substr(0,lastdot);
	std::string luafile = luaName.append(".lua");

	// register lua engine
	cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
	std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(luafile.c_str());
	LUA_LINK_THIS;
	pEngine->executeScriptFile(path.c_str());
}

void CCBLuaContainer::Enter( MainFrame* )
{
	std::map<int, int>::iterator it = mScriptHandlers.find(ENTER);
	if(it!=mScriptHandlers.end())
	{
		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		LUA_LINK_THIS;
		pEngine->executeEvent(it->second,"enter");
	}
}

void CCBLuaContainer::Execute( MainFrame* )
{
	std::map<int, int>::iterator it = mScriptHandlers.find(EXECUTE);
	if(it!=mScriptHandlers.end())
	{
		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		LUA_LINK_THIS;
		pEngine->executeEvent(it->second,"execute");
	}
}

void CCBLuaContainer::Exit( MainFrame* )
{
	std::map<int, int>::iterator it = mScriptHandlers.find(EXIT);
	if(it!=mScriptHandlers.end())
	{
		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		LUA_LINK_THIS;
		pEngine->executeEvent(it->second,"exit");
	}
}

void CCBLuaContainer::load( void )
{
	std::map<int, int>::iterator it = mScriptHandlers.find(LOAD);
	if(it!=mScriptHandlers.end())
	{
		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		LUA_LINK_THIS;
		pEngine->executeEvent(it->second,"load");
	}
}

void CCBLuaContainer::unload( void )
{
	std::map<int, int>::iterator it = mScriptHandlers.find(UNLOAD);
	if(it!=mScriptHandlers.end())
	{
		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		LUA_LINK_THIS;
		pEngine->executeEvent(it->second,"unload");
	}
	CCBContainer::unload();
}

static CCBLuaContainer* _searchChild(cocos2d::CCArray* childern, int tag)
{
	if(childern && childern->count() > 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(childern, child)
		{
			CCNode* pNode = (CCNode*) child;
			if(pNode && pNode->getTag() == tag && dynamic_cast<CCBLuaContainer*>(pNode))
			{
				return dynamic_cast<CCBLuaContainer*>(pNode);
			}
			else
			{
				CCBLuaContainer* ret = _searchChild(pNode->getChildren(),tag);
				if(ret) return ret;
			}
		}
	}
	return 0;
}
CCBLuaContainer* CCBLuaContainer::searchChildByTag( int tag )
{
	return _searchChild(getChildren(),tag);
}

CCBLuaContainer* CCBLuaContainer::searchParentByTag( int tag )
{
	CCNode* pNode = this;
	while(pNode->getParent()!=0)
	{
		pNode = pNode->getParent();
		if(pNode->getTag() == tag && dynamic_cast<CCBLuaContainer*>(pNode))
		{
			return dynamic_cast<CCBLuaContainer*>(pNode);
		}
	}
	return 0;
}

void CCBLuaContainer::setTag( int tag )
{
	CCNode::setTag(tag);
}

int CCBLuaContainer::getTag()
{
	return CCNode::getTag();
}

CCBLuaContainer* CCBLuaContainer::mLualink = 0;
