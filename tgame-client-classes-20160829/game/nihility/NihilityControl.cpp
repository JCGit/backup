#include "NihilityControl.h"
#include "SceneManager.h"
#include "UserObj.h"
#include "NihilityUtils.h"
#include "TextHelper.h"
#include "defines.h"
#include "vlib/CVRand.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"
#include "fight/BaseFightScene.h"

#include "WeaponCfgMaster.h"
#include "NothinglandSweepItemCfgMaster.h"

CNihilityControl::CNihilityControl()
:_score(0)
//,_maxScore(0)
,_coin(0)
,_floor(1)
//,_maxFloor(0)
,_needRush(0)
,_floorScore(0)
,_scoreAdd(0)
,_coinAdd(0)
,m_bIsAutoFight(false)
,m_uScoreMd5(0)
,m_uCoinMd5(0)
,m_uRandData(0)
,_lastEvolutionCnt(0)
,_bIsContinue(false)
{
	m_stAwardFloor.Clear();
	m_stAllWeaponBulletVal.Clear();
}

CNihilityControl::~CNihilityControl(){

}

void CNihilityControl::sendInfiniteGenInfoRqst(){
	m_stProxy.sendInfiniteGenInfoRqst();
}

void CNihilityControl::sendInfiniteBuyGenRqst(unsigned int idx){
	m_stProxy.sendInfiniteBuyGenRqst(idx);
}

void CNihilityControl::sendNihilityFightStartRqst(){
	//1表示继续挑战没完成的，0表示重新开始
	unsigned int isContinue = _bIsContinue?1:0;
	m_stProxy.sendNihilityFightStartRqst(isContinue);
}

void CNihilityControl::sendNihilityFightEndRqst(unsigned int evolutioncnt){
	//数据校验
	if(!checkData(_score, m_uScoreMd5)){
		resumeData(_score, m_uScoreMd5);
	}

	if(!checkData(_coin, m_uCoinMd5)){
		resumeData(_coin, m_uCoinMd5);
	}

	m_stProxy.sendNihilityFightEndRqst(evolutioncnt + _lastEvolutionCnt, _floorScore, _coin, _floor - 1, _score, m_stAwardFloor);
}

void CNihilityControl::sendNihilityUnfinishedQueryRqst(){
	m_stProxy.sendNihilityUnfinishedQueryRqst();
}

void CNihilityControl::sendNihilityMissonReportRqst(unsigned int evolutioncnt){
	//数据校验
	if(!checkData(_score, m_uScoreMd5)){
		resumeData(_score, m_uScoreMd5);
	}

	if(!checkData(_coin, m_uCoinMd5)){
		resumeData(_coin, m_uCoinMd5);
	}

	m_stProxy.sendNihilityMissonReportRqst(evolutioncnt + _lastEvolutionCnt, _floorScore, _coin, _floor - 1, _score, m_stAwardFloor);
}

CNihilityPanel* CNihilityControl::getNihilityPanel(){
	if(!m_stNihilityPanel.isInited()){
		m_stNihilityPanel.init();
	}

	return &m_stNihilityPanel;
}

CNihilityShowPanel* CNihilityControl::getShowPanel(){
	if(!m_stShowPanel.isInited()){
		m_stShowPanel.init();
	}

	return &m_stShowPanel;
}

CNihilityResultPanel* CNihilityControl::getResultPanel(){
	if(!m_stResultPanel.isInited()){
		m_stResultPanel.init();
	}

	return &m_stResultPanel;
}

CNihilityItemPanel* CNihilityControl::getItemPanel(){
	if(!m_stItemPanel.isInited()){
		m_stItemPanel.init();
	}

	return &m_stItemPanel;
}

CNihilityGenPanel* CNihilityControl::getGenPanel(){
	if(!m_stGenPanel.isInited()){
		m_stGenPanel.init();
	}

	return &m_stGenPanel;
}

CNihilityContinuePanel* CNihilityControl::getContinuePanel(){
	if(!m_stContinuePanel.isInited()){
		m_stContinuePanel.init();
	}

	return &m_stContinuePanel;
}

void CNihilityControl::doGenInfoRsp(CSInfiniteGenInfo* pstRsp){
	m_stGenInfo = *pstRsp;

	if(m_stGenPanel.isInited() && m_stGenPanel.isOpen()){
		m_stGenPanel.updateUI();
	}
}

