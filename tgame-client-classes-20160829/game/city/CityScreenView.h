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

	void flyArrived(); // ���ܿ�����Ч�ɵ�Ŀ�ĵ�

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

	int m_nRemainTime; // ��һ�λظ�����ʣ��ʱ��

	BottomEntryLayout *m_pBottomEntryLayout;

	TopEntryLayout *m_pTopEntryLayout;

	HeadEntryLayout *m_pHeadEntryLayout;

	UIButton *m_pFuncBtn; // ���ܿ����õ���ʱ��ť

	UILabel *m_pFuncWordsTxt; // ���ܿ��ŵ�����

	UILayout *m_pBgMask; // ���ܿ����õ�����

	CCArmature *m_pBtnEffArmature; // ���ܿ����õ���Ч

	unsigned int m_uNowModuleID; // �ոտ��ŵ�ModuleID

	CCArmature *m_pTaskArmature;

};

#endif // CityScreenView_h__