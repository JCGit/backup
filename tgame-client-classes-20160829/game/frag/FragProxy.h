#ifndef FragProxy_h__
#define FragProxy_h__

#include "basic/QBaseProxy.h"

class FragProxy: public QBaseProxy
{
public:
	FragProxy();
	virtual ~FragProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);
    virtual void fault(const vmsg::CSCommonErrMsg &err);
	// override end

	void sendQueryDebrisBagRqst();
    void sendDebrisComposeRqst(const int iCfgID);
private:
	
};

#endif // FragProxy_h__