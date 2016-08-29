#ifndef CityScreenView_h__
#define CityScreenView_h__

#include "basic/BaseLayout.h"

#include "guide/GuideControl.h"

class ImageNumber;
class BottomEntryLayout;
class TopEntryLayout;
class HeadEntryLayout;

class CityScreenView: public BaseLayout, public GuideContainer
{
public:
	CityScreenView();
	virtual ~CityScreenView();

	static CityScreenView* create();

	// override start

	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	// override end

	void updateUI();

	void updateExp();

	void updateFightPower();

	void updateGold();

	void updateCoin();

	void updateManual();

	void updateTask();

	void updateEntry();

	void mailNotify();

	void manualTimer(float dt);

	void moduleOpen(unsigned int uModuleID);

	void foldEntry();

	void unfoldEntry();

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum);

	void bringAssetsToTop();

	void bringAssetsToBottom();

	void updateOnlineAndMorrow();
private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void buyManual();

	void onBtnEffPlayEvent(CCArmature* pArmature, MovementEventType type, const char *pszActionName);

	void onBtnEffFrameEvent(CCBone *pBone, const char *pszFrameName, int a, int b);

	void flyArrived(); // 功能开放特效飞到目的点

	string getModuleText(unsigned int uModuleID);

private:
	ImageNumber *m_pLevelTxt;

	ImageNumber *m_pVIPTxt;

	ImageNumber *m_pFightPowerTxt;

	UILayout *m_pTaskTraceLayout;

	UILabel *m_pTaskNameTxt;

	UILabel *m_pTaskDescTxt;

	UILoadingBar *m_pExpProgress;

	UILabel *m_pExpProgressTxt;

	UILayout *m_pAssetsLayout;

	UILabel *m_pCoinValTxt;

	UIButton *m_pCoinAddBtn;

	UILabel *m_pGoldValTxt;

	UIButton *m_pGoldAddBtn;

	UILabel *m_pManualTimerTxt;

	UILabel *m_pManualValTxt;

	UIButton *m_pManualAddBtn;

	int m_nRemainTime; // 下一次回复体力剩余时间

	BottomEntryLayout *m_pBottomEntryLayout;

	TopEntryLayout *m_pTopEntryLayout;

	HeadEntryLayout *m_pHeadEntryLayout;

	UIButton *m_pFuncBtn; // 功能开放用的临时按钮

	UILabel *m_pFuncWordsTxt; // 功能开放的描述

	UILayout *m_pBgMask; // 功能开放用的遮罩

	CCArmature *m_pBtnEffArmature; // 功能开放用的特效

	unsigned int m_uNowModuleID; // 刚刚开放的ModuleID

	CCArmature *m_pTaskArmature;

};

#endif // CityScreenView_h__