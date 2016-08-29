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

    void sendQueryBagInfoRqst(); // ��ѯ������Ϣ

    void sendEnlargeBagRqst(int nEnlargeCnt); // ��չ����

	void sendTakeOnEquipRqst(int nCellIdx); // ����װ��

	void sendTakeOffEquipRqst(UINT64_t uEquipGUID); // ����װ��

    void sendBagSellRqst(int nBagIdx); // ����

	void sendChangeArmorRqst(); // �л��׾�

    void sendOpenPackageRqst(const int iPkgIdx, const int iPkgCfgID);

	void sendBuyItemRqst(unsigned int uItemType, unsigned int uItemCfgID, unsigned int uCnt);	//�������

	void sendConsumeItemRqst(unsigned int uItemType, unsigned int uItemID, unsigned int uBagIdx, unsigned int uCnt = 1);		//ʹ�õ���
private:
    
};

#endif // BagProxy_h__

