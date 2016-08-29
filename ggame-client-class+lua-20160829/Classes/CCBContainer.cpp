
#include "stdafx.h"

#include "CCBContainer.h"
#include "Language.h"
#include "GameMessages.h"
#include "waitingManager.h"
#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "MessageBoxPage.h"
#include <algorithm>
#include "CCTagedRichTTF.h"
USING_NS_CC;

USING_NS_CC_EXT;

CCBContainer::CCBContainer(CCBContainerListener* ccContainerListener, int tag)
	:mActionManager(0)
	,mCCBContainerListener(ccContainerListener)
	,mCCBTag(tag)
{
	//containterCount++;
	//CCLOG("Container size:%d",containterCount);
}

CCBContainer::CCBContainer( void )
	:mActionManager(0)
	,mCCBContainerListener(0)
	,mCCBTag(0)
{
	//containterCount++;
	//CCLOG("Container size:%d",containterCount);
}


void CCBContainer::setListener( CCBContainerListener* listener , int tag)
{
	mCCBContainerListener = listener;
	mCCBTag = tag;
}

void CCBContainer::setAllChildColor(unsigned char r, unsigned char g, unsigned char b)
{
	//
	ccColor3B c;
	c.r = r;
	c.g = g;
	c.b = b;
	for (VARIABLE_MAP::iterator it = mVariables.begin(); it != mVariables.end(); it++)
	{
		CCNodeRGBA *pSpr = dynamic_cast<CCNodeRGBA*>(it->second);
		if (!pSpr)
			continue;

		//
		pSpr->setColor(c);	
	}// end for

	/*CCObject* pObject = NULL;
	CCARRAY_FOREACH(m_pChildren, pObject)
	{
		CCNodeRGBA* pChild = dynamic_cast<CCNodeRGBA*>(pObject);
		if (pChild)
		{
			pChild->setColor(c);
		}
	}*/
}

CCBContainer::~CCBContainer(void)
{
// 	containterCount--;
// 	CCLOG("Container size:%d",containterCount);
	unload();
	//if(mActionManager)mActionManager->release();
}

void CCBContainer::loadCcbiFile( const std::string& filename, bool froceLoad/* = false*/ )
{
	if(getLoaded())
	{
		if(froceLoad) {
			removeAllChildren();
		}
		else
			return;
	}
	mLoadedCCBFile = filename;
	CCLOG("CCBContainer::begin load ccbi file: %s", mLoadedCCBFile.c_str());

	/* Create an autorelease CCNodeLoaderLibrary. */
	cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	/* Create an autorelease CCBReader. */
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary,this);

	/* Read a ccbi file. */
	cocos2d::CCNode *node = ccbReader->readNodeGraphFromFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.c_str()).c_str(),this);

	mActionManager = ccbReader->getAnimationManager();
	//mActionManager->runAnimations("Default");
	mActionManager->retain();
	mActionManager->setAnimationCompletedCallback(this,callfunc_selector(CCBContainer::_animationDone));
	ccbReader->release();
	if(node)
	{
		Language::Get()->updateNode(node);
		addChild(node);
		setContentSize(node->getContentSize());
	}
	else
	{
		char msgStr[512]={0};
		sprintf(msgStr,"CCBContainer.loadCcbiFile %s Failed",filename.c_str());
		MSG_BOX(msgStr);
	}

	CCLOG("CCBContainer::end load ccbi file: %s\n\n", mLoadedCCBFile.c_str());
}

void CCBContainer::playAutoPlaySequence()
{
	if (mActionManager && mActionManager->getAutoPlaySequenceId() != -1 && !mActionManager->jsControlled)
	{
		mActionManager->runAnimationsForSequenceIdTweenDuration(mActionManager->getAutoPlaySequenceId(), 0);
	}
}

