#ifndef __EQUIP_LIB_OBJ_h__
#define __EQUIP_LIB_OBJ_h__

#include "BasicTypes.h"
#include "CSEquipItemMsg.pb.h"
#include "CSEquipOpMsg.pb.h"

//װ����Ϣ,���°�װһ��ʹ��
struct EquipLibOneInfo {
public:
    EquipLibOneInfo(const vmsg::CSEquipInfo& equip, bool isWear, int bagIdx):equipInfo(equip), bIsWear(isWear), iBagIdx(bagIdx) {
    }
    EquipLibOneInfo() {
        equipInfo.Clear();
        iBagIdx = -1;
        bIsWear = false;
    }
public:
    vmsg::CSEquipInfo equipInfo;
    bool bIsWear; //��ǰʹ�õ�װ��Ϊtrue;���÷��ߺͱ���װ��Ϊfalse
    int  iBagIdx; //װ���ڱ����е�index����ɫװ��Ϊ-1
};

#define EQUIP_SEL_MAT_MAX_NUM 6 //���ѡ��6������

//ѡ���������õ������ݽṹ
struct EquipSelMatOneInfo {
    EquipSelMatOneInfo():stEquip(),itemCfgID(-1) {
    }
public:
    EquipLibOneInfo stEquip;
    int itemCfgID;
};

class EquipLibObj
{
public:
    EquipLibObj();
    ~EquipLibObj(){

    }

    void onSelEquip(const UINT64_t& iSelEquipGuid){
        m_iSelEquipGuid = iSelEquipGuid;
    }
    const UINT64_t& getSelEquipGuid(){
        return m_iSelEquipGuid;
    }
    void resetSelEquip() {
        m_iSelEquipGuid = 0;
    }
    const EquipLibOneInfo getEquipInfo(const UINT64_t uEquipGuid);
    
    int getEquipMaxLvUpLv(const int iEquipCfgID); //��������
    int getFullLvNeedExp(const int iEquipCfgID, const int iNowLv); //��ǰ�ȼ�������Ҫ���پ���

    bool addOneLvUpMat(const EquipSelMatOneInfo& info); //���һ���������ϣ�����true��ʾ��ӳɹ���false��ʾ����
    bool isLvUpMatFull() { 
        return m_iNowSelLvUpMatNum >= EQUIP_SEL_MAT_MAX_NUM; 
    }
    void clearLvUpMat();
    const map<UINT64_t, vmsg::CSEquipLvUpConsume>& getLvUpMat() {
        return m_stLvUpMat;
    }
    int getMatTotalExp() {
        return m_iTotalAddExp;
    }

    int getCfgIDByGuid(const UINT64_t guid) {
        if(m_stCfgIDMap.count(guid) > 0 ) {
            return m_stCfgIDMap[guid];
        }
        return 0;
    }

    //����uiExpAdd�����ܹ�������
    int getLevelsByExp(const int iEquipCfgID, const UINT64_t uEquipExp, const int iLv, const unsigned int uiExpAdd);

    //װ���̵������ӵľ���
    UINT64_t getEquipEatExp(const UINT64_t uTotalExp, const int iEquipCfgID);
private:
    UINT64_t m_iSelEquipGuid; //װ��ѡ�����
	
    map<UINT64_t, vmsg::CSEquipLvUpConsume> m_stLvUpMat; //����������� <EquipGuid/ItemCfgID, info>,����ʱcnt�ɵ���
    map<UINT64_t, int> m_stCfgIDMap; //����������� <EquipGuid/ItemCfgID, CfgID>,����������ʾװ��ͼ��
    int m_iNowSelLvUpMatNum; //��ǰѡ���˼�������������
    int m_iTotalAddExp; //��ǰѡ��Ĳ����ܹ��Ӷ��پ���

};


#endif // __EQUIP_LIB_OBJ_h__
