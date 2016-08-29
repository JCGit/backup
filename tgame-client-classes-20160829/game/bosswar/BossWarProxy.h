#ifndef BossWarProxy_h__
#define BossWarProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"

class CBossWarProxy : public QBaseProxy
{
public:
	CBossWarProxy();
	virtual ~CBossWarProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendQueryBossChallengeRqst();
	void sendDamageReportRqst(vmsg::CSBossChallengeBigDamageReport& stRqst);
	void sendDelegateRqst(unsigned int uDelegate);		//0标示取消，1标示托管
	void sendAttendOrLeaveRqst(unsigned int uLeave);	//0标示离开，1标示加入
	void sendReliveRqst();								//复活

protected:
private:
};

#endif // BossWarProxy_h__
