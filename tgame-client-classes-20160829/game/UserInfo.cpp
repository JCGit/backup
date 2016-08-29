#include "UserInfo.h"
#include "set/SetSysControl.h"

UserInfo::UserInfo()
{
    uin = 0;
	sex = 0;
	level = 0;
    exp = 0;
    gold = 0;
    coin = 0;
	vipLv = 0;
    remainManual = 0;
    updateRemainTime = 0;
	maxManualCnt = 1;
	buyManualCnt = 0;
	sceneID = 1;
    newestWarMapID = 0;
	weaponCfgID = 0;
	guildID = 0;
	hp = 0;
	mp = 0;
	atk = 0;
	atkSpeed = 0;
	userAtk = 0;
	def = 0;
	atkType = 0;
	defType = 0;
	moveSpeed = 0;
	runSpeed = 0;
	backSpeed = 0;
	fightPower = 0;
	showID = 0;
	dodgeDist = 0;
	dodgeCD = 0;
	evolutionSkillID = 0;
	hit = 0;
	hitWithOutInitVal = 0;
	hpAddEff = 0;
	atkAddEff = 0;
	defAddEff = 0;
	totalAtkAdd = 0;
	totalHpAdd = 0;
	totalDefAdd = 0;

	strSettingParam = SetSysControl::INIT_SET;
	isVIPDailyAwardClaimed = false;
}

UserInfo::~UserInfo()
{
}
