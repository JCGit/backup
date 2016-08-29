#ifndef RefitProxy_h__
#define RefitProxy_h__

#include "basic/QBaseProxy.h"

class RefitProxy: public QBaseProxy
{
public:
	RefitProxy();
	virtual ~RefitProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	void sendRefitQueryRqst(); // ��ѯ

	void sendRefitComposeRqst(unsigned int uDotaItemID); // �ϳ�

	void sendRefitMosaicRqst(unsigned int uPart, unsigned int uDotaItemID); // ��Ƕ

	void sendRefitLvUpRqst(unsigned int uPart); // ����

};

#endif // RefitProxy_h__