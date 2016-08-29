#ifndef PlayerProxy_h__
#define PlayerProxy_h__

#include "basic/QBaseProxy.h"
#include "CSPlayerMsg.pb.h"

class PlayerProxy: public QBaseProxy
{
public:
	PlayerProxy();
	virtual ~PlayerProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	void sendGetMonthCardAwardRqst(); // ��ȡ�¿�����

	void sendGetVIPDailyAwardRqst(); // ��ȡvip�ճ�����

};

#endif // PlayerProxy_h__