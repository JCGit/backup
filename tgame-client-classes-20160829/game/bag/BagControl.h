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

	int getIdxByEquipGUID(const UINT64_t uEquipGUID); // ��ȡװ���ڱ����е��±꣬���û�У��򷵻�-1

	int getIdxByItemCfgID(unsigned int uItemCfgID); // ��ȡ��Ʒ�ڱ����е��±꣬���û�У��򷵻�-1

	bool checkItem(const unsigned int uItemCfgID, const unsigned int uItemCnt); // ��鱳�������Ʒ�Ƿ�

    unsigned int getItemCnt(unsigned int uItemCfgID); // �����е���Ʒ����

	void updateItemCnt(unsigned int uItemCfgID, const unsigned int uItemCnt);	// ���±����е���Ʒ����

    // �����е�װ������,uIgnoreGuidΪ��Ҫ�ų�����װ��guid
    int getEquipCnt(const unsigned int uEquipCfgID, const UINT64_t uIgnoreGuid); 
    const set<UINT64_t>* getBagEquipGuidLst(const unsigned int uEquipCfgID);

	bool isFull(); // ��鱳���Ƿ�����

	unsigned int getEmptyCellCnt();

	bool hasAdvancedEquipInBag(); // �������и�ǿװ��

	void notifyAdvancedEquipInBag();

	void updateBag();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg); // �ذ�����

	void sendQueryBagInfoRqst(); // ��ѯ������Ϣ

	void sendEnlargeBagRqst(int nEnlargeCnt); // ��չ����

	void sendTakeOnEquipRqst(int nCellIdx); // ����װ��

	void sendTakeOffEquipRqst(UINT64_t uEquipGUID); // ����װ��

	void sendBagSellRqst(int nBagIdx); // ����

	void sendChangeArmorRqst(); // �л��׾�

    void sendOpenPackageRqst(const int iPkgIdx, const int iPkgCfgID);

	void sendBuyItemRqst(unsigned int uItemType, unsigned int uItemCfgID, unsigned int uCnt);	//�������

	void sendConsumeItemRqst(unsigned int uItemType, unsigned int uItemID, unsigned int uBagIdx, unsigned int uCnt = 1);		//ʹ�õ���

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

    map<int, int> m_stItemNumMap; //<ItemCfgID, cnt>, �����е�����������map�����ڲ�ѯ
    map<int, set<UINT64_t> > m_stEquipNumMap; //<EquipCfgID, <guid list> >, ������װ��guid����map�����ڲ�ѯ
    
};

#define P_BAG_CTRL CVLib::CVSingleton<BagControl>::Instance()

#endif // BagControl_h__
