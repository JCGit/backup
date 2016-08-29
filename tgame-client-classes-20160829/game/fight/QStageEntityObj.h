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
	unsigned int atk;						//�����ܹ�����
	unsigned int atkSpeed;					//���ﱾ��Ĺ����ٶ�
	unsigned int totalAtkSpeed;				//�ܹ��Ĺ����ٶ�
	unsigned int originalAtk;				//����ԭ��������
	unsigned int def;
	unsigned int atkType;
	unsigned int defType;
	unsigned int roleParty;
	unsigned int roleType;
	unsigned int dodgeDist;					//���ܾ���
	unsigned int dodgeCD;					//����cd
	unsigned int weaponCfgID;				//ǹеcfg id
	unsigned int evolutionSkillID;			//��������id
	unsigned int hit;						//����ֵ
	unsigned int initScale;					//��ʼx��,0����,1����
	int hurtColor;							//���˵���ɫ
	bool isBlock;							//�Ƿ��赲
	string hurtResPath;
	string dieResPath;						//������Ч
	string emergeVoice;						//������Ч
	string hurtVoice;						//������Ч
	string dieVoice;						//������Ч
	vmsg::CSWeapon weapon;					//������Ϣ
	vmsg::CSPlayerAttr atkAttr;				//��������
	uint32 stMagicAtkRatio[MAX_MAGIC_CNT];		//���й������ԣ�����˳��0Ϊ������
	uint32 stMagicDefRatio[MAX_MAGIC_CNT];     //�������Կ���
	CMovements stMovements;
protected:
private:

public:
	CQStageEntityObj(){}
	~CQStageEntityObj(){}

	CREATE_FUNC(CQStageEntityObj);
	bool init(){return true;}

	void setInfoByMonsterCfg(MonsterCfg* cfg);				//ͨ��monster cfg����
	void setInfoByUserInfo(const UserInfo* userInfo);		//ͨ����ɫ��Ϣ����
	void setInfoByDetaiInfo(const vmsg::CSPlayerDetailInfo& detaiInfo, unsigned int roleParty = ROLE_PARTY_FRIEND);	//ͨ����ϸ��Ϣ����
};

#endif // QStageEntityObj_h__
