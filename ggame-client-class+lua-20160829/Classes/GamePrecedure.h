#pragma once

#include "Singleton.h"
#include "StateMachine.h"
#include "CCScheduler.h"
#include "cocos2d.h"
#include <map>
#include <google/protobuf/message.h>
#include "PacketManager.h"
#include "BulletinManager.h"

class CCBContainer;
class LogoFrame;
class LoadingFrame;
class MainFrame;

#define AFTER_LOGIN_PACKET_NUM 10

class GamePrecedure : 
	public cocos2d::CCObject,
	public PacketHandler,
	public Singleton<GamePrecedure>
{
protected:
	LogoFrame *mLogoFrame;
	LoadingFrame *mLoadingFrame;
	MainFrame *mMainFrame;

	CCBContainer *mLoadFrameCcbiPreLoad;

	StateMachine<GamePrecedure> *m_pStateMachine;
	//RythemTouchState * m_pTouchState;

	cocos2d::CCScheduler* m_pScheduler;
	float mFrameTime;
	float mTotalTime;
    bool mInLoadingScene;
	bool gotoMainScene;
	bool mIsReEnterLoadingFrame;	//zhengxin 2014-08-10是否是从MainFrame退回到LoadingFrame的
	float mGotHeartBeatTime;		//zhengxin 2014-08-2310秒钟未收到心跳回报的情况下，重置一次强制needreconnect

	int mServerID;
	std::string mUin;

	float mHeartbeatTime;
    bool mInitialized;
	int mServerTime;

	bool mReadTables;
	bool mInitLuaDone;

	bool mHasAlertServerUpdating;

	std::string mPlayerUin;
	//--begin xinzheng 2013-12-7
	bool mStartLoginJiShi;	//处于登录收包计时，20秒菊花，超时玩家可重新点击进入游戏（disconnect、connect）
	int mLoginPacketsReceivedCount;
	int mLoginRequestPacketsTotalCount;
	//--end

	bool m_loginPacketAssemblySuccess;


	//判断是否roleId为0，来表示该玩家是否进入选角色页面
	bool mHasMainRole;

	//add by dylan 20140107 玩家被从游戏内踢出标志位，如果符合不进行重连调戏
	bool mUserKickStatus;
	int mServerOpenDay;
	struct REQUEST_PACKAGE
	{
		int opcode;
		int opcoderet;
		::google::protobuf::Message* message;
		bool bfirstsent;
	};
	//std::list<REQUEST_PACKAGE> mRequestPackages;
    BulletinManager* m_pBulletinMgr;

	bool _requestPackage(int opcode, int opcoderet, ::google::protobuf::Message* message);
	void _gotPackage(int opcoderet);
	void _failedPakcage(int opcode, bool isSendFaild);
public:
	GamePrecedure()
		:mLoadingFrame(0)
		,mLogoFrame(0)
		,mMainFrame(0)
		,mFrameTime(0)
		,mLoadFrameCcbiPreLoad(0)
		,mTotalTime(0),mGotHeartBeatTime(0.f)
		,m_pScheduler(0)
		,m_pStateMachine(0)
        ,mInLoadingScene(true),mIsReEnterLoadingFrame(false)
		,mHasMainRole(true)
		,gotoMainScene(false)
		,mHeartbeatTime(0)
        ,mInitialized(false)
		,mServerTime(0)
		,mHasAlertServerUpdating(false),mLoginRequestPacketsTotalCount(0)
		,mServerOpenDay(0),mStartLoginJiShi(false),mLoginPacketsReceivedCount(0)
		,mUserKickStatus(false)
		,mPlayerUin("")
		,mInitLuaDone(false)
		,m_loginPacketAssemblySuccess(false)
        ,m_pBulletinMgr(NULL)
		//,m_pTouchState(0)
	{
		
	}

	~GamePrecedure();

	static GamePrecedure* getInstance(){return GamePrecedure::Get();}

	void init();

	//--begin xinzheng 2013-6-3
	void unInit();//clear memory
	//--end

	void reset();

	LoadingFrame* getLoadingFrame();

	void enterLogoMovie();

	void enterLoading();
	void reEnterLoading();
	void preEnterMainMenu();
	void readTables();
	//void requestPackages();
	//void reRequestPackages();

	//登录失败返回ERROR code之后，返回登录开始页面 by zhenhui 2014/8/22
	void showLoadingEnter();

	void enterMainMenu();
	void enterGame();
	void enterInstruction(int id = -1);//-1 means start from the first
	void exitGame();
	void update(float dt);

    bool isInLoadingSceneAndNeedExit();
    bool isInLoadingScene(){return mInLoadingScene;}
	bool isReEnterLoadingFrame(){return mIsReEnterLoadingFrame;}
	void setInLoadingScene(){mInLoadingScene = true;}
	float getFrameTime();
	float getTotalTime();
	void setGotHeartBeatTime(){mGotHeartBeatTime = 0.f;}
    
	void enterBackGround();
    void enterForeGround();

	void initLuaEnv();

	bool getLoginAssemblyFinished(){return m_loginPacketAssemblySuccess;};
	void setLoginAssemblyFinished(bool flag){m_loginPacketAssemblySuccess = flag;};
	void setHasMainRole(bool has){mHasMainRole = has;};

	void setServerID(int serverID){mServerID = serverID;}
	int getServerID(){return mServerID;}
	void setLoginUin(std::string uid) { mPlayerUin=uid;}
	std::string getLoginUin() { return mPlayerUin;}

	int getServerTime(){ return mServerTime;};
	void setServerTime(int serverTime){ mServerTime=serverTime;};
	void setUin(const std::string& uin){mUin = uin;}
	const std::string& getUin(){return mUin;}
	void setServerOpenTime(int _openDay){ mServerOpenDay=_openDay;};
	int getServerOpenTime(){ return mServerOpenDay;};
	bool hasAlertServerUpdating(){return mHasAlertServerUpdating;}
	void setAlertServerUpdating(bool hasAlert){mHasAlertServerUpdating = hasAlert;}
	//--begin xinzheng 2013-5-23
	//引擎层给游戏截图，返回png文件存储绝对路径
	const std::string gameSnapshot();
	//--end

	void setUserKickedByServer(){ mUserKickStatus=true;};

	bool getUserKickedByServer(){return mUserKickStatus;};
	//监听所有超时重连发送的包接收情况,若全部接收完成后,判定为重连成功,
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet){_gotPackage(opcode);}
	//重连后，包若发送失败继续发送
	virtual void onSendPacketFailed(const int opcode){_failedPakcage(opcode,true);}
	virtual void onReceivePacketFailed() { _failedPakcage(0, false); } ;
	virtual void onConnectFailed(std::string ip, int port);
	virtual void onTimeout(const int opcode){_failedPakcage(opcode,true);}
	virtual void onPacketError(const int opcode){_failedPakcage(opcode,true);_failedPakcage(opcode,false);}
    
    void showBulletin();
    void closeBulletin();
   };

   void ReEnterLoading(bool boo);

