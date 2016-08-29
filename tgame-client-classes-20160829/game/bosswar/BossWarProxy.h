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
	void sendDelegateRqst(unsigned int uDelegate);		//0��ʾȡ����1��ʾ�й�
	void sendAttendOrLeaveRqst(unsigned int uLeave);	//0��ʾ�뿪��1��ʾ����
	void sendReliveRqst();								//����

protected:
private:
};

#endif // BossWarProxy_h__
