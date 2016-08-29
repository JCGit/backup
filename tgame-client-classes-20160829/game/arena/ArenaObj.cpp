#include "ArenaObj.h"
#include "bag/BagControl.h"
#include "UserObj.h"

#include "NameCfgMaster.h"
#include "ArenaRandHeroCfgMaster.h"

CArenaObj::CArenaObj(){
    m_iFightBackRecTime = 0;
	_myRank = -1;
}

CArenaObj::~CArenaObj(){

}

void CArenaObj::setArenaInfo(const CSArenaQueryRsp& stMsg){
	m_stArenaInfo = stMsg;
	//m_stArenaInfo.set_isfirstfight(1);
}

void CArenaObj::doGetAwardInfo(){
	m_stArenaInfo.set_ishavetodayaward(0);
}

void CArenaObj::setRecordInfo(const CSArenaQueryRecordRsp& stMsg){
	m_stRecordInfo = stMsg;
}

void CArenaObj::setFindInfo(const CSArenaFindOpponentRsp& stMsg){
	m_stFindInfo = stMsg;

	if(m_stArenaInfo.isfirstfight()){  //第一次战斗，数据造假
		m_stFindInfo.set_uin(0);
		m_stFindInfo.set_name(NAME_CFG_MASTER->GetRandName());
		m_stFindInfo.set_score(ARENA_RAND_HERO_CFG_MASTER->GetCfg().score());
		m_stFindInfo.set_fightpower(ARENA_RAND_HERO_CFG_MASTER->GetCfg().fightpower());
		m_stFindInfo.set_showid(ARENA_RAND_HERO_CFG_MASTER->GetCfg().showid());
	}

	_opponentUin = stMsg.uin();

	//更新匹配次数
	m_stArenaInfo.set_todaymatchnum(stMsg.todaymatchnum());
}

void CArenaObj::setBuyNumInfo(const CSArenaBuyNumRsp& stMsg){
	m_stBuyNumInfo = stMsg;

	m_stArenaInfo.set_todaybuynum(stMsg.todaybuynum());
	m_stArenaInfo.set_todayfightnum(stMsg.todayfightnum());
}

void CArenaObj::setFightStartInfo(const CSArenaStartFightRsp& stMsg){
	m_stFightStartInfo = stMsg;

	if(m_stArenaInfo.isfirstfight()){  //第一次战斗，数据造假
		CSPlayerDetailInfo stDetailInfo;
		genRandOpDetailInfo(stDetailInfo);
		*(m_stFightStartInfo.mutable_opponentdetail()) = stDetailInfo;
	}

	//自己的详细信息
	P_USER_OBJ->updateUserDetailInfo(stMsg.mydetail());
}

void CArenaObj::setFightEndInfo(const CSArenaEndFightRsp& stMsg){
	m_stFightEndInfo = stMsg;
}

void CArenaObj::setStoreInfo(const CSArenaStroeInfo& stMsg){
	m_stStoreInfo = stMsg;

	P_BAG_CTRL->setBagInfo(stMsg.baginfo());
}

void CArenaObj::genRandOpDetailInfo(CSPlayerDetailInfo& stDetailInfo){
	const ArenaRandHeroCfg& stCfg = ARENA_RAND_HERO_CFG_MASTER->GetCfg();

	//stDetailInfo.Clear();
	stDetailInfo.set_hp(stCfg.hp());
	stDetailInfo.set_mp(stCfg.mp());
	stDetailInfo.set_atk(stCfg.atk());
	stDetailInfo.set_atkspeed(stCfg.atkspeed());
	stDetailInfo.set_def(stCfg.def());
	stDetailInfo.set_atktype(stCfg.atktype());
	stDetailInfo.set_deftype(stCfg.deftype());
	stDetailInfo.set_movespeed(stCfg.movespeed());
	stDetailInfo.set_backspeed(stCfg.backspeed());
	stDetailInfo.set_runspeed(stCfg.movespeed());
	stDetailInfo.set_fightpower(stCfg.fightpower());
	stDetailInfo.set_showid(stCfg.showid());
	stDetailInfo.set_dodgedist(stCfg.dodgedist());
	stDetailInfo.set_dodgecd(stCfg.dodgecd());
	stDetailInfo.set_evolutionskillid(stCfg.evolutionskillid());
	stDetailInfo.set_hit(stCfg.hit());
	stDetailInfo.set_hitwithoutinitval(stCfg.hitwithoutinitval());
	stDetailInfo.set_level(stDetailInfo.level() % 10 + 5);

	stDetailInfo.mutable_weapon()->set_cfgid(stCfg.weaponcfgid());
	stDetailInfo.mutable_weapon()->set_level(stCfg.weaponlevel());
	stDetailInfo.mutable_weapon()->set_quality(stCfg.weaponquality());
	stDetailInfo.mutable_weapon()->set_exp(0);
	stDetailInfo.mutable_weapon()->set_totalexp(0);
	stDetailInfo.mutable_weapon()->set_weaponguid(9999);
}

