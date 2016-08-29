#ifndef __EQUIP_LIB_OBJ_h__
#define __EQUIP_LIB_OBJ_h__

#include "BasicTypes.h"
#include "CSEquipItemMsg.pb.h"
#include "CSEquipOpMsg.pb.h"

//装备信息,重新包装一下使用
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
    bool bIsWear; //当前使用的装备为true;备用防具和背包装备为false
    int  iBagIdx; //装备在背包中的index；角色装备为-1
};

#define EQUIP_SEL_MAT_MAX_NUM 6 //最多选择6个材料

//选择材料面板用到的数据结构
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
    
    int getEquipMaxLvUpLv(const int iEquipCfgID); //升级上限
    int getFullLvNeedExp(const int iEquipCfgID, const int iNowLv); //当前等级升满需要多少经验

    bool addOneLvUpMat(const EquipSelMatOneInfo& info); //添加一个升级材料，返回true表示添加成功，false表示满了
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

    //计算uiExpAdd经验能够升几级
    int getLevelsByExp(const int iEquipCfgID, const UINT64_t uEquipExp, const int iLv, const unsigned int uiExpAdd);

    //装备吞掉后增加的经验
    UINT64_t getEquipEatExp(const UINT64_t uTotalExp, const int iEquipCfgID);
private:
    UINT64_t m_iSelEquipGuid; //装备选择面板
	
    map<UINT64_t, vmsg::CSEquipLvUpConsume> m_stLvUpMat; //升级所需材料 <EquipGuid/ItemCfgID, info>,道具时cnt可叠加
    map<UINT64_t, int> m_stCfgIDMap; //升级所需材料 <EquipGuid/ItemCfgID, CfgID>,便于升级显示装备图标
    int m_iNowSelLvUpMatNum; //当前选择了几个升级材料了
    int m_iTotalAddExp; //当前选择的材料总共加多少经验

};


#endif // __EQUIP_LIB_OBJ_h__
