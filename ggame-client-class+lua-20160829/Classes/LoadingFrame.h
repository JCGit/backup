#pragma once


#include "cocos2d.h"
#include "cocos-ext.h"
#include "StateMachine.h"
#include "GamePrecedure.h"
#include "MessageManager.h"
#include "CCBManager.h"
#include "CCBContainer.h"
#include "GamePackets.h"
#include "lib91.h"
#include "libOS.h"
#include "PackageLogic.h"
//#include "BulletinBoardPage.h"
#include "Concurrency.h"
#include "SeverConsts.h"
#include "ContentBase.h"
// class MainPage;
// class TeamPage;
//#define Macro_AndroidTestJiHuoMa

#define LoadinAniPageTag 15678

using namespace cocos2d;


class ServerListContent
	: public ContentBase
	, public cocos2d::extension::CCReViSvItemNodeFacade


{
public:
	ServerListContent(unsigned int id,CCBContainerListener* listener)
		: ContentBase(id)
		, mListener(listener)
	{};
	virtual ~ServerListContent(){};
	virtual void refreshContent(void){};

	

	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender, int tag){};


	virtual void rebuildAllItem(void){};
	virtual void clearAllItem(void){};
	virtual void buildItem(){};
	virtual void addItem(ContentBase* item){};
	virtual void refreshAllItem(){};
	virtual void removeItem(ContentBase* item){};

	virtual void initItemView();
	virtual void refreshItemView(cocos2d::extension::CCReViSvItemData* pItem);

protected:
	virtual const std::string& getCcbiFileName(void) const
	{
		static std::string ccbiFileName = "LoadingFrameSeverList.ccbi";
		return ccbiFileName;
	}

	virtual const std::string& getBtnCcbiFileName(void) const
	{
		static std::string ccbiStarFileName = "LoadingFrameSever.ccbi";
		return ccbiStarFileName;
	}

private:
	CCBContainerListener* mListener;
	static std::list<SeverConsts::SEVER_ATTRIBUTE> mOrderlist;
};




class LoadingFrame
	: public BasePage
	, public State<GamePrecedure>
	, public MessageHandler
	, public CCBContainer::CCBContainerListener
	, public PacketHandlerGeneral
    , public platformListener
	, public libOSListener
//    , public BulletinBoardPageListener
{
private:
	enum CHILD_TAG
	{
		TAG_CONTENT,
	};
	enum MsgBoxErrCode
	{
		Err_CheckingFailed=300,
		Err_UpdateFailed,
		Err_ConnectFailed,
		Err_ErrMsgReport,
	};
	cocos2d::CCScene* mScene;
    bool mLogined;
    bool m91checked;
	int mSelectedSeverID;
	void showSevers(bool);
    bool mIsUpdateDown;
    bool mNetWorkNotWorkMsgShown;
	bool mIsFirstLoginNotServerIDInfo;
	bool mIsServerListBuild;
	int mIsNeedShowSeversOnLogin;
	bool mSendLogin;
    std::string gPuid;

	//
	SeverConsts::CHECK_STATE checkstate;
	SeverConsts::UPDATE_STATE updatestate;
    
	CCSprite* mTryUserLoginSprite;
	CCSprite* mUserLoginSprite;
	//
	SingleThreadExecuter mThread;
	//
public:
	LoadingFrame(void);
	~LoadingFrame(void);
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender);
	CREATE_FUNC(LoadingFrame);

	virtual void load(void);

	virtual void onShowOrHidBtn(bool v,const std::string btnName);

	//
	CCScene* getRootSceneNode();

	//this will execute when the state is entered
	virtual void Enter(GamePrecedure*);

	//this is the states normal update function
	virtual void Execute(GamePrecedure*);

	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(GamePrecedure*);

	virtual void Recycle();

	virtual void onReceiveMassage(const GameMessage * message);
	
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet);
	virtual void onConnectFailed(std::string ip, int port);
	virtual void onSendPacketFailed(const int opcode){showEnter();};
	virtual void onReceivePacketFailed(){showEnter();};
	virtual void onTimeout(const int opcode){showEnter();};
	virtual void onPacketError(const int opcode){showEnter();};
private:
	virtual int _getOpcode() {return LOGIN_S;};
public:
    virtual void onLogin(libPlatform*, bool success, const std::string& log);
    virtual void onPlatformLogout(libPlatform*);
    virtual void onUpdate(libPlatform*,bool ok, std::string msg);
	virtual void onMessageboxEnter(int tag);
    virtual void onBuyinfoSent(libPlatform*, bool success, const BUYINFO&, const std::string& log){};
	virtual void onMenuItemAction(const std::string& itemName, cocos2d::CCObject* sender, int tag);
    
    virtual void onStartLoad(){};
    virtual void onFinishLoad(){};
    virtual void onFailLoadWithError(const std::string error){};
    virtual void onBtnAction();
    virtual void onLoadingTimeOut();
    virtual void onInputboxEnter(const std::string& content);
    
    virtual bool isUpdateDown(){return mIsUpdateDown;};

	void showEnter();
	void showLoginingInPercent(int pct);//点进入游戏后的收发包进度
	void hideLoginingInPercent();

	void showLoadingAniPage();
	void hidLoadingAniPage();
private:
	void _updateLocalSeverId();
	void showUpdateChecking();
	void showUpdateCheckingFailed();
	void showUpdateFileChecking();
	void showUpdateUpdating();
	void showPersent(float persentage,std::string sizeTip="");
	void showUpdateFailed();
	void showUpdateDone();
	
	void showEnterAndLoginingIn();

	void updateSeverName();
	void updateLoginName();
	
	int getDefaultSeverID();

	void showAnnounce();
	void clearAnnounce();
};

