#include "ArenaControl.h"
#include "SceneManager.h"
#include "UserObj.h"
#include "city/CityControl.h"
#include "city/CityProxy.h"
#include "game/notification/NotificationControl.h"

#include "ArenaLvCfgMaster.h"
#include "WeaponCfgMaster.h"

#include "game/Constant.h"

#define MAX_ARENA_RANK_NUM 50

CArenaControl::CArenaControl()
:m_bHasNewRecord(false)
{

}

CArenaControl::~CArenaControl(){

}

void CArenaControl::sendArenaQueryRqst(){
	m_stArenaProxy.sendArenaQueryRqst();
}

void CArenaControl::sendArenaQueryRecordRqst(){
	m_stArenaProxy.sendArenaQueryRecordRqst();
}

void CArenaControl::sendArenaFindOpponentRqst(){
	m_stArenaProxy.sendArenaFindOpponentRqst();
}

void CArenaControl::sendArenaGetAwardRqst(){
	m_stArenaProxy.sendArenaGetAwardRqst();
}

void CArenaControl::sendArenaBuyFightNumRqst(){
	m_stArenaProxy.sendArenaBuyFightNumRqst();
}

void CArenaControl::sendArenaStartFightRqst(const UIN_t uin){
	m_stArenaProxy.sendArenaStartFightRqst(uin);
}

void CArenaControl::sendArenaEndFightRqst(const UIN_t uin, unsigned int isMyWin){
	m_stArenaProxy.sendArenaEndFightRqst(uin, isMyWin);
}

void CArenaControl::sendStoreQueryRqst(){
	m_stArenaProxy.sendStoreQueryRqst();
}

void CArenaControl::sendStoreBuyRqst(unsigned int itemIdx, unsigned int itemCfgID){
	m_stArenaProxy.sendStoreBuyRqst(itemIdx, itemCfgID);
}

void CArenaControl::sendStoreRefreshRqst(){
	m_stArenaProxy.sendStoreRefreshRqst();
}

void CArenaControl::sendQueryArenaArmyRankRqst(unsigned int uLv, const UIN_t uin /* = 0 */){
	//当前军阶
	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetOneCfg(uLv);
	unsigned int startScore = stCfg->score();  //start 最大，stop 最小
	unsigned int stopScore = 0;
	if(uLv == 1){
		stopScore = 0;
	}else{
		const ArenaLvCfg* stPreCfg = ARENA_LV_CFG_MASTER->GetOneCfg(stCfg->level() - 1);
		stopScore = stPreCfg->score() + 1;
	}

	m_stArenaProxy.sendQueryArenaArmyRankRqst(startScore, stopScore, MAX_ARENA_RANK_NUM, uin);
}

void CArenaControl::sendCleanCDRqst(){
	m_stArenaProxy.sendCleanCDRqst();
}

CArenaPanel* CArenaControl::getArenaPanel(){
	if(!m_stArenaPanel.isInited()){
		m_stArenaPanel.init();
	}

	return &m_stArenaPanel;
}

CMatchPanel* CArenaControl::getMatchPanel(){
	if(!m_stMatchPanel.isInited()){
		m_stMatchPanel.init();
	}

	return &m_stMatchPanel;
}

CRecordPanel* CArenaControl::getRecordPanel(){
	if(!m_stRecordPanel.isInited()){
		m_stRecordPanel.init();
	}

	return &m_stRecordPanel;
}

CExchangePanel* CArenaControl::getExchangePanel(){
	if(!m_stExchangePanel.isInited()){
		m_stExchangePanel.init();
	}

	return &m_stExchangePanel;
}

CArenaRankPanel* CArenaControl::getRankPanel(){
	if(!m_stRankPanel.isInited()){
		m_stRankPanel.init();
	}

	return &m_stRankPanel;
}

CArenaRulePanel* CArenaControl::getRulePanel(){
	if(!m_stRulePanel.isInited()){
		m_stRulePanel.init();
	}

	return &m_stRulePanel;
}

CArenaResultPanel* CArenaControl::getResultPanel(){
	if(!m_stResultPanel.isInited()){
		m_stResultPanel.init();
	}

	return &m_stResultPanel;
}

CArenaStartPanel* CArenaControl::getStartPanel(){
	if(!m_stStartPanel.isInited()){
		m_stStartPanel.init();
	}

	return &m_stStartPanel;
}

