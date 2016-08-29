
#include "stdafx.h"
#include "ScriptContentBase.h"
#include "CCLuaEngine.h"
#include "script_support/CCScriptSupport.h"
#include "Gamelua.h"
#include "ResManager.h"
#include "Language.h"
#define RUN_SCRIPT_FUN(funname) \
	if(mScriptFunHandler) \
{ \
	cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine(); \
	validateFunctionHandler(funname);\
	pEngine->executeEvent(mScriptFunHandler,funname,this,"ScriptContentBase"); \
}

//--begin zhengxin at 2014-08-11
typedef std::list<ScriptContentBase*> ScriptContentBaseList;
typedef std::map<std::string, ScriptContentBaseList> ScriptContentBaseListMap;
static ScriptContentBaseListMap stSCBPool;
//
static ScriptContentBase* createAndLoadSCB( const std::string & ccbfile )
{
	ScriptContentBaseListMap::iterator mapit = stSCBPool.find(ccbfile);
	if(mapit == stSCBPool.end())
	{
		ScriptContentBase *ccb = new ScriptContentBase(ccbfile);

		//CCLOG("[ScriptContentBase1] ccbfile: %s", ccbfile.c_str());

		ScriptContentBaseList clist;
		clist.push_back(ccb);

		stSCBPool.insert(std::make_pair(ccbfile,clist));

		return ccb;
	}
	else
	{
		ScriptContentBaseList::iterator listit = mapit->second.begin();
		for(; listit != mapit->second.end(); ++listit)
		{
			ScriptContentBase* ccb = *listit;
			if(ccb->isSingleReference())
			{
				//CCLOG("[ScriptContentBase2] ccbfile: %s", ccbfile.c_str());
				return ccb;
			}
		}

		ScriptContentBase *ccb = new ScriptContentBase(ccbfile);

		//CCLOG("[ScriptContentBase3] ccbfile: %s", ccbfile.c_str());

		mapit->second.push_back(ccb);

		return ccb;
	}
}
//--end

ScriptContentBase* ScriptContentBase::create( const std::string& fileName, int tag /*= 0*/ )
{
	ScriptContentBase *pRet = new ScriptContentBase(fileName); 
	pRet->setTag(tag);
	pRet->autorelease();
	//
	//ScriptContentBase *pRet = createAndLoadSCB(fileName);
	//pRet->setTag(tag);
	//
	return pRet; 
}

ScriptContentBase::ScriptContentBase()
	: mScriptFunHandler(0)
	, mItemDate(NULL)
{
	m_pItemNode = this;
}

ScriptContentBase::ScriptContentBase( std::string fileName )
	:mScriptFunHandler(0)
	,mItemDate(NULL)
{
	loadCcbiFile(fileName);
	this->m_pItemNode = this;
	mScriptFileName = fileName;
	this->retain();
	mScriptFunHandler = NULL;
}

ScriptContentBase::~ScriptContentBase()
{
	unregisterFunctionHandler();
	removeFromParent();
}


void ScriptContentBase::validateFunctionHandler(const std::string& functionName)
{
	cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
	bool validateFlag = pEngine->checkHandlerValidate(mScriptFunHandler); 
	if (validateFlag == false)
	{
#ifdef _DEBUG
		char msg[256];
		sprintf(msg,"@ScriptContentBase::load ,script function handler %d not fount,page name is %s, function name is %s, register again",mScriptFunHandler,mScriptFileName.c_str(),functionName.c_str());
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
#endif
		CCLOG("@ScriptContentBase::load ,script function handler %d not fount,page name is %s, function name is %s, register again",mScriptFunHandler,mScriptFileName.c_str(),functionName.c_str());
		/*	pEngine->executeString(("require \""+mScriptFileName+"\"").c_str());
		std::string funname = "luaCreat_" + mScriptFileName;
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeGlobalFunctionByName(funname.c_str(),this,"CCBScriptContainer");*/
	}
}


void ScriptContentBase::initItemView()
{
	RUN_SCRIPT_FUN("luaInitItemView");
}

void ScriptContentBase::refreshItemView( cocos2d::extension::CCReViSvItemData* pItem )
{
	mItemDate = pItem;
	RUN_SCRIPT_FUN("luaRefreshItemView");
}

void ScriptContentBase::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender )
{
	RUN_SCRIPT_FUN(itemName.c_str());
}

void ScriptContentBase::onAnimationDone( const std::string& animationName )
{
	RUN_SCRIPT_FUN("luaOnAnimationDone");
}

void ScriptContentBase::registerFunctionHandler( int nHandler )
{
	unregisterFunctionHandler();
	mScriptFunHandler = nHandler;
}

void ScriptContentBase::unregisterFunctionHandler()
{
	if (mScriptFunHandler)
	{
		cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(mScriptFunHandler);
		mScriptFunHandler = 0;
	}
}

cocos2d::extension::CCReViSvItemData* ScriptContentBase::getItemDate()
{
	return mItemDate;
}

void ScriptContentBase::setDynamicIconTex(std::string frameName ,std::string iconName , std::string iconStr)
{
	//此宏定义和方法可以放到CCBContainer内,
	CCB_SET_ICON_TEX(this,frameName,iconName,iconStr);
}

void ScriptContentBase::setFrameColorByQuality(std::string frameName ,int quality)
{
	//此宏定义和方法可以放到CCBContainer内,
	CCB_SET_FRAME_TEX(this,frameName,quality);
}

void ScriptContentBase::setLabelColorByQuality(std::string labelName ,int quality)
{
	CCB_SET_LABEL_COLOR(this,labelName,quality);
}

std::string ScriptContentBase::dumpInfo()
{
	return CCBContainer::dumpInfo();
}

void ScriptContentBase::runAnimation( const std::string& actionname )
{
	CCBContainer::runAnimation(actionname);
}

void ScriptContentBase::loadCcbiFile(const std::string& filename, bool froceLoad /* = false */)
{
	CCBContainer::loadCcbiFile(filename, froceLoad);
	mScriptFileName = filename;
}