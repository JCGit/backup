#include "DailyInstanceControl.h"
#include "ToolUtils.h"
#include "DailyInstanceCfgMaster.h"
#include "PopTipLayer.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"

DailyInstanceControl::DailyInstanceControl()
{
    m_pDailyInstancePanel = NULL;
}

DailyInstanceControl::~DailyInstanceControl()
{
    finalizePanel();
}

void DailyInstanceControl::finalizePanel() {
    CC_SAFE_RELEASE_NULL(m_pDailyInstancePanel);
    CToolUtils::removePlistTexFile("ui_v2/dailyinstance.plist");
    CToolUtils::removeArmatureFile("effect/ut00090/ut00090.ExportJson");
    char buff[100] = {0};
    for(int i = 0; i < DAILY_INST_CFG_MASTER->GetCfg().instancecfg_size(); ++i) {
        snprintf(buff, 100, "ui_bg/dailyinstancebg/%s.plist", DAILY_INST_CFG_MASTER->GetCfg().instancecfg(i).bgname().c_str() );
        CToolUtils::removePlistTexFile(buff);
    }
    CCTextureCache::sharedTextureCache()->removeTextureForKey("ui_bg/dailyinstancebg/mask_lv0.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("ui_bg/dailyinstancebg/mask_lv1.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("ui_bg/dailyinstancebg/mask_lv2.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("ui_bg/dailyinstancebg/mask_lv3.png");
}

DailyInstancePanel* DailyInstanceControl::getDailyInstancePanel() {
	if(m_pDailyInstancePanel == NULL) {
		m_pDailyInstancePanel = DailyInstancePanel::create();
		m_pDailyInstancePanel->retain(); 
	}

	return m_pDailyInstancePanel;
}


void DailyInstanceControl::updateQueryRsp(const CSQueryDailyInstanceRsp& stRsp) {
    m_stInfo.clear();
    for (int i = 0; i < stRsp.instances_size(); ++i)
    {
        const CSDailyInstanceElmt& stElmt = stRsp.instances(i);
        m_stInfo[stElmt.key()] = stElmt.challengecnt();
    }
    if(m_pDailyInstancePanel && m_pDailyInstancePanel->isOpen()) {
        m_pDailyInstancePanel->updateUI();
    }
}

void DailyInstanceControl::updatePassRsp(const CSDailyInstancePassRsp& stRsp) {
    m_stInfo.clear();
    for (int i = 0; i < stRsp.instances_size(); ++i)
    {
        const CSDailyInstanceElmt& stElmt = stRsp.instances(i);
        m_stInfo[stElmt.key()] = stElmt.challengecnt();
    }
    if(m_pDailyInstancePanel && m_pDailyInstancePanel->isOpen()) {
        m_pDailyInstancePanel->updateUI();
    }
    //show award
    for(int i = 0; i < stRsp.awardlst_size(); ++i) {
        const CSAwardElement& elmt = stRsp.awardlst(i);
        if(vmsg::THING_TYPE_COIN == elmt.elementtype()) {
            continue; //铜钱会从通知里单独飘字
        }
        POP_TIP_LAYER_MASTER->ShowAwardTip(elmt.elementtype(), elmt.elementid(), elmt.elementcnt());
    }

	P_FIGHT_OBJ->setDailyEndInfo(stRsp);
	P_FIGHT_CTRL->doDailyFightEnd();
}

int DailyInstanceControl::getRemainChallengeCnt(const int iCfgKey) {
    if(m_stInfo.count(iCfgKey)) {
        return m_stInfo[iCfgKey];
    }
    return 0;
}



void DailyInstanceControl::setNowFightInfo(const int iSelCfgKey, const int iSelLvIdx) {
    m_iSelCfgKey = iSelCfgKey;
    m_iSelLvIdx = iSelLvIdx;
    const DailyInstanceCfg* pCfg = DAILY_INST_CFG_MASTER->getOneCfg(m_iSelCfgKey);
    if(NULL == pCfg) {
        return;
    }
    if(m_iSelLvIdx >= 0 && m_iSelLvIdx < pCfg->todayinstances_size()) {
        m_iWarMapID = pCfg->todayinstances(m_iSelLvIdx).instanceid();
    } else {
        m_iWarMapID = 0;
    }
}
int DailyInstanceControl::getWarMapID() {
    return m_iWarMapID;
}

void DailyInstanceControl::doWarWin() {
    getProxy()->sendPassRqst(m_iSelCfgKey, m_iSelLvIdx);
}

