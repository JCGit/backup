#ifndef ManualProxy_h__
#define ManualProxy_h__

#include "basic/QBaseProxy.h"

class ManualProxy: public QBaseProxy
{
public:
	ManualProxy();
	virtual ~ManualProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

	void sendQueryManualRqst(); // 查询体力

	void sendBuyManualRqst(); // 购买体力

	void sendQueryBuyManualRqst(); // 查询购买体力

private:

};

#endif // ManualProxy_h__
