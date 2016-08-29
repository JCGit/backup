#ifndef VIPControl_h__
#define VIPControl_h__

#include "vlib/base/CVSingleton.h"

#include "CSPlayerMsg.pb.h"

#include "VIPProxy.h"
#include "VIPPanel.h"

class BuyCoinPanel;

class VIPControl 
{
public:
	VIPControl();
	~VIPControl();

	VIPPanel* getVIPPanel();

	void updateGold();

	const vmsg::CSVIPInfos& getVIPInfos();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg); // 处理消息

	void sendQueryVIPInfoRqst(); // 查询VIP信息

	void sendGetVIPAwardRqst(unsigned int uVIPLv); // 领取VIP奖励

    void finalizePanel();

    //招财符信息
    BuyCoinPanel* getBuyCoinPanel();
    const vmsg::CSBuyCoinRsp& getBuyCoinRsp() {
        return m_stBuyCoinRsp;
    }

    VIPProxy* getProxy() {
        return &m_stVIPProxy;
    }
private:
	VIPProxy m_stVIPProxy;

	VIPPanel *m_pVIPPanel;

	vmsg::CSVIPInfos m_stVIPInfos;

    vmsg::CSBuyCoinRsp m_stBuyCoinRsp; //招财符信息
    BuyCoinPanel* m_pBuyCoinPanel;
};

#define P_VIP_CTRL CVLib::CVSingleton<VIPControl>::Instance()

#endif // VIPControl_h__