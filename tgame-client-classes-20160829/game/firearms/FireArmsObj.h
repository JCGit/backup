#ifndef FireArmsObj_h__
#define FireArmsObj_h__

#include "vlib/base/CVSingleton.h"
#include "CSWeaponMsg.pb.h"
#include "CSBagMsg.pb.h"
#include "BasicTypes.h"

using namespace std;

/*
enum {
	STATE_EQUIP = 1,
	STATE_CHOOSE = 2,
};
*/

class CFireArmsObj
{
public:
    enum 
    {
        LV_UP_MAT_MAX_SIZE = 6, //�����������6��
        LV_UP_MAT_SIZE_WEAPON = 2, //ǹеռ2��
        LV_UP_MAT_SIZE_ITEM = 1, //����ռ1��
    };
	CFireArmsObj();
	~CFireArmsObj();

	void setWeaponBag(const vmsg::CSWeaponBag& info);
	const vmsg::CSWeaponBag& getWeaponBag();

    bool isWeaponBagFull();
    int getWeaponIdx(UNIT64_t weaponGUID);

	void setMaterialBag(const vmsg::CSBagInfo& info);
	const vector<vmsg::CSBagCellInfo>& getMaterialBag();
	bool static compareMaterial(const vmsg::CSBagCellInfo& first, const vmsg::CSBagCellInfo& second);

	void setAllWeaponBag(const vmsg::CSWeaponBag& info);
	const vector<vmsg::CSWeapon>& getAllWeaponBag();
	const vector<vmsg::CSWeapon>& getChangeWeaponLst();

    const vmsg::CSWeapon& getOneWeaponByGUID(UNIT64_t uID);
    const vmsg::CSWeapon* getWeaponByGUID(UNIT64_t uID);
    int getWeaponNumByCfgID(const int iWeaponCfgID, UNIT64_t iIgnoreWeaponGuid = 0); //�ұ�����ӵ�м�����iWeaponCfgID��ǹе����������ʹ�õģ�

	void setPreWeaponGUID(UNIT64_t guid);
	UNIT64_t getPreWeaponGUID();

	void updateWeaponBag(const vmsg::CSWeapon& info);

	//����
	//����ѡ�е�ǹе
	void resetUpWeapon();
	void setUpWeapon(const vmsg::CSWeapon& info);
	const vmsg::CSWeapon& getUpWeapon();

	//����ѡ�еĲ���
	void resetUpMaterials();
	void addUpMaterial(UNIT64_t uID, unsigned int type, unsigned int idx);
	void removeUpMaterial(UNIT64_t uID, unsigned int idx);
    const map<UINT64_t, vmsg::CSWeaponLvUpConsume>& getUpMaterials();
    int getUpMatLogicSize(); //��ȡ���������ж��ٸ�(ǹеռ2��������ռ1�������6��)
    bool canAddUpMat(const int iType); //�Ƿ��п�λ���һ�������͵Ĳ���
	bool isInUpMaterial(unsigned int uIdx);		//�������Ƿ�Ϊѡ�е���������
    int getFullLvNeedExp(const int iWeaponCfgID, const int iWeaponLv); //��������ʱ��Ҫ���پ���

	const vector<vmsg::CSWeaponLvUpConsume> getSortUpMaterials();
	const vector<vmsg::CSBagCellInfo>& getLvUpMaterials();

    //����
	bool checkCanStarWeapon(unsigned int uCfgID, UINT64_t uWeaponGUID = 0);

	void getWeaponsInCfg(vector<int>& stAllWeaponInCfg, const unsigned int type, const vector<vmsg::CSWeapon>& stMyWeapons);	//��ȡ������ȥ�����������е�����ǹе

	void getWeaponsInBagByType(const unsigned int type, vector<vmsg::CSWeapon>& stWeapons);  //��ȡ�����д����͵�����ǹе

    //��ȡ2���ȼ����ۼƾ���(������toLev, toLev > fromLev, uID���ڼ���ý׶ξ���ı���
    unsigned int getExpByTwoLevels(unsigned int fromLev, unsigned int toLev, unsigned int uID);

    //ͨ����ǰ�ȼ������飬�Լ����ӵľ����ȡ�������ٵȼ�, uID���ڼ���ý׶ξ���ı���
    unsigned int getLevelsByExp(const vmsg::CSWeapon& stWeapon, unsigned int addExp);

    //ͨ����ǰ�ȼ������飬�Լ����ӵľ����ȡ������ĵ�ǰ�ȼ����еľ���, uID���ڼ���ý׶ξ���ı���
    unsigned int getUpLeftExp(const vmsg::CSWeapon& stWeapon, unsigned int addExp);

	//ǹеս��
	void setWeaponInBattleInfo(const vmsg::CSWeaponInBattleRsp& stInfo);
	const vmsg::CSWeaponInBattleRsp& getWeaponInBattleInfo(){
		return m_stWeaponInBattle;
	}

	//�л�������ʱ����Ϣ
	void setOldWeapon(UNIT64_t uGUID, int iArrayIdx);
	const vmsg::CSInbattleWeaponElmtBrief& getOldWeapon(){
		return m_stOldWeapon;
	}

	int getWeaponIdleNumByCfgID(unsigned int uCfgID, UNIT64_t iIgnoreWeaponGuid = 0);

protected:
    bool static compareByLev(const vmsg::CSWeapon& first, const vmsg::CSWeapon& second);
	bool static compareByLevUp(const vmsg::CSWeapon& first, const vmsg::CSWeapon& second);
    bool static compareWeaponByLv(const int cfgIDA, const int cfgIDB);	//����ǹе��������
    //����ǹе��������
    bool static compareWeaponInfoByLv(const vmsg::CSWeapon& weaponA, const vmsg::CSWeapon& weaponB);
    bool static compareUpMaterial(vmsg::CSWeaponLvUpConsume first, vmsg::CSWeaponLvUpConsume second);
private:
	//����
    vmsg::CSWeaponBag weaponBag;	
    vector<vmsg::CSWeapon> m_stAllWeaponBag;		//���е�ǹе���������ϵ�

	vector<vmsg::CSWeapon> m_stChangeWeaponLst;		//���л���ǹе�б�

	//��ǹ
	UNIT64_t m_uPreWeaponGUID;						//��ǹǰ��guid

	//����
	vmsg::CSWeapon m_stUpWeapon;					//��ǰ����ǹе
    map<UINT64_t, vmsg::CSWeaponLvUpConsume> m_stUpMaterials;		//������Ӧ��ǰ����ʹ�õĲ���<index, Info>
    vector<vmsg::CSBagCellInfo> m_stLvUpMaterials;	//��������
    int m_iLvUpMatLoginSize; //���������м����ˣ�ǹеռ2��������ռ1�������6����
    
    map<int, set<UINT64_t> > m_stWeaponNumMap; //<WeaponCfgID, <guid list> >, ������guid����map�����ڲ�ѯ(ȫ����ǹе�����������ǹе)

	vmsg::CSWeaponInBattleRsp m_stWeaponInBattle;

	vmsg::CSInbattleWeaponElmtBrief m_stOldWeapon;
};

#define P_ARM_OBJ CVLib::CVSingleton<CFireArmsObj>::Instance()

#endif // FireArmsObj_h__
