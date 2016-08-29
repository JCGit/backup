#ifndef CityControl_h__
#define CityControl_h__

#include "vlib/base/CVSingleton.h"

#include "CityProxy.h"
#include "CityScene.h"
#include "CityScreenView.h"
#include "CityObj.h"
#include "SubEntryPanel.h"
#include "LevelUpPanel.h"
#include "GetAwardPanel.h"
#include "AwardPromptPanel.h"

class CityControl
{
public:
	CityControl();
	~CityControl();

	void init();

	const unsigned int getArriveNPCID();

	void setArriveNPCID(const unsigned int uArriveNPCID);

	CityProxy* getProxy();

	CityScene* getCityScene();

	CityScreenView* getCityScreenView();

	SubEntryPanel* getSubEntryPanel();

	LevelUpPanel* getLevelUpPanel();

	GetAwardPanel* getGetAwardPanel();

	AwardPromptPanel* getAwardPromptPanel();

	void updateExp();

	void updateFightPower();

	void updateGold();

	void updateCoin();

	void updateManual();

	void updateTask();

	void updateMail();

	void updateOnlineAndMorrow();

	unsigned int getTaskNPCID();

	void setTaskNPCID(unsigned int uTaskNPCID);

	bool isHeroMoving();

	bool isNPCInScene(unsigned int uSceneID, unsigned int uNPCID);

	bool isNPCInCurScene(unsigned int uNPCID);

	void moveToNPC(unsigned int uNPCID);

	void updateNPCTaskState(unsigned int uTaskID);

	void moveToBattlePortal();

	void showPlotDialog();

	bool checkModuleOpen(unsigned int uTaskID);
	unsigned int getModuleIDByTaskID(unsigned int uTaskID);

	bool isModuleOpen(unsigned int uModuleID);

	bool isSceneOpen(unsigned int uSceneID);

	bool isInCity(); // �Ƿ���ͬ��������

    void showLvUpEffect(); //����������Ч

    void updateWeaponAvatar(); //ǹе���׺����

    void updateMoneyTopBar(); //Ǯ�仯�������ʾ

	vector<unsigned int>& getOpenModules();

	unsigned int getDelayedEntryID(); // ս��ʧ�ܴ򿪵����

	void setDelayedEntryID(unsigned int uEntryID);

private:
	CityObj m_stCityObj;

	CityProxy *m_pCityProxy;

	CityScene *m_pCityScene;

	CityScreenView *m_pCityScreenView;

	SubEntryPanel *m_pSubEntryPanel;

	LevelUpPanel *m_pLevelUpPanel;

	GetAwardPanel *m_pGetAwardPanel;

	AwardPromptPanel *m_pAwardPromptPanel;

	unsigned int m_uTaskNPCID; // �糡�������npcid

	unsigned int m_uMaxCommitedTaskID; // ���ύ��������taskid

	vector<unsigned int> m_stOpenModules; // <ModuleID>

	unsigned int m_uDelayedEntryID;

};

#define P_CITY_CTRL CVLib::CVSingleton<CityControl>::Instance()

#endif // CityControl_h__
