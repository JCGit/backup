#ifndef UserInfo_h__
#define UserInfo_h__

#include <string>
#include <vector>
#include "BasicTypes.h"
#include "CSEquipItemMsg.pb.h"
#include "CSPlayerMsg.pb.h"
#include "CSWeaponMsg.pb.h"

struct CWeaponArrInfo{
	vmsg::CSWeapon weaponInfo;
	int iIDx;
	int iState;
	CWeaponArrInfo(){
		weaponInfo.Clear();
		iIDx = 0;
		iState = 0;		
	}
};

class UserInfo
{
public:
	UserInfo();
	~UserInfo();

    std::string loginInfo; //loginInfo
    std::string openID; //uid
    std::string token; // token of the channel session
    int zoneid; //当前登录的服务器id
    std::string zoneName; //当前登录的服务器名称
    std::string cyouOID; //cyou oid(第三方渠道的用户原始ID(Android聚合渠道使用) )

	unsigned long uin;

	std::string nick;

	unsigned int sex;

	unsigned int level;

	unsigned long exp;

    unsigned int gold;
	unsigned int totalGoldByMoney;
    UINT64_t coin;

	unsigned int vipLv;

    int remainManual; //剩余体力
    int updateRemainTime; //更新体力时间
	int maxManualCnt;	//最大购买体力次数
	int buyManualCnt;	//已经购买体力次数

	unsigned int sceneID;	//上次登录的场景的id

	unsigned int newestWarMapID;  //最新通关的关卡ID

    int weaponCfgID;

	UINT64_t guildID;

	//角色详细信息
	unsigned int hp;			//血量
	unsigned int mp;			//能量值
	unsigned int atk;			//攻击力
	unsigned int atkSpeed;		//攻击速度(纯粹的人物攻击速速)
	unsigned int userAtk;		//人物纯粹攻击
	unsigned int def;			//防御值
	unsigned int atkType;		//攻击类型
	unsigned int defType;		//防御类型
	unsigned int moveSpeed;		//普通移动速度
	unsigned int runSpeed;		//疾行速度
	unsigned int backSpeed;		//攻击时速度
	UINT64_t fightPower;	//战力
	unsigned int showID;		//avatar id
	unsigned int dodgeDist;		//闪避距离
	unsigned int dodgeCD;		//闪避cd
	unsigned int evolutionSkillID;	//进化技能id 
	unsigned int hit;			//命中值
	unsigned int hitWithOutInitVal;
	unsigned int hpAddEff;		//HP加成
	unsigned int atkAddEff;		//攻击加成
	unsigned int defAddEff;		//防御加成

	//基因强化
	unsigned int totalAtkAdd;
	unsigned int totalHpAdd;
	unsigned int totalDefAdd;

	vmsg::CSWeapon weapon;						//武器
	vmsg::CSEquipInfo spareEquip;				//备用甲具
	std::vector<vmsg::CSEquipInfo> stEquipLst;	//装备列表
	std::vector<vmsg::CSPlayerAttr> stAtkAttrs;	//攻击属性
	std::vector<vmsg::CSPlayerAttr> stDefAttrs;	//防御属性
	std::vector<CWeaponArrInfo> stWeaponArr;	//战斗中枪械阵位信息

	vmsg::CSNothinglandInfo stNihilityInfo;		//虚无之地的信息

    vmsg::CSDailyRegInfo stDailyRegInfo; //签到信息

	std::string strSettingParam;			//玩家设置参数

	vmsg::CSMonthCardInfo stMonthCardInfo; // 月卡信息

	bool isVIPDailyAwardClaimed; // VIP日常奖励是否领取

private:
};

#endif // UserInfo_h__
