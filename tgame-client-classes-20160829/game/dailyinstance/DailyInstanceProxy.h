#ifndef DailyInstanceProxy_h__
#define DailyInstanceProxy_h__

#include "basic/QBaseProxy.h"

class DailyInstanceProxy: public QBaseProxy
{
public:
	DailyInstanceProxy();
	virtual ~DailyInstanceProxy();

	// override start 

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

    void sendQueryRqst();  
    void sendPassRqst(const int iCfgKey, const int iPassLvIdx);
private:
	
};

#endif // DailyInstanceProxy_h__