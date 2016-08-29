#ifndef RefitControl_h__
#define RefitControl_h__

#include "vlib/base/CVSingleton.h"

#include "RefitProxy.h"
#include "RefitPanel.h"
#include "RefitMaterialPanel.h"
#include "RefitDonePanel.h"

class RefitControl
{
public:
	RefitControl();
	~RefitControl();

	void init();

	void finalize();

	RefitPanel* getRefitPanel();

	RefitMaterialPanel* getRefitMaterialPanel();

	RefitDonePanel* getRefitDonePanel();

	const vmsg::CSDotaItemBag& getBagInfo();

	const vmsg::CSDotaPartInfo& getPartInfo();

	unsigned int getItemCnt(unsigned int uItemID);

	bool isItemCanCompose(unsigned int uItemID);

	unsigned int getItemCntAddCompose(unsigned int uItemID); // ���Ͽ��Ժϳɵ�

	bool isItemCanComposeCountCompose(unsigned int uItemID); // ���Ͽɺϳɵ����ж�

	bool isPartCanAdd(unsigned int uPart); // ָ����λ�Ƿ���Item������Ƕ��ϳ�

	bool isPartCanRefit(unsigned int uPart); // ָ����λ�Ƿ���Ը�װ

	void notifyRefit();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg& stMsg);

	void sendRefitQueryRqst(); // ��ѯ

	void sendRefitComposeRqst(unsigned int uDotaItemID); // �ϳ�

	void sendRefitMosaicRqst(unsigned int uPart, unsigned int uDotaItemID); // ��Ƕ

	void sendRefitLvUpRqst(unsigned int uPart); // ����

private:
	bool m_bInitialized;

	RefitProxy m_stRefitProxy;

	RefitPanel *m_pRefitPanel;

	RefitMaterialPanel *m_pRefitMaterialPanel;

	RefitDonePanel *m_pRefitDonePanel;

	vmsg::CSDotaItemBag m_stBagInfo; // bag

	vmsg::CSDotaPartInfo m_stPartInfo;

	unsigned int m_uMosaicPart;

	unsigned int m_uMosaicDotaItemID;

};

#define P_REFIT_CTRL CVLib::CVSingleton<RefitControl>::Instance()

#endif // RefitControl_h__