void CCBContainer::runAnimation( const std::string& actionname )
{
	if(mActionManager)
	{
		int seqId = mActionManager->getSequenceId(actionname.c_str());
		if(seqId<0)
		{
			char outStr[256];
			sprintf(outStr,"Faild to run animation:%s",actionname.c_str());
			CCMessageBox(outStr,"error!");
			onAnimationDone(actionname);
			if(mCCBContainerListener)
				mCCBContainerListener->onAnimationDone(actionname);
		}
		else
		{
			mActionManager->runAnimations(actionname.c_str());
		}
	}
		
}

bool CCBContainer::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	if(pTarget!=this) return false;
	//
	std::string var = pMemberVariableName;
	//CCBI文件自身保证不重名节点
	std::transform(var.begin(),var.end(),var.begin(),tolower);//xinzheng 2013-05-27 重做的一版资源 存在命名大小写出入，程序处理以支持
	VARIABLE_MAP::iterator it = mVariables.find(var);
	
	if(it!=mVariables.end())
	{
		if( it->second!=pNode )
		{
#ifdef _WIN32
			//检查在ccbi中是否有重名的变量 by zhenhui
			if (var != "")
			{
				char outStr[256];
				sprintf(outStr,"Duplicate variable found in %s, variable name is :%s",mLoadedCCBFile.c_str(),var.c_str());
				CCLOG(outStr);
				CCMessageBox(outStr,"Duplicate variable found in ccbi");
			}		
#endif
			
			it->second->release();
			it->second = pNode;
			pNode->retain();
			return true;
		}
		else
			return true;
	}

	pNode->retain();
	mVariables.insert(std::make_pair(var,pNode));
	return true;
}

cocos2d::CCObject* CCBContainer::getVariable( const std::string& variablename )
{
	std::string var(variablename);
	std::transform(var.begin(),var.end(),var.begin(),tolower);//xinzheng 2013-05-27 重做的一版资源 存在命名大小写出入，程序处理以支持
	VARIABLE_MAP::iterator it = mVariables.find(var);
	if(it!=mVariables.end())
		return it->second;
	else
	{
#ifdef _DEBUG
		//add by zhenhui to check whether the variable is in the specific ccbi.
		/*char msg[256];
		sprintf(msg,"variable %s is not find in the ccbi %s!",variablename.c_str(),mLoadedCCBFile.c_str());
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());*/
#endif // _DEBUG
		return NULL;
	}
}

#define getVariableAs(_type_) \
	_type_* CCBContainer::get##_type_##FromCCB( const std::string& _var_ ) \
{ \
	CCObject* __obj__ = getVariable(_var_);\
	_type_* __node__ = __obj__?dynamic_cast<_type_*>(__obj__):0; \
	return __node__; \
};

getVariableAs(CCNode);
getVariableAs(CCSprite);
getVariableAs(CCLabelBMFont);
getVariableAs(CCLabelTTF);
//getVariableAs(CCBFileNew);
getVariableAs(CCMenuItemImage);
getVariableAs(CCScrollView);
getVariableAs(CCMenuItemCCBFile);


cocos2d::SEL_MenuHandler CCBContainer::onResolveCCBCCMenuItemSelectorWithSender( cocos2d::CCObject * pTarget, const char* pSelectorName, cocos2d::CCNode* sender )
{
	if(pTarget!=this)
		return NULL;
	std::string name = pSelectorName;
	MENUITEM_MAP::iterator it = mMenus.find(sender);
	if(it!=mMenus.end())
		it->second = name;
	else
		mMenus.insert(std::make_pair(sender,name));

	return menu_selector(CCBContainer::_menuItemClicked);
}

