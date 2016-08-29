#ifndef NihilityProxy_h__
#define NihilityProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"
#include "vlib/CVArray.h"

class CNihilityProxy : public QBaseProxy
{
public:
	CNihilityProxy();
	virtual ~CNihilityProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	//����ǿ��
	void sendInfiniteGenInfoRqst();
	void sendInfiniteBuyGenRqst(unsigned int idx);

	//1��ʾ������սû��ɵģ�0��ʾ���¿�ʼ
	void sendNihilityFightStartRqst(unsigned int isContinue);			

	//ÿ��һ���ϱ�һ��
	void sendNihilityMissonReportRqst(unsigned int evolutioncnt, unsigned int floorScore, unsigned int coin, unsigned int floor, unsigned int score, CVArray<int, 100> stAwardFloor);	

	//��ѯ�ϴ��Ƿ������˳�
	void sendNihilityUnfinishedQueryRqst();	

	//stAwardFloor:�������Ĳ�������
	void sendNihilityFightEndRqst(unsigned int evolutioncnt, unsigned int floorScore, unsigned int coin, unsigned int floor, unsigned int score, CVArray<int, 100> stAwardFloor);
protected:
private:
};

#endif // NihilityProxy_h__
