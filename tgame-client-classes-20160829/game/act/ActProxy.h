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

protected:
private:
};

#endif // ActProxy_h__
