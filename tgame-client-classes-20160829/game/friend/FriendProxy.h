#ifndef __FRIEND_PROXY__
#define __FRIEND_PROXY__

#include "QBaseProxy.h"
#include "BasicTypes.h"

class FriendProxy: public QBaseProxy
{
public:
	FriendProxy();
	virtual ~FriendProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);
    virtual void fault(const vmsg::CSCommonErrMsg &err);

    void sendQueryFriendRqst();

    void sendAddReqRqst(const std::string& strName);
    void sendDelRqst(const UIN_t uUin);
    void sendAddBlackRqst(const UIN_t uUin);
    void sendAdminAddRqst(const UIN_t uUin);
    void sendAdminRefuseRqst(const UIN_t uUin);
    void sendAdminClearRqst();
    void sendGetProposeRqst();
    void sendSendManualRqst(const UIN_t uUin);
    void sendRecvManualRqst(const UIN_t uUin);
    void sendBatRecvManualRqst();

    void sendBatchAddRqst(const std::vector<UIN_t>& stUinLst);
};

#endif // __FRIEND_PROXY__
