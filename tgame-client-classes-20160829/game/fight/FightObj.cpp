#include "FightObj.h"
#include "role/Enemy.h"
#include "UserObj.h"
#include "ClientSvrMsg.pb.h"

#include "WarMapCfgMaster.h"

using namespace vmsg;

CFightObj::CFightObj(){
    _totalTime = 0;
    _mark = 0;
    _maxKill = 0;
	m_uMapID = 0;
	m_uReliveCost = 0;
	m_uFillBulletCost = 0;
	m_fCurBulletVal = 0;
}

CFightObj::~CFightObj(){

}


void CFightObj::setFightMapID(unsigned int mapID){
	m_uMapID = mapID;

	resetCheckInfo();
}

void CFightObj::resetCheckInfo(){
	checkInfo.Clear();

	WarMapCfg* cfg = WAR_MAP_CFG->GetWarMapCfgByID(m_uMapID);
	google::protobuf::RepeatedPtrField<MapMonster> monsterInfos = cfg->monsters();
	google::protobuf::RepeatedPtrField<MapMonster>::iterator monsterInfoIter;

	for(monsterInfoIter = monsterInfos.begin(); monsterInfoIter != monsterInfos.end(); monsterInfoIter++){
		vmsg::CSKillMonsterInfo* pstKillMonsterInfo = checkInfo.add_monsterinfos();
		pstKillMonsterInfo->set_monsterid(monsterInfoIter->monsterid());
		pstKillMonsterInfo->set_monstercnt(0);
	}

	checkInfo.set_warmapid(m_uMapID);
	checkInfo.set_bossid(0);
	checkInfo.set_evolutionusecnt(0);
	checkInfo.set_score(0);
}

void CFightObj::setWarHideCondition(const vector<unsigned int>& stVec){
	if(stVec.size() <= 0){
		return;
	}

	for(unsigned int i = 0; i < stVec.size(); i++){
		checkInfo.add_hidenawardcell(stVec.at(i));
	}
}

vmsg::CSWarMapEndRqst& CFightObj::getCheckInfo(){
	return checkInfo;
}

void CFightObj::setAwardInfo(const vmsg::CSWarMapEndRsp& info){
	awardInfo = info;
}

void CFightObj::setEliteEndInfo(const vmsg::CSEndEliteWarMapRsp& info){
	awardInfo.Clear();

	google::protobuf::RepeatedPtrField<CSAwardElement> stAward = info.eliteaward().award();
	google::protobuf::RepeatedPtrField<CSAwardElement>::iterator stIter;

	for(stIter = stAward.begin(); stIter != stAward.end(); stIter++){
		CSAwardElement* pstAward = awardInfo.add_packawards();
		pstAward->set_elementid(stIter->elementid());
		pstAward->set_elementtype(stIter->elementtype());
		pstAward->set_elementcnt(stIter->elementcnt());
	}

	awardInfo.set_awardcoin(0);
	awardInfo.set_awardexp(0);
}

void CFightObj::setDailyEndInfo(const vmsg::CSDailyInstancePassRsp& info){
	awardInfo.Clear();

	google::protobuf::RepeatedPtrField<CSAwardElement> stAward = info.awardlst();
	google::protobuf::RepeatedPtrField<CSAwardElement>::iterator stIter;

	for(stIter = stAward.begin(); stIter != stAward.end(); stIter++){
		if(stIter->elementtype() == vmsg::THING_TYPE_COIN){
			awardInfo.set_awardcoin(stIter->elementcnt());
		}else if(stIter->elementtype() == vmsg::THING_TYPE_EXP){
			awardInfo.set_awardexp(stIter->elementcnt());
		}else{
			CSAwardElement* pstAward = awardInfo.add_packawards();
			pstAward->set_elementid(stIter->elementid());
			pstAward->set_elementtype(stIter->elementtype());
			pstAward->set_elementcnt(stIter->elementcnt());
		}
	}
}

vmsg::CSWarMapEndRsp& CFightObj::getAwardInfo(){
	return awardInfo;
}

void CFightObj::setRealWeapon(const vmsg::CSWeapon& stInfo){
	m_stRealWeapon = stInfo;
}

void CFightObj::resetRealWeapon(){
	m_stRealWeapon.Clear();
}

void CFightObj::restoreRealWeapon(){
	if(m_stRealWeapon.weaponguid() != 0 && P_USER_OBJ->getUserInfo()->weapon.weaponguid() != m_stRealWeapon.weaponguid()){
		CCLOG("[restoreRealWeapon]::the real weapon is %u", m_stRealWeapon.cfgid());
		P_USER_OBJ->updateWeaponInfo(m_stRealWeapon);
	}

	resetRealWeapon();
}

void CFightObj::swichToRealWeapon(){
	if(m_stRealWeapon.weaponguid() != 0 && P_USER_OBJ->getUserInfo()->weapon.weaponguid() != m_stRealWeapon.weaponguid()){
		CCLOG("[swichToRealWeapon]::the real weapon is %u", m_stRealWeapon.cfgid());
		P_USER_OBJ->updateWeapon(m_stRealWeapon);
	}

	resetRealWeapon();
}

void CFightObj::resetCost(){
	m_uReliveCost = 0;
	m_uFillBulletCost = 0;
}