void CCBContainer::_menuItemClicked( cocos2d::CCObject * pSender )
{
	if(waitingManager::Get()->getWaiting())
	{
//提示所等待的opcode。但某些特定情况，如点击-好友-邀请按钮，会同时触发onclickBtn  以及onContent，所以会出现“假等待”情况，可以忽略该情况
#ifdef _DEBUG
#ifdef WIN32
		MSG_BOX(waitingManager::Get()->getWaitingOpcode());
#else
		//MSG_BOX_LAN("@SendPacketWaiting");
#endif
#endif	
		return;
	}

	MENUITEM_MAP::iterator it = mMenus.find(pSender);
	if(it!=mMenus.end())
	{
		
        MsgButtonPressed msg;
		msg.ccb = mLoadedCCBFile;
		msg.func = it->second;
		CCMenuItem* menu = dynamic_cast<CCMenuItem*>(pSender);
		if (menu)
		{
			msg.tag = menu->getTag();			
		}
		else
		{
			msg.tag = mCCBTag;
		}
		MessageManager::Get()->sendMessage(&msg);
		CCLOG("CCB:Button pressed! ccb:%s, func:%s, tag:%d",msg.ccb.c_str(),msg.func.c_str(),msg.tag);
        
		int ret = 0;
		if(CCScriptEngineManager::sharedManager()->getScriptEngine())
		{
			std::string luaName = mLoadedCCBFile;
			size_t lastdot = mLoadedCCBFile.find_last_of('.');
			if(lastdot!=std::string::npos)
				luaName = mLoadedCCBFile.substr(0,lastdot);
			std::string funname = luaName + "_"+ it->second;
			ret = CCScriptEngineManager::sharedManager()->getScriptEngine()->executeGlobalFunctionByName(funname.c_str(), this, "CCBContainer");
		}

		if(!ret)
		{
            if(pSender)
            {
                onMenuItemAction(it->second,pSender);
                if(mCCBContainerListener)
                    mCCBContainerListener->onMenuItemAction(it->second, pSender, mCCBTag);
            }
		}
	}
}

bool CCBContainer::getLoaded()
{
	return getChildrenCount()>0;
}

void CCBContainer::unload()
{
	if(mActionManager)mActionManager->release();
	mActionManager = 0;
	VARIABLE_MAP::iterator it = mVariables.begin();
	for(;it!=mVariables.end();++it)
		if(it->second)it->second->release();

	mVariables.clear();
	mMenus.clear();
	mCCBContainerListener = 0;
	removeAllChildren();
}

void CCBContainer::_animationDone()
{
	onAnimationDone(mActionManager->getLastCompletedSequenceName());
	if(mCCBContainerListener)
		mCCBContainerListener->onAnimationDone(mActionManager->getLastCompletedSequenceName());
}

bool CCBContainer::init()
{
	mCCBContainerListener = 0;;
	mCCBTag = 0;
	mLoadedCCBFile = "";
	return true;
}

std::string CCBContainer::dumpInfo()
{
	std::string info;
	info = "CCBContainer variables name info :\n";
	VARIABLE_MAP::iterator it = mVariables.begin();
	for(;it!=mVariables.end();++it)
	{
		info += it->first + "  " + typeid(*(it->second)).name();
		info += "\n";
	}

	info += "CCBContainer menuitems name info :\n";
	MENUITEM_MAP::iterator itr = mMenus.begin();
	for(;itr!=mMenus.end();++itr)
	{
		info += itr->second;
		info += "\n";
	}

	return info;
}

void CCBContainer::updateRichTextNode(cocos2d::CCNode* node)
{
	cocos2d::CCLabelTTF *ttf = dynamic_cast<cocos2d::CCLabelTTF* >(node);
	if(ttf)
	{	
		std::string content = ttf->getString();
		if (CCTagedRichTTF::isTagedRichText(content))
		{
			ttf->setString("");
			CCTagedRichTTF(content, ttf);
		}
	}
	cocos2d::CCObject* pObj = NULL;
	CCARRAY_FOREACH(node->getChildren(), pObj)
	{
		cocos2d::CCNode* subnode = dynamic_cast<cocos2d::CCNode*>(pObj);
		if(subnode)
		{
			updateRichTextNode(subnode);
		}
	}
}

unsigned long CCBContainer::containterCount = 0;

