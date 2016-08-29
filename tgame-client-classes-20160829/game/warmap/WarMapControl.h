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

	bool isEliteWarOpen(); // ��Ӣ�����Ƿ��ѿ���

	void gotoEliteWar(); // ǰ����Ӣ����

	unsigned int getMaxEliteWarChapterID(); // �ѿ������Ӣ�½�

	bool isEnterWarManualNotEnough(); // �ؿ���������

	void setEnterWarManualNotEnough(bool bManualNotEnough);

	unsigned int getWarMapID();

	void setWarMapID(unsigned int uWarMapID);

	unsigned int getEliteWarMapID();

	void setEliteWarMapID(unsigned int uEliteWarMapID);

	//WarEntryPanel* getWarEntryPanel();

	//EliteWarEntryPanel* getEliteWarEntryPanel();

	//WarSweepPanel* getWarSweepPanel();

	//EliteWarSweepPanel* getEliteWarSweepPanel();

    void sendQueryWarMapInfoRqst(); //��ѯ��ǰ�Ĺؿ���Ϣ

	void sendGetStarAwardRqst(unsigned int uWarID, unsigned int uStarCnt); // ��ȡ���Ǳ���

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

	unsigned int m_uWarMapID; // �ؿ�ID, ս���������عؿ������õ��ļ�¼

	unsigned int m_uEliteWarMapID; // ��Ӣ�ؿ�ID, ս���������عؿ������õ��ļ�¼

};

#define P_WAR_MAP_CTRL CVLib::CVSingleton<WarMapControl>::Instance()

#endif // WarMapControl_h__