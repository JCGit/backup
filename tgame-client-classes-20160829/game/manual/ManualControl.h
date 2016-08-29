#ifndef ManualControl_h__
#define ManualControl_h__

#include "vlib/base/CVSingleton.h"
#include "ClientSvrMsg.pb.h"

#include "ManualProxy.h"
#include "ManualObj.h"

class ManualControl
{
public:
	ManualControl();
	~ManualControl();

	const vmsg::CSBuyManualInfo& getBuyManualInfo();

	void startSyncManual();

	void endSyncManual();

	void notifyManualNotEnough();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendQueryManualRqst(); // 查询体力

	void sendBuyManualRqst(); // 购买体力

	void sendQueryBuyManualRqst(); // 查询购买体力

private:
	ManualProxy m_stManualProxy;

	ManualObj m_stManualObj;
};

#define  P_MANUAL_CTRL CVLib::CVSingleton<ManualControl>::Instance()

#endif // ManualControl_h__
