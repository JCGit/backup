
#pragma once
#include "cocos2d.h"
#include "core/Singleton.h"
#include "Message.h"
#include "Constant.h"
#include "game/ui//SelectWorldPage.h"
#include "game/ui//ShopPage.h"
#include "game/ui//SelectLevelPage.h"
#include "game/ui//dialogs/LevelDialog.h"
#include "game/ui//GamePage.h"
#include "game/ui//dialogs/FailDialog.h"
#include "game/ui//dialogs/ResultDialog.h"
#include "game/ui//dialogs/PauseDialog.h"
#include "game/ui//dialogs/GenerlDialog.h"
#include "game/ui/dialogs/ShopDialog.h"
#include "game/ui/dialogs/SetDialog.h"
#include "game/ui/dialogs/PvpDialog.h"
#include "game/ui/dialogs/RankingDialog.h"
#include "game/ui//system/GameScene.h"
#include "game/ui//system/Window.h"	
#include "game/achieve/AchievePanel.h"
#include "game/winner/WinnerDialog.h"
#include "game/ui/dialogs/ProDialog.h"
#include "game/ui/dialogs/SetNameDialog.h"
#include "game/ui/dialogs/PVPResult.h"
#include "game/ui/dialogs/PowerDialog.h"
#include "game/ui//system/ActionGain.h"



class PageFunction :public Singleton<PageFunction>, public MessageHandler
{
public:

	static Scene* mScen;

	PageFunction();
	~PageFunction();

	virtual void onReceiveMassage(GameMessage * message);

	virtual void gotoFunction(const ePageType massageType, const std::string messageName = "", const int mesType = 0, 
							bool isCacheRes = false);

	void showPage(Scene* pScene);
	 
	void setDiamondPanel(bool _visible, GameScene* nowScene = NULL);
	Node* getDiamondPanel();
	ActionGain* getActionPowerLayer();
	ActionGain* getActionDiamondLayer();
	ActionGain* getActionPackageLayer();
	ActionGain* getActionChestLayer();
	void enableOptionButton(bool visible);

	//����һ���Ѵ��ڵ�Window.
	Window* getWindowByPageName(std::string pageName);
	//����һ���Ѵ��ڵ�Window,��������ȥ����
	Window* createWindow(const ePageType massageType, const std::string messageName = "", const int luaType = 0,
		bool isCacheRes = false);

	void logout();

protected:
	bool checkMassage(const ePageType massageType);
	bool isSence(const ePageType massageType);
	void showWaitPage();

	Node* mDiamondPanel;

	Node* mNetworkTips;

	//��ȡ����������������
	ActionGain* mActionPowerLayer;
	ActionGain* mActionDiamondLayer;
	ActionGain* mActionPackageLayer;
	ActionGain* mActionChestLayer;

	//���ڵ�����ֻΪһ�����ð�ť�������������, �������Ż�ui�Ŀ�ܲŻ�������
	CC_SYNTHESIZE(int, mCurrentUIType, CurrentUIType);

	bool mIsLoadSceneRes;//�Ƿ��ڼ��س�����Դ��
	std::vector<std::string> mWaitShowPageVec;
	bool mIsRePlay;
	ePageType mCurrentLoadingPage;//��ǰ���ڼ��ص�Scene
};

#define sgPageFunction() PageFunction::Get()
