#include "WarMapControl.h"

#include "Macros.pb.h"

#include "CfgMaster/WarMapCfgMaster.h"
#include "CfgMaster/MonsterCfgMaster.h"

#include "basic/TextHelper.h"
#include "basic/TimeEx.h"

#include "game/UserObj.h"
#include "game/fight/FightControl.h"
#include "game/fight/LevelObj.h"

WarMapControl::WarMapControl()
:m_bInitialized(false)
,m_pWarMapPanel(NULL)
,m_pBeStrongerPanel(NULL)
,m_pLevelUpEntryPanel(NULL)
,m_pAwardBoxTipPanel(NULL)
,m_pStarAwardPanel(NULL)
,m_bEnterWarManualNotEnough(false)
,m_uWarMapID(0)
,m_uEliteWarMapID(0)
//,m_pWarEntryPanel(NULL)
//,m_pEliteWarEntryPanel(NULL)
//,m_pWarSweepPanel(NULL)
//,m_pEliteWarSweepPanel(NULL)
{
}

WarMapControl::~WarMapControl()
{
}

void WarMapControl::init() {
	if(!m_bInitialized) {
		m_pWarMapPanel = WarMapPanel::create();
		CC_SAFE_RETAIN(m_pWarMapPanel);

		m_pBeStrongerPanel = BeStrongerPanel::create();
		CC_SAFE_RETAIN(m_pBeStrongerPanel);

		m_pLevelUpEntryPanel = LevelUpEntryPanel::create();
		CC_SAFE_RETAIN(m_pLevelUpEntryPanel);

		m_pAwardBoxTipPanel = AwardBoxTipPanel::create();
		CC_SAFE_RETAIN(m_pAwardBoxTipPanel);

		m_pStarAwardPanel = StarAwardPanel::create();
		CC_SAFE_RETAIN(m_pStarAwardPanel);

		/*m_pWarEntryPanel = WarEntryPanel::create();
		CC_SAFE_RETAIN(m_pWarEntryPanel);*/

		/*m_pEliteWarEntryPanel = EliteWarEntryPanel::create();
		CC_SAFE_RETAIN(m_pEliteWarEntryPanel);*/

		/*m_pWarSweepPanel = WarSweepPanel::create();
		CC_SAFE_RETAIN(m_pWarSweepPanel);*/

		/*m_pEliteWarSweepPanel = EliteWarSweepPanel::create();
		CC_SAFE_RETAIN(m_pEliteWarSweepPanel);*/

		m_bInitialized = true;
	}
}

void WarMapControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pWarMapPanel);
		CC_SAFE_RELEASE_NULL(m_pBeStrongerPanel);
		CC_SAFE_RELEASE_NULL(m_pLevelUpEntryPanel);
		CC_SAFE_RELEASE_NULL(m_pAwardBoxTipPanel);
		CC_SAFE_RELEASE_NULL(m_pStarAwardPanel);
		//CC_SAFE_RELEASE_NULL(m_pWarEntryPanel);
		//CC_SAFE_RELEASE_NULL(m_pEliteWarEntryPanel);
		//CC_SAFE_RELEASE_NULL(m_pWarSweepPanel);
		//CC_SAFE_RELEASE_NULL(m_pEliteWarSweepPanel);

		m_bInitialized = false;
	}
}

WarMapPanel* WarMapControl::getWarMapPanel() {
	return m_pWarMapPanel;
}

BeStrongerPanel* WarMapControl::getBeStrongerPanel() {
	return m_pBeStrongerPanel;
}

LevelUpEntryPanel* WarMapControl::getLevelUpEntryPanel() {
	return m_pLevelUpEntryPanel;
}

AwardBoxTipPanel* WarMapControl::getAwardBoxTipPanel() {
	return m_pAwardBoxTipPanel;
}

StarAwardPanel* WarMapControl::getStarAwardPanel()
{
	return m_pStarAwardPanel;
}

string WarMapControl::getWinCondText(unsigned int uWinType, unsigned int uValue1, unsigned int uValue2) {
	string text = "";

	if(uWinType == WIN_TYPE_CLEAN_ALL) {
		text = TextStr(TEXT_NORMAL_WAR_PASS_TIP);
	} else if(uWinType == WIN_TYPE_KILL_ONE) {
		//消灭BOSS
		const MonsterCfg* pMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(uValue1);

		if(pMonsterCfg != NULL) {
			char str[50];
			snprintf(str, 50, TextStr(TEXT_BOSS_WAR_PASS_TIP), pMonsterCfg->monstername().c_str());
			text = str;
		}
	} else if(uWinType == WIN_TYPE_KILL_NUM) {
		//消灭XX个敌人
		char str[50];
		snprintf(str, 50, TextStr(TEXT_WAR_PASS_TIP_KILL_NUM), uValue1);
		text = str;
	} else if(uWinType == WIN_TYPE_KILL_ONE_NUM) {
		//消灭XX个【XX】
		const MonsterCfg* pMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(uValue1);

		if(pMonsterCfg != NULL) {
			char str[50];
			snprintf(str, 50, TextStr(TEXT_WAR_PASS_TIP_KILL_NUM_SPEC), uValue2, pMonsterCfg->monstername().c_str());
			text = str;
		}
	} else if(uWinType == WIN_TYPE_LAST_TIME) {
		//坚持战斗XXX
		char str[50];
		snprintf(str, 50, TextStr(TEXT_WAR_PASS_TIP_TIME), TimeEx::getTimeStr(uValue1, "ms").c_str() );
		text = str;
	} else {
		text = TextStr(TEXT_NORMAL_WAR_PASS_TIP);
	}

	return text;
}

