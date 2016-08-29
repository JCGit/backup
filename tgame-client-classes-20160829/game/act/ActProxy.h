#ifndef ActProxy_h__
#define ActProxy_h__

#include "basic/QBaseProxy.h"
#include "CSCouponMsg.pb.h"
#include <string>

using namespace std;

class CActProxy : public QBaseProxy
{
public:
	CActProxy();
	virtual ~CActProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	void sendClaimCouponRqst(const string couponID);		//兑换cdk
	void sendClaimFirstChargePkgRqst();						//领取首冲礼包
	void sendQueryAllPkgStatusRqst();						//查询所有礼包状态

	void sendClaimStoredManualRqst(); // 领取累积的体力

	void sendClaimSpringPkgRqst(unsigned int uDay); // 领取春节活动礼包

	void sendSumChargeRqst(int _lvl);	// 领取累计充值奖励
	void sendDailyChargeRqst(int _lvl);	// 领取每日充值奖励
	void sendDailyLoginRqst(int _lvl);	// 领取连续登陆奖励
	void sendGradeGeekRqst(int _lvl);	// 领取等级达人奖励
	void sendFightGeekRqst(int _lvl);	// 领取战力达人奖励
	void sendVipGiftRqst(int _lvl); //领取VIP奖励
	void sendDailyConsumeRqst(int _lvl);//领取每日消费奖励
	void sendSumConsumeRqst(int _lvl);//领取累计消费奖励

	void sendOnlinePkgRqst(int _lvl); //领取在线奖励
	void sendNextDayPkgRqst(int _lvl);//领取次日登录礼包

protected:
private:
};

#endif // ActProxy_h__
