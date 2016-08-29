#ifndef RefitProxy_h__
#define RefitProxy_h__

#include "basic/QBaseProxy.h"

class RefitProxy: public QBaseProxy
{
public:
	RefitProxy();
	virtual ~RefitProxy();

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	void sendRefitQueryRqst(); // 查询

	void sendRefitComposeRqst(unsigned int uDotaItemID); // 合成

	void sendRefitMosaicRqst(unsigned int uPart, unsigned int uDotaItemID); // 镶嵌

	void sendRefitLvUpRqst(unsigned int uPart); // 升级

};

#endif // RefitProxy_h__