void CArenaControl::openExchangeInfoPanel(unsigned int cfgID, unsigned int cfgType, unsigned int cfgCnt, unsigned int idx, unsigned int coin){
	if(!m_stExchangeInfoPanel.isInited()){
		m_stExchangeInfoPanel.init();
	}

	m_stExchangeInfoPanel.open();
	m_stExchangeInfoPanel.setInfo(cfgID, cfgType, cfgCnt, idx, coin);
}

void CArenaControl::setHasNewRecord(bool bHasNewRecord) {
	m_bHasNewRecord = bHasNewRecord;
	notifyArenaNewRecord();
}

void CArenaControl::notifyArenaNewRecord() {
	if(m_bHasNewRecord) {
		PUSH_NOTIFICATION(ENTRY_ARENA, NOTIFY_TAG_ARENA_NEW_RECORD, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_ARENA, NOTIFY_TAG_ARENA_NEW_RECORD, 0);
	}
}

bool CArenaControl::hasFreeChallenge() {
	const CSArenaQueryRsp& stArenaInfo = m_stArenaObj.getArenaInfo();
	unsigned int uRemianNum = stArenaInfo.ByteSize() > 0 ? MAX_ARENA_FREE_NUM + stArenaInfo.todaybuynum() - stArenaInfo.todayfightnum() : 0;

	return uRemianNum > 0 && stArenaInfo.remainfightcd() <= 0;
}

void CArenaControl::notifyArenaFreeChallenge() {
	if(hasFreeChallenge()) {
		PUSH_NOTIFICATION(ENTRY_ARENA, NOTIFY_TAG_ARENA_FREE_CHALLENGE, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_ARENA, NOTIFY_TAG_ARENA_FREE_CHALLENGE, 0);
	}
}

void CArenaControl::doArenaQueryRsp(const CSArenaQueryRsp& stMsg){
	m_stArenaObj.setArenaInfo(stMsg);

	if(m_stArenaPanel.isInited() && m_stArenaPanel.isOpen()){
		m_stArenaPanel.updateUI();
	}

	//请求排行信息
	//当前军阶
	const ArenaLvCfg* stCfg = ARENA_LV_CFG_MASTER->GetCfgByScore(stMsg.score());
	sendQueryArenaArmyRankRqst(stCfg->level(), P_USER_OBJ->getUserInfo()->uin);

	notifyArenaFreeChallenge();
}

void CArenaControl::doRecordRsp(const CSArenaQueryRecordRsp& stMsg){
	m_stArenaObj.setRecordInfo(stMsg);
	setHasNewRecord(stMsg.recordlst_size() > m_stArenaObj.getRecordInfo().recordlst_size());

	if(m_stRecordPanel.isInited() && m_stRecordPanel.isOpen()){
		m_stRecordPanel.updateUI();
	}
}

void CArenaControl::doBuyFightNumRsp(const CSArenaBuyNumRsp& stMsg){
	m_stArenaObj.setBuyNumInfo(stMsg);

	if(m_stArenaPanel.isInited() && m_stArenaPanel.isOpen()){
		m_stArenaPanel.updateNum(stMsg.todayfightnum(), stMsg.todaybuynum());
	}

	notifyArenaFreeChallenge();
}

void CArenaControl::doFindOpponentRsp(const CSArenaFindOpponentRsp& stMsg){
	m_stArenaObj.setFindInfo(stMsg);

	if(m_stMatchPanel.isInited() && m_stMatchPanel.isOpen()){
		m_stMatchPanel.updateUI();
	}

	if(m_stArenaPanel.isInited() && m_stArenaPanel.isOpen()){
		m_stArenaPanel.updateMatchCost();
	}
}

void CArenaControl::doGetAwardRsp(){
	m_stArenaObj.doGetAwardInfo();

    P_ARENA_CTRL->sendArenaQueryRqst();
    /*
	if(m_stArenaPanel.isInited() && m_stArenaPanel.isOpen()){
		m_stArenaPanel.updateAwardBtn();
	}*/
}

void CArenaControl::doStoreInfoRsp(const CSArenaStroeInfo& stMsg){
	m_stArenaObj.setStoreInfo(stMsg);

	if(m_stExchangePanel.isInited() && m_stExchangePanel.isOpen()){
		m_stExchangePanel.updateUI();
	}
}

void CArenaControl::doArenaFightStartRsp(const CSArenaStartFightRsp& stMsg){
	m_stArenaObj.setFightStartInfo(stMsg);

	if(m_stMatchPanel.isInited() && m_stMatchPanel.isOpen()){
		m_stMatchPanel.close();
	}

	if(m_stArenaPanel.isInited() && m_stArenaPanel.isOpen()){
		m_stArenaPanel.close();
	}

	if(m_stRecordPanel.isInited() && m_stRecordPanel.isOpen()){
		m_stRecordPanel.close();
	}

	sendArenaQueryRqst(); // 更新下

	P_CITY_CTRL->getProxy()->stopOSD();
	SceneManager::sharedSceneManager()->enterScene(60001);
}

