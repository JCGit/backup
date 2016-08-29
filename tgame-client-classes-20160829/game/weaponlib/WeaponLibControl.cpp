#include "WeaponLibControl.h"
#include "firearms/FireArmsObj.h"
#include "bag/BagObj.h"
#include "UserObj.h"
#include "bag/BagControl.h"
#include "firearms/FireArmsProxy.h"
#include "city/CityControl.h"
#include "fight/FightObj.h"
#include "game/notification/NotificationControl.h"

#include "CfgMaster/WeaponCfgMaster.h"
#include "CfgMaster/WeaponStarStepCfgMaster.h"
#include "CfgMaster/WeaponExpItemCfgMaster.h"

CWeaponLibControl::CWeaponLibControl()
{
}

CWeaponLibControl::~CWeaponLibControl(){

}


CWeaponLibPanelNew* CWeaponLibControl::getWeaponLibPanel(){
    if(!(m_stWeaponLibPanel.isInited())) {
        m_stWeaponLibPanel.init();
    }
    return &m_stWeaponLibPanel;
}


WeaponInfoPanel* CWeaponLibControl::getWeaponInfoPanel(){
    if(!(m_stWeaponInfoPanel.isInited())) {
        m_stWeaponInfoPanel.init();
    }
    return &m_stWeaponInfoPanel;
}

WeaponSelMatPanel* CWeaponLibControl::getSelMatPanel(){
    if(!(m_stSelMatPanel.isInited())) {
        m_stSelMatPanel.init();
    }
    return &m_stSelMatPanel;
}

WeaponAdvDonePanel* CWeaponLibControl::getWeaponAdvDonePanel(){
    if(!(m_stAdvDonePanel.isInited())) {
        m_stAdvDonePanel.init();
    }
    return &m_stAdvDonePanel;
}

WeaponSelPanel* CWeaponLibControl::getWeaponSelPanel(){
    if(!(m_stWeaponSelPanel.isInited())) {
        m_stWeaponSelPanel.init();
    }
    return &m_stWeaponSelPanel;
}

CWeaponArrayPanel* CWeaponLibControl::getArrayPanel(){
	if(!(m_stArrayPanel.isInited())) {
		m_stArrayPanel.init();
	}
	return &m_stArrayPanel;
}

CWeaponArrRulePanel* CWeaponLibControl::getArrRulePanel(){
	if(!(m_stArrRulePanel.isInited())) {
		m_stArrRulePanel.init();
	}
	return &m_stArrRulePanel;
}

CWeaponArrOpenPanel* CWeaponLibControl::getArrOpenPanel(){
	if(!(m_stArrOpenPanel.isInited())) {
		m_stArrOpenPanel.init();
	}
	return &m_stArrOpenPanel;
}

CWeaponArrInfoPanel* CWeaponLibControl::getArrInfoPanel(){
	if(!(m_stArrInfoPanel.isInited())) {
		m_stArrInfoPanel.init();
	}
	return &m_stArrInfoPanel;
}

bool CWeaponLibControl::isWeaponCanAdvance(const vmsg::CSWeapon &stWeapon) {
	bool bRet = false;

	const WeaponCfg* pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
	const WeaponStarStepCfg* pWeaponAdvanceCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pWeaponCfg->initstar(), pWeaponCfg->initstep());

	if(pWeaponAdvanceCfg == NULL
	|| (pWeaponAdvanceCfg->star() == pWeaponAdvanceCfg->nextstar() && pWeaponAdvanceCfg->step() == pWeaponAdvanceCfg->nextstep())) {
		bRet = false;
	} else {
		if(stWeapon.level() >= pWeaponAdvanceCfg->needlevel()
		&& P_USER_OBJ->checkCoin(pWeaponAdvanceCfg->costcoin()) == 0
		&& P_ARM_OBJ->checkCanStarWeapon(stWeapon.cfgid(), stWeapon.weaponguid())) {
			bRet = true;
		}
	}

	return bRet;
}

void CWeaponLibControl::notifyWeaponLvUp() {
	const NotificationCfg *pNotificationCfg = NOTIFICATION_CFG_MASTER->getNotificationCfg(ENTRY_WEAPON_CULTURE, NOTIFY_TAG_WEAPON_LVUP);
	
	if(pNotificationCfg != NULL) {
		unsigned int uTotalCnt = 0;
		unsigned int uThresholdCnt = pNotificationCfg->wordsparams(0);
		const WeaponExpItemCfgSet &stCfgSet = WEAPON_EXP_CFG->GetCfg();

		for(int i = 0; i < stCfgSet.weaponexpitemcfg_size(); i++) {
			const WeaponExpItemCfg &stWeaponExpItemCfg = stCfgSet.weaponexpitemcfg(i);
			unsigned int uItemCfgID = stWeaponExpItemCfg.itemcfgid();

			uTotalCnt += P_BAG_CTRL->getItemCnt(uItemCfgID);

			if(uTotalCnt >= uThresholdCnt) {
				break;
			}
		}

		if(uTotalCnt >= uThresholdCnt) {
			PUSH_NOTIFICATION(ENTRY_WEAPON_CULTURE, NOTIFY_TAG_WEAPON_LVUP, 1);
		} else {
			PUSH_NOTIFICATION(ENTRY_WEAPON_CULTURE, NOTIFY_TAG_WEAPON_LVUP, 0);
		}
	}
}

