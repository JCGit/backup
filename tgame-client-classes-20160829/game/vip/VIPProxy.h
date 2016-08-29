#ifndef VIPProxy_h__
#define VIPProxy_h__

#include "basic/QBaseProxy.h"

class VIPProxy: public QBaseProxy
{
public:
	VIPProxy();
	virtual ~VIPProxy();

	// override start 

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

	void sendQueryVIPInfoRqst(); // ��ѯVIP��Ϣ

	void sendGetVIPAwardRqst(unsigned int uVIPLv); // ��ȡVIP����

    void sendQueryBuyCoinRqst();
    void sendBuyCoinRqst();
private:
	
};

#endif // VIPProxy_h__