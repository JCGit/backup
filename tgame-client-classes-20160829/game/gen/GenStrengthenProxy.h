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

	//查询培养的属性
	void sendQueryGenStrengthenRqst();	

	//主角培养
	void sendGenStrengthenRqst(unsigned int uType);

	//主角培养的属性确认信息(1标示接受)
	void sendGenStrengthenConfirmRqst(unsigned int uIsAccept);
};

#endif // GenStrengthenProxy_h__