void CWeaponLibControl::notifyWeaponAdvance() {
	const vector<vmsg::CSWeapon> &stWeapons = P_ARM_OBJ->getAllWeaponBag();
	bool hasWeaponCanAdvance = false;

	for(unsigned int i = 0; i < stWeapons.size(); i++) {
		if(isWeaponCanAdvance(stWeapons[i])) {
			hasWeaponCanAdvance = true;
			break;
		}
	}

	if(hasWeaponCanAdvance) {
		PUSH_NOTIFICATION(ENTRY_WEAPON_CULTURE, NOTIFY_TAG_WEAPON_ADVANCE, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_WEAPON_CULTURE, NOTIFY_TAG_WEAPON_ADVANCE, 0);
	}
}

void CWeaponLibControl::sendWeaponBagRqst(){
    m_stProxy.sendWeaponBagRqst();
}

void CWeaponLibControl::sendWeaponChangeRqst(unsigned int bagIdx, UNIT64_t weaponGUID){
    P_ARM_OBJ->setPreWeaponGUID(weaponGUID);
    m_stProxy.sendWeaponChangeRqst(bagIdx, weaponGUID);
}

void CWeaponLibControl::sendWeaponLvUpRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<vmsg::CSWeaponLvUpConsume> stConsume){
    m_stProxy.sendWeaponLvUpRqst(bagIdx, weaponGUID, stConsume);
}

void CWeaponLibControl::sendWeaponStarStepRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<UNIT64_t> stNeedWeaponGUIDs){
    m_stProxy.sendWeaponStarStepRqst(bagIdx, weaponGUID, stNeedWeaponGUIDs);
}

void CWeaponLibControl::sendWeaponRefitRqst(unsigned int bagIdx, UNIT64_t weaponGUID){
    m_stProxy.sendWeaponRefitRqst(bagIdx, weaponGUID);
}

void CWeaponLibControl::sendQueryWeaponInBattleRqst(){
	m_stProxy.sendQueryWeaponInBattleRqst();
}

void CWeaponLibControl::sendChangeWeaponInBattleRqst(const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon, const vmsg::CSInbattleWeaponElmtBrief& stNewWeapon){
	m_stProxy.sendChangeWeaponInBattleRqst(stOldWeapon, stNewWeapon);
}

void CWeaponLibControl::doQueryWeaponBagRsp(const vmsg::CSWeaponBag* info){
    CCLOG("[trace]CWeaponLibControl::doQueryWeaponBagRsp, update bag info.");
    P_ARM_OBJ->setWeaponBag(*info);
    //neilwu mod
    if(m_stWeaponLibPanel.isInited() && m_stWeaponLibPanel.isOpen()) {
        m_stWeaponLibPanel.updateUI();
        CCLOG("[trace]CWeaponLibControl::doQueryWeaponBagRsp, m_stWeaponLibPanel.updateUI.");
    }
    if(m_stSelMatPanel.isInited() && m_stSelMatPanel.isOpen()) {
        m_stSelMatPanel.updateUI();
        CCLOG("[trace]CWeaponLibControl::doQueryWeaponBagRsp, m_stSelMatPanel.updateUI.");
    }
    if(m_stWeaponSelPanel.isInited() && m_stWeaponSelPanel.isOpen()) {
        m_stWeaponSelPanel.updateUI();
        CCLOG("[trace]CWeaponLibControl::doQueryWeaponBagRsp, m_stWeaponSelPanel.updateUI.");
    }
    if(m_stWeaponInfoPanel.isInited() && m_stWeaponInfoPanel.isOpen()) {
        m_stWeaponInfoPanel.updateNowWeapon();
        CCLOG("[trace]CWeaponLibControl::doQueryWeaponBagRsp, m_stWeaponInfoPanel.updateNowWeapon.");
    }

	notifyWeaponAdvance();
}

void CWeaponLibControl::doWeaponLvUpRsp(const vmsg::CSWeaponLvUpRsp* info){
    const unsigned int iOldLv = P_ARM_OBJ->getUpWeapon().level();
    P_ARM_OBJ->resetUpMaterials();
    P_ARM_OBJ->updateWeaponBag(info->weapon());
    P_ARM_OBJ->setUpWeapon(info->weapon());
    P_BAG_CTRL->setBagInfo(info->bag());
    P_ARM_OBJ->setMaterialBag(info->bag()); 

    if(m_stWeaponInfoPanel.isInited() && m_stWeaponInfoPanel.isOpen()){
        //协议回包里已经更新背包了，不需要重新查询背包
        sendWeaponBagRqst(); //如果吞噬了枪械，重新查询一下
        m_stWeaponInfoPanel.doUpdateUI(&(info->weapon()), true); //只用当前默认来更新
        const int iDeltaLv = info->weapon().level() - iOldLv;
        if(iDeltaLv >= 0) {
            m_stWeaponInfoPanel.playLvUpEff(iDeltaLv);
        }
        
    }
}


