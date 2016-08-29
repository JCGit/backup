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


class CCBScriptContainer
	: public CustomPage
	, public State<MainFrame>
	, public MessageHandler
	, public PacketHandler
	, public libOSListener
{
private:
	std::string mScriptFileName;
	int mScriptFunHandler;
	static CCBScriptContainer* createFromNew(const std::string& filename, int tag = 0);
	static std::map<std::string,std::list<CCBScriptContainer*> > ccbsPool;

	GameMessage* mMessage;

	int mSendOpcode;
	int mRecOpcode;
	std::string mPktBuffer;
	int mMessageBoxTag;
	std::string mInputBoxContent;
	std::string mCurAnimDoneName;
public:
	CCBScriptContainer(){};
	~CCBScriptContainer();

	PAGE_TYPE getPageType(){return PT_LUA;}
	bool init();

	virtual void Enter(MainFrame*);
	virtual void Execute(MainFrame*);
	virtual void Exit(MainFrame*);

	virtual void load(void);
	virtual void unload(void);

	virtual void onAnimationDone(const std::string& animationName);
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
	virtual void onReceiveMassage(const GameMessage * message);
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet);
	virtual void onReceivePacket(const int opcode, const std::string& str);
	virtual void onSendPacketFailed(const int opcode);
	virtual void onConnectFailed(std::string ip, int port);
	virtual void onTimeout(const int opcode);
	virtual void onPacketError(const int opcode);
	virtual Handler_Type getHandleType(void) {return Scripty_Handler;};

	virtual void onInputboxEnter(const std::string& content);
	virtual void onMessageboxEnter(int tag);

	//for lua
	static CCBScriptContainer* create(const std::string& ccbfile, int tag = 0);
	static void purgeCachedData(void);

	void registerPacket(int opcode);
	void removePacket(int opcode);
	void sendPakcet(int opcode, char* buff, int length, bool needWaiting = true);

	void validateFunctionHandler(const std::string& functionName);

	//根据外frame的边框的缩放scale和要显示的纹理大小，动态设置里面ICON的纹理图片和缩放
	void setDynamicIconTex(std::string frameName ,std::string iconName , std::string iconStr);

	void setFrameColorByQuality(std::string frameName ,int quality);

	void setLabelColorByQuality(std::string labelName ,int quality);
	
	 void autoAdjustResizeScrollview(cocos2d::extension::CCScrollView *pView);
	void autoAdjustResizeScale9Sprite(cocos2d::extension::CCScale9Sprite * pscale9Sprite);


	int getSendPacketOpcode(){return mSendOpcode;}
	int getRecPacketOpcode(){return mRecOpcode;}
	int getRecPacketBufferLength(){return mPktBuffer.length();}
	std::string  getRecPacketBuffer(){return mPktBuffer;}


	void registerMessage(int msgID);
	void removeMessage(int msgID);
	
	GameMessage* getMessage(){return mMessage;}

	void registerLibOS();
	void removeLibOS();
	const char* getInputboxContent(){return mInputBoxContent.c_str();}
	int getMessageboxTag(){return mMessageBoxTag;}
	
	virtual void loadCcbiFile(const std::string& filename, bool froceLoad = false);
	void runAnimation(const std::string& actionname);

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
	//add by zhenhui 2014/7/11
	LUA_GETVAR_FUNC_REPLACE(cocos2d::extension::CCMenuItemCCBFile,getVarMenuItemCCB);
	LUA_GETVAR_FUNC_REPLACE(cocos2d::CCLayer,getVarLayer);

	void setTag(int tag);
	int getTag();
	//search for the first child element with tag
	CCBScriptContainer* searchChildByTag(int tag);
	//search for the first parent element with tag
	CCBScriptContainer* searchParentByTag(int tag);

	void registerFunctionHandler(int nHandler);
	void unregisterFunctionHandler();

	virtual std::string dumpInfo();
	std::string getCurAnimationDoneName(){return mCurAnimDoneName;}
};

