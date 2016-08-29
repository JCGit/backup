#ifndef  _QBASE_PROXY_
#define  _QBASE_PROXY_

#include "ClientSvrMsg.pb.h"
#include "PlayerLoginMsg.pb.h"

class QBaseProxy
{
public:
    virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) = 0;
	
	virtual void fault(const vmsg::CSCommonErrMsg &err) {};
};


#endif // _QBASE_PROXY_



