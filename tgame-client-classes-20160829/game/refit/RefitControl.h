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

	unsigned int getItemCntAddCompose(unsigned int uItemID); // 加上可以合成的

	bool isItemCanComposeCountCompose(unsigned int uItemID); // 算上可合成的再判断

	bool isPartCanAdd(unsigned int uPart); // 指定部位是否有Item可以镶嵌或合成

	bool isPartCanRefit(unsigned int uPart); // 指定部位是否可以改装

	void notifyRefit();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg& stMsg);

	void sendRefitQueryRqst(); // 查询

	void sendRefitComposeRqst(unsigned int uDotaItemID); // 合成

	void sendRefitMosaicRqst(unsigned int uPart, unsigned int uDotaItemID); // 镶嵌

	void sendRefitLvUpRqst(unsigned int uPart); // 升级

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