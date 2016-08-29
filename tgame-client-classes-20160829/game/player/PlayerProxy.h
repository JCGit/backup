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

	void sendGetMonthCardAwardRqst(); // 领取月卡奖励

	void sendGetVIPDailyAwardRqst(); // 领取vip日常奖励

};

#endif // PlayerProxy_h__