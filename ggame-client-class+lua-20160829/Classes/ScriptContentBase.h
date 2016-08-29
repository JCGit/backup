#pragma once
#include "CCBContainer.h"
#include "cocos-ext.h"

#define LUA_GETVAR_FUNC_REPLACE(_type,_func) \
	_type* _func( char* name ){ \
	cocos2d::CCObject* obj = getVariable(name); \
	_type* node = obj?dynamic_cast<_type*>(obj):0; \
	return node;}

class ScriptContentBase
	: public CCBContainer
	, public cocos2d::extension::CCReViSvItemNodeFacade
{
public:
	ScriptContentBase();
	ScriptContentBase(std::string fileName);
	virtual ~ScriptContentBase();

	virtual void initItemView();
	virtual void refreshItemView(cocos2d::extension::CCReViSvItemData* pItem);

// #ifdef _DEBUG
// 	unsigned int m_iIdx;
// #endif

	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
	virtual void onAnimationDone(const std::string& animationName);

	CREATE_FUNC(ScriptContentBase);
	//for lua
	static ScriptContentBase* create(const std::string& fileName, int tag = 0);

	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCNode,getVarNode);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCSprite,getVarSprite);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCLabelBMFont,getVarLabelBMFont);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCLabelTTF,getVarLabelTTF);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCMenu,getVarMenu);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCMenuItem,getVarMenuItem);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCMenuItemSprite,getVarMenuItemSprite);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCMenuItemImage,getVarMenuItemImage);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::extension::CCScrollView,getVarScrollView);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::extension::CCScale9Sprite,getVarScale9Sprite);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCParticleSystem, getParticleSystem);
	//add by zhenhui 2014/7/22
	LUA_GETVAR_FUNC_REPLACE(cocos2d::extension::CCMenuItemCCBFile,getVarMenuItemCCB);

	void registerFunctionHandler(int nHandler);
	void unregisterFunctionHandler();

	cocos2d::extension::CCReViSvItemData* getItemDate();

	virtual void loadCcbiFile(const std::string& filename, bool froceLoad = false);
	void runAnimation(const std::string& actionname);

	void validateFunctionHandler(const std::string& functionName);

	//根据外frame的边框的缩放scale和要显示的纹理大小，动态设置里面ICON的纹理图片和缩放
	void setDynamicIconTex(std::string frameName ,std::string iconName , std::string iconStr);
	
	void setFrameColorByQuality(std::string frameName ,int quality);
	
	void setLabelColorByQuality(std::string labelName ,int quality);

	virtual std::string dumpInfo();
private:
	std::string mScriptFileName;
	int mScriptFunHandler;
	cocos2d::extension::CCReViSvItemData* mItemDate;
};