void CWeaponLibControl::doQueryMaterialBagRsp(const vmsg::CSBagInfo* info){
    //背包更新时调用
    //neil mod,更新比较频繁,仅当面板打开时再更新数据吧
    bool bNeedUpdate = true;
    if(m_stSelMatPanel.isInited() && m_stSelMatPanel.isOpen()){
        P_ARM_OBJ->setMaterialBag(*info); //更新
        bNeedUpdate = false;
        m_stSelMatPanel.updateUI();
    }
    if(m_stWeaponLibPanel.isInited() && m_stWeaponLibPanel.isOpen()) {
        if(bNeedUpdate) {
            P_ARM_OBJ->setMaterialBag(*info); 
            bNeedUpdate = false;
        }
    }
    if(m_stWeaponInfoPanel.isInited() && m_stWeaponInfoPanel.isOpen()){
        if(bNeedUpdate) {
            P_ARM_OBJ->setMaterialBag(*info); 
            m_stWeaponInfoPanel.updateNowWeapon();
            bNeedUpdate = false;
        }
        
    }
    
}


void CWeaponLibControl::doWeaponChangeRsp(const vmsg::CSWeaponBag* info){
    //人物属性值发生改变
	P_FIGHT_OBJ->setRealWeapon(info->playerweapon());
    P_USER_OBJ->updateWeapon(info->playerweapon());
    P_ARM_OBJ->setWeaponBag(*info);

    bool bIsUpdateBag = true;
    if(m_stWeaponInfoPanel.isInited() && m_stWeaponInfoPanel.isOpen()){
        //获取当前已经装备的枪械
        P_BAG_CTRL->sendQueryBagInfoRqst();
        bIsUpdateBag = false;
        m_stWeaponInfoPanel.doUpdateUI( &(info->playerweapon()) );
    }
    if(m_stWeaponLibPanel.isInited() && m_stWeaponLibPanel.isOpen()){
        if(bIsUpdateBag) {
            P_BAG_CTRL->sendQueryBagInfoRqst();
        }
        m_stWeaponLibPanel.updateUI();
    }

	if(m_stArrayPanel.isInited() && m_stArrayPanel.isOpen()){
		m_stArrayPanel.doUpdateUI();
	}
    
	notifyWeaponAdvance();
}

//枪械升星/进阶
void CWeaponLibControl::doWeaponStarStepRsp(const vmsg::CSWeaponBag* info){
    P_ARM_OBJ->setWeaponBag(*info);

    if(m_stWeaponInfoPanel.isInited() && m_stWeaponInfoPanel.isOpen()){
        P_BAG_CTRL->sendQueryBagInfoRqst(); //neil mod,更新比较频繁,仅当面板打开时再更新数据吧
        m_stWeaponInfoPanel.doUpdateUI(NULL, true); //只用当前默认来更新
        getWeaponAdvDonePanel()->open();
        m_stWeaponInfoPanel.playAdvEffect();
    }
    P_CITY_CTRL->updateWeaponAvatar(); //升星后更新avatar

	notifyWeaponAdvance();
}


//枪械改装
void CWeaponLibControl::doWeaponRefitRsp(const vmsg::CSWeaponBag* info){
    P_ARM_OBJ->setWeaponBag(*info);
    if(m_stWeaponInfoPanel.isInited() && m_stWeaponInfoPanel.isOpen()){
        P_BAG_CTRL->sendQueryBagInfoRqst(); //neil mod,更新比较频繁,仅当面板打开时再更新数据吧
        
        m_stWeaponInfoPanel.doUpdateUI(NULL); //只用当前默认来更新
        m_stWeaponInfoPanel.playRefitEffect();
    }

	notifyWeaponAdvance();
}

void CWeaponLibControl::doQueryWeaponInBattleRsp(const vmsg::CSWeaponInBattleRsp& stRsp){
	P_ARM_OBJ->setWeaponInBattleInfo(stRsp);
	P_USER_OBJ->updateAddEff(stRsp.hpaddeff(), stRsp.atkaddeff(), stRsp.defaddeff());
	P_USER_OBJ->updateWeaponArr(stRsp.equippedweapon(), stRsp.inbattleweapons());

	if(m_stArrayPanel.isInited() && m_stArrayPanel.isOpen()){
		m_stArrayPanel.updateUI();
	}
}

void CWeaponLibControl::doChangeWeaponInBattleRsp(const vmsg::CSWeaponInBattleRsp& stRsp){
	P_ARM_OBJ->setWeaponInBattleInfo(stRsp);
	P_USER_OBJ->updateAddEff(stRsp.hpaddeff(), stRsp.atkaddeff(), stRsp.defaddeff());
	P_USER_OBJ->updateWeaponArr(stRsp.equippedweapon(), stRsp.inbattleweapons());

	if(m_stArrayPanel.isInited() && m_stArrayPanel.isOpen()){
		m_stArrayPanel.updateUI();
	}
}

