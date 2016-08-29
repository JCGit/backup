#ifndef ActControl_h__
#define ActControl_h__

#include "vlib/base/CVSingleton.h"

#include "BasicTypes.h"

#include "ActProxy.h"
#include "ActPanel.h"
#include "CDKPanel.h"
#include "HappyNewYearPanel.h"
#include "OnlineAwardPanel.h"
#include "NextDasyAwardPanel.h"

class ActCumulativeRechargeLayout;
class CActControl
{
public:
	CActControl();
	~CActControl();

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

	CActPanel* getActPanel();
	CDKPanel* getCDKPanel();
	OnlineAwardPanel* getOnlinePanel();
	NextDasyAwardPanel* getNextDayPanel();

	HappyNewYearPanel* getHappyNewYearPanel();
	ActCumulativeRechargeLayout* getSumRechargePanel();

	void doClaimCouponRsp(const vmsg::CSClaimCouponRsp& stMsg);
	void doClaimFirstChargePkgRsp(const vmsg::CSClaimFirstChargePkgRsp& stMsg);
	void doQueryAllPkgStatusRsp(const vmsg::CSQueryAllPkgStatusRsp& stMsg);
	void doClaimSpringPkgRsp(const vmsg::CSClaimSpringPkgRsp& stRsp);
	void doSumChargePkgRsp(const vmsg::SumChargePkgPkgRsp& stRsp);
	void doDailyChargePkgRsp(const vmsg::DailyChargePkgRsp& stRsp);
	void doSumLoginPkgRsp(const vmsg::DailyLoginPkgRsp& stRsp);
	void doGradeGeekPkgRsp(const vmsg::GradeGeekPkgRsp& stRsp);
	void doFightGeekPkgRsp(const vmsg::FightGeekPkgRsp& stRsp);
	void doVipGiftPkgRsp(const vmsg::VipGiftPkgRsp& stRsp);
	void doDailyConsumePkgRsp(const vmsg::DailyConsumePkgRsp& stRsp);
	void doSumConsumePkgRsp(const vmsg::SumConsumePkgRsp& stRsp);
	void doOnlinePkgRsp(const vmsg::OnlinePkgRsp& stRsp);
	void doNextDayPkgRsp(const vmsg::NextDayPkgRsp& stRsp);

	const vmsg::CSQueryAllPkgStatusRsp& getStatusInfo(){
		return m_stStatusInfo;
	}

	const vmsg::CSCouponActStatusItem& getItemInfoByType(unsigned int uTyp);

	bool IsActCanGet(const vmsg::CSCouponActStatusItem &stItemInfo);
protected:

private:

	void notifyAct();
	bool hasActGet();
	bool hasOnlineActGet();
	bool hasNextDayActGet();
private:
	CActProxy m_stProxy;

	CActPanel m_stActPanel;

	CDKPanel m_stCDKPanel;

	OnlineAwardPanel m_sOnlinePanel;
	NextDasyAwardPanel m_sNextDayPanel;

	HappyNewYearPanel *m_pHappyNewYearPanel;
	ActCumulativeRechargeLayout* m_pSumRechargePaenl;
	vmsg::CSQueryAllPkgStatusRsp m_stStatusInfo;
};

#define P_ACT_CTRL CVLib::CVSingleton<CActControl>::Instance()

#endif // ActControl_h__
