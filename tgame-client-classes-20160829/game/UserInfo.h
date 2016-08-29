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
    int zoneid; //��ǰ��¼�ķ�����id
    std::string zoneName; //��ǰ��¼�ķ���������
    std::string cyouOID; //cyou oid(�������������û�ԭʼID(Android�ۺ�����ʹ��) )

	unsigned long uin;

	std::string nick;

	unsigned int sex;

	unsigned int level;

	unsigned long exp;

    unsigned int gold;
	unsigned int totalGoldByMoney;
    UINT64_t coin;

	unsigned int vipLv;

    int remainManual; //ʣ������
    int updateRemainTime; //��������ʱ��
	int maxManualCnt;	//�������������
	int buyManualCnt;	//�Ѿ�������������

	unsigned int sceneID;	//�ϴε�¼�ĳ�����id

	unsigned int newestWarMapID;  //����ͨ�صĹؿ�ID

    int weaponCfgID;

	UINT64_t guildID;

	//��ɫ��ϸ��Ϣ
	unsigned int hp;			//Ѫ��
	unsigned int mp;			//����ֵ
	unsigned int atk;			//������
	unsigned int atkSpeed;		//�����ٶ�(��������﹥������)
	unsigned int userAtk;		//���﴿�⹥��
	unsigned int def;			//����ֵ
	unsigned int atkType;		//��������
	unsigned int defType;		//��������
	unsigned int moveSpeed;		//��ͨ�ƶ��ٶ�
	unsigned int runSpeed;		//�����ٶ�
	unsigned int backSpeed;		//����ʱ�ٶ�
	UINT64_t fightPower;	//ս��
	unsigned int showID;		//avatar id
	unsigned int dodgeDist;		//���ܾ���
	unsigned int dodgeCD;		//����cd
	unsigned int evolutionSkillID;	//��������id 
	unsigned int hit;			//����ֵ
	unsigned int hitWithOutInitVal;
	unsigned int hpAddEff;		//HP�ӳ�
	unsigned int atkAddEff;		//�����ӳ�
	unsigned int defAddEff;		//�����ӳ�

	//����ǿ��
	unsigned int totalAtkAdd;
	unsigned int totalHpAdd;
	unsigned int totalDefAdd;

	vmsg::CSWeapon weapon;						//����
	vmsg::CSEquipInfo spareEquip;				//���ü׾�
	std::vector<vmsg::CSEquipInfo> stEquipLst;	//װ���б�
	std::vector<vmsg::CSPlayerAttr> stAtkAttrs;	//��������
	std::vector<vmsg::CSPlayerAttr> stDefAttrs;	//��������
	std::vector<CWeaponArrInfo> stWeaponArr;	//ս����ǹе��λ��Ϣ

	vmsg::CSNothinglandInfo stNihilityInfo;		//����֮�ص���Ϣ

    vmsg::CSDailyRegInfo stDailyRegInfo; //ǩ����Ϣ

	std::string strSettingParam;			//������ò���

	vmsg::CSMonthCardInfo stMonthCardInfo; // �¿���Ϣ

	bool isVIPDailyAwardClaimed; // VIP�ճ������Ƿ���ȡ

private:
};

#endif // UserInfo_h__
