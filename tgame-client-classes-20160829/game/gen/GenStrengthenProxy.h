#ifndef GenStrengthenProxy_h__
#define GenStrengthenProxy_h__

#include "QBaseProxy.h"
#include "BasicTypes.h"

class CGenStrengthenProxy: public QBaseProxy
{
public:
	CGenStrengthenProxy();
	virtual ~CGenStrengthenProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	//��ѯ����������
	void sendQueryGenStrengthenRqst();	

	//��������
	void sendGenStrengthenRqst(unsigned int uType);

	//��������������ȷ����Ϣ(1��ʾ����)
	void sendGenStrengthenConfirmRqst(unsigned int uIsAccept);
};

#endif // GenStrengthenProxy_h__
