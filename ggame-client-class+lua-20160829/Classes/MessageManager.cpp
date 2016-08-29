
#include "stdafx.h"

#include <map>
#include <set>
#include <list>
#include "Singleton.h"
#include "cocos2d.h"

#include "MessageManager.h"

#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "Gamelua.h"

void MessageManager::update()
{
	MESSAGE_LIST msgs;
	mMessages.swap(msgs);
	while(!msgs.empty())
	{
		GameMessage* message = msgs.front();
		unsigned int const id = message->getTypeId();

		MESSAGE_HANDLER_MAP::iterator it = mHandlers.find(id);
		if(it != mHandlers.end())
		{
			std::set<MessageHandler*> handlerset;
			handlerset.insert(it->second.begin(),it->second.end());

			std::set<MessageHandler*>& handlerset_ref = it->second;
			std::set<MessageHandler*>::iterator hanIt = handlerset.begin();
			for( ; hanIt!= handlerset.end(); ++hanIt)
			{
				if(handlerset_ref.find(*hanIt)!=handlerset_ref.end())
					(*hanIt)->onReceiveMassage(message);
			}
		}

		delete message;
		msgs.pop_front();
	}
}
void MessageManager::sendMessage(const GameMessage* message)
{
	if(message == 0)
		return;
	mMessages.push_back(message->clone());
	CCLOG("Message in queue! ID:%d",message->getTypeId());
}
void MessageManager::sendMessageForScript(GameMessage* message)
{
	mMessages.push_back(message);
	CCLOG("Message in queue! ID:%d",message->getTypeId());
}

void MessageManager::regisiterMessageHandler(unsigned int id, MessageHandler* handler)
{
	if(handler == 0)
		return;

	MESSAGE_HANDLER_MAP::iterator it = mHandlers.find(id);
	if(it == mHandlers.end())
	{
		std::set<MessageHandler*> sec;
		sec.insert(handler);
		mHandlers.insert(std::make_pair(id,sec));
	}
	else 
	{
		std::set<MessageHandler*>& handlerset = it->second;
		if(handlerset.find(handler)==handlerset.end())
			handlerset.insert(handler);
	}
}
void MessageManager::removeMessageHandler(unsigned int id, MessageHandler* messagehandler)
{
	if(messagehandler == 0)
		return;

	MESSAGE_HANDLER_MAP::iterator it = mHandlers.find(id);
	if(it != mHandlers.end())
	{
		std::set<MessageHandler*>& handlerset = it->second;
		if(handlerset.find(messagehandler) != handlerset.end())
			handlerset.erase(messagehandler);
	}

}
void MessageManager::removeMessageHandler(MessageHandler* messagehandler)
{
	if(messagehandler == 0)
		return;

	MESSAGE_HANDLER_MAP::iterator it = mHandlers.begin();
	for(;it!=mHandlers.end();++it)
	{
		std::set<MessageHandler*>& handlerset = it->second;
		if(handlerset.find(messagehandler)!= handlerset.end())
		{
			handlerset.erase(messagehandler);
		}
	}
}

bool MessageManager::registerFactory(int id, GameMessageFactory* factory)
{
	mMessageFactoryMap.insert(std::make_pair(id,factory));
	return true;
}


void MessageScriptHandler::onReceiveMassage( const GameMessage * message )
{
	cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
	bool validateFlag = pEngine->checkHandlerValidate(mScriptFunHandler); 
	
	if (validateFlag == false)
	{
#ifdef _DEBUG
		char msg[256];
		sprintf(msg,"@MessageScriptHandler::onReceiveMassage ,script function handler %d not fount, message type id is %d, register again",mScriptFunHandler,message->getTypeId());
		cocos2d::CCMessageBox(msg,"Error found");
#endif
		CCLOG("@MessageScriptHandler::onReceiveMassage ,script function handler %d not fount, message type id is %d, register again",mScriptFunHandler,message->getTypeId());
		//If handler is not validate, require the PackageLogicForLua and register the handler again.
		pEngine->executeString("require \"ListenMessage\"");
		std::string funname = "validateAndRegisterMsgHandler";
		cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()->executeGlobalFunctionByName(funname.c_str(),NULL,"MessageScriptHandler");
	}
	pEngine->executeEvent(mScriptFunHandler,"luaReceiveMassage" ,const_cast<GameMessage *>(message),"GameMessage");
}

void MessageScriptHandler::registerFunctionHandler( int nHandler )
{
	unregisterFunctionHandler();
	mScriptFunHandler = nHandler;
}

void MessageScriptHandler::unregisterFunctionHandler()
{
	if (mScriptFunHandler)
	{
		cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(mScriptFunHandler);
		mScriptFunHandler = 0;
	}
}

MessageScriptHandler::~MessageScriptHandler()
{
	if (mScriptFunHandler)
	{
		cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(mScriptFunHandler);
		mScriptFunHandler = 0;
	}
}

MessageScriptHandler::MessageScriptHandler( int nHandler ) : mScriptFunHandler(nHandler)
{

}
