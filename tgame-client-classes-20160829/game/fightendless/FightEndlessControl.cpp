#include "FightEndlessControl.h"
#include "TextHelper.h"
#include "defines.h"
#include "WeaponCfgMaster.h"
#include "fight/FightObj.h"
#include "SceneManager.h"
#include "UserObj.h"

CFightEndlessControl::CFightEndlessControl(){
	_scoreAdd = 0;
	_coinAdd = 0;
}

CFightEndlessControl::~CFightEndlessControl(){

}

void CFightEndlessControl::sendInfiniteGenInfoRqst(){
	m_stEndlessProxy.sendInfiniteGenInfoRqst();
}

void CFightEndlessControl::sendInfiniteBuyGenRqst(unsigned int idx){
	m_stEndlessProxy.sendInfiniteBuyGenRqst(idx);
}

void CFightEndlessControl::sendEndlessStartRqst(){
	m_stEndlessProxy.sendEndlessStartRqst();
}

void CFightEndlessControl::sendEndlessEndRqst(const UNIT64_t score, const unsigned int packageCnt, const unsigned int waveCnt, 
											  unsigned int coin, CVArray<int, 4> stMonsterCnt, unsigned int evolutionCnt){
	m_stEndlessProxy.sendEndlessEndRqst(score, packageCnt, waveCnt, coin, stMonsterCnt, evolutionCnt);
}

CFightEndlessShowPanel* CFightEndlessControl::getShowPanel(){
	if(!m_stShowPanel.isInited()){
		m_stShowPanel.init();
	}

	return &m_stShowPanel;
}

CFightEndlessRankPanel* CFightEndlessControl::getEndlessRankPanel(){
	if(!m_stRankPanel.isInited()){
		m_stRankPanel.init();
	}

	return &m_stRankPanel;
}

CStrengthenPanel* CFightEndlessControl::getStrengthenPanel(){
	if(!m_stStrengthenPanel.isInited()){
		m_stStrengthenPanel.init();
	}

	return &m_stStrengthenPanel;
}

CFightEndlessResultPanel* CFightEndlessControl::getResultPanel(){
	if(!m_stResultPanel.isInited()){
		m_stResultPanel.init();
	}

	return &m_stResultPanel;
}

void CFightEndlessControl::doGenInfoRsp(CSInfiniteGenInfo* pstRsp){
	m_stGenInfo = *pstRsp;

	if(m_stStrengthenPanel.isInited() && m_stStrengthenPanel.isOpen()){
		m_stStrengthenPanel.updateUI();
	}
}

void CFightEndlessControl::doBuyGenRsp(CSInfiniteGenInfo* pstRsp){
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

		if(isUpdate && m_stStrengthenPanel.isInited() && m_stStrengthenPanel.isOpen()){
			m_stStrengthenPanel.updateItemByIdx(idx, stGen);
		}

		idx++;
	}

	m_stGenInfo = *pstRsp;
}

void CFightEndlessControl::doInfiniteEnd(CSInfiniteEndRsp* pstRsp){
	m_stEndInfo = *pstRsp;
	_maxScore = pstRsp->maxscore();

	if(P_FIGHT_OBJ->getIsForceQuit()){  //强制退出
		SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
	}else{
		if(pstRsp->awardlist_size() <= 0){
			getResultPanel()->open();
			getResultPanel()->updateUI();
		}else{
			getShowPanel()->open();
			getShowPanel()->showResult(*pstRsp);
		}
	}
}

const string CFightEndlessControl::getDescByGenInfo(const CSGen& info){
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
	default:
		break;
	}

	return str;
}

void CFightEndlessControl::updateUserInfoByGenLst(){
	_scoreAdd = 0;
	_coinAdd = 0;
	m_stRandWeapon.Clear();

	const CSGen& stRandGen = m_stGenInfo.genlist(0);

	int len = m_stGenInfo.genlist_size();
	for(int i = 1; i < len; i++){
		const CSGen& stGen = m_stGenInfo.genlist(i);

		//随机强化顶替
		if(stRandGen.cnt() > 0){
			if(stGen.cnt() > 0 && stGen.type() == stRandGen.type() && stGen.param1() < stRandGen.param1()){
				updateUserInfoByGenInfo(stRandGen);
				continue;
			}
		}

		if(stGen.cnt() > 0){
			updateUserInfoByGenInfo(stGen);
		}
	}

	if(stRandGen.type() == GEN_STRENGTHEN_WEAPON && stRandGen.cnt() > 0){  //抽中枪械
		updateUserInfoByGenInfo(stRandGen);
	}
}

void CFightEndlessControl::updateUserInfoByGenInfo(const CSGen& stGen){
	UserInfo* stUserObj = P_USER_OBJ->getMutableUserInfo();
	CSWeapon weapon;		

	switch(stGen.type())
	{
	case GEN_STRENGTHEN_ATK:
		stUserObj->atk = stUserObj->atk*(1 + 1.0*stGen.param1()/10000);
		break;
	case GEN_STRENGTHEN_DEFNLIFE:
		stUserObj->def = stUserObj->def*(1 + 1.0*stGen.param1()/10000);
		stUserObj->hp = stUserObj->hp*(1 + 1.0*stGen.param1()/10000);
		break;
	case GEN_STRENGTHEN_SCORE:
		_scoreAdd = stGen.param1();
		break;
	case GEN_STRENGTHEN_COIN:
		_coinAdd = stGen.param1();
		break;
	case GEN_STRENGTHEN_WEAPON:
		stUserObj->weaponCfgID = stGen.param1();  //cfg id
		m_stRandWeapon.set_weaponguid(stUserObj->weapon.weaponguid());
		m_stRandWeapon.set_cfgid(stGen.param1());
		m_stRandWeapon.set_level(stGen.param2());
		m_stRandWeapon.set_quality(1);
		m_stRandWeapon.set_exp(0);
		m_stRandWeapon.set_totalexp(0);
		stUserObj->weapon = m_stRandWeapon;
		break;
	default:
		break;
	}
}