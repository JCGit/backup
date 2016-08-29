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

	void sendQueryManualRqst(); // ��ѯ����

	void sendBuyManualRqst(); // ��������

	void sendQueryBuyManualRqst(); // ��ѯ��������

private:

};

#endif // ManualProxy_h__
