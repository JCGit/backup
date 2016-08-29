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

	void sendQueryManualRqst(); // ��ѯ����

	void sendBuyManualRqst(); // ��������

	void sendQueryBuyManualRqst(); // ��ѯ��������

private:
	ManualProxy m_stManualProxy;

	ManualObj m_stManualObj;
};

#define  P_MANUAL_CTRL CVLib::CVSingleton<ManualControl>::Instance()

#endif // ManualControl_h__
