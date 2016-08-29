#include "UserObj.h"
#include "defines.h"
#include "instance/InstanceEventUtils.h"
#include "set/SetSysControl.h"
#include "ClientSvrMsg.pb.h"

#include "WeaponCfgMaster.h"

UserObj::UserObj()
{
    m_stUserInfo.uin = 0;
    m_stUserInfo.exp = 0;
    m_stUserInfo.level = 0;
	m_bLoginPrepare = false;

    m_uQuickLoginKey = 0;
}

UserObj::~UserObj()
{
}

void UserObj::setLoginPrepare(bool var){
	m_bLoginPrepare = var;
}

bool UserObj::getLoginPrepare(){
	return m_bLoginPrepare;
}

void UserObj::setUin(const UIN_t uUin)
{
    m_stUserInfo.uin = uUin;
}

void UserObj::updateUserInfo(const vmsg::CSPlayerInfo &playerInfo) {
	m_stUserInfo.uin = playerInfo.uin();
	m_stUserInfo.nick = playerInfo.nick();
	m_stUserInfo.level = playerInfo.level();
	m_stUserInfo.exp = playerInfo.exp();
    m_stUserInfo.coin = playerInfo.coin();
	m_stUserInfo.showID = playerInfo.showid();
	m_stUserInfo.remainManual = playerInfo.remainmanual();
	m_stUserInfo.weaponCfgID = playerInfo.weaponcfgid();
	m_stUserInfo.updateRemainTime = playerInfo.updateremaintime();
	m_stUserInfo.sceneID = playerInfo.sceneid();
	m_stUserInfo.vipLv = playerInfo.viplv();
	m_stUserInfo.newestWarMapID = playerInfo.newestwarmapid();
	m_stUserInfo.guildID = playerInfo.guildid();
    //m_stUserInfo.stEquipLst = playerInfo.
	m_stUserInfo.stNihilityInfo = playerInfo.nothinglandinfo();
	if(playerInfo.settingparam() != ""){  //未设置过，使用默认值
		m_stUserInfo.strSettingParam = playerInfo.settingparam();
	}

    m_uQuickLoginKey = playerInfo.quickloginkey();

    updateDailyRegInfo(playerInfo.dailyreginfo());

	updateMonthCardInfo(playerInfo.monthcardinfo());

	m_stUserInfo.isVIPDailyAwardClaimed = playerInfo.vipdailyclaimed() == 1; // 1表示已领取

	//init set param
	P_SET_CTRL->initSet();
}

bool UserObj::isUserEquip(const UNIT64_t ulEquipGuid)
{
    std::vector<vmsg::CSEquipInfo>& stEquipInfo = m_stUserInfo.stEquipLst;
    for (unsigned int i = 0; i < stEquipInfo.size(); ++i)
    {
        if (ulEquipGuid == stEquipInfo[i].equipguid())
        {
            return true;
        }
    }
    if(ulEquipGuid == m_stUserInfo.spareEquip.equipguid()) {
        return true;
    }
    return false;
}

void UserObj::updateGold(const unsigned int iGold, const unsigned int uTotalGoldByMoney)
{
    m_stUserInfo.gold = iGold;
	m_stUserInfo.totalGoldByMoney = uTotalGoldByMoney;
}
 void UserObj::updateCoin(const UINT64_t iCoin) {
     m_stUserInfo.coin = iCoin;
 }

 void UserObj::updateLvExp(const int lv, const int exp){
     m_stUserInfo.level = lv;
     m_stUserInfo.exp = exp;
 }

 void UserObj::updateVIPLv(const unsigned int uVIPLv) {
	 m_stUserInfo.vipLv = uVIPLv;
 }

 void UserObj::updateFightPower(const UINT64_t iFightPower){
	 m_stUserInfo.fightPower = iFightPower;
 }

int UserObj::checkGold(const unsigned int iNeedGold)
{
    if (m_stUserInfo.gold >= iNeedGold){
        return 0;
    }

    return 1;
}

int UserObj::checkCoin(const UNIT64_t iNeedCoin)
{
    if (m_stUserInfo.coin >= iNeedCoin){
        return 0;
    }
	
    return 1;
}

