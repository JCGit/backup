#pragma once
#include <map>
#include <set>
#include <list>
#include "Singleton.h"
#include "cocos2d.h"

class GameMessage : public cocos2d::CCObject
{
public:
	virtual const unsigned int getTypeId() const= 0;
	virtual GameMessage *clone() const = 0;
    virtual ~GameMessage(){};
};

class GameScriptMessage : public GameMessage
{
public:
	GameScriptMessage(unsigned int type):mTypeId(type){};
	virtual const unsigned int getTypeId() const {return mTypeId;};
	virtual GameMessage *clone() const {GameScriptMessage*ret = new GameScriptMessage(mTypeId);return ret;};

	unsigned int mTypeId;
};


class MessageHandler
{
public:
	virtual void onReceiveMassage(const GameMessage * message) = 0;
};

class MessageScriptHandler : public MessageHandler 
{
public:
	MessageScriptHandler( int nHandler );;
	virtual ~MessageScriptHandler();
	void registerFunctionHandler(int nHandler);
	void unregisterFunctionHandler();
	virtual void onReceiveMassage( const GameMessage * message);
private:
	int mScriptFunHandler;
};
/**
Lua Interface
func got 2 parameters.
First is a string "luaGameMessage". 
Second is a LuaGameMessObj contain MessageID and parameters in a char* string.
*/


class GameMessageFactory
{
public:
	virtual GameMessage* create(const std::string& paras = "") = 0;
};

class MessageManager : public Singleton<MessageManager>
{
private:
	typedef std::map<unsigned int,std::set<MessageHandler*> > MESSAGE_HANDLER_MAP;
	MESSAGE_HANDLER_MAP mHandlers;
	typedef std::list<GameMessage*> MESSAGE_LIST;
	MESSAGE_LIST mMessages;
	std::map<int,GameMessageFactory*> mMessageFactoryMap;
public:
	static MessageManager* getInstance(){return MessageManager::Get();}
	void update();
	void sendMessage(const GameMessage* message);
	void sendMessageForScript(GameMessage* message);
	void regisiterMessageHandler(unsigned int id, MessageHandler* handler);
	void removeMessageHandler(unsigned int id, MessageHandler* messagehandler);
	void removeMessageHandler(MessageHandler* messagehandler);
	bool registerFactory(int id, GameMessageFactory* factory);

};

