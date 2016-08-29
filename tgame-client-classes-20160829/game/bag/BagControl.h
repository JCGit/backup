#ifndef BagControl_h__
#define BagControl_h__

#include "vlib/base/CVSingleton.h"

#include "BagProxy.h"
#include "BagObj.h"
#include "RoleInfoPanel.h"
#include "EquipInfoPanel.h"
#include "ItemInfoPanel.h"
#include "OtherRoleInfoPanel.h"
#include "PackageInfoPanel.h"
#include "BuyItemPanel.h"

#include <map>

using namespace std;

class BagControl
{
public:
	BagControl();
	~BagControl();
    
	ItemInfoPanel* getItemInfoPanel();

	EquipInfoPanel* getEquipInfoPanel();
	
	RoleInfoPanel* getRoleInfoPanel();

	OtherRoleInfoPanel* getOtherRoleInfoPanel();

	PackageInfoPanel* getPackageInfoPanel();

	CBuyItemPanel* getBuyItemPanel();

	void requestOtherRoleInfo(UIN_t uin);

	void updateOtherRoleInfo();

	const vmsg::CSBagInfo& getBagInfo();

	vmsg::CSBagInfo& mutableBagInfo();

	void setBagInfo(const vmsg::CSBagInfo& stMsg);

	const vmsg::CSBagCellInfo& getSelectedBagCellInfo();

	void setSelectedBagCellInfo(const vmsg::CSBagCellInfo &stBagCellInfo);

	int getIdxByEquipGUID(const UINT64_t uEquipGUID); // 获取装备在背包中的下标，如果没有，则返回-1

	int getIdxByItemCfgID(unsigned int uItemCfgID); // 获取物品在背包中的下标，如果没有，则返回-1

	bool checkItem(const unsigned int uItemCfgID, const unsigned int uItemCnt); // 检查背包里的物品是否够

    unsigned int getItemCnt(unsigned int uItemCfgID); // 背包中的物品数量

	void updateItemCnt(unsigned int uItemCfgID, const unsigned int uItemCnt);	// 更新背包中的物品数量

    // 背包中的装备数量,uIgnoreGuid为需要排除掉的装备guid
    int getEquipCnt(const unsigned int uEquipCfgID, const UINT64_t uIgnoreGuid); 
    const set<UINT64_t>* getBagEquipGuidLst(const unsigned int uEquipCfgID);

	bool isFull(); // 检查背包是否已满

	unsigned int getEmptyCellCnt();

	bool hasAdvancedEquipInBag(); // 背包中有更强装备

	void notifyAdvancedEquipInBag();

	void updateBag();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg); // 回包处理

	void sendQueryBagInfoRqst(); // 查询背包信息

	void sendEnlargeBagRqst(int nEnlargeCnt); // 扩展背包

	void sendTakeOnEquipRqst(int nCellIdx); // 穿上装备

	void sendTakeOffEquipRqst(UINT64_t uEquipGUID); // 脱下装备

	void sendBagSellRqst(int nBagIdx); // 出售

	void sendChangeArmorRqst(); // 切换甲具

    void sendOpenPackageRqst(const int iPkgIdx, const int iPkgCfgID);

	void sendBuyItemRqst(unsigned int uItemType, unsigned int uItemCfgID, unsigned int uCnt);	//购买道具

	void sendConsumeItemRqst(unsigned int uItemType, unsigned int uItemID, unsigned int uBagIdx, unsigned int uCnt = 1);		//使用道具

    void resetOnConnClose();

private:
	BagProxy m_stBagProxy;

	BagObj m_stBagObj;

	RoleInfoPanel *m_pRoleInfoPanel;

	ItemInfoPanel *m_pItemInfoPanel;

	EquipInfoPanel *m_pEquipInfoPanel;

	OtherRoleInfoPanel *m_pOtherRoleInfoPanel;

	PackageInfoPanel *m_pPackageInfoPanel;

	CBuyItemPanel m_stBuyItemPanel;

    map<int, int> m_stItemNumMap; //<ItemCfgID, cnt>, 背包中道具数量做个map，便于查询
    map<int, set<UINT64_t> > m_stEquipNumMap; //<EquipCfgID, <guid list> >, 背包中装备guid做个map，便于查询
    
};

#define P_BAG_CTRL CVLib::CVSingleton<BagControl>::Instance()

#endif // BagControl_h__