bool WarMapControl::isEliteWarOpen() {
	const vector<WarMapCfg*>* pWarChapter1 = WAR_MAP_CFG->getWarChapter(1);
	unsigned int uMaxWarMapID = P_USER_OBJ->getUserInfo()->newestWarMapID; // 这个才是实时的
	
	return pWarChapter1 != NULL && uMaxWarMapID >= pWarChapter1->size();
}

void WarMapControl::gotoEliteWar() {
	if(m_pWarMapPanel != NULL && !m_pWarMapPanel->isOpen()) {
		m_pWarMapPanel->open();
		m_pWarMapPanel->switchTab(WarMapPanel::WAR_TAB_ELITE);
	}
}

unsigned int WarMapControl::getMaxEliteWarChapterID() {
	unsigned int uChapterID = 0;
	unsigned int uMaxWarMapID = P_USER_OBJ->getUserInfo()->newestWarMapID; // 这个才是实时的
	const WarMapCfg *pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(uMaxWarMapID+1);//精英副本的处理
	
	if(pWarMapCfg != NULL) {
		const vector<WarMapCfg*>* pWarChapter = WAR_MAP_CFG->getWarChapter(pWarMapCfg->warid());
		uChapterID = uMaxWarMapID == pWarChapter->size() ? pWarMapCfg->warid() : pWarMapCfg->warid() - 1;
	}else if (uMaxWarMapID == WAR_MAP_CFG->GetNormalSize())//最后一章节时
	{
		const WarMapCfg *pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(uMaxWarMapID);
		uChapterID = pWarMapCfg->warid();
	}
	return uChapterID;
}

bool WarMapControl::isEnterWarManualNotEnough() {
	return m_bEnterWarManualNotEnough;
}

void WarMapControl::setEnterWarManualNotEnough(bool bManualNotEnough) {
	m_bEnterWarManualNotEnough = bManualNotEnough;
}

unsigned int WarMapControl::getWarMapID() {
	return m_uWarMapID;
}

void WarMapControl::setWarMapID(unsigned int uWarMapID) {
	m_uWarMapID = uWarMapID;
}

unsigned int WarMapControl::getEliteWarMapID() {
	return m_uEliteWarMapID;
}

void WarMapControl::setEliteWarMapID(unsigned int uEliteWarMapID) {
	m_uEliteWarMapID = uEliteWarMapID;
}

//WarEntryPanel* WarMapControl::getWarEntryPanel() {
//	return m_pWarEntryPanel;
//}

//EliteWarEntryPanel* WarMapControl::getEliteWarEntryPanel() {
//	return m_pEliteWarEntryPanel;
//}

//WarSweepPanel* WarMapControl::getWarSweepPanel() {
//	return m_pWarSweepPanel;
//}

//EliteWarSweepPanel* WarMapControl::getEliteWarSweepPanel() {
//	return m_pEliteWarSweepPanel;
//}

void WarMapControl::sendQueryWarMapInfoRqst() {

	unsigned int warMapID = P_USER_OBJ->getUserInfo()->newestWarMapID;

    //查询关卡1到最新通关关卡的下一关
	const WarMapCfg* pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(warMapID + 1);

    if(pWarMapCfg == NULL) {
		pWarMapCfg = WAR_MAP_CFG->GetWarMapCfgByID(warMapID); // 全部通关

		if (!pWarMapCfg)
		{
			CCLOG("[error]WarMapControl::sendQueryWarMapInfoRqst,fail to get WarMap:%d info.Please Match cfgs in client and server.", warMapID);
			return;
		}
    }
    unsigned int uChapterID = pWarMapCfg->warid();

    pair<unsigned int, unsigned int> m_stIDX = WAR_MAP_CFG->GetIDXByWarID(uChapterID);
    P_FIGHT_CTRL->getFightProxy()->sendQueryWarMapInfoRqst(1, m_stIDX.second);
}

void WarMapControl::sendGetStarAwardRqst(unsigned int uWarID, unsigned int uStarCnt) {
	P_FIGHT_CTRL->getFightProxy()->sendGetStarAwardRqst(uWarID, uStarCnt);
}