void CArenaControl::doArenaFightOverRsp(const CSArenaEndFightRsp& stMsg){
	getResultPanel()->closeLoading();

    m_stArenaObj.setFightBackInfo(0); //用后重置
	m_stArenaObj.setFightEndInfo(stMsg);

	getResultPanel()->open();
	getResultPanel()->updateUI();

}

void CArenaControl::doArenaRankRsp(const CSRankQueryArenaArmyRankRsp& stMsg){
	m_stArenaObj.setRankInfo(stMsg);

	if(m_stArenaPanel.isInited() && m_stArenaPanel.isOpen() 
		&& stMsg.has_myarmyrank() && stMsg.myarmyrank() >= 0){
		m_stArenaPanel.updateMyRank(stMsg.myarmyrank());
	}

	if(m_stRankPanel.isInited() && m_stRankPanel.isOpen()){
		m_stRankPanel.updateUI();
	}
}

void CArenaControl::updateDetailInfoByWeapon(CSPlayerDetailInfo* stInfo, const CSWeapon& stWeapon){
	unsigned int uPreCfgID = stInfo->weapon().cfgid();
	unsigned int uPreQuality = stInfo->weapon().quality();
	unsigned int uPreLv = stInfo->weapon().level();

	stInfo->mutable_weapon()->set_cfgid(stWeapon.cfgid());

	//枪械相关属性
	//攻击力
	const WeaponCfg* stPreCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(uPreCfgID);
	const WeaponCfg* stNewCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
	int roleAtk = stInfo->atk() - WEAPON_CFG_MASTER->GetWeaponAtkAdd(uPreCfgID, uPreQuality, uPreLv);
	int nowAtk = roleAtk + WEAPON_CFG_MASTER->GetWeaponAtkAdd(stWeapon.cfgid(), stWeapon.quality(), stWeapon.level());		//换枪后
	stInfo->set_atk(nowAtk);
	int roleHit = stInfo->hit() - stPreCfg->hit();
	int nowHit = roleHit + stNewCfg->hit();
	stInfo->set_hit(nowHit);

	int roleAtkSpeed = stInfo->atkspeed() - WEAPON_CFG_MASTER->GetWeaponCfgByID(stPreCfg->cfgid())->shotspeed();
	int nowAtkSpeed = roleAtkSpeed + WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid())->shotspeed();
	stInfo->set_atkspeed(nowAtkSpeed);

	//攻击属性
	for(int i = 0; i < stInfo->atkattrs_size(); i++){
		vmsg::CSPlayerAttr* stAttr = stInfo->mutable_atkattrs(i);

		if(stAttr->attrtype() == stPreCfg->atkattrtype()){  //枪械攻击属性类型
			unsigned int roleAttr = stAttr->attrval() - WEAPON_CFG_MASTER->GetWeaponAtkAttrAdd(uPreCfgID, uPreLv, uPreQuality);
			stAttr->set_attrval(roleAttr);		//人物本身的属性
			break;
		}
	}

	//换枪后
	for(int i = 0; i < stInfo->atkattrs_size(); i++){
		vmsg::CSPlayerAttr* stAttr = stInfo->mutable_atkattrs(i);
		if(stAttr->attrtype() == stNewCfg->atkattrtype()){  //枪械攻击属性类型
			unsigned int nowAttr = stAttr->attrval() + WEAPON_CFG_MASTER->GetWeaponAtkAttrAdd(stWeapon.cfgid(), stWeapon.level(), stWeapon.quality());
			stAttr->set_attrval(nowAttr);		//换枪后
			break;
		}
	}

	//攻击类型
	stInfo->set_atktype(stNewCfg->atktype());

	//移动速度相关
	unsigned int uPreMoveSpeed = stInfo->movespeed() + stPreCfg->decmovespeed();
	unsigned int uPreBackSpeed = stInfo->backspeed() + stPreCfg->decmovespeed();
	//换枪后
	stInfo->set_movespeed(uPreMoveSpeed - stNewCfg->decmovespeed());
	stInfo->set_backspeed(uPreBackSpeed - stNewCfg->decmovespeed());
}

void CArenaControl::doCleanCDRsp(){
	getArenaPanel()->updateCleanCD();
}