void UserObj::updateUserDetailInfo(const vmsg::CSPlayerDetailInfo& detailInfo){
	m_stUserInfo.hp = detailInfo.hp();
	m_stUserInfo.mp = detailInfo.mp();
	m_stUserInfo.atk = detailInfo.atk();
	m_stUserInfo.atkSpeed = detailInfo.atkspeed();
	m_stUserInfo.def = detailInfo.def();
	m_stUserInfo.atkType = detailInfo.atktype();
	m_stUserInfo.defType = detailInfo.deftype();
	m_stUserInfo.moveSpeed = detailInfo.movespeed();
	m_stUserInfo.runSpeed = detailInfo.runspeed();
	m_stUserInfo.backSpeed = detailInfo.backspeed();
	m_stUserInfo.fightPower = detailInfo.fightpower();
	m_stUserInfo.showID = detailInfo.showid();
	m_stUserInfo.weapon = detailInfo.weapon();
	m_stUserInfo.spareEquip = detailInfo.spareequip();
	m_stUserInfo.dodgeDist = detailInfo.dodgedist();
	m_stUserInfo.dodgeCD = detailInfo.dodgecd();
	m_stUserInfo.weaponCfgID = detailInfo.weapon().cfgid();
	m_stUserInfo.evolutionSkillID = detailInfo.evolutionskillid();
	m_stUserInfo.hit = detailInfo.hit();
	m_stUserInfo.hitWithOutInitVal = detailInfo.hitwithoutinitval();
	m_stUserInfo.hpAddEff = detailInfo.hpaddeff();
	m_stUserInfo.atkAddEff = detailInfo.atkaddeff();
	m_stUserInfo.defAddEff = detailInfo.defaddeff();
	m_stUserInfo.totalAtkAdd = detailInfo.totalatkenforceadd();
	m_stUserInfo.totalDefAdd = detailInfo.totaldefenforceadd();
	m_stUserInfo.totalHpAdd = detailInfo.totalhpenforceadd();

	m_stUserInfo.stEquipLst.clear(); //reset
	for (int i = 0; i < detailInfo.equiplst_size(); ++i){
		m_stUserInfo.stEquipLst.push_back(detailInfo.equiplst(i));
	}
	
	m_stUserInfo.stAtkAttrs.clear();
	for(int i = 0; i < detailInfo.atkattrs_size(); i++){
		m_stUserInfo.stAtkAttrs.push_back(detailInfo.atkattrs(i));
	}

	m_stUserInfo.stDefAttrs.clear();
	for(int i = 0; i < detailInfo.defattrs_size(); i++){
		m_stUserInfo.stDefAttrs.push_back(detailInfo.defattrs(i));
	}

	updateWeaponArr(detailInfo.weapon(), detailInfo.inbattleweapons());
}

std::string UserObj::getMyZoneNick() {
    std::string strNick(m_stUserInfo.nick);
    strNick += ".s" + intToString(GetZoneID(m_stUserInfo.uin));
    return strNick;
}

std::string UserObj::genZoneNick(const std::string& strNick) {
    //如果strNick已经含有.s，则返回，否则，说明是同服的，根据自己的uin组一下返回
    std::size_t found = strNick.find(".s");
    if (found != std::string::npos) {
        return strNick;
    }
    std::string str(strNick);
    str += ".s" + intToString(GetZoneID(m_stUserInfo.uin));
    return str;
}

void UserObj::updateManual(const vmsg::CSPlayerManualNotify &stManualNotify) {
	//m_stPlayerInfo.set_remainmanual(stManualNotify.remainmanual());
	//m_stPlayerInfo.set_updateremaintime(stManualNotify.updateremaintime());
    m_stUserInfo.remainManual = stManualNotify.remainmanual();
    m_stUserInfo.updateRemainTime = stManualNotify.updateremaintime();
}

void UserObj::updateWeapon(const vmsg::CSWeapon& stWeapon){
	updateWeaponInfo(stWeapon);

	CCString* pstStr = CCString::createWithFormat("%d", 1);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(HERO_WEAPON_CHANGE_EVENT, pstStr);
}

