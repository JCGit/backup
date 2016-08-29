#pragma once
#include "CustomPage.h"
#include "StateMachine.h"
#include "MainFrame.h"
#include <string>

#define LUA_GETVAR_FUNC_REPLACE(_type,_func) \
	_type* _func( char* name ){ \
	cocos2d::CCObject* obj = getVariable(name); \
	_type* node = obj?dynamic_cast<_type*>(obj):0; \
	return node;}


class CCBLuaContainer
	: public CustomPage
	, public State<MainFrame>
{
private:
	std::string mCcbfilename;
	static CCBLuaContainer* mLualink;
	std::map<std::string, int> mFuncHandlers;
	std::map<int, int> mScriptHandlers;
public:
	PAGE_TYPE getPageType(){return PT_LUA;}
	bool init();

	virtual void Enter(MainFrame*);
	virtual void Execute(MainFrame*);
	virtual void Exit(MainFrame*);

	virtual void load(void);
	virtual void unload(void);
	void run();

	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
	
	//for lua
	static CCBLuaContainer* create(const std::string& ccbfile, int tag = 0);
	static CCBLuaContainer* lualink();
	enum FuncType
	{
		ENTER,
		EXECUTE,
		EXIT,
		LOAD,
		UNLOAD,
	};

	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCNode,getVarNode);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCSprite,getVarSprite);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCLabelBMFont,getVarLabelBMFont);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCLabelTTF,getVarLabelTTF);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCMenu,getVarMenu);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCMenuItem,getVarMenuItem);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCMenuItemSprite,getVarMenuItemSprite);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::extension::CCScrollView,getVarScrollView);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::extension::CCScale9Sprite,getVarScale9Sprite);

	void setTag(int tag);
	int getTag();
	//search for the first child element with tag
	CCBLuaContainer* searchChildByTag(int tag);
	//search for the first parent element with tag
	CCBLuaContainer* searchParentByTag(int tag);

	void registerFunctionHandler(char* func, int nHandler);
	void unregisterFunctionHandler(char* func);

 	void registerScriptHandler(FuncType type, int nHandler);
 	void unregisterScriptTapHandler(FuncType type);
	
};