void CNihilityControl::doBuyGenRsp(CSInfiniteGenInfo* pstRsp){
	google::protobuf::RepeatedPtrField<CSGen> genList = pstRsp->genlist();
	google::protobuf::RepeatedPtrField<CSGen>::iterator genIter;

	unsigned int idx = 0;
	for(genIter = genList.begin(); genIter != genList.end(); genIter++){
		const CSGen& stGen = *genIter;
		const CSGen& preGen = m_stGenInfo.genlist(idx);
		bool isUpdate = true;

		if(stGen.type() == preGen.type() && stGen.param1() == preGen.param1() 
			&& stGen.param2() == preGen.param2() && stGen.cnt() == preGen.cnt()
			&& stGen.costcoin() == preGen.costcoin()){  //数据没有改变
				isUpdate = false;
		}

		if(isUpdate && m_stGenPanel.isInited() && m_stGenPanel.isOpen()){
			m_stGenPanel.updateItemByIdx(idx, stGen);
		}

		idx++;
	}

	m_stGenInfo = *pstRsp;
}

void CNihilityControl::doNihilityHistoryRsp(const CSNothingLandHistoryRsp& stRsp){
	m_stHistoryInfo = stRsp;
	_lastEvolutionCnt = 0;
	_bIsContinue = (stRsp.score() != 0);

	getNihilityPanel()->closeLoad();
}

void CNihilityControl::doNihilityStartRsp(){
	if(getNihilityPanel()->isOpen()){
		getNihilityPanel()->close();
	}

	if(getGenPanel()->isOpen()){
		getGenPanel()->close();
	}

	if(getContinuePanel()->isOpen()){
		getContinuePanel()->close();
	}

	m_bIsAutoFight = false;

	if(!_bIsContinue){   //继续上次挑战，不使用基因数据
		updateUserInfoByGenLst();
	}else{
		_score = m_stHistoryInfo.score();
		_coin = m_stHistoryInfo.coin();
		_floor = m_stHistoryInfo.floor() + 1;
		_floorScore = m_stHistoryInfo.floorscore();
		_lastEvolutionCnt = m_stHistoryInfo.evolutioncnt();

		for(int i = 0; i < m_stHistoryInfo.awardfloor_size(); i++){
			addAwarFloor(m_stHistoryInfo.awardfloor(i));
		}
	}

	//随机得到加密数据
	m_uRandData = 9527 + CRandom::GetRandHaveMax(EGAME_DENOM);
	updateData(_coin, m_uCoinMd5);
	updateData(_score, m_uScoreMd5);

	//进入场景
	SceneManager::sharedSceneManager()->enterScene(NIHILITY_FIGHT_SCENE);		
}

void CNihilityControl::doNihilityEndRsp(CSNothingLandEndRsp* pstRsp){
	getResultPanel()->closeLoading();

	//重置枪械
	P_FIGHT_OBJ->restoreRealWeapon();

	//更新数据
	m_stEndInfo = *pstRsp;
	CSNothinglandInfo stInfo;
	stInfo.set_floor(pstRsp->maxfloor());
	stInfo.set_score(pstRsp->maxscore());
	stInfo.set_maxfloorscore(pstRsp->maxfloorscore());
	P_USER_OBJ->updateNihilityInfo(stInfo);

	if(getIsForceQuit()){  //强制退出
		SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
	}else{
		//结算面板
		getResultPanel()->open();
		getResultPanel()->updateUI();
	}
}

void CNihilityControl::addAwarFloor(unsigned int floor){
	m_stAwardFloor.AddOneItem(floor);
}

void CNihilityControl::addCoin(unsigned int addCnt){
	//数据校验
	if(!checkData(_coin, m_uCoinMd5)){
		resumeData(_coin, m_uCoinMd5);
	}

	_coin += addCnt*(1 + 1.0*_coinAdd/EGAME_DENOM);

	updateData(_coin, m_uCoinMd5);
}

void CNihilityControl::addScore(unsigned int addCnt){
	//数据校验
	if(!checkData(_score, m_uScoreMd5)){
		resumeData(_score, m_uScoreMd5);
	}

	unsigned int uCfgID = P_USER_OBJ->getUserInfo()->weapon.cfgid();
	unsigned int uLv = P_USER_OBJ->getUserInfo()->weapon.level();
	const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(uCfgID);
	unsigned int weaponAdd = stCfg->scoreadd()*uLv;
	_score += addCnt*(1 + 1.0*_scoreAdd/EGAME_DENOM)*(1 + 1.0*weaponAdd/EGAME_DENOM);

	updateData(_score, m_uScoreMd5);
}

bool CNihilityControl::checkData(unsigned int uPreData, unsigned int uMd5Data){
	if(uPreData + m_uRandData == uMd5Data){
		return true;
	}

	return false;
}

void CNihilityControl::updateData(unsigned int uPreData, unsigned int& uMd5Data){
	uMd5Data = uPreData + m_uRandData;
}

void CNihilityControl::resumeData(unsigned int& uPreData, unsigned int uMd5Data){
	uPreData = uMd5Data - m_uRandData;
}

void CNihilityControl::resetNihility(){
	_score = 0;
	_coin = 0;
	_floor = 1;
	_scoreAdd = 0;
	_coinAdd = 0;
	_floorScore = 0;
	_lastEvolutionCnt = 0;

	_isForceQuit = false;
	m_stAwardFloor.Clear();
	m_stAllWeaponBulletVal.Clear();
}

