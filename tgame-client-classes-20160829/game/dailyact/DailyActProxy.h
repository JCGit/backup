#ifndef __DAILY_ACT_PROXY__
#define __DAILY_ACT_PROXY__

#include "QBaseProxy.h"
#include "BasicTypes.h"

USING_NS_CC;
class DailyActProxy: public QBaseProxy
{
public:
	DailyActProxy();
	~DailyActProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

    void sendQueryDailyAct();

    void sendHitMonsterRqst(vmsg::CSCVZHitMonsterRqst& stRqst);

    void sendHitedRqst(const int iHP);
    void sendDestroyBaseRqst(const int iBaseID);

    //monster osd ,�ϱ��ҿ��ƵĹ����״̬��Ϣ
    void sendSubmitMonsterPosRqst(const vmsg::CSCVZMonsters& stMonsterInfo);
    void sendSubmitMonsterSkillRqst(const vmsg::CSCVZMonsterInfo& stOneInfo);

	//��ȡ����
	void sendGetManualActAwardRqst();

private:
    void doSvrNotify(const vmsg::CSServerNotifyRespond& stNotify);

	void getNotifyStr(const vmsg::CSServerNotify& stNotify);
};

#endif // __DAILY_ACT_PROXY__
