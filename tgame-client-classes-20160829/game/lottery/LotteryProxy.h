#ifndef LotteryProxy_h__
#define LotteryProxy_h__

#include "basic/QBaseProxy.h"

class LotteryProxy: public QBaseProxy
{
public:
	LotteryProxy();
	virtual ~LotteryProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	void sendQueryLotteryRqst(); // ��ѯ�鿨��Ϣ

	void sendLotteryRqst(unsigned int uLotteryType); // �鿨

};

#endif // LotteryProxy_h__