const string CNihilityControl::getDescByGenInfo(const CSGen& info){
	string str = "";

	switch(info.type())
	{
	case GEN_STRENGTHEN_RANDOM:
		str = TextStr(ENDLESS_RAND_GEN);
		str += to_string(info.param1()/100) + "%";
		break;
	case GEN_STRENGTHEN_ATK:
		str = TextStr(ENDLESS_ADD_ATK_GEN);
		str += to_string(info.param1()/100) + "%";
		break;
	case GEN_STRENGTHEN_DEFNLIFE:
		str = TextStr(ENDLESS_ADD_HP_DEF_GEN);
		str += to_string(info.param1()/100) + "%";
		break;
	case GEN_STRENGTHEN_SCORE:
		str = TextStr(ENDLESS_ADD_SCORE_GEN);
		str += to_string(info.param1()/100) + "%";
		break;
	case GEN_STRENGTHEN_COIN:
		str = TextStr(ENDLESS_ADD_COIN_GEN);
		str += to_string(info.param1()/100) + "%";
		break;
	case GEN_STRENGTHEN_WEAPON:
		str = "LV." + to_string(info.param2());
		str += WEAPON_CFG_MASTER->GetWeaponCfgByID(info.param1())->weaponname();
		break;
	case GEN_STRENGTHEN_RUSH:
		char rushStr[100];
		snprintf(rushStr, 100,  TextStr(ENDLESS_RUSH_GEN) , info.param1()); 
		str = rushStr;
		break;
	default:
		break;
	}

	return str;
}


void CNihilityControl::updateUserInfoByGenLst(){
	_scoreAdd = 0;
	_coinAdd = 0;
	_needRush = 0;
	_floor = 1;

	const CSGen& stRandGen = m_stGenInfo.genlist(0);

	int len = m_stGenInfo.genlist_size();
	for(int i = 1; i < len; i++){
		const CSGen& stGen = m_stGenInfo.genlist(i);

		//随机强化顶替
		if(stRandGen.cnt() > 0 && stGen.type() == stRandGen.type()){
			if(stGen.cnt() <= 0){  //没有对应的强化，只有随机强化
				updateUserInfoByGenInfo(stRandGen);
			}else{
				if(stGen.param1() < stRandGen.param1()){
					updateUserInfoByGenInfo(stRandGen);
				}else{
					updateUserInfoByGenInfo(stGen);
				}
			}
			continue;
		}

		if(stGen.cnt() > 0){
			updateUserInfoByGenInfo(stGen);
		}
	}

	if(stRandGen.type() == GEN_STRENGTHEN_WEAPON && stRandGen.cnt() > 0){  //抽中枪械
		updateUserInfoByGenInfo(stRandGen);
	}
}

void CNihilityControl::updateUserInfoByGenInfo(const CSGen& stGen){
	UserInfo* stUserObj = P_USER_OBJ->getMutableUserInfo();

	switch(stGen.type())
	{
	case GEN_STRENGTHEN_ATK:
		stUserObj->atk = stUserObj->atk*(1 + 1.0*stGen.param1()/EGAME_DENOM);
		break;
	case GEN_STRENGTHEN_DEFNLIFE:
		stUserObj->def = stUserObj->def*(1 + 1.0*stGen.param1()/EGAME_DENOM);
		stUserObj->hp = stUserObj->hp*(1 + 1.0*stGen.param1()/EGAME_DENOM);
		break;
	case GEN_STRENGTHEN_SCORE:
		_scoreAdd = stGen.param1();
		break;
	case GEN_STRENGTHEN_COIN:
		_coinAdd = stGen.param1();
		break;
	case GEN_STRENGTHEN_WEAPON:
		updateTempWeapon(stGen);
		break;
	case GEN_STRENGTHEN_RUSH:
		_needRush = 1;
		_floor = stGen.param1();
		break;
	default:
		break;
	}
}

void CNihilityControl::updateTempWeapon(const CSGen& stGen){
	UserInfo* stUserObj = P_USER_OBJ->getMutableUserInfo();

	vmsg::CSWeapon stTempWeapon;
	stTempWeapon.set_cfgid(stGen.param1());
	stTempWeapon.set_weaponguid(TEMP_WEAPON_GUID);
	stTempWeapon.set_level(stGen.param2());
	stTempWeapon.set_exp(0);
	stTempWeapon.set_totalexp(0);
	stTempWeapon.set_quality(stUserObj->weapon.quality()); 
	P_USER_OBJ->updateWeaponInfo(stTempWeapon);
}

void CNihilityControl::updateAllWeaponBulletVal(CVArray<float, 6> stArr){
	m_stAllWeaponBulletVal.Clear();

	for(int i = 0; i < stArr.GetCount(); i++){
		m_stAllWeaponBulletVal.AddOneItem(stArr[i]);
	}
}


