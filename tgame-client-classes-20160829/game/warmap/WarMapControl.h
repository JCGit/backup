#ifndef WarMapControl_h__
#define WarMapControl_h__

#include "vlib/base/CVSingleton.h"

#include "WarMapPanel.h"
//#include "WarEntryPanel.h"
//#include "EliteWarEntryPanel.h"
//#include "WarSweepPanel.h"
//#include "EliteWarSweepPanel.h"
#include "BeStrongerPanel.h"
#include "LevelUpEntryPanel.h"
#include "AwardBoxTipPanel.h"
#include "StarAwardPanel.h"

class WarMapControl
{
public:
	WarMapControl();
	~WarMapControl();

	void init();

	void finalize();

	WarMapPanel* getWarMapPanel();

	BeStrongerPanel* getBeStrongerPanel();

	LevelUpEntryPanel* getLevelUpEntryPanel();

	AwardBoxTipPanel* getAwardBoxTipPanel();

	StarAwardPanel* getStarAwardPanel();

	string getWinCondText(unsigned int uWinType, unsigned int uValue1, unsigned int uValue2);

	bool isEliteWarOpen(); // 精英副本是否已开放

	void gotoEliteWar(); // 前往精英副本

	unsigned int getMaxEliteWarChapterID(); // 已开放最大精英章节

	bool isEnterWarManualNotEnough(); // 关卡体力不足

	void setEnterWarManualNotEnough(bool bManualNotEnough);

	unsigned int getWarMapID();

	void setWarMapID(unsigned int uWarMapID);

	unsigned int getEliteWarMapID();

	void setEliteWarMapID(unsigned int uEliteWarMapID);

	//WarEntryPanel* getWarEntryPanel();

	//EliteWarEntryPanel* getEliteWarEntryPanel();

	//WarSweepPanel* getWarSweepPanel();

	//EliteWarSweepPanel* getEliteWarSweepPanel();

    void sendQueryWarMapInfoRqst(); //查询当前的关卡信息

	void sendGetStarAwardRqst(unsigned int uWarID, unsigned int uStarCnt); // 领取星星宝箱

private:
	bool m_bInitialized;

	WarMapPanel *m_pWarMapPanel;

	//WarEntryPanel *m_pWarEntryPanel;

	//EliteWarEntryPanel *m_pEliteWarEntryPanel;

	//WarSweepPanel *m_pWarSweepPanel;

	//EliteWarSweepPanel *m_pEliteWarSweepPanel;

	BeStrongerPanel *m_pBeStrongerPanel;

	LevelUpEntryPanel *m_pLevelUpEntryPanel;

	AwardBoxTipPanel *m_pAwardBoxTipPanel;

	StarAwardPanel* m_pStarAwardPanel;

	bool m_bEnterWarManualNotEnough;

	unsigned int m_uWarMapID; // 关卡ID, 战斗结束返回关卡界面用到的记录

	unsigned int m_uEliteWarMapID; // 精英关卡ID, 战斗结束返回关卡界面用到的记录

};

#define P_WAR_MAP_CTRL CVLib::CVSingleton<WarMapControl>::Instance()

#endif // WarMapControl_h__