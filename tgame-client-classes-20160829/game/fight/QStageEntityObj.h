#ifndef QStageEntityObj_h__
#define QStageEntityObj_h__

#include "cocos2d.h"
#include "CSPlayerMsg.pb.h"
#include "UserInfo.h"
#include "MonsterCfgMaster.h"
#include "Macros.pb.h"
#include "vlib/Common.h"

USING_NS_CC;

using namespace std;

struct CMovements{
	string emergeAct;

	CMovements(){
		emergeAct = "";
	}
};

class CQStageEntityObj : public CCObject
{
public:
	UINT64_t hp;
	unsigned int ep;
	unsigned int level;
	unsigned int walkSpeed;
	unsigned int runSpeed;
	unsigned int backSpeed;
	unsigned int atk;						//人物总共秒伤
	unsigned int atkSpeed;					//人物本身的攻击速度
	unsigned int totalAtkSpeed;				//总共的攻击速度
	unsigned int originalAtk;				//人物原本的秒伤
	unsigned int def;
	unsigned int atkType;
	unsigned int defType;
	unsigned int roleParty;
	unsigned int roleType;
	unsigned int dodgeDist;					//闪避距离
	unsigned int dodgeCD;					//闪避cd
	unsigned int weaponCfgID;				//枪械cfg id
	unsigned int evolutionSkillID;			//进化技能id
	unsigned int hit;						//命中值
	unsigned int initScale;					//初始x轴,0向右,1向左
	int hurtColor;							//受伤的颜色
	bool isBlock;							//是否阻挡
	string hurtResPath;
	string dieResPath;						//死亡特效
	string emergeVoice;						//出现音效
	string hurtVoice;						//受伤音效
	string dieVoice;						//死亡音效
	vmsg::CSWeapon weapon;					//武器信息
	vmsg::CSPlayerAttr atkAttr;				//攻击属性
	uint32 stMagicAtkRatio[MAX_MAGIC_CNT];		//所有攻击属性，按照顺序，0为无属性
	uint32 stMagicDefRatio[MAX_MAGIC_CNT];     //所有属性抗性
	CMovements stMovements;
protected:
private:

public:
	CQStageEntityObj(){}
	~CQStageEntityObj(){}

	CREATE_FUNC(CQStageEntityObj);
	bool init(){return true;}

	void setInfoByMonsterCfg(MonsterCfg* cfg);				//通过monster cfg设置
	void setInfoByUserInfo(const UserInfo* userInfo);		//通过角色信息设置
	void setInfoByDetaiInfo(const vmsg::CSPlayerDetailInfo& detaiInfo, unsigned int roleParty = ROLE_PARTY_FRIEND);	//通过详细信息设置
};

#endif // QStageEntityObj_h__
