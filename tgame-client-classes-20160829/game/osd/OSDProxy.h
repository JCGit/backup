#ifndef OSDProxy_h__
#define OSDProxy_h__

#include "basic/QBaseProxy.h"

class OSDProxy: public QBaseProxy
{
public:
	OSDProxy();
	virtual ~OSDProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

private:

};

#endif // OSDProxy_h__
