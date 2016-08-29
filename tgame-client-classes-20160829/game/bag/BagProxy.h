#ifndef BagProxy_h__
#define BagProxy_h__

#include "QBaseProxy.h"
#include "basic/BasicTypes.h"

class BagProxy: public QBaseProxy
{
public:
	BagProxy();
	virtual ~BagProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	// override end

    void sendQueryBagInfoRqst(); // 查询背包信息

    void sendEnlargeBagRqst(int nEnlargeCnt); // 扩展背包

	void sendTakeOnEquipRqst(int nCellIdx); // 穿上装备

	void sendTakeOffEquipRqst(UINT64_t uEquipGUID); // 脱下装备

    void sendBagSellRqst(int nBagIdx); // 出售

	void sendChangeArmorRqst(); // 切换甲具

    void sendOpenPackageRqst(const int iPkgIdx, const int iPkgCfgID);

	void sendBuyItemRqst(unsigned int uItemType, unsigned int uItemCfgID, unsigned int uCnt);	//购买道具

	void sendConsumeItemRqst(unsigned int uItemType, unsigned int uItemID, unsigned int uBagIdx, unsigned int uCnt = 1);		//使用道具
private:
    
};

#endif // BagProxy_h__

