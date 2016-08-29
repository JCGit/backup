#ifndef LotteryProxy_h__
#define LotteryProxy_h__

#include "basic/QBaseProxy.h"

class LotteryProxy: public QBaseProxy
{
public:
	LotteryProxy();
	virtual ~LotteryProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	void sendQueryLotteryRqst(); // ²éÑ¯³é¿¨ÐÅÏ¢

	void sendLotteryRqst(unsigned int uLotteryType); // ³é¿¨

};

#endif // LotteryProxy_h__
