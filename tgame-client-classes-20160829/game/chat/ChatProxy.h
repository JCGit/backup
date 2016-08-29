#ifndef ChatProxy_h__
#define ChatProxy_h__

#include "basic/QBaseProxy.h"
#include "CSChatMsg.pb.h"
#include "basic/BasicTypes.h"

class ChatProxy: public QBaseProxy
{
public:
	ChatProxy();
	virtual ~ChatProxy();

	// override start

	virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);

	// override end

	void sendChatRqst(const unsigned int uChannelType, const string &msg); // адлЛ

	void sendSecretChatRqst(const UIN_t uin, const string &msg); // к╫ад

private:
	
};

#endif // ChatProxy_h__