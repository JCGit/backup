#ifndef QianghunProxy_h__
#define QianghunProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"

class CQianghunProxy: public QBaseProxy
{
public:
	CQianghunProxy();
	virtual ~CQianghunProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendQiangHunQueryRqst();
	void sendQiangHunLayerQueryRqst(unsigned int layer);
	void sendQiangHunFlopRqst(unsigned int pos, unsigned int layer);
};

#endif // QianghunProxy_h__