void UserObj::updateWeaponInfo(const vmsg::CSWeapon& stWeapon){
	vmsg::CSWeapon stPreWeapon = m_stUserInfo.weapon;

	m_stUserInfo.weapon.set_cfgid(stWeapon.cfgid());
	m_stUserInfo.weapon.set_weaponguid(stWeapon.weaponguid());
	m_stUserInfo.weapon.set_level(stWeapon.level());
	m_stUserInfo.weapon.set_exp(stWeapon.exp());
	m_stUserInfo.weapon.set_totalexp(stWeapon.totalexp());
	m_stUserInfo.weapon.set_quality(stWeapon.quality());

	m_stUserInfo.weaponCfgID = stWeapon.cfgid();

	//枪械相关属性
	//攻击力
	const WeaponCfg* stPreCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stPreWeapon.cfgid());
	const WeaponCfg* stNewCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
	int preWeaponAtk = WEAPON_CFG_MASTER->GetWeaponAtkAdd(stPreWeapon.cfgid(), stPreWeapon.quality(), stPreWeapon.level());
	int nowWeaponAtk = WEAPON_CFG_MASTER->GetWeaponAtkAdd(stWeapon.cfgid(), stWeapon.quality(), stWeapon.level());
	m_stUserInfo.atkAddEff;
	int roleAtk = m_stUserInfo.atk - preWeaponAtk;
	m_stUserInfo.atk = roleAtk + nowWeaponAtk;		//换枪后
	int roleHit = m_stUserInfo.hit - stPreCfg->hit();
	m_stUserInfo.hit = roleHit + stNewCfg->hit();

	int roleAtkSpeed = m_stUserInfo.atkSpeed - WEAPON_CFG_MASTER->GetWeaponCfgByID(stPreCfg->cfgid())->shotspeed();
	m_stUserInfo.atkSpeed = roleAtkSpeed + WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid())->shotspeed();

	//攻击属性
	for(unsigned int i = 0; i < m_stUserInfo.stAtkAttrs.size(); i++){
		vmsg::CSPlayerAttr stAttr = m_stUserInfo.stAtkAttrs.at(i);

		if(stAttr.attrtype() == stPreCfg->atkattrtype()){  //枪械攻击属性类型
			unsigned int roleAttr = stAttr.attrval() - WEAPON_CFG_MASTER->GetWeaponAtkAttrAdd(stPreWeapon.cfgid(), stPreWeapon.level(), stPreWeapon.quality());
			stAttr.set_attrval(roleAttr);		//人物本身的属性
			break;
		}
	}

	//换枪后
	for(unsigned int i = 0; i < m_stUserInfo.stAtkAttrs.size(); i++){
		vmsg::CSPlayerAttr stAttr = m_stUserInfo.stAtkAttrs.at(i);
		if(stAttr.attrtype() == stNewCfg->atkattrtype()){  //枪械攻击属性类型
			unsigned int nowAttr = stAttr.attrval() + WEAPON_CFG_MASTER->GetWeaponAtkAttrAdd(stWeapon.cfgid(), stWeapon.level(), stWeapon.quality());
			stAttr.set_attrval(nowAttr);		//换枪后
			break;
		}
	}

	//攻击类型
	m_stUserInfo.atkType = stNewCfg->atktype();

	//移动速度相关
	unsigned int uPreMoveSpeed = m_stUserInfo.moveSpeed + stPreCfg->decmovespeed();
	unsigned int uPreBackSpeed = m_stUserInfo.backSpeed + stPreCfg->decmovespeed();
	//换枪后
	m_stUserInfo.moveSpeed = uPreMoveSpeed - stNewCfg->decmovespeed();
	m_stUserInfo.backSpeed = uPreBackSpeed - stNewCfg->decmovespeed();
}

int UserObj::checkManual(const int iNeedManual){
	if (m_stUserInfo.remainManual >= iNeedManual){
		return 0;
	}
	return 1;
}

void UserObj::updateNewestWarMap(unsigned int warMapID){
	m_stUserInfo.newestWarMapID = warMapID;
}

void UserObj::updateNihilityInfo(const vmsg::CSNothinglandInfo &stInfo){
	m_stUserInfo.stNihilityInfo = stInfo;
}


void UserObj::setQuickLoginKey(const UINT64_t uNewKey) {
    UINT64_t uOldKey = m_uQuickLoginKey;
    m_uQuickLoginKey = uNewKey;
    CCLOG("[trace]UserObj,set new quick login key %llu, old key %llu", uNewKey, uOldKey);
}


void UserObj::updateDailyRegInfo(const vmsg::CSDailyRegInfo& stInfo) {
    m_stUserInfo.stDailyRegInfo = stInfo;
    CCLOG("[trace]UserObj,update daily reg info. reg cnt %d, can ret %d", stInfo.regcnt(), stInfo.canreg() );
}

void UserObj::updateMonthCardInfo(const vmsg::CSMonthCardInfo &stMonthCardInfo) {
	m_stUserInfo.stMonthCardInfo = stMonthCardInfo;
}

void UserObj::updateAddEff(unsigned int hpAdd, unsigned int atkAdd, unsigned int defAdd){
	m_stUserInfo.hpAddEff = hpAdd;
	m_stUserInfo.atkAddEff = atkAdd;
	m_stUserInfo.defAddEff = defAdd;
}

void UserObj::updateGenInfo(unsigned int totalAtkAdd, unsigned int totalDefAdd, unsigned int totalHpAdd){
	m_stUserInfo.totalAtkAdd = totalAtkAdd;
	m_stUserInfo.totalDefAdd = totalDefAdd;
	m_stUserInfo.totalHpAdd = totalHpAdd;
}

void UserObj::updateWeaponArr(const vmsg::CSWeapon& stEquipWeapon, google::protobuf::RepeatedPtrField<vmsg::CSInbattleWeaponElmt> stWeaponArr){
	m_stUserInfo.stWeaponArr.clear(); //reset
	
	CWeaponArrInfo stInfo;
	//身上装备放在第一位
	stInfo.weaponInfo = stEquipWeapon;
	stInfo.iIDx = 0;
	stInfo.iState = vmsg::WEAPON_ARRAY_CELL_UNLOCKED;
	m_stUserInfo.stWeaponArr.push_back(stInfo);

	for (int i = 0; i < stWeaponArr.size(); ++i){
		CWeaponArrInfo stElmt;
		stElmt.weaponInfo = stWeaponArr.Get(i).weapon();
		stElmt.iIDx = stWeaponArr.Get(i).arrayidx() + 1;
		stElmt.iState = stWeaponArr.Get(i).state();
		m_stUserInfo.stWeaponArr.push_back(stElmt);
	}
}

