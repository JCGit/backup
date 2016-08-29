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

	void sendClaimCouponRqst(const string couponID);		//�һ�cdk
	void sendClaimFirstChargePkgRqst();						//��ȡ�׳����
	void sendQueryAllPkgStatusRqst();						//��ѯ�������״̬

	void sendClaimStoredManualRqst(); // ��ȡ�ۻ�������

	void sendClaimSpringPkgRqst(unsigned int uDay); // ��ȡ���ڻ���


	void sendSumChargeRqst(int _lvl);	// ��ȡ�ۼƳ�ֵ����
	void sendDailyChargeRqst(int _lvl);	// ��ȡÿ�ճ�ֵ����
	void sendDailyLoginRqst(int _lvl);	// ��ȡ������½����
	void sendGradeGeekRqst(int _lvl);	// ��ȡ�ȼ����˽���
	void sendFightGeekRqst(int _lvl);	// ��ȡս�����˽���
	void sendVipGiftRqst(int _lvl); //��ȡVIP����
	void sendDailyConsumeRqst(int _lvl);//��ȡÿ�����ѽ���
	void sendSumConsumeRqst(int _lvl);//��ȡ�ۼ����ѽ���
	void sendOnlinePkgRqst(int _lvl); //��ȡ���߽���
	void sendNextDayPkgRqst(int _lvl);//��ȡ���յ�¼���

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
