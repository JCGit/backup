#include "LevelControl.h"
#include "LevelObj.h"
#include "FightControl.h"

#include "WarMapCfgMaster.h"
#include "TextHelper.h"
#include "MonsterCfgMaster.h"
#include "TimeEx.h"

CLevelControl::CLevelControl()
:pEnterPanel(NULL)
,pCardDrawPanel(NULL)
,pResultPanel(NULL)
,pFailPanel(NULL)
,m_pEliteWarEntryPanel(NULL)
,m_pWarSweepInfoPanel(NULL)
{

}

CLevelControl::~CLevelControl(){
	CC_SAFE_DELETE(pEnterPanel);
	CC_SAFE_DELETE(pCardDrawPanel);
	CC_SAFE_DELETE(pResultPanel);
	CC_SAFE_DELETE(pFailPanel);

	CC_SAFE_RELEASE_NULL(m_pEliteWarEntryPanel);
	CC_SAFE_RELEASE_NULL(m_pWarSweepInfoPanel);
}

CLevelEnterPanel* CLevelControl::getEnterPanel(){
	if(pEnterPanel == NULL){
		pEnterPanel = new CLevelEnterPanel();
		pEnterPanel->init();
	}
	return pEnterPanel;
}

CCardDrawPanel* CLevelControl::getCardPanel(){
	if(pCardDrawPanel == NULL){
		pCardDrawPanel = new CCardDrawPanel();
		pCardDrawPanel->init();
	}
	return pCardDrawPanel;
}

CResultPanel* CLevelControl::getResultPanel(){
	if(pResultPanel == NULL){
		pResultPanel = new CResultPanel();
		pResultPanel->init();
	}
	return pResultPanel;
}

CFailPanel* CLevelControl::getFailPanel(){
	if(pFailPanel == NULL){
		pFailPanel = new CFailPanel();
		pFailPanel->init();
	}

	return pFailPanel;
}

FriendHelpPanel* CLevelControl::getFriendHelpPanel(){
    if(!m_stFriendHelpPanel.isInited()) {
        m_stFriendHelpPanel.init();
    }
    return &m_stFriendHelpPanel;
}

CPausePanel* CLevelControl::getPausePanel(){
	if(!m_stPausePanel.isInited()){
		m_stPausePanel.init();
	}

	return &m_stPausePanel;
}

CSweepInfoPanel* CLevelControl::getSweepInfoPanel(){
	if(!m_stSweepInfoPanel.isInited()){
		m_stSweepInfoPanel.init();
	}

	return &m_stSweepInfoPanel;
}

EliteWarEntryPanel* CLevelControl::getEliteWarEntryPanel() {
	if(m_pEliteWarEntryPanel == NULL) {
		m_pEliteWarEntryPanel = EliteWarEntryPanel::create();
		CC_SAFE_RETAIN(m_pEliteWarEntryPanel);
	}
	
	return m_pEliteWarEntryPanel;
}

WarSweepInfoPanel* CLevelControl::getWarSweepInfoPanel() {
	if(m_pWarSweepInfoPanel == NULL) {
		m_pWarSweepInfoPanel = WarSweepInfoPanel::create();
		CC_SAFE_RETAIN(m_pWarSweepInfoPanel);
	}

	return m_pWarSweepInfoPanel;
}

CRelivePanel* CLevelControl::getRelivePanel() {
	if(!m_stReilivePanel.isInited()){
		m_stReilivePanel.init();
	}

	return &m_stReilivePanel;
}

CFillBulletPanel* CLevelControl::getFillBulletPanel() {
	if(!m_stFillBulletPanel.isInited()){
		m_stFillBulletPanel.init();
	}

	return &m_stFillBulletPanel;
}

CBuyMedicalPanel* CLevelControl::getBuyMedicalPanel(){
	if(!m_stBuyMedicalPanel.isInited()){
		m_stBuyMedicalPanel.init();
	}

	return &m_stBuyMedicalPanel;
}

void CLevelControl::openBossEmergePanelByCfgID(unsigned int cfgID, CCObject* target, SEL_CallFunc callbackFun){
	if(!m_stBossEmergePanel.isInited()){
		m_stBossEmergePanel.init();
	}

	m_stBossEmergePanel.setCfgID(cfgID);
	m_stBossEmergePanel.openWithCallFun(target, callbackFun);
}

void CLevelControl::openSweepPanelByLevelID(unsigned int levelID){
	if(!m_stSweepPanel.isInited()){
		m_stSweepPanel.init();
	}

	m_stSweepPanel.setLevelID(levelID);
	m_stSweepPanel.open();
}

void CLevelControl::doQueryFriendLstRsp(){
    if(m_stFriendHelpPanel.isInited() && m_stFriendHelpPanel.isOpen()) {
        m_stFriendHelpPanel.updateUI();
    }
}

void CLevelControl::doInviteFriendFightRsp(const vmsg::CSPlayerDetailInfo& stMsg){
	P_LEVEL_OBJ->setFriendDetailInfo(stMsg);
	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
}


void CLevelControl::releasePanel() {
    CCLOG("[trace]CLevelControl::releasePanel");
    CC_SAFE_RELEASE_NULL(pResultPanel);
    CC_SAFE_RELEASE_NULL(pFailPanel);
    CC_SAFE_RELEASE_NULL(pCardDrawPanel);
}

const std::string CLevelControl::getWinTypeStr(const int iLevel) {
    WarMapCfg* warCfg = WAR_MAP_CFG->GetWarMapCfgByID(iLevel);
    if(NULL == warCfg) {
        return TextStr(TEXT_NORMAL_WAR_PASS_TIP); //默认
    }
    std::string strTip;
    if( 1 == warCfg->wintype() ) {
        strTip = TextStr(TEXT_NORMAL_WAR_PASS_TIP); //默认
    } else if( 2 == warCfg->wintype() ) {
        //消灭BOSS
        const MonsterCfg* pBoss = MONSTER_CFG_MASTER->GetMonsterCfgByID(warCfg->winvalue1());
        if(pBoss) {
            char str[50];
            snprintf(str, 50, TextStr(TEXT_BOSS_WAR_PASS_TIP), pBoss->monstername().c_str());
            strTip = (str);
        }
    } else if( 3 == warCfg->wintype() ) {
        //消灭XX个敌人
        char str[50];
        snprintf(str, 50, TextStr(TEXT_WAR_PASS_TIP_KILL_NUM), warCfg->winvalue1() );
        strTip = (str);
    } else if( 4 == warCfg->wintype() ) {
        //消灭XX个【XX】
        const MonsterCfg* pBoss = MONSTER_CFG_MASTER->GetMonsterCfgByID(warCfg->winvalue1());
        if(pBoss) {
            char str[50];
            snprintf(str, 50, TextStr(TEXT_WAR_PASS_TIP_KILL_NUM_SPEC), warCfg->winvalue2(), pBoss->monstername().c_str());
            strTip = (str);
        }
    } else if( 5 == warCfg->wintype() ) {
        //坚持战斗XXX
        char str[50];
        snprintf(str, 50, TextStr(TEXT_WAR_PASS_TIP_TIME), TimeEx::getTimeStr(warCfg->winvalue1(), "ms").c_str() );
        strTip = (str);
    } 
    if(strTip.size() <= 0) {
        strTip = TextStr(TEXT_NORMAL_WAR_PASS_TIP); //默认
    }
    return strTip;
}

